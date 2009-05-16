/*
File: ParticleSystemObject.h
Abstract: Particle Systems.

Version: 2.0
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