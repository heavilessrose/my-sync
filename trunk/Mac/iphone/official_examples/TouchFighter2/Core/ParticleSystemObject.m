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

File: ParticleSystemObject.m
Abstract: Particle Systems.

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

#import "ParticleSystemObject.h"
#import "GameObject_Private.h"
#import "Utils.h"

//CONSTANTS:

const Generator GeneratorZero = (Generator){{{0.,0.,0.}, {0.,0.,0.}}, {{0.,0.,0.}, {0.,0.,0.}}};

//FUNCTIONS:

Cube3 Cube3Make(CGFloat x, CGFloat y, CGFloat z, CGFloat width, CGFloat height, CGFloat depth) {
	return (Cube3) {{x, y, z}, {width, height, depth}};
}

//CACHE GLOBAL:

static struct {
	BOOL				enabled;
	GLuint				texture;
	NSUInteger			used,
						capacity,
						psCapacity;
	Point3*				vertices,
						attenuation;
	Point4*				colors;
	CGPoint*			texcoords;
	GLushort*			indices;
	ParticleType		type;
} _ptCache;

//CLASS IMPLEMENTATIONS:

@implementation ParticleSystemObject
@synthesize systemVelocity=_systemVelocity, systemAcceleration=_systemAcceleration;
@synthesize _count, _particlePositions, _texture, _transformedPosition;

- (id) initWithTexture:(Texture2D*)texture generator:(Generator)generator count:(NSUInteger)count
{
	if ((self = [super init])) {
		//Initializes ivars.
		_texture = [texture retain];
		_particleRotations = calloc(count, sizeof(Point3));
		_particlePositions = calloc(count, sizeof(Point3));
		_particleVelocities = calloc(count, sizeof(Point3));
		_particleAccelerations = calloc(count, sizeof(Point3)); 
		_systemAcceleration = _systemVelocity = Point3Zero;
		_count = count;		
		_generator = generator;
	}
	
	return self;
}

- (void) dealloc
{
	[_texture release];
	free(_particleRotations);
	free(_particlePositions);
	free(_particleVelocities);	
	free(_particleAccelerations);	

	[super dealloc];
}

- (void) _generateNewParticleAtIndex:(NSUInteger)index generator:(Generator)generator
{
	_particlePositions[index] = Point3Make(Random(generator.position.origin.x, generator.position.origin.x + generator.position.size.width),
											Random(generator.position.origin.y, generator.position.origin.y + generator.position.size.height),
											Random(generator.position.origin.z, generator.position.origin.z + generator.position.size.depth));

	_particleVelocities[index] = Point3Make(Random(generator.velocity.origin.x, generator.velocity.origin.x + generator.velocity.size.width) + _systemVelocity.x,
											Random(generator.velocity.origin.y, generator.velocity.origin.y + generator.velocity.size.height) + _systemVelocity.y,
											Random(generator.velocity.origin.z, generator.velocity.origin.z + generator.velocity.size.depth) + _systemVelocity.z);

	if (!Point3EqualsPoint3(_systemAcceleration, Point3Zero)) {
		_particleAccelerations[index] = _systemAcceleration;
	}
}

- (void) _generateNewParticleAtIndex:(NSUInteger)index
{
	[self _generateNewParticleAtIndex:index generator:_generator];	
}

- (void) initializeParticles:(Generator)generator
{
	NSUInteger		i;
	CGFloat			x;
	
	for (i=0; i<_count; ++i) {
		x = (CGFloat)i/_count;
		_particlePositions[i] = Point3Make(generator.position.origin.x + x * generator.position.size.width,
											generator.position.origin.y + x * generator.position.size.height,
											generator.position.origin.z + x * generator.position.size.depth);

		_particleVelocities[i] = Point3Make(generator.velocity.origin.x + x * generator.velocity.size.width + _systemVelocity.x,
											 generator.velocity.origin.y + x * generator.velocity.size.height + _systemVelocity.y,
											 generator.velocity.origin.z + x * generator.velocity.size.depth + _systemVelocity.z);
			
		if (!Point3EqualsPoint3(_systemAcceleration, Point3Zero)) {
			_particleAccelerations[i] = _systemAcceleration;
		}
	}
}

