/*
File: PhysicalObject.h
Abstract: A subclass of GameObject that describes an object with physical properties.

Version: 2.0
*/

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "GameObject.h"

@class Texture2D;

//CLASS INTERFACES:

/* A subclass of GameObject
 * Describe an object with a velocity, an acceleration, and friction */
 
@interface PhysicalObject : GameObject <NSCoding> {
@protected
	Point3			_velocity, _acceleration, _friction;
}

/* Dynamics */
@property Point3 velocity;
@property Point3 acceleration;
@property Point3 friction;
@end
