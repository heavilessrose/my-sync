/*
File: EnemyShip.m
Abstract: Enemy ships (darts and mothership).

Version: 2.0
*/

#import "EnemyShip.h"
#import "ParticleSystemObject.h"
#import "Maths.h"
#import "GameObject_Private.h"
#import "Texture2D.h"
#import "Bullets.h"
#import "Utils.h"
#import "SoundEngineManager.h"
#import "TouchFighterAppDelegate.h"

//MODELS:

#import "enemy.h"
#import "mothership.h"

//PRIVATE CLASSES:

@interface EnemyShipInfo : PhysicalObject
{
	GameObject*		_laser;
	CGFloat			_damage;
}
@property(retain) GameObject*	laser;
@property CGFloat	damage;
@end

//EXTERNS:
extern void				RenderEnemyShip();

//CONSTANTS:

#define kDefaultMaxAge		500.	
#define kHighlightOffset	.2

@implementation EnemyShip
@synthesize highlight=_highlight, laser=_laser, damage = _damage;

- (id) init 
{
	Generator			generator;
	
	if ((self = [super init])) {
		_texture = EnemyShipTexture();	
		_friction = Point3Make(.1, .1, 0.);

		//Initialize damage fire.
		generator = GeneratorZero;
		generator.velocity =  Cube3Make(-0.5, -0.5, 0., 1., 1., 0.);
		_fire = [[Fire alloc] initWithTexture:StarTexture() generator:generator type:kParticleTriangleStrip count:10];
		_fire.radius = 2.;
		_fire.particleDecay = 4.;
		_fire.systemColor = Point4Make(1.0, 1.0, 0.5, 1.);
		_fire.systemAcceleration = Point3Make(0., 0., -30.);
		generator.position = Cube3Make(0., 0., -_fire.radius, 0., 0., _fire.radius);
		[_fire initializeParticles:generator];
		
		//Initialize damage flames.
		generator = GeneratorZero;
		generator.velocity =  Cube3Make(-0.5, -0.5, 0., 1., 1., 0.);
		_flames = [[Fire alloc] initWithTexture:FlameTexture() generator:generator type:kParticleTriangleStrip count:10];
		_flames.scale = Point3Make(3., 3., 3.);
		_flames.radius = 12.;
		_flames.particleDecay = 1.;
		_flames.systemColor = Point4Make(.5, .2, 0., 1.);
		_flames.systemAcceleration = Point3Make(0., 0., -10.);
		generator.position = Cube3Make(0., 0., -_flames.radius, 0., 0., _flames.radius);
		[_flames initializeParticles:generator];
		_width = _height = _depth = 3.;
	}
	
	return self;
}

- (void) updateWithGameObjectInfo:(EnemyShipInfo*)info
{
	GameObject*		laser;
	Point3			currentPosition, newPosition;
	CGFloat			alpha = 0.2;		//Smoothing
	
	currentPosition = self.position;
	newPosition = info.position;
	newPosition.x = newPosition.x*alpha + (1.-alpha)*currentPosition.x;
	newPosition.y = newPosition.y*alpha + (1.-alpha)*currentPosition.y;
	newPosition.z = newPosition.z*alpha + (1.-alpha)*currentPosition.z;
	info.position = newPosition;
	
	[super updateWithGameObjectInfo:info];

	_damage = info.damage;

	laser = info.laser;
	if (laser) {
		if (!_laser) {
			[self fireAtPointX:0. Y:0. Z:0.];
		}
		[_laser updateWithGameObjectInfo:laser];
	}
}

- (GameObject*) gameObjectInfo
{	
	EnemyShipInfo*		info = [EnemyShipInfo new];
	[info updateWithGameObjectInfo:self];
		
	info.damage = _damage;
	if (_laser) {
		info.laser = [_laser gameObjectInfo];
	}
	
	return [info autorelease];
}

- (void) dealloc
{
#ifdef __EXTRA_EFFECTS__
	[_flames release];
	[_fire release];
#endif
	[_laser release];

	[super dealloc];
}