- (void) setSystemAcceleration:(Point3)acceleration
{
	NSUInteger		i;
	
	if (!Point3EqualsPoint3(_systemAcceleration, acceleration)) {
		for (i=0; i<_count; ++i)
		_particleAccelerations[i] = acceleration;

		_systemAcceleration = acceleration;
	}
}

- (void) setSystemVelocity:(Point3)velocity
{
	NSUInteger		i;
	
	if (!Point3EqualsPoint3(_systemVelocity, velocity)) {
		for (i=0; i<_count; ++i)
		_particleVelocities[i] = velocity;

		_systemVelocity = velocity;
	}
}

- (void) _updateParticlePositionsAtTime:(NSTimeInterval) time
{
	NSUInteger			i;
	
	if (_previousRenderTime < 0.) {
		_previousRenderTime = time;
	}
	
	for (i=0; i<_count; ++i) {
		_particleVelocities[i] = Point3Add(_particleVelocities[i], Point3Mult(_particleAccelerations[i], time - _previousRenderTime));
		_particlePositions[i] = Point3Add(_particlePositions[i], Point3Mult(_particleVelocities[i], time - _previousRenderTime));
	}
}

- (void) renderParticlesAtTime:(NSTimeInterval)time
{
	//Subclasses
}

- (void) updatePositionAtTime:(NSTimeInterval)time
{
	[self _updateParticlePositionsAtTime:time];
	[super updatePositionAtTime:time];
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	[self renderParticlesAtTime:time];
}

@end

@implementation Fire
@synthesize radius=_radius, particleDecay=_particleDecay, systemColor=_systemColor, attenuation=_attenuation;
@synthesize _particleColors, _type;

- (id) initWithTexture:(Texture2D*)texture generator:(Generator)generator type:(ParticleType)type count:(NSUInteger)count
{
	if ((self = [super initWithTexture:texture generator:generator count:count])) {
		_particleColors = calloc(count, sizeof(Point4));
		_systemColor = Point4Make(1.0, 1.0, 0.5, 1.0);
		_attenuation = Point3Make(0., 1., 0.0);
		_radius = 1.;
		_particleDecay = 1.;
		_type = type;
		
		_renderingManager = [RenderingManager new];
	}
	
	return self;
}

- (void) dealloc
{
	[(id)_renderingManager release];

	free(_particleColors);

	[super dealloc];
}

- (void) _updateParticlePositionsAtTime:(NSTimeInterval)time
{
	NSUInteger			i;
	CGFloat				norm;
	Point3				positionOffset;
	
	[super _updateParticlePositionsAtTime:time];
	
	for (i=0; i<_count; ++i) {
		norm = Point3Norm(_particlePositions[i]);
		
		if (norm > _radius) {
			positionOffset = Point3Mult(_particlePositions[i], 1. / Point3Norm(_particlePositions[i]) * (norm  - (int)(norm / _radius) * _radius));
			[self _generateNewParticleAtIndex:i];
			_particlePositions[i] = Point3Add(_particlePositions[i], positionOffset);
			norm = Point3Norm(_particlePositions[i]);
		}
		_particleColors[i] = Point4Mult(_systemColor, 1. - pow(MAX(norm - _radius/5., 0.) / _radius, 1./_particleDecay));
		_particleColors[i].w = 1.0;				
	}
}

- (void) renderParticlesAtTime:(NSTimeInterval)time
{
	[_renderingManager render:self];
}

@end

@implementation RenderingManager

/* We allow to cache particle drawing, so that multiple particle systems are drawn via one draw call only for better performances 
   NOTE: Such cache is obviously not thread-safe in its current implementation */

