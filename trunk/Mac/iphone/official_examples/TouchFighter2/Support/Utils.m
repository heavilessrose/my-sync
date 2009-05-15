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

File: Utils.m
Abstract: Utility functions and macros.

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

#import "Utils.h"
#import "GameObject.h"

//Shared textures

static Texture2D* _shipTexture = nil;
static Texture2D* _enemyShipTexture = nil;
static Texture2D* _starTexture = nil;
static Texture2D* _flameTexture = nil;
static Texture2D* _missileTexture = nil;

Texture2D*	ShipTexture()
{
	if (!_shipTexture) {
#if !TARGET_IPHONE_SIMULATOR
		_shipTexture = [[Texture2D alloc] initWithPVR4DataFile:@"dart256.pvr4" pixelsWide:256 pixelsHigh:256];
#else
		_shipTexture = [[Texture2D alloc] initWithImagePath:@"dart.png"];
#endif
	}
	
	return _shipTexture;
}

void ShipTextureRelease()
{
	[_shipTexture release];
	_shipTexture = nil;
}

Texture2D*	EnemyShipTexture()
{
	if (!_enemyShipTexture) {
		_enemyShipTexture = [[Texture2D alloc] initWithImagePath:@"enemy.png"];
	}
	
	return _enemyShipTexture;
}

void EnemyShipTextureRelease()
{
	[_enemyShipTexture release];
	_enemyShipTexture = nil;
}

Texture2D*	StarTexture()
{	
	if (!_starTexture) {
		_starTexture = [[Texture2D alloc] initWithImagePath:@"glow.png"];
	}

	return _starTexture;
}

void StarTextureRelease()
{
	[_starTexture release];
	_starTexture = nil;
}

Texture2D*	FlameTexture()
{
	if (!_flameTexture) {
		_flameTexture = [[Texture2D alloc] initWithImagePath:@"cloud.png"];
	}

	return _flameTexture;
}

void FlameTextureRelease()
{
	[_flameTexture release];
	_flameTexture = nil;
}

Texture2D*	MissileTexture()
{
	if (!_missileTexture) {
		_missileTexture = [[Texture2D alloc] initWithImagePath:@"missile.png"];
	}

	return _missileTexture;
}

void MissileTextureRelease()
{
	[_missileTexture release];
	_missileTexture = nil;
}

void SetOpenGLStates()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void UnsetOpenGLStates()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
}

// This is a general routine to make a new material active
void SetMaterialParameters(float shininess, float r_diffuseColor, float g_diffuseColor, float b_diffuseColor, float r_specularColor, float g_specularColor, float b_specularColor)
{
	GLfloat glfModelSpecular[4];
	
	/* Set the specular shading color. */
	glfModelSpecular[0] = r_specularColor;
	glfModelSpecular[1] = g_specularColor;
	glfModelSpecular[2] = b_specularColor;
	glfModelSpecular[3] = 1.f;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glfModelSpecular); 
	glColor4f(r_diffuseColor, g_diffuseColor, b_diffuseColor, 1.f);
}

// random number generation adapted from http://qbrundage.com/michaelb/pubs/essays/random_number_generation
#define R250_LEN     250

int r250_index;
unsigned long r250_buffer[R250_LEN];

void fast_rand_init() {
    int i = R250_LEN;
    unsigned long mask1 = 1;
    unsigned long mask2 = 0xFFFFFFFF;
	
    while (i-- > 31) {
        r250_buffer[i] = rand();
    }
    
    /*
	 Establish linear independence of the bit columns
	 by setting the diagonal bits and clearing all bits above
	 */
    while (i-- > 0) {
        r250_buffer[i] = (rand() | mask1) & mask2;
        mask2 ^= mask1;
        mask1 >>= 1;
    }
    r250_buffer[0] = mask1;
    r250_index = 0;
}

#define R250_IA  (sizeof(unsigned long)*103)
#define R250_IB  (sizeof(unsigned long)*R250_LEN - R250_IA)

unsigned long fast_rand() {
    int i1 = r250_index;
    unsigned char * b1 = (unsigned char*)r250_buffer;
    unsigned long * tmp1;
    unsigned long r;
    int j1;
    
    j1 = i1 - R250_IB;
    if (j1 < 0)
	{
        j1 = i1 + R250_IA;
	}
    
    tmp1 = (unsigned long *)(b1 + i1);
    r = (*(unsigned long *)(b1 + j1)) ^ (*tmp1);
    *tmp1 = r;
    
    i1 = (i1 != sizeof(unsigned long)*(R250_LEN-1)) ? (i1 + sizeof(unsigned long)) : 0;
    r250_index = i1;

    return r;
}

@implementation Texture2D(TouchFighterSupport)

+ (void) drawTexture:(Texture2D*)texture atPoint:(CGPoint)point depth:(CGFloat)depth
{
	GLfloat				maxS = texture.maxS,
						maxT = texture.maxT,
						pixelsWide = texture.pixelsWide,
						pixelsHigh = texture.pixelsHigh;						
	GLfloat				coordinates[] = {
							0,				maxT,
							maxS,			maxT,
							0,				0,
							maxS,			0
						};
	GLfloat				width = (GLfloat)pixelsWide * maxS,
						height = (GLfloat)pixelsHigh * maxT;
	GLfloat				vertices[] = {
							-width / 2 + point.x,		-height / 2 + point.y,		depth,
							width / 2 + point.x,		-height / 2 + point.y,		depth,
							-width / 2 + point.x,		height / 2 + point.y,		depth,
							width / 2 + point.x,		height / 2 + point.y,		depth
						};
	
	[GameObject bindTexture2D:texture];
	[GameObject setVertexPointer:vertices size:3 type:GL_FLOAT];
	[GameObject setTexcoordPointer:coordinates size:2 type:GL_FLOAT];
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

- (id) initWithString:(NSString*)string dimensions:(CGSize)dimensions fontSize:(CGFloat)size
{
	return [self initWithString:string dimensions:dimensions alignment:UITextAlignmentLeft font:[UIFont systemFontOfSize:size]];
}

- (id) initWithPVR4DataFile:(NSString*)path pixelsWide:(NSUInteger)width pixelsHigh:(NSUInteger)height
{
	if(![path isAbsolutePath])
	{
		path = [[NSBundle mainBundle] pathForResource:path ofType:nil];
	}
	
	return [self initWithData:[[NSData dataWithContentsOfFile:path] bytes] pixelFormat:kTexture2DPixelFormat_RGBA_PVRTC4 pixelsWide:width pixelsHigh:height contentSize:CGSizeMake(width, height)];
}

@end
