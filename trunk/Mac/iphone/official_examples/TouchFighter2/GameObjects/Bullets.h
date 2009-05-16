/*
File: Bullets.h
Abstract: All the projectiles used in the game: lasers, missiles, bombs.

Version: 2.0
*/

#import "PhysicalObject.h"
#import "ParticleSystemObject.h"

@class SmokeTrail, AnimationObject;

typedef enum _MissileType {
	kMissileTypeLaser = 0,
	kMissileTypeMissile,
	kMissileTypeBomb,
	kMissileTypeCount
} MissileType;

/* Blue lasers on main SpaceShip */

@interface Laser : PhysicalObject {
@private
	Fire*				_fire;
	CGFloat				_age,
						_maxAge;
	Point3				_color;
	float				_tolerance;
}
@property(readonly) BOOL		alive;
@property CGFloat		age;
@end

/* Purple lasers from enemies */

@interface EnemyLaser : Fire {
@private
	CGFloat		_age,
				_maxAge;
	Point3		_targetDirection;
}
- (id) initWithPosition:(Point3)position impulsion:(Point3)impulsion;

@property(readonly) BOOL		alive;
@end

/* Missiles from main SpaceShip */

@interface Missile : PhysicalObject {
@private
	CGFloat				_age,
						_maxAge;
	Point3				_color,
						_rotationAxis;
	float				_tolerance,
						_rotationAngle;
	Point3				_growRate;
	GameObject*			_target;
	Fire*				_propulsionFire;
	SmokeTrail*			_smoke;
}
- (id) initWithImpulsion:(Point3Ptr)impulsion target:(GameObject*)target;
- (void) renderSmokeAtTime:(NSTimeInterval)time;

@property(readonly) BOOL		alive;
@property(readonly) CGFloat		age;
@end

/* Bomb for Mothership */

@interface Bomb : GameObject {
@private
	AnimationObject*	_fire;
	Point3				_growRate;
	CGFloat				_age,
						_maxAge;
	float				_tolerance;
}

- (id) initWithPosition:(Point3)position;

@property(readonly) BOOL		alive;
@property Point3		growRate;
@end