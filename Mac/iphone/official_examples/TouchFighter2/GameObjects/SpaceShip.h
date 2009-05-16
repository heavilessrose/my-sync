/*
File: SpaceShip.h
Abstract: Main spaceship.

Version: 2.0
*/

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "PhysicalObject.h"
#import "Maths.h"
#import "ParticleSystemObject.h"
#import <pthread.h>

#define kMissileWingAngle -40.

@class Texture2D, Missile, AnimationObject, Fire, SmokeTrail, Bomb, SpriteObject, ShipInfo;

// TYPEDEFS:

typedef enum _SpaceShipType {
	kSpaceShipTypeDart = 0,
	kSpaceShipTypeEnemy = 1,
	kSpaceShipTypeUnknown
} SpaceShipType;

typedef enum _SpaceShipLaserPosition {
	kSpaceShipLaserPosition_Left = 0,
	kSpaceShipLaserPosition_Right,
} SpaceShipLaserPosition;

typedef enum _SpaceShipMode {
	kSpaceShipModeNormal = 0,
	kSpaceShipModeMissileInitiated,
	kSpaceShipModeMissileFired,
	kSpaceShipModeBombCreated,
	kSpaceShipModeFinal
} SpaceShipMode;

//CLASS INTERFACES:

@interface SpaceShip : PhysicalObject {
@private
	SpaceShipType		_type;
	
	Texture2D*			_texture;
	
	//SpaceShip model and lasers
	Point3				_shipRotation,
						_shipPosition,
						_offset;
	Fire*				_propulsionFire;
	Fire*				_damages;
	CGFloat				_damage;

	//Crosshair and targetting
	SpriteObject*		_crosshair;
	Point3				_fireDirection,
						_firePosition;
	BOOL				_targetInFocus,
						_isFiring,
						_nextLasersAreTopLasers;
	GameObject*			_mothership;			//Not retained
	
	//Bullets
	CFMutableArrayRef	_lasers,
						_missiles;
	Bomb*				_bomb[2];

	//Wing effects
	CGFloat				_wingAngle,
						_wingDefaultAngle,
						_shipFallback;

	//Current mode of the spaceship (normal, final missile, ...)
	SpaceShipMode		_mode;

	//ModelView of the spaceship, used for converting lasers coordinates to the world referential
	Matrix				_modelview;
}

- (id) initWithType:(SpaceShipType)type;

/* Control */

- (void) fireLasersAtPosition:(SpaceShipLaserPosition)position;
- (void) fireMissileAtPosition:(SpaceShipLaserPosition)position withTarget:(GameObject*)target;
- (void) fireMissiles;
- (BOOL) willHit:(PhysicalObject*)object;

/* When being hit */

- (void) hitLeft;
- (void) hitRight;

/* Drawing */

- (void) renderCrosshairAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix;					//modelview needs to be CAMERA modelview for billboard rendering 
- (void) renderLasersAtTime:(NSTimeInterval)time;		
- (void) renderMissilesAtTime:(NSTimeInterval)time;
- (void) renderBombAtTime:(NSTimeInterval)time;

@property SpaceShipMode						mode;
@property BOOL								targetInFocus;
@property(readonly) CFMutableArrayRef		lasers;
@property(readonly) CFMutableArrayRef		missiles;
@property(retain) GameObject*				mothership;
@end

@interface ShipInfo	: PhysicalObject <NSCoding>
{
@public
	Point3			_shipPosition;
	NSArray*		_lasers;
	NSArray*		_missiles;
}
@property(retain)NSArray* lasers;
@property(retain)NSArray* missiles;
@property Point3 shipPosition;
@end
