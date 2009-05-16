/*
File: Starfield.m
Abstract: Warping starfied.

Version: 2.0
*/

#import "StarField.h"
#import "GameObject_Private.h"
#import "Utils.h"

//CONSTANTS:
#define kTexWidth				100.0
#define kTexHeight				1.5
#define VERTEX_COORD_INDEX(X)	(X*12)
#define TEX_COORD_INDEX(X)		(X*8)
#define	kStartRotation			M_PI/2. + M_PI/4.

struct _Stars {
	Texture2D*		_texture;
	CGFloat*		_particleVertices;
	long			_particleCount,
					_vertexCount;
	GLushort*		_particleIndices;
};

typedef struct _Stars Stars;
typedef Stars* StarsRef;

//FUNCTIONS:

static void GetLocation(CGFloat* pv, long index, CGFloat position[3])
{
	position[0] = (pv[0] + pv[9]) / 2.;
	position[1] = (pv[1] + pv[10]) / 2.;
	position[2] = (pv[2] + pv[11]) / 2.;
}

static void SetNewParticleTriangleStrip(CGFloat* pv, CGFloat position[3], CGFloat rotation[3], CGFloat size[2])
{
	Point3		vx = (Point3){size[0]/2., 0.0, 0.},
				vy = (Point3){0.0, size[1]/2., 0.};
	
	if (rotation) {
		MakeRotationPoint3((Point3Ptr)&vx, 0.0, RadiansToDegrees(-rotation[0]), 0.0);
		MakeRotationPoint3((Point3Ptr)&vy, 0.0, 0.0, RadiansToDegrees(M_PI/2.+rotation[1]));
	}
	
	pv[0] = position[0] - vx.x - vy.x;
	pv[1] = position[1] - vx.y - vy.y;
	pv[2] = position[2] - vx.z - vy.z;
	
	pv[3] = position[0] + vx.x - vy.x;
	pv[4] = position[1] + vx.y - vy.y;
	pv[5] = position[2] + vx.z - vy.z;
	
	pv[6] = position[0] - vx.x + vy.x;
	pv[7] = position[1] - vx.y + vy.y;
	pv[8] = position[2] - vx.z + vy.z;
	
	pv[9] = position[0] + vx.x + vy.x;
	pv[10] = position[1] + vx.y + vy.y;
	pv[11] = position[2] + vx.z + vy.z;
}

static void SetNewParticle(StarsRef system, long index, BOOL back, CGFloat* rotation)
{
	long		vertexIndex, texIndex;
	CGFloat		position[3], 
				size[2],
				angle,
				radius;

	size[0] = (CGFloat)system->_texture.pixelsWide * system->_texture.maxS/system->_texture.pixelsWide * kTexWidth,
	size[1] = (CGFloat)system->_texture.pixelsHigh * system->_texture.maxT/system->_texture.pixelsWide * kTexHeight;
	
	vertexIndex = VERTEX_COORD_INDEX(index);
	texIndex = VERTEX_COORD_INDEX(system->_particleCount) + TEX_COORD_INDEX(index);
	
	angle = Random(0., M_PI);
	radius = Random(400., 600.);
	position[2] = (back ? -100. - radius * sin(angle):  Random(-800., 0));
	position[0] = - 200.;
	position[1] = radius * cos(angle);
	rotation[0] = kStartRotation; 
	rotation[1] = angle;
	rotation[2] = 0.0;

	SetNewParticleTriangleStrip(&(system->_particleVertices[vertexIndex]), position, rotation,  size);
	
	system->_particleVertices[texIndex] = 0;
	system->_particleVertices[texIndex+1] = system->_texture.maxT;
	system->_particleVertices[texIndex+2] = system->_texture.maxS;
	system->_particleVertices[texIndex+3] = system->_texture.maxT;
	system->_particleVertices[texIndex+4] = 0;
	system->_particleVertices[texIndex+5] = 0;
	system->_particleVertices[texIndex+6] = system->_texture.maxS;;
	system->_particleVertices[texIndex+7] = 0;
}

void SetNewPosition(CGFloat* v, CGFloat size, CGFloat texsize[2])
{
	CGFloat			position[3] = {0., 0., 0.};
	
	if (size) {
		position[0] = Random(-1., 1.) * size;
		position[1] = Random(-1., 1.) * size;
		position[2] = Random(-1., 1.) * size;
	}
	
	SetNewParticleTriangleStrip(v, position, NULL, texsize);
}

void SetNewVelocity(Point3Ptr v, CGFloat speed)
{
	v->x = Random(-1., 1.) * speed;
	v->y = Random(-1., 1.) * speed;
	v->z = Random(-1., 1.) * speed;
}

StarsRef CreateSystem(Texture2D* texture, long count)
{
	long				i;
	StarsRef		system = malloc (sizeof(Stars));
	CGFloat				rotation[3] = {0.0, 0.0, 0.0};

	system->_particleCount = count;
	system->_texture = [texture retain];	
			
	system->_vertexCount = count*4;
	system->_particleVertices = malloc (sizeof(CGFloat) * count * 20);		//HACK: The tex coords are at the end of the vertex coord array	
	system->_particleIndices = malloc (sizeof(GLushort) * count * 6);
	
	for (i=0; i<count; ++i) {
		system->_particleIndices[i*6+0] = i*4 + 0;
		system->_particleIndices[i*6+1] = i*4 + 1;
		system->_particleIndices[i*6+2] = i*4 + 2;
		system->_particleIndices[i*6+3] = i*4 + 1;
		system->_particleIndices[i*6+4] = i*4 + 2;
		system->_particleIndices[i*6+5] = i*4 + 3;
	}
	
	for (i=0; i<count; ++i)
	SetNewParticle(system, i, NO, rotation);

	return system;
}

