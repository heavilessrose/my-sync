/*
File: Explosion.m
Abstract: Explosion animation.

Version: 2.0
*/

#import "Explosion.h"

@implementation Explosion

- (id) initWithTexture:(Texture2D*)tex frameCount:(int)count
{
	if ((self = [super init]))
	{
		_front = [[AnimationObject alloc] initWithTexture:tex frameCount:count];
		_front.loop = kLoopTypeHoldLast;
		_front.step = 0.40;
		_front.holdAlphaDelta = -.01;
		_front.holdScaleDelta = .01;
		_middle = [[AnimationObject alloc] initWithTexture:tex frameCount:count];
		_middle.loop = kLoopTypeHoldLast;
		_middle.step = 0.45;
		_back = [[AnimationObject alloc] initWithTexture:tex frameCount:count];
		_back.loop = kLoopTypeHoldLast;
		_back.step = 0.50;
		
		_front.rotation = Point3Make(0., 0., fast_unit_rand() * 360.);
		_front.width = _front.height = .5;
		_middle.rotation = Point3Make(0., 0., fast_unit_rand() * 360.);
		_middle.width = _middle.height = .75;
		_back.rotation = Point3Make(0., 0., fast_unit_rand() * 360.);
		_back.width = _back.height = 1.0;
		
		_currentFrame = 0;
		
		self.scale = Point3Make(10., 10., 1.);
	}
	return self;
}

- (void) dealloc
{
	[_front release];
	[_middle release];
	[_back release];
	[super dealloc];
}

- (void) renderObjectAtTime:(NSTimeInterval)time
{
	_back.position = Point3Make(0., 0., -(.5 + _currentFrame * .05));
	[_back renderAtTime:time];
	[_middle renderAtTime:time];
	_front.position = Point3Make(0., 0., +(.5 + _currentFrame * .05));
	[_front renderAtTime:time];
	_currentFrame++;
}

- (BOOL) alive
{
	if (![_front alive]) {
		[_front release];
		_front = nil;
	}
	if (![_middle alive]) {
		[_middle release];
		_middle = nil;
	}
	if (![_back alive]) {
		[_back release];
		_back = nil;
	}
	
	return _front || _middle || _back;
}

@end