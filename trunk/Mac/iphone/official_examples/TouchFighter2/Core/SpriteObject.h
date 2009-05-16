/*
File: Sprite.h
Abstract: A sprite is simply a quad with texture.

Version: 2.0
*/

#import "PhysicalObject.h"

@interface SpriteObject : PhysicalObject {
@protected
	Texture2D*			_texture;
	Point4				_color;
	GLenum				_blendModeSrc,
						_blendModeDst;
}

- (id) initWithTexture:(Texture2D*)texture;

/* Quad color */
@property Point4				color;

/* Blend modes */
@property GLenum				blendModeSrc, blendModeDst;
@end
