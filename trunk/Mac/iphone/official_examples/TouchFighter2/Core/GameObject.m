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

File: GameObject.m
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

#import "GameObject.h"
#import "Utils.h"

//CLASS IMPLEMENTATIONS:

@implementation GameObject

@synthesize	width=_width, height=_height, depth=_depth, rotation = _rotation, position = _position, scale = _scale;

- (id) init
{
	if ((self = [super init])) {
		//Initializes ivars.
		_rotation = _position = Point3Zero;
		_width = _height = _depth = _scale.x = _scale.y = _scale.z = 1.f;
		_previousRenderTime = -1.;
	}
	
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder
{
	NSData*		data;
	
	data = [NSData dataWithBytes:&_position length:sizeof(Point3)];
	[aCoder encodeObject:data forKey:@"position"];
	data = [NSData dataWithBytes:&_rotation length:sizeof(Point3)];
	[aCoder encodeObject:data forKey:@"rotation"];
	[aCoder encodeFloat:_width forKey:@"width"];
	[aCoder encodeFloat:_height forKey:@"height"];
	[aCoder encodeFloat:_depth forKey:@"depth"];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
	const void*		bytes;

	if ((self = [super init])) {
		bytes = [(NSData*)[aDecoder decodeObjectForKey:@"position"] bytes];
		_position = *(Point3*)bytes;
		bytes = [(NSData*)[aDecoder decodeObjectForKey:@"rotation"] bytes];
		_rotation = *(Point3*)bytes;
		_width = [aDecoder decodeFloatForKey:@"width"];
		_height = [aDecoder decodeFloatForKey:@"height"];
		_depth = [aDecoder decodeFloatForKey:@"depht"];
	}
	return self;
}

- (GameObject*) gameObjectInfo
{
	return self;
}

- (void) updateWithGameObjectInfo:(GameObject*)info
{
	self.position = info.position;
	self.rotation = info.rotation;
	self.width = info.width;	
	self.depth = info.depth;	
	self.height = info.height;	
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	//SUBCLASSES
}

- (void) renderAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix
{
	Matrix		m;
	Point3		position = _position;

	glPushMatrix(); {
		//If a transformation matrix is given, we transform the object position so that it is draw in that new referential (used for billboarding).
		if (matrix) {
			_transformedPosition = Point3Transform(position, matrix);
		}
		else {
			_transformedPosition = _position;
		}
		
		//We transform the modelview matrix to the one matching _transformedPosition, _rotation, and _scale.
		MakeTransformationMatrix(m, _rotation.x, _rotation.y, _rotation.z, _transformedPosition.x, _transformedPosition.y, _transformedPosition.z);
		glMultMatrixf(m);
		glScalef(_scale.x, _scale.y, _scale.z);
		
		//Render the object now.
		[self renderObjectAtTime:(NSTimeInterval)time];
	}
	glPopMatrix();
}

- (void) renderAtTime:(NSTimeInterval)time
{
	[self renderAtTime:time transformPosition:NULL];
}

// Trivial collision detection: finds if 2 boxes overlaps. Does not handle rotations
- (BOOL) collideWith:(id<CollisionDetection>)object tolerance:(float)tolerance
{
	CGFloat		dx, dy, dz;
	Point3		box = self.boundingBox,
				objectBox = object.boundingBox;
	
	dx = ABS(object.position.x - _position.x);
	dy = ABS(object.position.y - _position.y);
	dz = ABS(object.position.z - _position.z);

	return (dx - (box.x/2. + objectBox.x/2.) < tolerance) && (dy - (box.y/2. + objectBox.y/2.) < tolerance) && (dz - (box.z/2. + objectBox.z/2.) < tolerance);
}

- (BOOL) collideWith:(id<CollisionDetection>)object
{
	return [self collideWith:object tolerance:kCollisionTolerance];
}

- (Point3) boundingBox
{
	return Point3Make(_scale.x * _width, _scale.y * _height, _scale.x * _depth);
}

@end

@implementation GameObject(OpenGL)

static struct {
	GLuint texName;

	GLvoid *vertexPtr;
	GLvoid *texcoordPtr;
	GLvoid *normalPtr;
	GLvoid *colorPtr;
	
	BOOL useTexcoordArray;
	BOOL useNormArray;
} glState;

+ (void) bindTexture2D:(Texture2D *)tex
{
	if (tex.name != glState.texName) {
		glBindTexture(GL_TEXTURE_2D, tex.name);
		glState.texName = tex.name;
	}
}

+ (void) bindTextureName:(GLuint)name
{
	if (name != glState.texName) {
		glBindTexture(GL_TEXTURE_2D, name);
		glState.texName = name;
	}
}

+ (void) setVertexPointer:(GLvoid *)ptr size:(GLint)size type:(GLenum)type
{
	if (ptr != glState.vertexPtr) {
		glVertexPointer(size, type, 0, ptr);
		glState.vertexPtr = ptr;
	}
}

+ (void) setTexcoordPointer:(GLvoid *)ptr size:(GLint)size type:(GLenum)type
{
	if (ptr != glState.texcoordPtr) {
		glTexCoordPointer(size, type, 0, ptr);
		glState.texcoordPtr = ptr;
	}
}

+ (void) setNormalPointer:(GLvoid *)ptr type:(GLenum)type
{
	if (ptr != glState.normalPtr) {
		glNormalPointer(type, 0, ptr);
		glState.normalPtr = ptr;
	}
}

+ (void) setColorPointer:(GLvoid *)ptr size:(GLint)size type:(GLenum)type
{
	if (ptr != glState.colorPtr) {
		glColorPointer(size, type, 0, ptr);
		glState.colorPtr = ptr;
	}
}

@end