- (void) fireAtPointX:(GLfloat)x Y:(GLfloat)y Z:(GLfloat)z
{
	Point3		dir;
	CGFloat		norm;

	//Create new laser to (x,y,z)
	dir = Point3Sub(Point3Make(x, y, z), _position);
	norm = Point3Norm(dir) / 20.;
	
	_laser = [[EnemyLaser alloc] initWithPosition:_position impulsion:Point3Make((x - _position.x)/norm, (y - _position.y)/norm, (z - _position.z)/norm)];
}

- (void) updatePositionAtTime:(NSTimeInterval)time
{
	static int		maxX = .5;
	float			newX = 0., newY = 0., newZ, speed;
	BOOL			host = ((TouchFighterAppDelegate*)[UIApplication sharedApplication].delegate).isHost;

	if (host && (time - _lastChangedTime > 1.)  && ((fast_unit_rand() > 0.997) || (_position.z > -10.) || (_position.z < -50.) || (ABS(_position.x/_position.z) > maxX) || (ABS(_position.y/_position.z) > maxX))) {			//Make the enemy ship change direction at times
		if (_position.z < -50.)
		newZ = fast_unit_rand() * 20. + 50. + _position.z;		
		else if ((_position.z > -10.) && (_position.z < 0.)) {
			if ((ABS(_position.x/_position.z) < maxX*2.) && (ABS(_position.y/_position.z) < maxX*2.)) {
				newZ = 10.;		
				if (_position.y > 0.)
				newY = (_position.y + 50.) * newZ;
				else
				newY = (_position.y - 50.) * newZ;
				if (_position.x > 0.)
				newX = (_position.x + 50.) * newZ;
				else
				newX = (_position.x - 50.) * newZ;
			}	
			else
			goto Skip;
		}
		else {
			newZ = fast_unit_rand() * -5. + 10. + _position.z;		
			
			if (_position.x/ABS(_position.z) < -maxX)
			newX = (fast_unit_rand() + 1.) * ABS(newZ);
			if (_position.x/ABS(_position.z) > maxX)
			newX = (-fast_unit_rand() - 1.) * ABS(newZ);
			else
			newX = (fast_unit_rand() - 0.5) * 2. * ABS(newZ);		
			
			if (_position.y/ABS(_position.z) < -maxX)
			newY = (fast_unit_rand() + 1.) * ABS(newZ);
			if (_position.y/ABS(_position.z) > maxX)
			newY = (-fast_unit_rand() - 1.) * ABS(newZ);
			else
			newY = (fast_unit_rand() - 0.5) * 2. * ABS(newZ);		
		}

		speed = fast_unit_rand()*15.+10.;
		_acceleration = Point3Mult(Point3Normalize(Point3Sub(Point3Make(newX, newY, newZ), _position)), speed);
		
		_lastChangedTime = time;
	}

Skip:
	[super updatePositionAtTime:time];
	
	_rotation.z = 140. * _velocity.y/30. + 270.;
	_rotation.y = 4. * (_velocity.x - ((_velocity.z > 0.) ? _velocity.z / 5. : 9.*_velocity.z)) - 5.;
	_rotation.y = Clamp(_rotation.y, -20., 180.);

	//Set position to self
	_flames.position = _position;
	_fire.position = _position;	
}