void ReleaseSystem(StarsRef system)
{
	free(system->_particleVertices);
	[system->_texture release];
	
	free(system);
}

//CLASS IMPLEMENTATIONS:

@implementation StarField

- (id) initWithGravityPoint:(Point3)gp texture:(Texture2D*)texture count:(long)count
{
	unsigned int i;
	
	self = [super init];

	if (self) {
		//Initialize system.
		_particleSystem = CreateSystem(texture, count);
		_particleRotations = (CGFloat*)malloc (3*count*sizeof(CGFloat));
		for (i = 0; i<count; ++i) {
			_particleRotations[3*i] = kStartRotation; 
			_particleRotations[3*i+1] = 0.0; 
			_particleRotations[3*i+2] = 0.0; 
		}
		
		//Initialize iars.
		_gravityPoint[0] = gp.x;
		_gravityPoint[1] = gp.y;
		_gravityPoint[2] = gp.z;	
		_particleDirection[0] = 1.;
		_particleDirection[1] = 0.;
		_particleDirection[2] = 0.;	

		return self;
	}
	else
	return nil;
}



- (void) dealloc
{
	ReleaseSystem(_particleSystem);
	
	[super dealloc];
}

- (long) count
{
	return ((StarsRef)_particleSystem)->_particleCount;
}

- (void) renderStars
{	
	//Set-up
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	[GameObject setVertexPointer:((StarsRef)_particleSystem)->_particleVertices size:3 type:GL_FLOAT];
	[GameObject setTexcoordPointer:&(((StarsRef)_particleSystem)->_particleVertices[VERTEX_COORD_INDEX(((StarsRef)_particleSystem)->_particleCount)]) size:2 type:GL_FLOAT];
	
	[GameObject bindTexture2D:((StarsRef)_particleSystem)->_texture];
	glDrawElements(GL_TRIANGLES, ((StarsRef)_particleSystem)->_particleCount * 6, GL_UNSIGNED_SHORT, ((StarsRef)_particleSystem)->_particleIndices);
	
	//Clean-up
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

- (void) updatePositionAtTime:(NSTimeInterval)time
{
	long			i, 
					count, 
					vertexIndex;
	CGFloat			theta = 0., 
					norm = 0.,
					dist,
					coeff, coeff2,
					location[3], 
					direction[3],
					texsize[2],
					rotation[3];

	if (_previousRenderTime < 0.) {
		_previousRenderTime = time;
	}
	else if (time == _previousRenderTime)
	return;
	_previousRenderTime = time;
	
	direction[0] = _particleDirection[0]; direction[1] = _particleDirection[1]; direction[2] = _particleDirection[2]; 
	if (!((_gravityPoint[0] < INFINITY) && (_gravityPoint[1] < INFINITY) && (_gravityPoint[2] < INFINITY)))
	theta = INFINITY;
	else
	norm = Norm(direction[0], direction[1], direction[2]);		

	for (i=0, count = ((StarsRef)_particleSystem)->_particleCount; i<count; ++i) {
		vertexIndex = VERTEX_COORD_INDEX(i);
		GetLocation(&(((StarsRef)_particleSystem)->_particleVertices[vertexIndex]), i, location);

		//Compute particle orientations.
		_particleRotations[3*i] -= 0.025;
		direction[0] = -norm * cos(_particleRotations[3*i]);
		direction[1] = direction[1];
		direction[2] = norm * sin(_particleRotations[3*i]);
		location[0] += direction[0];
		location[1] += direction[1];
		location[2] += direction[2];	

		//Stretch coefficients.
		dist = Norm(location[0], location[1], location[2]);
		coeff = Clamp((500.-dist)/400., 0., 1.);
		texsize[0] = MAX(coeff * kTexWidth, 4.*kTexHeight);
		texsize[1] = coeff * kTexHeight + (1.-coeff) * 3.*kTexHeight;
		
		coeff2 = (1.-coeff) * (1.-coeff);
		coeff2 *= coeff2;
		rotation[0] = _particleRotations[3*i] * (1.-coeff2) + M_PI * coeff2;
		rotation[1] = _particleRotations[3*i+1] * (1.-coeff2) + M_PI/2. * coeff2;
		rotation[2] = _particleRotations[3*i+2] * coeff2;
		SetNewParticleTriangleStrip(&(((StarsRef)_particleSystem)->_particleVertices[vertexIndex]), location, rotation, texsize);

		if (location[2] > 5)
		SetNewParticle(_particleSystem, i, YES, &_particleRotations[3*i]);
	}		
}

- (void) setDirectionX:(CGFloat)x Y:(CGFloat)y Z:(CGFloat)z
{
	_particleDirection[0] = x;
	_particleDirection[1] = y;
	_particleDirection[2] = z;
}

- (void) renderAtTime:(NSTimeInterval)time
{
	[self updatePositionAtTime:time];

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	[self renderStars];
	glDisable(GL_BLEND);	
}

@end