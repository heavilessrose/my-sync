#import "GlissView.h"
#import <GLUT/glut.h>

#define LIGHT_X_TAG 0
#define THETA_TAG 1
#define RADIUS_TAG 2

@implementation GlissView

- (void)prepare
{
	NSLog(@"prepare");
	
	// The GL context must be active for these functions to have an effect
    NSOpenGLContext *openGLContext = [self openGLContext];
    [openGLContext makeCurrentContext];	
	
	// Configure the view
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	// Add some ambient lighting.
	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	
	// Initialize the light
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	// and switch it on.
	glEnable(GL_LIGHT0);
	
	// Set the properties of the material under ambient light
	GLfloat mat[] = { 0.1, 0.7, 0.1, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	
	// Set the properties of the material under diffuse light
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);	
	
}

/* FIXME: this one doesn't work right :(
- (id)initWithFrame:(NSRect)frameRect
{
	NSLog(@"initWithFrame:");

	NSOpenGLPixelFormatAttribute attrs[] = 
	{   NSOpenGLPFAAllRenderers, NSOpenGLPFADoubleBuffer,
	    NSOpenGLPFADepthSize, 16,
		NSOpenGLPFAColorSize, 32,
	0 };
	NSOpenGLPixelFormat* pixFmt ;
	pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs] ;
	[super initWithFrame:frameRect pixelFormat:pixFmt];
	[self prepare];
	return self;
}
*/
- (id)initWithCoder:(NSCoder *)c
{
	NSLog(@"initWithCoder:");
	self = [super initWithCoder:c];
	[self prepare];
	return self;
}


// Called when the view resizes
- (void)reshape
{
	NSLog(@"reshaping");
    NSRect rect = [self bounds];
    glViewport(0,0, rect.size.width, rect.size.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,
                   rect.size.width/rect.size.height,
                   0.2, 7);
}
- (void)awakeFromNib
{
    [self changeParameter:self];
}

- (IBAction)changeParameter:(id)sender
{
    lightX = [[sliderMatrix cellWithTag:LIGHT_X_TAG] floatValue];
    theta = [[sliderMatrix cellWithTag:THETA_TAG] floatValue];
    radius = [[sliderMatrix cellWithTag:RADIUS_TAG] floatValue];
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)r
{
    GLfloat lightPosition[] = {lightX, 1, 3, 0.0};
    
    // Clear the background
    glClearColor (0.1, 0.3, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
    
    // Set the view point
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(radius * sin(theta), 0,  radius * cos(theta), 0, 0, 0, 0, 1, 0);
    
    // Put the light in place
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    if (!displayList)
    {
        displayList = glGenLists(1);
        glNewList(displayList, GL_COMPILE_AND_EXECUTE);
        
        // Draw the stuff
        glTranslatef(0, 0, 0);
        glutSolidTorus(0.3, 0.9, 35, 31);
        glTranslatef(0, 0, -1.2);
        glutSolidCone(1, 1, 17, 17);
        glTranslatef(0, 0, 0.6);
        glutSolidTorus(0.3, 1.8, 35, 31);
        
        glEndList();
    } else {
        glCallList(displayList);
    }
    
    // Flush to screen
    glFinish();
}


@end
