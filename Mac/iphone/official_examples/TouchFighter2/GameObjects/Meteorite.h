/*
File: Meteorite.h
Abstract: This class disforms a sphere to create to simulate a meteorite.

Version: 2.0
*/

#import "PhysicalObject.h"
#import "GameObject_Private.h"

@class Fire;

@interface Meteorite : PhysicalObject {
	GLfloat			*_vtxData;
	GLshort			*_texData;
	GLubyte			*_colData;
	CGFloat			_distortion;			//0. to create a sphere
	Texture2D		*_tex;
	GLint			_stacks, _slices;
	GLfloat			_axisAngle;
	Fire*			_flames;
}

- (id) initWithTexture:(Texture2D*)tex stacks:(GLint)stacks slices:(GLint)slices;
- (void) renderFireTrailAtTime:(NSTimeInterval)time transformPosition:(Matrix)m;
- (BOOL) alive;
- (void) reset;

@end