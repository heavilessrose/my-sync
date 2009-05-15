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

File: Planet.m
Abstract: Planet.

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

#import "Planet.h"

@implementation Planet

- (id) initWithTexture:(Texture2D*)tex glowTexture:(Texture2D*)glow stacks:(GLint)stacks slices:(GLint)slices
{
	if ((self = [super init])) {
		_stacks = stacks;
		_slices = slices;

		_tex = [tex retain];
		_tex_rotation = 0.0;

		int glowVerticies = 0;

		if(glow) {
			_glow = [glow retain];
			glowVerticies = _stacks *2 * 2;
		}
		else
			_glow = nil;

		_vtxData = NULL;
		_texData = NULL;
		_colData = NULL;
		GLfloat *vPtr = _vtxData = (GLfloat*)malloc(sizeof(GLfloat) * 3 * ((_slices*2+2) * (_stacks) + glowVerticies));
		GLshort *tPtr = _texData = (GLshort*)malloc(sizeof(GLshort) * 2 * ((_slices*2+2) * (_stacks) + glowVerticies));
		GLubyte *cPtr = _colData = (GLubyte*)malloc(sizeof(GLubyte) * 4 * ((_slices*2+2) * (_stacks) + glowVerticies));

		unsigned int	phiIdx, thetaIdx;
		const GLfloat	light[3] = {-0.6f, 0.6f, -0.4f};

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
				cosTheta = cos(theta);
				sinTheta = sin(theta);

				vPtr[0] = cosPhi0 * cosTheta; vPtr[1] = cosPhi0 * sinTheta; vPtr[2] = sinPhi0;
				vPtr[3] = cosPhi1 * cosTheta; vPtr[4] = cosPhi1 * sinTheta; vPtr[5] = sinPhi1;

				const GLfloat tScale = 32767.0f;
				GLfloat texX = tScale * (float)thetaIdx * (1.0f/(float)(_slices-1));
				tPtr[0] = texX; tPtr[1] = tScale * (float)(phiIdx+0) * (1.0f/(float)(_stacks));
				tPtr[2] = texX; tPtr[3] = tScale * (float)(phiIdx+1) * (1.0f/(float)(_stacks));

				GLfloat	dot0 = vPtr[0]*light[0] + vPtr[1]*light[1] + vPtr[2]*light[2],
						dot1 = vPtr[3]*light[0] + vPtr[4]*light[1] + vPtr[5]*light[2];

				if(dot0 < 0)  { dot0 = 0.0f; }
				if(dot1 < 0)  { dot1 = 0.0f; }
				if(dot0 > 1.0) { dot0 = 1.0f; }
				if(dot1 > 1.0) { dot1 = 1.0f; }

				// ambient light = 40;
				cPtr[0] = cPtr[1] = cPtr[2] = 40 + (int)(215 * dot0);
				cPtr[4] = cPtr[5] = cPtr[6] = 40 + (int)(215 * dot1);
				cPtr[3] = cPtr[7] = 255;

				vPtr += 2*3;
				tPtr += 2*2;
				cPtr += 2*4;
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

		if(_glow)
		{
			for(phiIdx=0; phiIdx < _stacks*2; phiIdx++)
			{
				GLfloat theta = 2.f * M_PI * (-0.5f + ((float)phiIdx) * (1.0f/(float)(_stacks*2-1)));
				GLfloat cosTheta = -cos(theta);
				GLfloat sinTheta = sin(theta);

				const
				GLfloat	innerRadius = 1.02f,
						outerRadius = 1.4f;
				vPtr[0] = cosTheta*innerRadius; vPtr[1] = sinTheta*innerRadius; vPtr[2] = 0.0f;
				vPtr[3] = cosTheta*outerRadius; vPtr[4] = sinTheta*outerRadius; vPtr[5] = 0.0f;

				tPtr[0] = tPtr[3] = tPtr[2] = 0;
				tPtr[1] = 32767;

				cPtr[0] = cPtr[1] = cPtr[2] = cPtr[3] = 255;
				cPtr[4] = cPtr[5] = cPtr[6] = cPtr[7] = 255;

				vPtr += 2*3;
				tPtr += 2*2;
				cPtr += 2*4;
			}
		}	
	}

	return self;
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	_tex_rotation = -0.002f *30. * time;

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	[GameObject setVertexPointer:_vtxData size:3 type:GL_FLOAT];
	[GameObject setTexcoordPointer:_texData size:2 type:GL_SHORT];
	[GameObject setColorPointer:_colData size:4 type:GL_UNSIGNED_BYTE];

	glPushMatrix(); {
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

		//For better look, we translate the texture instead of rotating vertices.
		glMatrixMode(GL_TEXTURE); {
			if(_glow) {
				glTranslatef(_tex_rotation*2, 0.5f, 0.0f);
				glScalef(1.0f/32767.0f*4.0f, 1.0f/32767.0f*2.0f, 1.0f);
			}
			else {
				glTranslatef(_tex_rotation, 0.0f, 0.0f);
				glScalef(1.0f/32767.0f, 1.0f/32767.0f, 1.0f);
			}

			[GameObject bindTexture2D:_tex];
			glDrawArrays(GL_TRIANGLE_STRIP, 0, (_slices+1)*2*_stacks-2);

			glLoadIdentity();
		}
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_CULL_FACE);
	}
	
	glPopMatrix();
	glPushMatrix();

	//Renders glow
	if(_glow) {
		[GameObject bindTexture2D:_glow];
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glRotatef(72, 0.0f, 1.0f, 0.0f);
		glScalef(1.65f, 1.65f, 1.65f);
		glMatrixMode(GL_TEXTURE);
		{
			glScalef(1.0f/32767.0, 1.0f/32767.0, 1.0f);
			glDrawArrays(GL_TRIANGLE_STRIP, (_slices+1)*2*_stacks, _stacks*2*2);
			glLoadIdentity();
		}
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_BLEND);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

- (void)dealloc
{
	[_tex release];
	[_glow release];

	if(_vtxData) { free(_vtxData); }
	if(_texData) { free(_texData); }
	if(_colData) { free(_colData); }

	[super dealloc];
}

@end