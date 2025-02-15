/*

    File: LevelMeter.h
Abstract: Base level metering class
 Version: 1.0.1

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
Inc. ("Apple") in consideration of your agreement to the following
terms, and your use, installation, modification or redistribution of
this Apple software constitutes acceptance of these terms.  If you do
not agree with these terms, please do not use, install, modify or
redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and
subject to these terms, Apple grants you a personal, non-exclusive
license, under Apple's copyrights in this original Apple software (the
"Apple Software"), to use, reproduce, modify and redistribute the Apple
Software, with or without modifications, in source and/or binary forms;
provided that if you redistribute the Apple Software in its entirety and
without modifications, you must retain this notice and the following
text and disclaimers in all such redistributions of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may
be used to endorse or promote products derived from the Apple Software
without specific prior written permission from Apple.  Except as
expressly stated in this notice, no other rights or licenses, express or
implied, are granted by Apple herein, including but not limited to any
patent rights that may be infringed by your derivative works or by other
works in which the Apple Software may be incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2009 Apple Inc. All Rights Reserved.


*/

#import <UIKit/UIKit.h>

#ifndef LEVELMETER_CLAMP
#define LEVELMETER_CLAMP(min,x,max) (x < min ? min : (x > max ? max : x))
#endif

// The LevelMeterColorThreshold struct is used to define the colors for the LevelMeter, 
// and at what values each of those colors begins.
typedef struct LevelMeterColorThreshold {
	CGFloat			maxValue; // A value from 0 - 1. The maximum value shown in this color
	UIColor			*color; // A UIColor to be used for this value range
} LevelMeterColorThreshold;

@interface LevelMeter : UIView {
	NSUInteger					_numLights;
	CGFloat						_level, _peakLevel;
	LevelMeterColorThreshold	*_colorThresholds;
	NSUInteger					_numColorThresholds;
	BOOL						_vertical;
	BOOL						_variableLightIntensity;
	UIColor						*_bgColor, *_borderColor;
}

// The current level, from 0 - 1
@property						CGFloat level;

// Optional peak level, will be drawn if > 0
@property						CGFloat peakLevel;

// The number of lights to show, or 0 to show a continuous bar
@property						NSUInteger numLights;

// Whether the view is oriented V or H. This is initially automatically set based on the 
// aspect ratio of the view.
@property(getter=isVertical)	BOOL vertical;

// Whether to use variable intensity lights. Has no effect if numLights == 0.
@property						BOOL variableLightIntensity;

// The background color of the lights
@property(retain)				UIColor *bgColor;

// The border color of the lights
@property(retain)				UIColor *borderColor;

// Returns a pointer to the first LevelMeterColorThreshold struct. The number of color 
// thresholds is returned in count
- (LevelMeterColorThreshold *)colorThresholds:(NSUInteger *)count;

// Load <count> elements from <thresholds> and use these as our color threshold values.
- (void)setColorThresholds:(LevelMeterColorThreshold *)thresholds count:(NSUInteger)count;

@end
