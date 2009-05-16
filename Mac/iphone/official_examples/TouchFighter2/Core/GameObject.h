/*
File: GameObject.h
Abstract: Main game object abstract class.

Version: 2.0
*/

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import "TouchFighter_Types.h"

@class Texture2D;

//CONSTANTS:

#define kCollisionTolerance 0.1

//PROTOCOLS:

@protocol CollisionDetection
@required
- (BOOL) collideWith:(id<CollisionDetection>)object;

@property(readonly) Point3 position;
@property(readonly) Point3 boundingBox;
@end

//CLASS INTERFACES:

/* Main game object abstract class.
 * Describe an object with a location, an orientation, dimension and scale */
 
@interface GameObject : NSObject <CollisionDetection, NSCoding> {
@protected
	Point3			_rotation,
					_position,
					_scale;
	CGFloat			_width, _height, _depth;
	NSTimeInterval	_previousRenderTime;
	
	Point3			_transformedPosition;				//Position at where the drawing will occur (can be the same or different from _position)
}

/* Function that renders the object */
- (void) renderAtTime:(NSTimeInterval)time;				//Requires GL_VERTEX_ARRAY state to be enabled

/* Orientation and Position */
@property Point3 rotation;
@property Point3 position;

/* Dimension and Scale */
@property Point3 scale;
@property CGFloat width;
@property CGFloat depth;
@property CGFloat height;

/* Bounding box for collision */
@property(readonly) Point3 boundingBox;

/* Necessary information to communicate in multiplayer configurations */
- (GameObject*) gameObjectInfo;
- (void) updateWithGameObjectInfo:(GameObject*)info;
@end

/* For Subclasses */
@interface GameObject(Customization)
- (void) renderObjectAtTime:(NSTimeInterval)time;
@end

/* Fast OpenGL state management
 * States are cached to minimise state changes sent to OpenGL framework */
 
@interface GameObject(OpenGL)
+ (void) bindTexture2D:(Texture2D*)tex;
+ (void) bindTextureName:(GLuint)name;
+ (void) setVertexPointer:(GLvoid *)ptr size:(GLint)size type:(GLenum)type;
+ (void) setTexcoordPointer:(GLvoid *)ptr size:(GLint)size type:(GLenum)type;
+ (void) setNormalPointer:(GLvoid *)ptr type:(GLenum)type;
+ (void) setColorPointer:(GLvoid *)ptr size:(GLint)size type:(GLenum)type;
@end
