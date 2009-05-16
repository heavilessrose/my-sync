/*
File: Meteorite.m
Abstract: This class disforms a sphere to create to simulate a meteorite.

Version: 2.0
*/

#import "Meteorite.h"
#import "GameObject_Private.h"
#import "ParticleSystemObject.h"
#import "Utils.h"

@implementation Meteorite

- (id) initWithTexture:(Texture2D*)tex stacks:(GLint)stacks slices:(GLint)slices
{
	if ((self = [super init])) {
		_stacks = stacks;
		_slices = slices;

		_tex = [tex retain];
		
		_distortion = 0.5;

		_vtxData = NULL;
		_texData = NULL;
		_colData = NULL;
		GLfloat *vPtr = _vtxData = (GLfloat*)malloc(sizeof(GLfloat) * 3 * ((_slices*2+2) * (_stacks)));
		GLshort *tPtr = _texData = (GLshort*)malloc(sizeof(GLshort) * 2 * ((_slices*2+2) * (_stacks)));
		GLubyte *cPtr = _colData = (GLubyte*)malloc(sizeof(GLubyte) * 4 * ((_slices*2+2) * (_stacks)));

		unsigned int	phiIdx, thetaIdx, i;
		const GLfloat	light[3] = {-0.6f, 0.6f, -0.4f};
		float			prevNorm[_slices];
		
		for (i=0; i<_slices; ++i)
		prevNorm[i] = 0.;

		for(phiIdx=0; phiIdx < _stacks; phiIdx++)
		{
			float phi0 = M_PI * ((float)(phiIdx+0) * (1.0f/(float)(_stacks)) - 0.5f);
			float phi1 = M_PI * ((float)(phiIdx+1) * (1.0f/(float)(_stacks)) - 0.5f);

			float cosPhi0 = cos(phi0);
			float sinPhi0 = sin(phi0);
			float cosPhi1 = cos(phi1);
			float sinPhi1 = sin(phi1);

			float cosTheta, sinTheta;

			for(thetaIdx=0; thetaIdx < _slices; thetaIdx++)
			{
				float theta = 2.0f*M_PI * ((float)thetaIdx) * (1.0f/(float)(_slices-1));
				float norm = fast_unit_rand()*_distortion+1.;
				if (prevNorm[thetaIdx] == 0)
				prevNorm[thetaIdx] = norm;
				cosTheta = cos(theta);
				sinTheta = sin(theta);

				vPtr[0] = cosPhi0 * cosTheta * prevNorm[thetaIdx]; vPtr[1] = cosPhi0 * sinTheta * prevNorm[thetaIdx]; vPtr[2] = sinPhi0 * prevNorm[thetaIdx];
				vPtr[3] = cosPhi1 * cosTheta * norm; vPtr[4] = cosPhi1 * sinTheta * norm; vPtr[5] = sinPhi1 * norm;

				const GLfloat tScale = 32767.0f;
				GLfloat texX = tScale * (float)thetaIdx * (1.0f/(float)(_slices-1));
				tPtr[0] = texX; tPtr[1] = tScale * (float)(phiIdx+0) * (1.0f/(float)(_stacks));
				tPtr[2] = texX; tPtr[3] = tScale * (float)(phiIdx+1) * (1.0f/(float)(_stacks));

				GLfloat	dot0 = vPtr[0]*light[0] + vPtr[1]*light[1] + vPtr[2]*light[2],
						dot1 = vPtr[3]*light[0] + vPtr[4]*light[1] + vPtr[5]*light[2];

				if(dot0 < 0)  dot0 = 0.0f;
				if(dot1 < 0)  dot1 = 0.0f;
				if(dot0 > 1.0)  dot0 = 1.0f;
				if(dot1 > 1.0)  dot1 = 1.0f;

				// ambient light = 40;
				cPtr[0] = cPtr[1] = cPtr[2] = 40 + (int)(215 * dot0);
				cPtr[4] = cPtr[5] = cPtr[6] = 40 + (int)(215 * dot1);
				cPtr[3] = cPtr[7] = 255;

				vPtr += 2*3;
				tPtr += 2*2;
				cPtr += 2*4;
				
				prevNorm[thetaIdx] = norm;
		}
	
			// degenerate triangle to connect stacks and maintain winding order
			vPtr[0] = vPtr[3] = vPtr[-3]; vPtr[1] = vPtr[4] = vPtr[-2]; vPtr[2] = vPtr[5] = vPtr[-1];
			tPtr[0] = tPtr[2] = tPtr[-2]; tPtr[1] = tPtr[3] = tPtr[-1];
			cPtr[0] = cPtr[1] = cPtr[2] = cPtr[4] = cPtr[5] = cPtr[6] = cPtr[-2];
			cPtr[3] = cPtr[7] = cPtr[1];

			vPtr += 2*3;
			tPtr += 2*2;
			cPtr += 2*4;
		}
		
		//Initialize damage flames.
		Generator generator = GeneratorZero;
	generator.velocity =  Cube3Make(-0.5, -0.5, 0., 1., 1., 2.);
		_flames = [[Fire alloc] initWithTexture:FlameTexture() generator:generator type:kParticleTriangleStrip count:10];
		_flames.scale = Point3Make(10, 10, 10);
		_flames.radius = 1.0;
		_flames.particleDecay = 1.;
		_flames.systemColor = Point4Make(.12, .05, 0., .35);
		generator.position = Cube3Make(0., 0., 0., 0., 0., 0.);
		[_flames initializeParticles:generator];
	}

	return self;
}

