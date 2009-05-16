/*
File: CubeMap.h
Abstract: Cube Map used for rotating background.

Version: 2.0
*/

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@class Texture2D;

/* Cube Map used for rotating background. */

@interface CubeMap : NSObject {
@private
	Texture2D*		_negX, *_negY, *_negZ,
					*_posX, *_posY, *_posZ;
	GLfloat			_vtxData[(4*4) * 6],
					_texData[(4*2) * 6];
}

- (id) initWithTextures:(Texture2D*)posX :(Texture2D*)posY :(Texture2D*)posZ :(Texture2D*)negX :(Texture2D*)negY :(Texture2D*)negZ;
- (void) renderAtTime:(NSTimeInterval)time;

@end
