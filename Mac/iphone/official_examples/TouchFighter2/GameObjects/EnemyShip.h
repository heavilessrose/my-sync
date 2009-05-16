/*
File: EnemyShip.h
Abstract: Enemy ships (darts and mothership).

Version: 2.0
*/

#import "PhysicalObject.h"
#import "Maths.h"

@class Fire, EnemyLaser;

@interface EnemyShip : PhysicalObject {
@private
	Texture2D*			_texture;			//Not retained
	Matrix				_modelview;
	BOOL				_highlight;
	int					_numTextures;
	CGFloat				_damage;
	
	EnemyLaser*			_laser;
	Fire*				_fire,
						*_flames;
						
	NSTimeInterval		_lastChangedTime;
}

/* Control */

- (void) fireAtPointX:(GLfloat)x Y:(GLfloat)y Z:(GLfloat)z;

/* Drawing */

- (void) renderHighlight;
- (void) renderLasersAtTime:(NSTimeInterval)time;
- (void) renderDamageFlamesAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix;
- (void) renderDamageFireAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix;

@property BOOL						highlight;
@property (readonly) EnemyLaser*	laser;
@property CGFloat					damage;
@end

@interface MotherShip : GameObject
@property(readonly, getter=bayOffset) Point3 bayOffset;
@end