- (void) reset
{
	Generator generator = GeneratorZero;

	_position = Point3Make(45*(fast_unit_rand() -0.5), 45*(fast_unit_rand() > 0.5 ? .5 : -.5), -45.);
	_velocity = Point3Mult(Point3Normalize(Point3Sub(Point3Make((fast_unit_rand() -0.5)*.3, (_position.y < 0. ? 1. : -1.) * fast_unit_rand(), _position.z), _position)), 20.);
	
	generator.velocity =  Cube3Make(-0.5, -0.5, -1., 1., 1., 2.);
	generator.position = Cube3Make(0., 0., 0., 0., 0., 0.);
	[_flames initializeParticles:generator];
	_flames.systemAcceleration = Point3Make(-_velocity.x*0.3, -_velocity.y*0.3, -_velocity.z*0.3);
}

- (void) updatePositionAtTime:(NSTimeInterval)time
{
	_axisAngle += (time - _previousRenderTime) * 180.;
	
	[super updatePositionAtTime:time];
	_flames.position = _position;
}

- (void) renderFireTrailAtTime:(NSTimeInterval)time transformPosition:(Matrix)m;
{
	[_flames renderAtTime:time transformPosition:m];
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	[GameObject setVertexPointer:_vtxData size:3 type:GL_FLOAT];
	[GameObject setTexcoordPointer:_texData size:2 type:GL_SHORT];
	[GameObject setColorPointer:_colData size:4 type:GL_UNSIGNED_BYTE];

	glPushMatrix(); {
		glScalef(1.5f, 1.f, 1.f);
		glRotatef(_axisAngle, 0.2f, 0.8f, 0.4f);
		
		glMatrixMode(GL_TEXTURE);
		{
			glScalef(1.0f/32767.0f, 1.0f/32767.0f, 1.0f);

			[GameObject bindTexture2D:_tex];
			glDrawArrays(GL_TRIANGLE_STRIP, 0, (_slices+1)*2*_stacks-2);

			glLoadIdentity();
		}
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_CULL_FACE);
	}
	
	glPopMatrix();
	glPushMatrix();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
}

- (BOOL) alive
{
	return ((ABS(_position.x/_position.z)<1.5) && (ABS(_position.y/_position.z)<1.5) && (_position.z > -60.) && (_position.z < 0.));
}

- (void)dealloc
{
	if(_vtxData) free(_vtxData);
	if(_texData) free(_texData);
	if(_colData) free(_colData);

	[super dealloc];
}

@end
