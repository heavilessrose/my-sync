/*
File: AnimationObject.m
Abstract: An animation (used for explosions).

Version: 2.0
*/

#import "AnimationObject.h"
#import "GameObject_Private.h"

@implementation AnimationObject

@synthesize texture=_texture, 
			loop=_loop,
			countingUp=_countingUp,  
			frameCount=_frameCount, currentFrame=_currentFrame,
			holdScaleEnd=_holdScaleEnd, holdScaleDelta=_holdScaleDelta, holdAlphaEnd=_holdAlphaEnd, holdAlphaDelta=_holdAlphaDelta, step=_step,
			holdAlpha=_holdAlpha, holdScale=_holdScale;

- (id) initWithTexture:(Texture2D*)texture frameCount:(NSUInteger)count
{
	NSUInteger		size = sqrt(count);
	
	// We only allow square textures
	if ((texture.maxS != 1.) || (texture.maxT != 1.)) {
		return nil;
	}
	
	if ((self = [super initWithTexture:texture])) {
		//Initializes ivars.
		_countingUp = YES;
		_frameCount = count;
		_currentFrame = 0;
		_loop = kLoopTypeNone;
		_pixelsWide = texture.pixelsWide / size;
		_pixelsHigh = texture.pixelsHigh / size;
		_holdScale = 1.0;
		_holdScaleEnd = 2.0;
		_holdScaleDelta = 0.1;
		_holdAlpha = 1.0;
		_holdAlphaEnd = 0.0;
		_holdAlphaDelta = -0.1;
		_step = 0.5;
	}
	return self;
}

- (void) updatePositionAtTime:(NSTimeInterval)time
{
	[super updatePositionAtTime:time];
	
	if (_countingUp) {
		_currentFrame += _step;
	} else {
		_currentFrame -= _step;
	}
	if (_currentFrame < 0. || _currentFrame >= (float)_frameCount) {
		switch(_loop) {
			case kLoopTypeReverse:
				_currentFrame = _countingUp ? (float)_frameCount - 2. * _step : _step;
				_countingUp = !_countingUp; break;
			case kLoopTypeRepeat:
				_currentFrame = (int)_currentFrame % _frameCount; break;
			case kLoopTypeHoldLast:
				_holdAlpha += _holdAlphaDelta;
				_holdScale += _holdScaleDelta;
				if (_holdAlpha < 0.f) {
					_holdAlpha = 0.f;
				} else if (_holdAlpha > 1.f) {
					_holdAlpha = 1.f;
				} else {
					_currentFrame = (float)_frameCount - (_countingUp ? _step : -_step);
				}
			default: break;
		}
	}

}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	float		xMin, xMax, yMin, yMax;
	int			curFrame = floorf(_currentFrame);
	float		inverseWidth = 1.f / _texture.pixelsWide, inverseHeight = 1.f / _texture.pixelsHigh;
	xMin = (curFrame * _pixelsWide) % _texture.pixelsWide;
	xMax = xMin + _pixelsWide;
	yMin = (curFrame * _pixelsWide) / _texture.pixelsWide;
	yMin *= _pixelsHigh;
	yMax = yMin + _pixelsHigh;
	xMin *= inverseWidth;
	xMax *= inverseWidth;
	yMin *= inverseHeight;
	yMax *= inverseHeight;
	GLfloat		coordinates[] = {
				xMin, yMax,
				xMax, yMax,
				xMin, yMin,
				xMax, yMin
				};
	static GLfloat		vertices[] = { -.5, -.5, 0.,
										.5, -.5, 0.,
									   -.5,  .5, 0.,
										.5,  .5, 0.};
	
	glEnable(GL_BLEND);
	glBlendFunc(_blendModeSrc, _blendModeDst);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	[GameObject bindTexture2D:_texture];
	[GameObject setVertexPointer:vertices size:3 type:GL_FLOAT];
	[GameObject setTexcoordPointer:coordinates size:2 type:GL_FLOAT];
	glPushMatrix(); {
		if (_loop == kLoopTypeHoldLast) {
			glPushMatrix();
			glColor4f(_holdAlpha * _color.x, _holdAlpha * _color.y, _holdAlpha * _color.z, _holdAlpha);
			glScalef(_holdScale, _holdScale, 1.0);
		}
		else
		glColor4f(_color.x, _color.y, _color.z, 1.);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		if (_loop == kLoopTypeHoldLast) {
			glPopMatrix();
		}
	}
	glPopMatrix();
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1., 1., 1., 1.);	
}

- (BOOL) alive
{
	if ((_currentFrame < 0. || _currentFrame >= (float)_frameCount) && (_loop == kLoopTypeHoldLast)) {
		if (_holdAlphaDelta > 0.f && _holdAlpha >= _holdAlphaEnd) {
			return NO;
		} else if (_holdAlphaDelta < 0.f && _holdAlpha <= _holdAlphaEnd) {
			return NO;
		}
		if (_holdScaleDelta > 0.f && _holdScale > _holdAlphaEnd) {
			return NO;
		} else if (_holdScaleDelta < 0.f && _holdScale < _holdAlphaEnd) {
			return NO;
		}
	}

	return YES;
}
@end