- (void) renderHighlight
{
	static GLfloat		vertices[] =	{
											-.5f,						-.5f + kHighlightOffset, 0.f,
											-.5f - kHighlightOffset,	-.5f + kHighlightOffset, 0.f,
											-.5f - kHighlightOffset,	+.5f + kHighlightOffset, 0.f,
											-.5f,						+.5f + kHighlightOffset, 0.f,
		
											+.5f,						-.5f + kHighlightOffset, 0.f,
											+.5f + kHighlightOffset,	-.5f + kHighlightOffset, 0.f,
											+.5f + kHighlightOffset,	+.5f + kHighlightOffset, 0.f,
											+.5f,						+.5f + kHighlightOffset, 0.f
										 };
	static GLushort		indices[] =		{0, 1, 1, 2, 2, 3, 4, 5, 5, 6, 6, 7};
	Matrix				currentModelView;
	Point3				position;
	
	//Transform ship position to current camera referential for billboard rendering.
	glGetFloatv(GL_MODELVIEW_MATRIX, currentModelView);
	position = Point3TransformFromModelViewToModelView(Point3Zero, _modelview, currentModelView);
	
	//Render highlight.
	glPushMatrix(); {
		glTranslatef(position.x, position.y, position.z);
		glScalef(3., 3., 3.);
		glDisable(GL_TEXTURE_2D);
		glColor4f(1.f, 0.f, 0.f, 1.f);
		[GameObject setVertexPointer:vertices size:3 type:GL_FLOAT];
		glDrawElements(GL_LINES, 12, GL_UNSIGNED_SHORT, indices);
		glColor4f(1.f, 1.f, 1.f, 1.f);		
	}
	glPopMatrix();
}

- (void) renderLasersAtTime:(NSTimeInterval)time
{
	//Render laser if alive.
	if (_laser.alive) {
		[_laser renderAtTime:time];
	} else {
		[_laser release];
		_laser = nil;
	}
}

- (void) renderAtTime:(NSTimeInterval)time
{
#if !TARGET_IPHONE_SIMULATOR
	float		position = _position.z;
	Sound*		sound;
#endif
	
	[super renderAtTime:time];

#if !TARGET_IPHONE_SIMULATOR
	if ((_position.z+.5)*(position+.5) < 0.) {
		sound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"wind"];
		sound.pitch = 2.;
		sound.volume = 0.1;
		sound.position = Point3Make(_position.x, _position.y, _position.z);
		[sound play];		
	}
#endif
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	glGetFloatv(GL_MODELVIEW_MATRIX, _modelview);

	SetOpenGLStates();
	RenderEnemyShip();
	UnsetOpenGLStates();
}

- (void) renderDamageFlamesAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix
{
	[_flames renderAtTime:time transformPosition:matrix];
}

- (void) renderDamageFireAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix
{
	CGFloat		scale = _damage / 50.;		//The fire size increases as damages et bigger.
	
	_fire.scale = Point3Make(scale, scale, scale);
	[_fire renderAtTime:time transformPosition:matrix];
}

@end

@implementation EnemyShipInfo
@synthesize laser=_laser;
@synthesize damage=_damage;
@end

@implementation MotherShip

- (id) init 
{
	if ((self = [super init])) {
		motherShipTexArray = (Texture2D**)malloc(sizeof(Texture2D*));
		motherShipTexArray[0] = [[[Texture2D alloc] initWithImagePath:@"mothership.png"] retain];	
	}
	
	return self;
}

- (void) dealloc
{
	[motherShipTexArray[0] release];
	if (motherShipTexArray != NULL) {
		free(motherShipTexArray);
	}
	
	[super dealloc];
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	glPushMatrix(); {
		glScalef(1., 1., -1.);
		glTranslatef(-1.4 + .25, -0.8, 5.271335);
		
		//Render model.
		SetOpenGLStates();
		MotherShipRender();
		glScalef(-1., 1., 1.);
		glTranslatef(-1.4 * 2. + .575, 0., 0.);
		MotherShipRender();
		UnsetOpenGLStates();
	}
	glPopMatrix();
}

- (Point3) bayOffset
{
	Point3 pos = Point3Zero;
	
	//Compute bay position (from which emeny ships take off).
	pos.y = .2 * cos(DegreesToRadians(_rotation.x + 270.));
	pos.z = .2 * sin(DegreesToRadians(_rotation.x + 270.));
	pos.x *= _width * _scale.x;
	pos.y *= _height * _scale.y;
	pos.z *= _depth * _scale.z;
	
	return pos;
}

@end
