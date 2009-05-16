/*
File: CubeMap.m
Abstract: Cube Map used for rotating background.

Version: 2.0
*/

#import "CubeMap.h"
#import "PhysicalObject.h"
#import "Utils.h"

@implementation CubeMap

- (id) initWithTextures:(Texture2D*)posX :(Texture2D*)posY :(Texture2D*)posZ :(Texture2D*)negX :(Texture2D*)negY :(Texture2D*)negZ
{
	float*			vPtr = _vtxData;
	float*			tPtr = _texData;
	unsigned		i;

	if ((self = [super init])) {
		_posX = [posX retain];
		_posY = [posY retain];
		_posZ = [posZ retain];
		_negX = [negX retain];
		_negY = [negY retain];
		_negZ = [negZ retain];
		
		Texture2D*	tex[6] = {_posX, _negZ, _negX, _posZ, _posY, _negY};
		for(i=0; i<6; i++)
		{
			GLfloat one  = 1.0f;
			GLfloat zero = 0.0f;
			tPtr[0] = zero; tPtr[1] = zero;  tPtr+=2;
			tPtr[0] = zero; tPtr[1] = one;   tPtr+=2;
			tPtr[0] = one;  tPtr[1] = zero;  tPtr+=2;
			tPtr[0] = one;  tPtr[1] = one;   tPtr+=2;
			
			[GameObject bindTexture2D:tex[i]];
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		
		//Set-up cube vertices and tex coords.
		
		// posx
		vPtr[0]=-1.0f; vPtr[1]= 1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]=-1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]= 1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]=-1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;

		// negz
		vPtr[0]=-1.0f; vPtr[1]= 1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]=-1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]= 1.0f; vPtr[1]= 1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]= 1.0f; vPtr[1]=-1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;

		// negx
		vPtr[0]= 1.0f; vPtr[1]= 1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]= 1.0f; vPtr[1]=-1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]= 1.0f; vPtr[1]= 1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]= 1.0f; vPtr[1]=-1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;

		// posz
		vPtr[0]= 1.0f; vPtr[1]= 1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]= 1.0f; vPtr[1]=-1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]= 1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]=-1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;

		// posy
		vPtr[0]= 1.0f; vPtr[1]= 1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]= 1.0f; vPtr[1]= 1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]= 1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]= 1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;

		// negy
		vPtr[0]= 1.0f; vPtr[1]=-1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]= 1.0f; vPtr[1]=-1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]=-1.0f; vPtr[2]= 1.0f; vPtr[3]=1.0f; vPtr+=4;
		vPtr[0]=-1.0f; vPtr[1]=-1.0f; vPtr[2]=-1.0f; vPtr[3]=1.0f; vPtr+=4;
	}
	
	
	return self;
}

- (void) dealloc
{
	[_posX release];
	[_posY release];
	[_posZ release];
	[_negX release];
	[_negY release];
	[_negZ release];

	[super dealloc];
}

- (void) renderAtTime:(NSTimeInterval)time
{
	unsigned	i;
	Texture2D*	tex[6] = {_posX, _negZ, _negX, _posZ, _posY, _negY};

	glPushMatrix();
	glScalef(1000.0f, 1000.0f, 1000.0f);

	glDepthFunc(GL_ALWAYS);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	[GameObject setVertexPointer:_vtxData size:4 type:GL_FLOAT];
	[GameObject setTexcoordPointer:_texData size:2 type:GL_FLOAT];

	glColor4f(1.0, 0.92, 0.83, 1.0);
	//Renders all cube faces.
	for(i=0; i<6; i++) {
		[GameObject bindTexture2D:tex[i]];
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
	
	glPopMatrix();
}

@end