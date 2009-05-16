/*
File: Sprite.m
Abstract: A sprite is simply a quad with texture.

Version: 2.0
*/

#import "SpriteObject.h"
#import "Utils.h"

@implementation SpriteObject
@synthesize blendModeSrc=_blendModeSrc, blendModeDst=_blendModeDst, color=_color;

- (id) initWithTexture:(Texture2D*)texture
{
	if ((self = [super init])) {
		//Initializes ivars.
		_texture = [texture retain];
		_blendModeSrc = GL_ONE;
		_blendModeDst = GL_ONE_MINUS_SRC_ALPHA;
		_color = Point4Make(1., 1., 1., 1.);
	}
	
	return self;
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	//Render quad and texture
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(_blendModeSrc, _blendModeDst);
	glColor4f(_color.x, _color.y, _color.z, 1.);
	[Texture2D drawTexture:_texture atPoint:CGPointMake(0., 0.) depth:0.];
	glDisable(GL_BLEND);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

- (void) dealloc
{
	[_texture release];
	
	[super dealloc];
}

@end
