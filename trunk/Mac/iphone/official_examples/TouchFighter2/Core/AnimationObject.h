/*
File: AnimationObject.h
Abstract: An animation (used for explosions).

Version: 2.0
*/

#import "SpriteObject.h"
#import "Utils.h"

typedef enum _LoopType {
	kLoopTypeNone,
	kLoopTypeRepeat,
	kLoopTypeReverse,
	kLoopTypeHoldLast,
	kLoopTypeCount
} LoopType;

/* An animation cycles through subsquares in a texture which each describe an individual frame. */

@interface AnimationObject : SpriteObject {
@private
	CGFloat			_currentFrame,
					_step,
					_holdScaleEnd,
					_holdScale,
					_holdScaleDelta,
					_holdAlphaEnd,
					_holdAlpha,
					_holdAlphaDelta;
	NSUInteger		_frameCount, _pixelsWide, _pixelsHigh;
	BOOL			_countingUp;
	LoopType		_loop;
}

- (id) initWithTexture:(Texture2D*)tex frameCount:(NSUInteger)count;

@property(readonly) Texture2D*			texture;
@property CGFloat				holdScaleEnd, holdScaleDelta, holdAlphaEnd, holdAlphaDelta, step, currentFrame, holdAlpha, holdScale;
@property NSUInteger			frameCount;
@property LoopType				loop;
@property BOOL					countingUp;

/* return NO if the animation has finished and we should no longer render, YES otherwise */
@property(readonly) BOOL				alive;
@end
