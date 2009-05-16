/*
File: GameObjectView.m
Abstract: A convenience view that visualize a GameObject with optional reflection and rotation.

Version: 2.0
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
