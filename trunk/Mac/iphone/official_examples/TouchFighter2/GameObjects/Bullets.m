/*
File: Bullets.m
Abstract: All the projectiles used in the game: lasers, missiles, bombs.

Version: 2.0
*/

#import "Bullets.h"
#import "Utils.h"
#import "GameObject_Private.h"
#import "ParticleSystemObject.h"
#import "missile.h"
#import "AnimationObject.h"

//CONSTANTS:

#define kSmokeFade				.99
#define kDefaultMaxAge			25.
#define kMissileMaxAge			200.
#define kBombMaxAge				70.
#define kMissileImpulse			10.

static void _RenderMissile(Point3 axis, float angle)
{
	glPushMatrix(); {
		glRotatef(180., 0., 1., 0.);
		glTranslatef(0., 2., 5.783);
		if (angle && Point3Norm(axis))
		glRotatef(axis.x, axis.y, axis.z, angle);
		MissileRender();
	}
	glPopMatrix();
}

//EXTERNAL DEFINITIONS:

extern Texture2D*		_missileTexture;

//CLASS INTERFACES:

@interface SmokeTrail : PhysicalObject {
	GLfloat		*_vtxData;
	GLfloat		*_texData;
	Texture2D	*_tex;
	int			_numVertices;
	int			_index;
}
- (id) initWithTexture:(Texture2D*)tex;
- (void) addPointX:(float)x Y:(float)y Z:(float)z;
@end

@interface EnemyLaserInfo : PhysicalObject
{
	Point3		_targetDirection;
}
@property Point3	targetDirection;
@end

//CLASS IMPLEMENTATIONS:

@implementation SmokeTrail

- (id) initWithTexture:(Texture2D*)tex
{
	if ((self = [super init])) {
		//Initialize ivars.
		_tex = [tex retain];
		_numVertices = 200;
		_vtxData = malloc(sizeof(GLfloat) * 6 * _numVertices);
		_texData = malloc(sizeof(GLfloat) * 4 * _numVertices);
		_index = 0;
		_position = Point3Zero;
	}
	return self;
}

- (void) addPointX:(float)x Y:(float)y Z:(float)z
{
	if (_index >= _numVertices - 1) {
		return;
	}
	
	int i;
	
	//Add new vertices to increase smoke trail length.
	_vtxData[6 * _index + 0] = x - .1;
	_vtxData[6 * _index + 1] = y - .1;
	_vtxData[6 * _index + 2] = z;
	
	_texData[4 * _index + 0] = (_index == 0) ? 0. : 1.;
	_texData[4 * _index + 1] = 1.;
	
	_vtxData[6 * _index + 3] = x + .1;
	_vtxData[6 * _index + 4] = y + .1;
	_vtxData[6 * _index + 5] = z;
	
	_texData[4 * _index + 2] = (_index == 0) ? 0. : 1.;
	_texData[4 * _index + 3] = 0.;

	//Start fading away the trail
	for(i = 0; i < _index - 1; i++)
	{
		_texData[4 * i + 0] *= kSmokeFade;
		_texData[4 * i + 2] *= kSmokeFade;
	}
	
	_texData[4 * (_index - 1) + 0] = .5;
	_texData[4 * (_index - 1) + 2] = .5;
	
	_index++;
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	if (_index < 1) {
		return;
	}
	
	//Set-up
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	[GameObject setVertexPointer:_vtxData size:3 type:GL_FLOAT];
	[GameObject setTexcoordPointer:_texData size:2 type:GL_FLOAT];
	[GameObject bindTexture2D:_tex];

	//Drawing
	glColor4f(.4, .4, .4, .4);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, _index * 2 - 2);
	glColor4f(1., 1., 1., 1.);
	
	//Clean-up
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

- (void) dealloc
{
	[_tex release];
	if (_vtxData) {
		free(_vtxData);
	}
	if (_texData) {
		free(_texData);
	}
	[super dealloc];
}

@end

@implementation Laser
@synthesize age=_age;

- (id) init
{
	Generator			generator = GeneratorZero;

	if ((self = [super init])) {
		_fire = [[Fire alloc] initWithTexture:StarTexture() generator:generator type:kParticlePointSprite count:5];
		_fire.particleDecay = .1;
		_fire.radius = 6.;
		_fire.attenuation = Point3Make(0.0, 0.3, 0.004);
		generator.velocity = (Cube3){{0.,0.,50.}, {0.,0.,100.}};	
		[_fire initializeParticles:generator];
		_color = Point3Make(0.3, 0.3, 1.0);
		_maxAge = kDefaultMaxAge;
	}

	return self;
}

