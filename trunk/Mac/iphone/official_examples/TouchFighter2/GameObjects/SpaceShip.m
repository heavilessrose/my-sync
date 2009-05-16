/*
File: SpaceShip.m
Abstract: Main spaceship.

Version: 2.0
*/

#import "SpaceShip.h"
#import "SpriteObject.h"
#import "GameObject_Private.h"
#import "PhysicalObject.h"
#import "Bullets.h"
#import "Maths.h"
#import "Utils.h"
#import "k_wing.h"
#import "enemy.h"
#if !TARGET_IPHONE_SIMULATOR
#import "SoundEngineManager.h"
#endif

//CONSTANTS:

#define kRefPositionX			-0.2
#define kRefPositionY			0.
#define kRefPositionZ			-1.
#define kWingAngleChangeRate	.5
#define kMissileStartImpulsion	900.
#define kFinalMissileImpulse	17.

//FUNCTIONS:

static void RenderShip(float wingAngle)
{
	glPushMatrix(); {
		glTranslatef(1.92f, -3.33f, 0.0);
		glPushMatrix(); {
			glScalef(12.f, 12.f, 12.f);
			glTranslatef(-4.116f, -.362f, 4.506f);
			k_wing_body();
			glScalef(-1.f, 1.f, 1.f);
			glTranslatef(-4.116f * 2.f + .31f, 0.f, 0.f);
			k_wing_body();
		}
		glPopMatrix();
		glPushMatrix(); {
			glScalef(6.f, 6.f, 6.f);
			glPushMatrix();
			{
				glTranslatef(-.21, .6, 0);
				glRotatef(wingAngle, 0, 0, 1);
				glTranslatef(.21, -.6, 0);
				glTranslatef(-3.916, -1.8, 10.9);
				k_wing_wing();
			}
			glPopMatrix();
			glScalef(-1.f, 1.f, 1.f);
			glPushMatrix(); {
				glTranslatef(.4, .6, 0);
				glRotatef(wingAngle, 0, 0, 1);
				glTranslatef(-.4, -.6, 0);
				glTranslatef(-3.916 + .615f, -1.8, 10.9);
				k_wing_wing();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void RenderEnemyShip()
{
	glPushMatrix(); {
		glScalef(8.0f / 61134.718750f, 8.0f / 61134.718750f, 8.0f / 61134.718750f);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glScalef(1.0f / 32767.0f, 1.0f / 32767.0f, 1.0f / 32767.0f);
		glMatrixMode(GL_MODELVIEW);
			
		//Render model.
		EnemyShipRender();

		glMatrixMode(GL_TEXTURE);
		glPopMatrix();			
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//CLASS INTERFACES:

// Simple class conforming to CollisionDetection protocol
@interface Target : NSObject<CollisionDetection> {
	Point3		_position;
}
@property Point3	position;
@end

//CLASS IMPLEMENTATIONS:

@implementation Target
@synthesize position=_position;

- (BOOL) collideWith:(id<CollisionDetection>)object
{
	return [object collideWith:self];
}

- (Point3) boundingBox
{
	return Point3Make(0.1, 0.1, 0.1);
}

@end

@implementation SpaceShip

@synthesize targetInFocus=_targetInFocus, lasers=_lasers, missiles=_missiles, mode=_mode, mothership=_mothership;

- (id) initWithType:(SpaceShipType)type
{
	Generator			generator;
	Texture2D*			texture;
	
	if ((self = [super init])) {
		_type = type;
		_lasers = CFArrayCreateMutable(kCFAllocatorDefault, 10, &kCFTypeArrayCallBacks);
		_missiles = CFArrayCreateMutable(kCFAllocatorDefault, 3, &kCFTypeArrayCallBacks);
		_bomb[0] = _bomb[1] = nil;

		_shipPosition = Point3Zero;
		_shipRotation = _offset = Point3Zero;
		_friction = Point3Make(90., 90., 90.);
		_mode = kSpaceShipModeNormal;
		_shipFallback = 0.;
		_wingAngle = 0.;
		_wingDefaultAngle = 0.;
		_width = _height = _depth = 0.3;			//bouding box used in collision detection
				
		// Load textures
		if (_type == kSpaceShipTypeDart) {
			_texture = ShipTexture();
		}
		else {
			_texture = EnemyShipTexture();
		}
		
		// Creating propulsion fire particle system
		generator.position = Cube3Make(0.,0.,0.,0.,0.,0.);
		generator.velocity = Cube3Make(-5.,-5.,0.,10.,10.,0.);
		_propulsionFire = [[Fire alloc] initWithTexture:StarTexture() generator:generator type:kParticlePointSprite count:25.];
		CHECK_CREATION(_propulsionFire);
		_propulsionFire.particleDecay = 8.;
		_propulsionFire.radius = 4.;
		_propulsionFire.systemVelocity = Point3Make(0.0, 0.0, -_propulsionFire.radius*20.);
		_propulsionFire.attenuation = Point3Make(0., 1., 0.004);
		if (_type == kSpaceShipTypeDart) {
			_propulsionFire.position = Point3Make(0., 0., -4.8);		
		}
		else {
			_propulsionFire.position = Point3Make(0., 0., -3.8);				
			_propulsionFire.systemColor = Point4Make(0.5, 0.5, 1., 1.);
		}
		generator.position = Cube3Make(0.,0.,-_propulsionFire.radius,0.,0.,_propulsionFire.radius);
		[_propulsionFire initializeParticles:generator];
		
		//Crosshair
		texture = [[Texture2D alloc] initWithImagePath:@"crosshairs.png"];
		_crosshair = [[SpriteObject alloc] initWithTexture:texture];
		[texture release];
		_crosshair.scale = Point3Make(.2, .2, .2);
		CHECK_CREATION(_crosshair);
		
		//Initializes particle system that represent ship damages (flames on right wing)
		generator = GeneratorZero;
		generator.velocity =  Cube3Make(-0.5, -0.5, 0., 1., 1., 0.);
		_damages = [[Fire alloc] initWithTexture:FlameTexture() generator:generator type:kParticleTriangleStrip count:10];
		if (!_damages) {
			[self release];
			return nil;
		}
		if (_type == kSpaceShipTypeDart) {
			_damages.position = Point3Make(-2.08, 1.17, -4.);
		}
		else {
			_damages.position = Point3Make(-2.08, 0., -4.);
		}
		_damages.radius = 12.;
		_damages.particleDecay = 2.;
		_damages.scale = Point3Make(1., 1., 1.);
		_damages.systemColor = Point4Make(.5, .2, 0., 1.);
		_damages.systemAcceleration = Point3Make(0., 0., -10.);
		generator.position = Cube3Make(0., 0., -_damages.radius, 0., 0., _damages.radius);
		[_damages initializeParticles:generator];	
	}
	
	return self;
}

- (void) dealloc
{
	if (_propulsionFire) {
		[_propulsionFire release];
	}
	if (_lasers) {
		CFRelease(_lasers);
	}
	if (_missiles) {
		CFRelease(_missiles);
	}
	
	[_bomb[0] release];
	[_bomb[1] release];
	[_damages release];
	[_crosshair release];

	[super dealloc];
}

- (BOOL) collideWith:(id<CollisionDetection>)object
{
	return [self collideWith:object tolerance:0.1];
}

- (void) setPosition:(Point3)position
{
	CGFloat			alpha = 0.1,
					oldY = _shipPosition.x,
					tempWingAngle;
	
	//Compute ship position and rotation.
	_shipPosition.x = Clamp(kRefPositionX-position.x/50., -0.5, 0.1);
	_shipPosition.y = Clamp(kRefPositionY-position.y/50., -0.7, 0.7);
	_shipPosition.z = position.z;
	_shipRotation.x = (position.x * MAX(1.-ABS(position.x/30.), 0.) + -position.x * ABS(position.x/30.)) * alpha + (1.-alpha) * _rotation.x;
	_shipRotation.y = (-position.y * MAX(1.-ABS(position.y/30.), 0.) + position.y * ABS(position.y/30.)) * alpha + (1.-alpha) * _rotation.y;
	_shipRotation.z = -position.y * 1.5;
	
	//Update angle between wings.
	tempWingAngle = 1000.f * (oldY - _shipPosition.x);	
	if (_mode == kSpaceShipModeNormal) {
		_wingAngle = Clamp(_wingAngle * .8 + tempWingAngle * .2, -20.0, 20.0);
	}
}

- (Point3) rotation
{
	return Point3Make(_rotation.x + _shipRotation.x, _rotation.y + _shipRotation.y, _rotation.z + _shipRotation.z);
}

- (void) fireFromPosition:(Point3Ptr)position withImpulsion:(Point3Ptr)impulsion target:(GameObject*)target type:(MissileType)type
{
	PhysicalObject*		bullet;
	
	if (type == kMissileTypeLaser) {	//Fire laser
		bullet = [Laser new];
		CFArrayAppendValue(_lasers, bullet);
	}
	else {								//Fire missile
		bullet = [[Missile alloc] initWithImpulsion:impulsion target:target];
		CFArrayAppendValue(_missiles, bullet);	
	}
	bullet.position = *position;
	bullet.acceleration = *impulsion;
	[bullet release];
	
	_isFiring = YES;
}

- (void) fireLasersAtPosition:(SpaceShipLaserPosition)side
{
	Point3				impulsion, position;
#if !TARGET_IPHONE_SIMULATOR
	Sound*				sound;
#endif
	 	
	impulsion.x = impulsion.y = 0.0;
	impulsion.z = kMissileStartImpulsion;
	
	//Create lasers in ship referential.
	if (side == kSpaceShipLaserPosition_Left) {
		if (!_nextLasersAreTopLasers) {
			position.x = 0.2; position.y = 0.0 + 0.2; position.z = 0.5;	
			[self fireFromPosition:&position withImpulsion:&impulsion target:nil type:kMissileTypeLaser];
		}
		else {
			position.x = 0.1; position.y = 0.05 + 0.2; position.z = 0.5;	
			[self fireFromPosition:&position withImpulsion:&impulsion target:nil type:kMissileTypeLaser];	
		}
	}
	else {
		if (!_nextLasersAreTopLasers) {
			position.x = -0.2; position.y = 0.0 + 0.2; position.z = 0.5;	
			[self fireFromPosition:&position withImpulsion:&impulsion target:nil type:kMissileTypeLaser];
		}
		else {
			position.x = -0.1; position.y = 0.05 + 0.2; position.z = 0.5;
			[self fireFromPosition:&position withImpulsion:&impulsion target:nil type:kMissileTypeLaser];
		}
	}
	_nextLasersAreTopLasers = !_nextLasersAreTopLasers;
	
#if !TARGET_IPHONE_SIMULATOR
	//Play sound
	sound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"laser"];
	sound.position = Point3Make(2., 2.*_position.y, _position.z);
	[sound play];
#endif	
}

- (void) fireMissileAtPosition:(SpaceShipLaserPosition)side withTarget:(GameObject*)target 
{
#if !TARGET_IPHONE_SIMULATOR
	Sound*		sound;
#endif
	Point3		impulsion, position;
	
	//Fire first missile in ship referential.
	impulsion = Point3Make(0., -5., 0.);
	position = Point3Make((side == kSpaceShipLaserPosition_Left) ? .4 : -0.3, -0.3, 0.5);
	[self fireFromPosition:&position withImpulsion:&impulsion target:target type:kMissileTypeMissile];

#if !TARGET_IPHONE_SIMULATOR
	//Play sound.
	sound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"missile"];
	sound.position = _position;
	[sound play];
#endif
}

- (void) fireMissiles
{
#if !TARGET_IPHONE_SIMULATOR
	Sound*		sound;
#endif
	Point3		impulsion, position;
	float		norm;
	
	//Fire first missile in ship referential.
	impulsion = Point3Make(-2., 0., 5.);
	norm = 1. / Norm(impulsion.x, impulsion.y, impulsion.z);
	norm *= kFinalMissileImpulse;
	impulsion = Point3Mult(impulsion, norm);
	position = Point3Make(-0.5, 0., -2.);
	[self fireFromPosition:&position withImpulsion:&impulsion target:_mothership type:kMissileTypeMissile];

	//Fire second missile in ship referential.
	impulsion = Point3Make(-1., -5., 5.);
	norm = 1. / Norm(impulsion.x, impulsion.y, impulsion.z);
	norm *= kFinalMissileImpulse;
	impulsion = Point3Mult(impulsion, norm);
	position = Point3Make(-0.5, 0., -2.);
	[self fireFromPosition:&position withImpulsion:&impulsion target:_mothership type:kMissileTypeMissile];

	//Fire third missile in ship referential.
	impulsion = Point3Make(-1., 5., 5.);
	norm = 1. / Norm(impulsion.x, impulsion.y, impulsion.z);
	norm *= kFinalMissileImpulse;
	impulsion = Point3Mult(impulsion, norm);
	position = Point3Make(-.5, 0., -2.);
	[self fireFromPosition:&position withImpulsion:&impulsion target:_mothership type:kMissileTypeMissile];
		
#if !TARGET_IPHONE_SIMULATOR
	//Play sound.
	sound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"missile"];
	sound.position = _position;
	[sound play];
#endif
	
	_mode = kSpaceShipModeMissileFired;
}

- (void) fireBomb
{
	//Bomb is made of 2 overlapping sprites.
	_bomb[0] = [[Bomb alloc] initWithPosition:Point3Make(_mothership.position.x, _mothership.position.y, _mothership.position.z - 20.)];
	_bomb[0].growRate = Point3Make(1.05, 1.95, 1.01);

	_bomb[1] = [[Bomb alloc] initWithPosition:Point3Make(_mothership.position.x, _mothership.position.y, _mothership.position.z - 20.)];
	_bomb[1].growRate = Point3Make(1.25, 1.25, 1.01);
}

- (BOOL) willHit:(PhysicalObject*)object
{
	static		Target*	targetObject = nil;
	Point3		target;
	GLfloat		projection;
	
	if (!targetObject) {
		targetObject = [Target new];
	}
	
	//Create a target at object position.
	target = Point3Make(object.position.x - _firePosition.x, 
						object.position.y - _firePosition.y, 
						object.position.z - _firePosition.z);
	projection = Point3DotProduct(target, _fireDirection);
	
	//Find if this target in laser focus, by colliding its projection on laser direction with itself.
	targetObject.position = Point3Make(projection * _fireDirection.x + _firePosition.x,
										projection * _fireDirection.y + _firePosition.y,
										projection * _fireDirection.z + _firePosition.z);
	
	return [object collideWith:targetObject tolerance:0.3];
}

- (void) hitLeft
{
	//When hit on the left, throw ship to the right.
	_acceleration.y += 9.;
	_shipPosition.x = _position.x;
	_shipPosition.y = _position.y;
	_shipPosition.z = _position.z;
	_damage++;
}

- (void) hitRight
{
	//When hit on the right, throw ship to the left.
	_acceleration.y -= 9.;
	_shipPosition.x = _position.x;
	_shipPosition.y = _position.y;
	_shipPosition.z = _position.z;
	_damage++;
}

- (void) updatePositionAtTime:(NSTimeInterval)time				//Override.
{
	unsigned			i;
	Point3				signs, tpoint;
	GLfloat				alpha = 0.1;
	
	if (_previousRenderTime < 0.) {
		_previousRenderTime = time;
	}
	
	//Updates offset given to the ship when hit.
	tpoint = Point3Mult(_friction, -(time - _previousRenderTime));
	_acceleration = Point3Add(_acceleration, Point3Make(_velocity.x * tpoint.x, _velocity.y * tpoint.y, _velocity.z * tpoint.z));
	signs = Point3Make(Sign(_velocity.x), Sign(_velocity.y), Sign(_velocity.z));
	_velocity = Point3Add(_velocity, Point3Mult(_acceleration, time - _previousRenderTime));

	for (i=0; i<3; ++i) {
		if ((POINT3_COORD_AT_INDEX(signs, i) != 0) && (POINT3_COORD_AT_INDEX(signs, i) * Sign(POINT3_COORD_AT_INDEX(_velocity, i)) <= 0)) {
			POINT3_COORD_AT_INDEX(_velocity, i) = 0.;
			POINT3_COORD_AT_INDEX(_acceleration, i) = 0.;
		}
	}
	_offset = Point3Add(_offset, Point3Mult(_velocity, time - _previousRenderTime));
	_offset	= Point3Mult(_offset, (1.-alpha));

	_rotation = Point3Mult(_rotation, (1.-alpha));
	_position = Point3Add(_shipPosition, _offset);
	_rotation.z = 180. * Clamp(_velocity.y/30., -0.5, 0.5);

	//Update angle between wings.
	if (_type == kSpaceShipTypeDart) {
		if (fabs(_wingAngle - _wingDefaultAngle) > kWingAngleChangeRate) {
			_wingAngle = .9 * _wingAngle + .1 * _wingDefaultAngle;
		} else {
			_wingAngle = _wingDefaultAngle;
		}
		
		if ((_wingAngle == kMissileWingAngle) && (_mode == kSpaceShipModeMissileInitiated)) {
			[self fireMissiles];
		}
	}
	
	_previousRenderTime = time;
}

- (void) renderCrosshairAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix
{
	_crosshair.color = (_targetInFocus ? Point4Make(1., 0., 0., 1.) : Point4Make(1., 1., 1., 1.));
	[_crosshair renderAtTime:time transformPosition:matrix];
}

- (void) renderLasersAtTime:(NSTimeInterval)time
{
	int					i;
	Laser*				laser;
	Matrix				m1, m2;
	Point4				pos, acc, center, dir;
	float				crosshairDistance = 70., norm;
		
	if (_isFiring || _crosshair) {
		glGetFloatv(GL_MODELVIEW_MATRIX, m1);
	}

	//Fire direction and position in ship referential
	center.x = center.y = center.z = 0.f;
	center.w =  1.f;
	dir.x = dir.y = 0.f;
	dir.z =  1.;
	dir.w =  1.f;
	
	//Converts fire position and direction to world referential
	dir = Point4TransformFromModelViewToModelView(dir, (MatrixPtr)_modelview, (MatrixPtr)m1);
	center = Point4TransformFromModelViewToModelView(center, (MatrixPtr)_modelview, (MatrixPtr)m1);
	TransposeMatrix_Copy((MatrixPtr)&_modelview, (MatrixPtr)&m2);

	_fireDirection = Point3Sub(*(Point3*)&dir, *(Point3*)&center);
	norm = Point3Norm(_fireDirection);
	_fireDirection =  Point3Mult(_fireDirection, 1./norm);
	_firePosition = *(Point3*)&center;

	//Position crosshair
	if (_mode == kSpaceShipModeNormal) {
		_crosshair.position = Point3Add(*(Point3*)&center, Point3Mult(_fireDirection, crosshairDistance));
	}

	//Render lasers
	[RenderingManager enablePtCache];
	for (i=CFArrayGetCount(_lasers)-1; i>=0; i--) {
		laser = (Laser*)CFArrayGetValueAtIndex(_lasers, i); 
		if (!laser.alive) {
			CFArrayRemoveValueAtIndex(_lasers, i);
		} else {
			if (laser.age == 0.) {		//If lasers has just been created
				//Transforms position from ship referential to world referential.
				*(Point3*)&pos = laser.position;
				pos.w = 1.;
				TransformPoint4_Copy((MatrixPtr)&m2, (Point4Ptr)&pos, (Point4Ptr)&pos);
				TransformPoint4_Copy((MatrixPtr)&m1, (Point4Ptr)&pos, (Point4Ptr)&pos);

				//Transforms acceleration from ship referential to world referential.
				*(Point3*)&acc = laser.acceleration;
				acc.w = 1.;				
				TransformPoint4_Copy((MatrixPtr)&m2, (Point4Ptr)&acc, (Point4Ptr)&acc);
				TransformPoint4_Copy((MatrixPtr)&m1, (Point4Ptr)&acc, (Point4Ptr)&acc);
				//*(Point3*)&acc = Point3Sub(*(Point3*)&acc, *(Point3*)&center);

				laser.acceleration = *(Point3*)&acc;
				laser.position = *(Point3*)&pos;
			}
			[laser renderAtTime:time];
		}
	}	
	[RenderingManager disablePtCache];
	[RenderingManager flushPtCache];

	if (CFArrayGetCount(_lasers) == 0) {
		_isFiring = NO;	
	}
}

- (void) renderMissilesAtTime:(NSTimeInterval)time
{
	NSInteger			i, count = CFArrayGetCount(_missiles);
#if !TARGET_IPHONE_SIMULATOR
	Sound*				sound;
#endif
	Matrix				m1, m2;
	Point4				pos, acc;
	Missile*			missile;
	BOOL				bomb = YES;

	//Render missile and smoke.
	if (count) {
		
		//We set the state once for all rendering.
		for (i=count-1; i>=0; i--) {
			missile = (Missile*)CFArrayGetValueAtIndex(_missiles, i); 
			if ((missile.age == 0.) && (_mode < kSpaceShipModeMissileInitiated)) {		//If lasers has just been created
				glGetFloatv(GL_MODELVIEW_MATRIX, m1);
				
				//Fire direction and position in ship referential
				TransposeMatrix_Copy((MatrixPtr)&_modelview, (MatrixPtr)&m2);

				//center = Point4TransformFromModelViewToModelView(center, (MatrixPtr)_modelview, (MatrixPtr)m1);

				//Transforms position from ship referential to world referential.
				*(Point3*)&pos = missile.position;
				pos.w = 1.;
				TransformPoint4_Copy((MatrixPtr)&m2, (Point4Ptr)&pos, (Point4Ptr)&pos);
				TransformPoint4_Copy((MatrixPtr)&m1, (Point4Ptr)&pos, (Point4Ptr)&pos);

				//Transforms acceleration from ship referential to world referential.
				*(Point3*)&acc = missile.acceleration;
				acc.w = 1.;				
				TransformPoint4_Copy((MatrixPtr)&m2, (Point4Ptr)&acc, (Point4Ptr)&acc);
				TransformPoint4_Copy((MatrixPtr)&m1, (Point4Ptr)&acc, (Point4Ptr)&acc);
				//*(Point3*)&acc = Point3Sub(*(Point3*)&acc, *(Point3*)&center);

				missile.acceleration = *(Point3*)&acc;
				missile.position = *(Point3*)&pos;
			}
			
			if (missile.alive) {
				[missile renderAtTime:time];
				bomb = NO;
			}
			else if(_mode != kSpaceShipModeMissileFired) {
				CFArrayRemoveValueAtIndex(_missiles, i);
			}
		}
		
		count = CFArrayGetCount(_missiles);
		for (i=count-1; i>=0; i--) {
			[(Missile*)CFArrayGetValueAtIndex(_missiles, i) renderSmokeAtTime:time];
		}
	}
	
	//If missiles all reached the mothership, fire the final bomb.
	if (_mode == kSpaceShipModeMissileFired) {
		if (bomb) {
			[self fireBomb];
			
	#if !TARGET_IPHONE_SIMULATOR
			//Play mothership explosion sound.
			sound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"mothership"];
			sound.position = Point3Zero;
			[sound play];
			
			//Fade out all sounds so that sound engine is silent when movie starts.
			[[SoundEngineManager sharedManager] fadeOut:1.6 delay:1.6];		
	#endif
			_mode = kSpaceShipModeBombCreated;
		}
	}
}

- (void) renderBombAtTime:(NSTimeInterval)time
{
	if (_bomb[0].alive && _bomb[1].alive) {		//Draw bomb if alive
		[_bomb[0] renderAtTime:time];
		[_bomb[1] renderAtTime:time];
	}
	else {										//If not, release the bomb and enter the "final" mode which will make final movie to play.
		CFRelease(_missiles);
		_missiles = nil;
		if (_bomb[0]) {
			[_bomb[0] release];
			_bomb[0] = nil;
		}
		if (_bomb[1]) {
			[_bomb[1] release];
			_bomb[1] = nil;
			_mode = kSpaceShipModeFinal;
		}	
	}
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
#if !TARGET_IPHONE_SIMULATOR
	Sound*			thrustSound;
#endif
	
	glPushMatrix(); {
		glRotatef(-90., 0.0, 0.0, 1.0);
		glRotatef(180., 0.0, 1.0, 0.0);
		glRotatef(_shipRotation.x, 1.0, 0.0, 0.0);
		glRotatef(_shipRotation.y, 0.0, 1.0, 0.0);
		glRotatef(_shipRotation.z, 0.0, 0.0, 1.0);
		
		//Ship
		glPushMatrix(); {
			if (_isFiring || _crosshair) {
				glGetFloatv(GL_MODELVIEW_MATRIX, _modelview);
			}
			
			glScalef(.06, .06, .06);
			//Handle ship position in end sequence, when missile are initiated.
			if (_mode == kSpaceShipModeMissileInitiated) {
				glTranslatef(0., _shipFallback, 0.);
				_shipFallback -= .05f;
			}
			//Handle ship position in end sequence, when missile are fired.
			else if (_mode >= kSpaceShipModeMissileFired) {
				glTranslatef(0., _shipFallback, 0.);
				glRotatef(_shipFallback * 4., 0., 0., 1.);
				_shipFallback += .25f;
			}
						
			//Render ship model.
			if (_type == kSpaceShipTypeDart) {
				SetOpenGLStates();
				RenderShip(_wingAngle);
				UnsetOpenGLStates();
			}
			else {
				SetOpenGLStates();
				RenderEnemyShip();
				UnsetOpenGLStates();
			}

			//Propusion fire
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			[_propulsionFire renderAtTime:time];
			glDepthMask(GL_TRUE);
			glDisable(GL_DEPTH_TEST);
			if ((_damage > 5.) && (_mode == kSpaceShipModeNormal)) {
				[_damages renderAtTime:time];
			}
		}
		glPopMatrix();
	}
	glPopMatrix();
	
#if !TARGET_IPHONE_SIMULATOR
	//Set thrust sound location and pitch from ship position.
	thrustSound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"thrusters"];
	thrustSound.position = Point3Make(0., 2.*_position.y, _position.z-3.5+6.*Norm(_position.x+0.4, _position.y, 0.0));
	thrustSound.pitch = Norm(_position.x, _position.y, 0.0)+1.;
	[thrustSound play];	
#endif
}

- (void) setMode:(SpaceShipMode)mode
{
	if (mode != _mode) {
		if (mode == kSpaceShipModeMissileInitiated) {
			//"Close" the wings.
			_wingDefaultAngle = kMissileWingAngle;
			[_crosshair release];
			_crosshair = nil;
		}
		
		_mode = mode;
	}
}

- (CFMutableArrayRef) lasers
{
	return ((_mode != kSpaceShipModeBombCreated) ? _lasers : (CFMutableArrayRef)[NSMutableArray arrayWithObjects:_bomb[0], _bomb[1], nil]);
}

- (void) updateWithGameObjectInfo:(ShipInfo*)info
{
	NSArray*			lasers = info.lasers,
						*missiles = info.missiles;
	Laser*				laser;
	Missile*			missile;
	NSUInteger			i, count;
	Point3				newPosition;
	float				alpha = 0.1;

	newPosition.x = info.shipPosition.x*alpha + (1.-alpha)*_shipPosition.x;
	newPosition.y = info.shipPosition.y*alpha + (1.-alpha)*_shipPosition.y;
	newPosition.z = info.shipPosition.z*alpha + (1.-alpha)*_shipPosition.z;

	[super updateWithGameObjectInfo:info];
	
	self.position = newPosition;
	_shipPosition = newPosition;
	
	//Lasers
	count = [lasers count];
	for (i=0; i<count; ++i) {
		if (i>=CFArrayGetCount(_lasers)) {
			[self fireLasersAtPosition:kSpaceShipLaserPosition_Left];
		}
		laser = (Laser*)CFArrayGetValueAtIndex(_lasers, i);
		[laser updateWithGameObjectInfo:[lasers objectAtIndex:i]];
	}
	
	//Missiles
	count = [missiles count];
	for (i=0; i<count; ++i) {
		if (i>=CFArrayGetCount(_missiles)) {
			[self fireMissileAtPosition:kSpaceShipLaserPosition_Left withTarget:nil];
		}
		missile = (Missile*)CFArrayGetValueAtIndex(_missiles, i);
		[missile updateWithGameObjectInfo:[missiles objectAtIndex:i]];
	}	
}

- (GameObject*) gameObjectInfo
{	
	ShipInfo*			info = [ShipInfo new];
	PhysicalObject*		bullet;
	NSMutableArray*		lasers = [NSMutableArray new],
						*missiles = [NSMutableArray new];
	NSUInteger			i, count = CFArrayGetCount(_lasers);
		
	[info updateWithGameObjectInfo:self];
	
	//Lasers
	for (i=0; i<count; ++i) {
		bullet = [PhysicalObject new];
		[bullet updateWithGameObjectInfo:(PhysicalObject*)CFArrayGetValueAtIndex(_lasers, i)];
		[lasers addObject:bullet];
		[bullet release];
	}
	info.lasers = lasers;
	[lasers release];

	//Missiles
	count = CFArrayGetCount(_missiles);
	for (i=0; i<count; ++i) {
		bullet = [PhysicalObject new];
		[bullet updateWithGameObjectInfo:(PhysicalObject*)CFArrayGetValueAtIndex(_missiles, i)];
		[missiles addObject:bullet];
		[bullet release];
	}
	info.missiles = missiles;
	[missiles release];
	
	info.shipPosition = _shipPosition;
	
	return [info autorelease];
}

@end

@implementation ShipInfo
@synthesize lasers=_lasers, missiles=_missiles, shipPosition=_shipPosition;

- (void)encodeWithCoder:(NSCoder *)aCoder
{
	NSData*		data;
	
	[super encodeWithCoder:aCoder];
	
	data = [NSData dataWithBytes:&_position length:sizeof(Point3)];
	[aCoder encodeObject:data forKey:@"shipPosition"];
	[aCoder encodeObject:_lasers forKey:@"lasers"];
	[aCoder encodeObject:_missiles forKey:@"missiles"];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
	const void*		bytes;

	if ((self = [super initWithCoder:aDecoder])) {
		_lasers = [[aDecoder decodeObjectForKey:@"lasers"] retain];
		_missiles = [[aDecoder decodeObjectForKey:@"missiles"] retain];
		bytes = [[aDecoder decodeObjectForKey:@"shipPosition"] bytes];
		_shipPosition = *(Point3*)bytes;
	}
	return self;
}

@end