+ (void) initPtCache
{
	//Initialize all variables needed for caching point sprites or triangle strips
	_ptCache.enabled = NO;
	_ptCache.texture = 0;
	_ptCache.used = 0;
	_ptCache.capacity = 200;
	_ptCache.vertices = (Point3*) malloc(200 * sizeof(Point3));
	_ptCache.colors = (Point4*) malloc(200 * sizeof(Point4));
	_ptCache.indices = (GLushort*) malloc(300 * sizeof(GLushort));
	_ptCache.texcoords = (CGPoint*) malloc(200 * sizeof(CGPoint));
	_ptCache.type = kParticlePointSprite;
}

+ (void) enablePtCache
{
	_ptCache.enabled = YES;	
}

+ (void) disablePtCache
{
	_ptCache.enabled = NO;
}

/* Grow the cache if needed */
+ (void) growPtCache:(NSUInteger)count
{
	_ptCache.capacity += ceilf((CGFloat)MAX(_ptCache.capacity, count) / 4.) * 4;			//Always a multiple of 4 for triangle strips
	_ptCache.vertices = (Point3*) realloc(_ptCache.vertices, _ptCache.capacity * sizeof(Point3));
	_ptCache.colors = (Point4*) realloc(_ptCache.colors, _ptCache.capacity * sizeof(Point4));
	_ptCache.texcoords = (CGPoint*) realloc(_ptCache.texcoords, _ptCache.capacity * sizeof(CGPoint));		
	_ptCache.indices = (GLushort*) realloc(_ptCache.indices, 6 / 4 * _ptCache.capacity * sizeof(GLushort));
}