- (BOOL) collideWith:(id<CollisionDetection>)object
{
	return [super collideWith:object];
}

- (void) updatePositionAtTime:(NSTimeInterval)time
{
	CGFloat		alpha, Q = -100;
	GLfloat		norm = Point3Norm(_velocity);	

	[super updatePositionAtTime:time];
	
	//Fade out lasers as their age increases.
	alpha = pow((_maxAge-_age)/_maxAge, 1.7);
	_fire.systemColor = Point4Make(alpha * _color.x, alpha * _color.y, alpha * _color.z, 1.0);
	
	//Lasers expand along their velocity direction
	if (norm && (_age > 0)) {
		_fire.systemVelocity = Point3Mult(_velocity, Q/norm);

		//We need to keep the particle system position in synch for particle caching, which draws in world referential.
		_fire.position = _position;
	}

	_age ++;
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{	
	[_fire renderAtTime:time];
}

- (BOOL) alive
{
	return (_age < _maxAge);
}

@end

extern int _numEnemies;
extern Texture2D* _enemyTexture;

@implementation Missile
@synthesize age=_age;

- (id) initWithImpulsion:(Point3Ptr)impulsion target:(GameObject*)target
{
	Generator		generator = GeneratorZero;

	if ((self = [super init])) {
		//Initialize ivars.
		_color.x = 1.0; _color.y = 0.3; _color.z = 0.3;
		_maxAge = kMissileMaxAge;
		_tolerance = 2.f;
		_target = [target retain];
		_acceleration = *impulsion;

		_missileTexture = [MissileTexture() retain];			

		//Initialize propulsion fire for missile.
		generator.velocity = Cube3Make(-30, -30, 0., 60., 60., 0.);
		_propulsionFire = [[Fire alloc] initWithTexture:StarTexture() generator:generator type:kParticlePointSprite count:25.];
		_propulsionFire.attenuation = Point3Make(0., .2, .0);
		_propulsionFire.particleDecay = 7.;
		_propulsionFire.radius = 12.;
		_propulsionFire.systemColor = Point4Make(.5, .5, 0.35, 1.);
		generator.position = Cube3Make(0.,0.,0.,0.,0.,_propulsionFire.radius);
		[_propulsionFire initializeParticles:generator];
		_propulsionFire.position = Point3Make(0., 0., .5);
		
		//Intialize smoke trail.
		_smoke = [[SmokeTrail alloc] initWithTexture:StarTexture()];
	}

	return self;
}

- (void) dealloc
{
	[_propulsionFire release];
	[_smoke release];
	[_target release];
	[_missileTexture release];
	
	[super dealloc];
}

- (BOOL) collideWith:(id<CollisionDetection>)object
{	
	return [self collideWith:object tolerance:_tolerance];
}

- (void) updatePositionAtTime:(NSTimeInterval)time
{
	Point3			toTarget, zAxis = Point3Make(0., 0., -1.);
	float			color;
		
	//Auto-guide us in to the target
	if (_age > 5.) {
		if (_target) {
			toTarget = Point3Sub(_target.position, _position);
			toTarget = Point3Mult(toTarget, 1./Point3Norm(toTarget));
			_acceleration = Point3Mult(toTarget, kMissileImpulse);
		}
		else if (_age == 6.)
		_acceleration = Point3Add(_acceleration, Point3Make(5., 0., -kMissileImpulse));
		
		[_smoke addPointX:_position.x Y:_position.y - .04 Z:_position.z + .4];

		_propulsionFire.systemVelocity = Point3Mult(Point3Mult(_velocity, 1./Point3Norm(_velocity)), -_propulsionFire.radius);
		color = MAX(1.+_position.z/100., 0.);
		_propulsionFire.systemColor = Point4Make(color*.5, color*.5, color*.35, color);
		_propulsionFire.scale = Point3Make(-_position.z/20.+1., -_position.z/20.+1., -_position.z/20.+1.);
	}

	GetPoint3RotationAxisAndAngle(&zAxis, &_velocity, &_rotationAxis, &_rotationAngle);
	_rotationAngle = RadiansToDegrees(_rotationAngle);
	
	[super updatePositionAtTime:time];

	_age++;
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{	
	//Render model.
	SetOpenGLStates();
	_RenderMissile(_rotationAxis, _rotationAngle);
	UnsetOpenGLStates();

	[_propulsionFire renderAtTime:time];
}

- (void) renderSmokeAtTime:(NSTimeInterval)time
{
	[_smoke renderAtTime:time];	
}

- (BOOL) alive
{
	return (_target ? _target.position.z < _position.z : _position.z>-100.);
}

@end

@implementation EnemyLaser

- (id) initWithPosition:(Point3)position impulsion:(Point3)impulsion
{
	Generator			generator = GeneratorZero;
	CGFloat				norm;
		
	if ((self = [super initWithTexture:StarTexture() generator:generator type:kParticleTriangleStrip count:25])) {
		//Set-up invars.
		self.particleDecay = .1;
		self.radius = 25.;
		self.position = position;
		self.velocity = impulsion;
		self.scale = Point3Make(0.3, 0.3, 0.3);
		self.attenuation = Point3Make(0.0, 0.3, 0.004);
		self.width = self.height = self.depth = 0.1;			//bouding box used in collision detection
		
		//Initialize particles
		norm = Point3Norm(impulsion);
		generator.position = Cube3Make(impulsion.x/norm*-self.radius,impulsion.y/norm*-self.radius,impulsion.z/norm*-self.radius, impulsion.x/norm*self.radius,impulsion.y/norm*self.radius,impulsion.z/norm*self.radius);	
		[self initializeParticles:generator];
		_targetDirection = impulsion; 
		_maxAge = kDefaultMaxAge*12.;
	}
	
	return self;
}

- (void) updateWithGameObjectInfo:(EnemyLaserInfo*)info
{
	[super updateWithGameObjectInfo:info];
	
	_targetDirection = info.targetDirection;
}

- (GameObject*) gameObjectInfo
{
	EnemyLaserInfo*		info = [EnemyLaserInfo new];

	[info updateWithGameObjectInfo:self];
	info.targetDirection = _targetDirection;
	
	return [info autorelease];
	
}

- (void) renderAtTime:(NSTimeInterval)time
{
	[super renderAtTime:time];
	_age++;
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	CGFloat			Q = -2., norm, alpha, radius = self.radius;

	self.velocity = Point3Make(sin(_age)*2. + _targetDirection.x, cos(_age)*2. + _targetDirection.y, _targetDirection.z);
	
	//Fade out lasers as their age increases.	
	norm = Norm (_velocity.x, _velocity.y, _velocity.z);	
	self.systemVelocity = Point3Make(_velocity.x/norm*Q*radius, _velocity.y/norm*Q*radius, _velocity.z/norm*Q*radius);

	//Lasers expand along their velocity direction
	alpha = pow((_maxAge-_age)/_maxAge, 1.7);
	self.systemColor = Point4Make(alpha * 1.0, alpha * 0.3, alpha * 1.0, 1.0);
	
	[super renderObjectAtTime:time];
}

- (BOOL) alive
{
	return ((_age < _maxAge) && (_position.z < -1));
}

@end

@implementation EnemyLaserInfo
@synthesize targetDirection=_targetDirection;
@end


@implementation Bomb
@synthesize growRate=_growRate;

- (id) initWithPosition:(Point3)position
{
	if ((self = [super init])) {
		//Initialize ivars.
		_fire = [[SpriteObject alloc] initWithTexture:StarTexture()];
		_fire.position = position;
		_fire.color = Point4Make(.9, .9, 1., 1.);
		_maxAge = kBombMaxAge;
	}

	return self;
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	//Grow the sprite along growRate.
	_fire.scale = Point3Make(_fire.scale.x * _growRate.x, _fire.scale.y * _growRate.y, _fire.scale.z * _growRate.z); 
	_tolerance += 7.;

	//Render.
	[_fire renderAtTime:time];
	_age++;
}

- (BOOL) alive
{
	return (_age < _maxAge);
}

- (BOOL) collideWith:(id<CollisionDetection>)object
{	
	return [self collideWith:object tolerance:_tolerance];
}

- (void) dealloc
{
	[_fire release];
	
	[super dealloc];
}

@end
