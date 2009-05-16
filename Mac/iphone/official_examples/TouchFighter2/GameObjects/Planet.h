/*
File: Planet.h
Abstract: Planet.

Version: 2.0
*/

#import "PhysicalObject.h"

@interface Planet : PhysicalObject {
@private
	GLfloat			*_vtxData;
	GLshort			*_texData;
	GLubyte			*_colData;
	Texture2D		*_tex, *_glow;
	GLint			_stacks, _slices;
	GLfloat			_tex_rotation,
					_planet_rotation;
}
- (id) initWithTexture:(Texture2D*)tex glowTexture:(Texture2D*)glow stacks:(GLint)stacks slices:(GLint)slices;
@end