/*
File: GameObject.m
Abstract: Main game object abstract class.

Version: 2.0
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