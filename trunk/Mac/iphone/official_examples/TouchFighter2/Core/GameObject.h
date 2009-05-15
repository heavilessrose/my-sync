/*

===== IMPORTANT =====

This is sample code demonstrating API, technology or techniques in development.
Although this sample code has been reviewed for technical accuracy, it is not
final. Apple is supplying this information to help you plan for the adoption of
the technologies and programming interfaces described herein. This information
is subject to change, and software implemented based on this sample code should
be tested with final operating system software and final documentation. Newer
versions of this sample code may be provided with future seeds of the API or
technology. For information about updates to this and other developer
documentation, view the New & Updated sidebars in subsequent documentation
seeds.

=====================

File: GameObject.h
Abstract: Main game object abstract class.

Version: 2.0

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

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
