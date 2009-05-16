/*
File: Explosion.h
Abstract: Explosion animation.

Version: 2.0
*/

#import "AnimationObject.h"

/* Explosion animation. */

@interface Explosion : PhysicalObject {
@private
	AnimationObject*		_front, *_middle, *_back;
	NSUInteger				_pixelsWide, _pixelsHigh, _currentFrame;
	CGFloat					_rotateFront, _rotateMiddle, _rotateBack;
}

@property(readonly) BOOL			alive;
@end
