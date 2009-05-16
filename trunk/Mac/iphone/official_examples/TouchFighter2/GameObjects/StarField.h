/*
File: Starfield.h
Abstract: Warping starfied.

Version: 2.0
*/

#import "PhysicalObject.h"

@class Texture2D;

//CLASS INTERFACE:

/* Warping starfied. */

//Note: This class could be easily made a subclass of ParticleSystemObject, and benefit from particle caching and dual point sprites/triangle strips rendering.
@interface StarField : GameObject {
@private
	void*			_particleSystem;
	CGFloat			_particleDirection[3],
					_gravityPoint[3];
	CGFloat*		_particleRotations;
	
}
- (id) initWithGravityPoint:(Point3)gp texture:(Texture2D*)texture count:(long)count;

- (void) setDirectionX:(GLfloat)x Y:(GLfloat)y Z:(GLfloat)z;
@end