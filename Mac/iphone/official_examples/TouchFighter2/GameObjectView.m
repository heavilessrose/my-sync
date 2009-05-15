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

File: GameObjectView.m
Abstract: A convenience view that visualize a GameObject with optional reflection and rotation.

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

#import "Maths.h"
#import "GameObjectView.h"
#import "GameObject.h"


@implementation GameObjectView
@synthesize hasReflection = _reflection, rotationSpeed=_rotationSpeed, gameObject=_gameObject;

- (id)initWithFrame:(CGRect)frame pixelFormat:(NSString*)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained
{
	const GLfloat			lightAmbient[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat			lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat			matAmbient[] = {0.3, 0.3, 0.35, 1.0};
	const GLfloat			matDiffuse[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat			matSpecular[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat			lightPosition[] = {3.5, -0.3, 1.0, 1.0};
	const GLfloat			lightShininess = 100.0;
	const GLfloat			zNear = 0.5,
							zFar = 2000.0,
							fieldOfView = 60.0;
	GLfloat					size = zNear * tanf(DegreesToRadians(fieldOfView) / 2.0);

	if((self = [super initWithFrame:frame pixelFormat:format depthFormat:depth preserveBackbuffer:retained])) {
		//Initialize ivars.
		_rotationSpeed = 90.;
		_previousTime = -1;
		_reflection = YES;

		//Initiliazing GL states and ligths.
		glEnable(GL_LIGHT0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, lightShininess);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); 			
		glShadeModel(GL_SMOOTH);
		glEnable(GL_NORMALIZE);
		glDisable(GL_DEPTH_TEST);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustumf(-size, size, -size / (frame.size.width / frame.size.height), size / (frame.size.width / frame.size.height), zNear, zFar);
		glMatrixMode(GL_MODELVIEW);
		
	}
	return self;
}

- (void)dealloc 
{
	[_gameObject release];

	[super dealloc];
}

- (void) setGameObject:(GameObject*)object
{
	if (object != _gameObject) {
		[_gameObject release];
		_gameObject = [object retain];
	}
}

- (void) render
{
	static const GLfloat	vtxData[3*4] = { 1.f, -1.f, -1.f,
										    -1.f, -1.f, -1.f,
										     1.f,  1.f, -1.f,
											-1.f,  1.f, -1.f };	
	NSTimeInterval			time;

	if (!_gameObject)
	return;
	
	time = [NSDate timeIntervalSinceReferenceDate];

	if (_previousTime < 0.)
	_previousTime = time;
	_rotation += _rotationSpeed * (time - _previousTime);
	_previousTime = time;

	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);	
	glPushMatrix();
	glTranslatef(-0.1f, 0.f, -1.f);
	glRotatef(_rotation, 1.f, 0.f, 0.f);
	if (_reflection) {
		glRotatef(180.f, 0.f, 0.f, 1.f);		
		[_gameObject renderAtTime:time];
		glPushMatrix();
		glLoadIdentity();
		glEnable(GL_BLEND);
		glDepthFunc(GL_ALWAYS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0., 0., 0., 0.7);
		[GameObject setVertexPointer:(GLvoid *)vtxData size:3 type:GL_FLOAT];
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_BLEND);
		glPopMatrix();
	}
	glRotatef(180.f, 0.f, 0.f, 1.f);		
	glTranslatef(0.1f, 0.f, 0.f);
	[_gameObject renderAtTime:time];
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);	
	
	[self swapBuffers];	
}

@end
