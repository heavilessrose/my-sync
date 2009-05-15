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

File: ParticleSystemObject.h
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

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import "PhysicalObject.h"

@class Texture2D, RenderingManager, ParticleSystemObject, Fire;

//STRUCTURES

typedef enum _ParticleType {
	kParticlePointSprite = 0,
	kParticleTriangleStrip = 1	
} ParticleType;

typedef struct _Size3 {
	CGFloat		width, height, depth;
} Size3;

typedef struct _Cube3 {
	Point3		origin;
	Size3		size;
} Cube3;

typedef struct _Generator {
	Cube3		position,
				velocity;
} Generator;

//CONSTANTS:

extern const Generator GeneratorZero;

//FUNCTIONS

extern Cube3 Cube3Make(CGFloat x, CGFloat y, CGFloat z, CGFloat width, CGFloat height, CGFloat depth);

//CLASS INTERFACES:

/* The Rendering Manager will concatenate particle systems as much as possible, 
 * so that the fewer draw calls are made 
 * e.g. for lasers, only one draw call is made even it many lasers */
 
@interface RenderingManager : NSObject
- (void) render:(Fire*)system;
@end

@interface RenderingManager(Cache)

/* Initialize the particle cache */
+ (void) initPtCache;

/* Turn particle caching on for all particle system to be drawn form now on */
+ (void) enablePtCache;

/* Turn particle caching off */
+ (void) disablePtCache;

/* Draw all cached particle systems so far */
+ (void) flushPtCache;
@end

/* A class that describes a system of independant points or quads with acceleration, velocity, applied force. */

@interface ParticleSystemObject : PhysicalObject
{
@protected
	Texture2D*		_texture;
	Point3*			_particleRotations,
					*_particlePositions, *_particleVelocities, *_particleAccelerations;
	Point3			_systemVelocity, _systemAcceleration;
	NSUInteger		_count;
	Generator		_generator;
}

- (id) initWithTexture:(Texture2D*)texture generator:(Generator)generator count:(NSUInteger)count;

/* By defaults particle are initialized at zero.
 * Use that function to generate their position uniformly in generator */
- (void) initializeParticles:(Generator)generator;

/* Velocity of the particle system (i.e. applied to all particles in the system) */
@property Point3 systemVelocity;

/* Acceleration of the particle system (i.e. applied to all particles in the system) */
@property Point3 systemAcceleration;

//Internal properties:
@property(readonly) NSUInteger		_count;
@property(readonly) Point3*			_particlePositions;
@property(readonly) Texture2D*		_texture;
@property(readonly) Point3  		_transformedPosition;
@end

/* A particle system that simulates fire. 
 * Supports rendering as point sprite or triangle strips.
 * When rendering is triangle strips, client is responsible of billboarding if needed
 * Point sprites have a max size of 32 */
 
@interface Fire : ParticleSystemObject
{
@protected
	Point4*					_particleColors,
							_systemColor;
	Point3					_attenuation;
	CGFloat					_radius, _particleDecay;
	ParticleType			_type;
	
	RenderingManager*		_renderingManager;
}

- (id) initWithTexture:(Texture2D*)texture generator:(Generator)generator type:(ParticleType)type count:(NSUInteger)count;

/* Color of the particle system (i.e. applied to all particles in the system) */
@property Point4	systemColor;

/* Describes how wide the system is */
@property CGFloat	radius;

/* Describes how fast the particle alpha decreases (depends on radius) */
@property CGFloat	particleDecay;

/* Point sprite attenuation coefficients */
@property Point3	attenuation;

//Internal properties:
@property(readonly) Point4*			_particleColors;
@property(readonly) ParticleType	_type;
@end