+ (void) flushPtCache
{
	if (!_ptCache.used) {
		return;
	}
	
	//Setup
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	[GameObject bindTextureName:_ptCache.texture];
	
	glEnable(GL_TEXTURE_2D);
	[GameObject setVertexPointer:_ptCache.vertices size:3 type:GL_FLOAT];
	glEnableClientState(GL_COLOR_ARRAY);
	[GameObject setColorPointer:_ptCache.colors size:4 type:GL_FLOAT];
	
	if (_ptCache.type == kParticlePointSprite) {
		glEnable(GL_POINT_SPRITE_OES);
		glPointParameterf(GL_POINT_SIZE_MAX, 32);
		glPointSize(32);
		
		glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, (GLfloat*)&_ptCache.attenuation);
		
		glTexEnvf(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);	
		
		//Drawing
		glDrawArrays(GL_POINTS, 0, _ptCache.used);

		//Clean-up
		glTexEnvf(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_FALSE);
		glDisable(GL_POINT_SPRITE_OES);
	}
	else {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		[GameObject setTexcoordPointer:_ptCache.texcoords size:2 type:GL_FLOAT];
				
		//Drawing
		glDrawElements(GL_TRIANGLES, (_ptCache.used / 4) * 6, GL_UNSIGNED_SHORT, _ptCache.indices);
		
		//Clean-up
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glDisableClientState(GL_COLOR_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
	
	glDisable(GL_BLEND);

	_ptCache.used = 0;
	_ptCache.texture = 0;
}

- (void) render:(Fire*)system
{	
	NSUInteger			i, count = system._count, usedIndices;
	Texture2D*			texture = system._texture;
	Point3				vertices[count * 4],
						vx = Point3Make(0.5, 0., 0.),
						vy = Point3Make(0., 0.5, 0.),
						*particlePositions = system._particlePositions,
						systemPosition = system._transformedPosition,	//position where the drawing will occur
						systemScale = system.scale,
						attenuation = system.attenuation;
	Point4				colors[count * 4],
						*particleColors = system._particleColors;
	CGPoint				texcoord[count * 4];
	ParticleType		type = system._type;
	GLushort			indices[count * 6];

/* In the case we're caching particles */
	if (_ptCache.enabled && count) {
		//Flush on texture change, point attenuation change or different type
		if ((_ptCache.texture && _ptCache.texture != texture.name) || ((type == kParticlePointSprite) && !Point3EqualsPoint3(_ptCache.attenuation, attenuation))  || (_ptCache.type != type)) {
			[RenderingManager flushPtCache];
		}
		_ptCache.texture = texture.name;
		_ptCache.attenuation = attenuation;
		_ptCache.type = type;

		// In the case of point Animation particles, simply copy from system to cache
		if (type == kParticlePointSprite) {							
			//Grow if necessary
			while (_ptCache.capacity - _ptCache.used < count)
			[[self class] growPtCache:count];

			memcpy(&_ptCache.vertices[_ptCache.used], particlePositions, count * sizeof(Point3));
			
			// We need to translate all coordinates of the system position, as all particles in cache needs to be in same referential
			for (i=0; i<count; ++i)
			_ptCache.vertices[_ptCache.used + i] = Point3Add(_ptCache.vertices[_ptCache.used + i], systemPosition);

			memcpy(&_ptCache.colors[_ptCache.used], particleColors, count * sizeof(Point4));
			
			_ptCache.used += count;													  
		}
		// In the case of triangle strips particles, we need to generate quads for each positions.
		else {
			//Grow if necessary
			while (_ptCache.capacity - _ptCache.used < 4*count)
			[[self class] growPtCache:4*count];

			usedIndices = _ptCache.used / 4 * 6;
			for (i=0; i<count; ++i) {
				// We need to translate all coordinates of the system position, as all particles in cache needs to be in same referential
				_ptCache.vertices[_ptCache.used + i * 4] = Point3Make((particlePositions[i].x - vx.x - vy.x) * systemScale.x + systemPosition.x, (particlePositions[i].y - vx.y - vy.y) * systemScale.y + systemPosition.y, (particlePositions[i].z - vx.z - vy.z) * systemScale.z + systemPosition.z);
				_ptCache.vertices[_ptCache.used + i * 4+1] = Point3Make((particlePositions[i].x + vx.x - vy.x) * systemScale.x + systemPosition.x, (particlePositions[i].y + vx.y - vy.y) * systemScale.y + systemPosition.y, (particlePositions[i].z + vx.z - vy.z) * systemScale.z + systemPosition.z);
				_ptCache.vertices[_ptCache.used + i * 4+2] = Point3Make((particlePositions[i].x  - vx.x + vy.x) * systemScale.x + systemPosition.x, (particlePositions[i].y - vx.y + vy.y) * systemScale.y + systemPosition.y, (particlePositions[i].z - vx.z + vy.z) * systemScale.z + systemPosition.z);
				_ptCache.vertices[_ptCache.used + i * 4+3] = Point3Make((particlePositions[i].x + vx.x + vy.x) * systemScale.x + systemPosition.x, (particlePositions[i].y + vx.y + vy.y) * systemScale.y + systemPosition.y, (particlePositions[i].z + vx.z + vy.z) * systemScale.z + systemPosition.z);

				_ptCache.texcoords[_ptCache.used + i * 4] = CGPointMake(0., texture.maxT);
				_ptCache.texcoords[_ptCache.used + i * 4+1] = CGPointMake(texture.maxS, texture.maxT);
				_ptCache.texcoords[_ptCache.used + i * 4+2] = CGPointMake(0., 0.);
				_ptCache.texcoords[_ptCache.used + i * 4+3] =  CGPointMake(texture.maxS, 0.);

				_ptCache.indices[usedIndices + i * 6] = _ptCache.used + i * 4;
				_ptCache.indices[usedIndices + i * 6+1] = _ptCache.used + i * 4+1;
				_ptCache.indices[usedIndices + i * 6+2] = _ptCache.used + i * 4+2;
				_ptCache.indices[usedIndices + i * 6+3] = _ptCache.used + i * 4+1;
				_ptCache.indices[usedIndices + i * 6+4] = _ptCache.used + i * 4+2;
				_ptCache.indices[usedIndices + i * 6+5] = _ptCache.used + i * 4+3;
			
				_ptCache.colors[_ptCache.used + i * 4] = _ptCache.colors[_ptCache.used + i * 4+1] = _ptCache.colors[_ptCache.used + i * 4+2] = _ptCache.colors[_ptCache.used + i * 4+3] = particleColors[i];
			}
			_ptCache.used += 4*count;													  
		}				
	}
/* Without cache, we're drawing directly point sprites*/
	else if (type == kParticlePointSprite) {
		//Set-up
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_POINT_SPRITE_OES);
		glEnable(GL_TEXTURE_2D);
		
		[GameObject setVertexPointer:particlePositions size:3 type:GL_FLOAT];
		glEnableClientState(GL_COLOR_ARRAY);
		[GameObject setColorPointer:particleColors size:4 type:GL_FLOAT];
		
		glPointParameterf(GL_POINT_SIZE_MAX, 32);
		glPointSize(32);
		
		glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, (GLfloat*)&attenuation);
		
		glTexEnvf(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);	
		
		//Drawing
		[GameObject bindTexture2D:texture];
		glDrawArrays(GL_POINTS, 0, count);
		
		//Clean-up
		glDisableClientState(GL_COLOR_ARRAY);

		glTexEnvf(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_FALSE);
		glDisable(GL_POINT_SPRITE_OES);
		glDisable(GL_TEXTURE_2D);
		
		glDisable(GL_BLEND);
	}
