/*

===== IMPORTANT =====

This is sample code demonstrating API, technology or techniques in development.
Although this sample code has been reviewed for technical accuracy, it is not
final. Apple is supplying this information to help you plan for the adoption of
the technologies and programming interfaces described herein. This information
is subject to change, and software implemented based on this sample code should
be tested with final operating system software and final documentation. Newer
versions of this sample code may be provided with future seeds of the API or
technology. For information about updates to this and other developer
documentation, view the New & Updated sidebars in subsequent documentation
seeds.

=====================

File: Explosion.m
Abstract: Explosion animation.

Version: 2.0

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

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