/* Without cache, we're generating and drawing triangle strip vertices and tex coordinates */
	else {
		for (i=0; i<count; ++i) {
			vertices[i*4] = Point3Make(particlePositions[i].x - vx.x - vy.x, particlePositions[i].y - vx.y - vy.y, particlePositions[i].z - vx.z - vy.z);
			vertices[i*4+1] = Point3Make(particlePositions[i].x + vx.x - vy.x, particlePositions[i].y + vx.y - vy.y, particlePositions[i].z + vx.z - vy.z);
			vertices[i*4+2] = Point3Make(particlePositions[i].x - vx.x + vy.x, particlePositions[i].y - vx.y + vy.y, particlePositions[i].z - vx.z + vy.z);
			vertices[i*4+3] = Point3Make(particlePositions[i].x + vx.x + vy.x, particlePositions[i].y + vx.y + vy.y, particlePositions[i].z + vx.z + vy.z);

			texcoord[i*4] = CGPointMake(0., texture.maxT);
			texcoord[i*4+1] = CGPointMake(texture.maxS, texture.maxT);
			texcoord[i*4+2] = CGPointMake(0., 0.);
			texcoord[i*4+3] =  CGPointMake(texture.maxS, 0.);

			indices[i*6] = i*4 + 0;
			indices[i*6+1] = i*4 + 1;
			indices[i*6+2] = i*4 + 2;
			indices[i*6+3] = i*4 + 1;
			indices[i*6+4] = i*4 + 2;
			indices[i*6+5] = i*4 + 3;
			
			colors[i*4] = colors[i*4+1] = colors[i*4+2] = colors[i*4+3] = particleColors[i];
		}
	
		//Set-up
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		[GameObject setVertexPointer:vertices size:3 type:GL_FLOAT];
		[GameObject setTexcoordPointer:texcoord size:2 type:GL_FLOAT];

		glEnableClientState(GL_COLOR_ARRAY);
		[GameObject setColorPointer:colors size:4 type:GL_FLOAT];
				
		//Drawing
		[GameObject bindTexture2D:texture];
		glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_SHORT, indices);
		
		//Clean-up
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);	
		
		glDisable(GL_BLEND);		
	}
}

@end