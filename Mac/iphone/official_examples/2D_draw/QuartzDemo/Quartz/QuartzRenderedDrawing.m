/*
     File: QuartzRenderedDrawing.m
 Abstract: Demonstrations of using Quartz for drawing gradients (GradientDrawing) and Patterns (PatternDrawing).
  Version: 2.1
 
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

#import "QuartzRenderedDrawing.h"

@implementation GradientDrawing

-(id)init
{
	self = [super init];
	if(self != nil)
	{
		CGColorSpaceRef rgb = CGColorSpaceCreateDeviceRGB();
		CGFloat colors[] =
		{
			204.0 / 255.0, 224.0 / 255.0, 244.0 / 255.0, 1.00,
			 29.0 / 255.0, 156.0 / 255.0, 215.0 / 255.0, 1.00,
			  0.0 / 255.0,  50.0 / 255.0, 126.0 / 255.0, 1.00,
		};
		gradient = CGGradientCreateWithColorComponents(rgb, colors, NULL, sizeof(colors)/(sizeof(colors[0])*4));
		CGColorSpaceRelease(rgb);
	}
	return self;
}

-(void)dealloc
{
	CGGradientRelease(gradient);
	[super dealloc];
}

// Returns an appropriate starting point for the demonstration of a linear gradient
CGPoint demoLGStart(CGRect bounds)
{
	return CGPointMake(bounds.origin.x, bounds.origin.y + bounds.size.height * 0.25);
}

// Returns an appropriate ending point for the demonstration of a linear gradient
CGPoint demoLGEnd(CGRect bounds)
{
	return CGPointMake(bounds.origin.x, bounds.origin.y + bounds.size.height * 0.75);
}

// Returns the center point for a radial gradient
CGPoint demoRGCenter(CGRect bounds)
{
	return CGPointMake(CGRectGetMidX(bounds), CGRectGetMidY(bounds));
}

// Returns an appropriate inner radius for the demonstration of the radial gradient
CGFloat demoRGInnerRadius(CGRect bounds)
{
	CGFloat r = bounds.size.width < bounds.size.height ? bounds.size.width : bounds.size.height;
	return r * 0.125;
}

// Returns an appropriate outer radius for the demonstration of the radial gradient
CGFloat demoRGOuterRadius(CGRect bounds)
{
	CGFloat r = bounds.size.width < bounds.size.height ? bounds.size.width : bounds.size.height;
	return r * 0.5;
}

-(void)drawView:(QuartzView*)view inContext:(CGContextRef)context bounds:(CGRect)bounds
{
	// The clipping rects we plan to use, which also defines the locations of each gradient
	CGRect clips[] =
	{
		CGRectMake(10.0, 30.0, 60.0, 90.0),
		CGRectMake(90.0, 30.0, 60.0, 90.0),
		CGRectMake(170.0, 30.0, 60.0, 90.0),
		CGRectMake(250.0, 30.0, 60.0, 90.0),
		CGRectMake(30.0, 140.0, 120.0, 120.0),
		CGRectMake(170.0, 140.0, 120.0, 120.0),
		CGRectMake(30.0, 280.0, 120.0, 120.0),
		CGRectMake(170.0, 280.0, 120.0, 120.0),
	};

	// Linear Gradients
	CGPoint start, end;

	// Clip to area to draw the gradient, and draw it. Since we are clipping, we save the graphics state
	// so that we can revert to the previous larger area.
	CGContextSaveGState(context);
	CGContextClipToRect(context, clips[0]);
	
	// A linear gradient requires only a starting & ending point.
	// The colors of the gradient are linearly interpolated along the line segment connecting these two points
	// A gradient location of 0.0 means that color is expressed fully at the 'start' point
	// a location of 1.0 means that color is expressed fully at the 'end' point.
	// The gradient fills outwards perpendicular to the line segment connectiong start & end points
	// (which is why we need to clip the context, or the gradient would fill beyond where we want it to).
	// The gradient options (last) parameter determines what how to fill the clip area that is "before" and "after"
	// the line segment connecting start & end.
	start = demoLGStart(clips[0]);
	end = demoLGEnd(clips[0]);
	CGContextDrawLinearGradient(context, gradient, start, end, 0);
	CGContextRestoreGState(context);
	
	CGContextSaveGState(context);
	CGContextClipToRect(context, clips[1]);
	start = demoLGStart(clips[1]);
	end = demoLGEnd(clips[1]);
	CGContextDrawLinearGradient(context, gradient, start, end, kCGGradientDrawsBeforeStartLocation);
	CGContextRestoreGState(context);
	
	CGContextSaveGState(context);
	CGContextClipToRect(context, clips[2]);
	start = demoLGStart(clips[2]);
	end = demoLGEnd(clips[2]);
	CGContextDrawLinearGradient(context, gradient, start, end, kCGGradientDrawsAfterEndLocation);
	CGContextRestoreGState(context);
	
	CGContextSaveGState(context);
	CGContextClipToRect(context, clips[3]);
	start = demoLGStart(clips[3]);
	end = demoLGEnd(clips[3]);
	CGContextDrawLinearGradient(context, gradient, start, end, kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
	CGContextRestoreGState(context);
	
	// Radial Gradients
	
	CGFloat startRadius, endRadius;

	CGContextSaveGState(context);
	CGContextClipToRect(context, clips[4]);
	
	// A radial gradient requires a start & end point as well as a start & end radius.
	// Logically a radial gradient is created by linearly interpolating the center, radius and color of each
	// circle using the start and end point for the center, start and end radius for the radius, and the color ramp
	// inherant to the gradient to create a set of stroked circles that fill the area completely.
	// The gradient options specify if this interpolation continues past the start or end points as it does with
	// linear gradients.
	start = end = demoRGCenter(clips[4]);
	startRadius = demoRGInnerRadius(clips[4]);
	endRadius = demoRGOuterRadius(clips[4]);
	CGContextDrawRadialGradient(context, gradient, start, startRadius, end, endRadius, 0);
	CGContextRestoreGState(context);

	CGContextSaveGState(context);
	CGContextClipToRect(context, clips[5]);
	start = end = demoRGCenter(clips[5]);
	startRadius = demoRGInnerRadius(clips[5]);
	endRadius = demoRGOuterRadius(clips[5]);
	CGContextDrawRadialGradient(context, gradient, start, startRadius, end, endRadius, kCGGradientDrawsBeforeStartLocation);
	CGContextRestoreGState(context);

	CGContextSaveGState(context);
	CGContextClipToRect(context, clips[6]);
	start = end = demoRGCenter(clips[6]);
	startRadius = demoRGInnerRadius(clips[6]);
	endRadius = demoRGOuterRadius(clips[6]);
	CGContextDrawRadialGradient(context, gradient, start, startRadius, end, endRadius, kCGGradientDrawsAfterEndLocation);
	CGContextRestoreGState(context);

	CGContextSaveGState(context);
	CGContextClipToRect(context, clips[7]);
	start = end = demoRGCenter(clips[7]);
	startRadius = demoRGInnerRadius(clips[7]);
	endRadius = demoRGOuterRadius(clips[7]);
	CGContextDrawRadialGradient(context, gradient, start, startRadius, end, endRadius, kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
	CGContextRestoreGState(context);
	
	// Show the clipping areas
	CGContextSetLineWidth(context, 2.0);
	CGContextSetRGBStrokeColor(context, 1.0, 0.0, 0.0, 1.0);
	CGContextAddRects(context, clips, sizeof(clips)/sizeof(clips[0]));
	CGContextStrokePath(context);
}

@end

@implementation PatternDrawing

// Colored patterns specify colors as part of their drawing
void ColoredPatternCallback(void *info, CGContextRef context)
{
	CGContextSetRGBFillColor(context, 29.0 / 255.0, 156.0 / 255.0, 215.0 / 255.0, 1.00);
	// Upper Left
	CGContextFillRect(context, CGRectMake(0.0, 0.0, 8.0, 8.0));
	// Lower Right
	CGContextFillRect(context, CGRectMake(8.0, 8.0, 8.0, 8.0));
	
	CGContextSetRGBFillColor(context, 204.0 / 255.0, 224.0 / 255.0, 244.0 / 255.0, 1.00);
	// Upper Right
	CGContextFillRect(context, CGRectMake(8.0, 0.0, 8.0, 8.0));
	// Lower Left
	CGContextFillRect(context, CGRectMake(0.0, 8.0, 8.0, 8.0));
}

// Uncolored patterns take their color from the given context
void UncoloredPatternCallback(void *info, CGContextRef context)
{
	CGContextFillRect(context, CGRectMake(0.0, 0.0, 8.0, 8.0));
	CGContextFillRect(context, CGRectMake(8.0, 8.0, 8.0, 8.0));
}

-(id)init
{
	self = [super init];
	if(self != nil)
	{
		// Colored Pattern setup
		CGPatternCallbacks coloredPatternCallbacks = {0, ColoredPatternCallback, NULL};
		// First we need to create a CGPatternRef that specifies the qualities of our pattern.
		CGPatternRef coloredPattern = CGPatternCreate(
			NULL, // 'info' pointer for our callback
			CGRectMake(0.0, 0.0, 16.0, 16.0), // the pattern coordinate space, drawing is clipped to this rectangle
			CGAffineTransformIdentity, // a transform on the pattern coordinate space used before it is drawn.
			16.0, 16.0, // the spacing (horizontal, vertical) of the pattern - how far to move after drawing each cell
			kCGPatternTilingNoDistortion,
			true, // this is a colored pattern, which means that you only specify an alpha value when drawing it
			&coloredPatternCallbacks); // the callbacks for this pattern.
		
		// To draw a pattern, you need a pattern colorspace.
		// Since this is an colored pattern, the parent colorspace is NULL, indicating that it only has an alpha value.
		CGColorSpaceRef coloredPatternColorSpace = CGColorSpaceCreatePattern(NULL);
		CGFloat alpha = 1.0;
		// Since this pattern is colored, we'll create a CGColorRef for it to make drawing it more efficient.
		// From here on, the colored pattern is referenced entirely via the associated CGColorRef rather than the
		// originally created CGPatternRef.
		coloredPatternColor = CGColorCreateWithPattern(coloredPatternColorSpace, coloredPattern, &alpha);
		CGColorSpaceRelease(coloredPatternColorSpace);
		CGPatternRelease(coloredPattern);
		
		// Uncolored Pattern setup
		CGPatternCallbacks uncoloredPatternCallbacks = {0, UncoloredPatternCallback, NULL};
		// As above, we create a CGPatternRef that specifies the qualities of our pattern
		uncoloredPattern = CGPatternCreate(
			NULL, // 'info' pointer
			CGRectMake(0.0, 0.0, 16.0, 16.0), // coordinate space
			CGAffineTransformIdentity, // transform
			16.0, 16.0, // spacing
			kCGPatternTilingNoDistortion,
			false, // this is an uncolored pattern, thus to draw it we need to specify both color and alpha
			&uncoloredPatternCallbacks); // callbacks for this pattern
		
		// With an uncolored pattern we still need to create a pattern colorspace, but now we need a parent colorspace
		// We'll use the DeviceRGB colorspace here. We'll need this colorspace along with the CGPatternRef to draw this pattern later.
		CGColorSpaceRef deviceRGB = CGColorSpaceCreateDeviceRGB();
		uncoloredPatternColorSpace = CGColorSpaceCreatePattern(deviceRGB);
		CGColorSpaceRelease(deviceRGB);
	}
	return self;
}

-(void)dealloc
{
	CGColorRelease(coloredPatternColor);
	CGPatternRelease(uncoloredPattern);
	CGColorSpaceRelease(uncoloredPatternColorSpace);
	[super dealloc];
}

-(void)drawView:(QuartzView*)view inContext:(CGContextRef)context bounds:(CGRect)bounds
{
	// Draw the colored pattern. Since we have a CGColorRef for this pattern, we just set
	// that color current and draw.
	CGContextSetFillColorWithColor(context, coloredPatternColor);
	CGContextFillRect(context, CGRectMake(10.0, 10.0, 90.0, 90.0));
	
	// You can also stroke with a pattern.
	CGContextSetStrokeColorWithColor(context, coloredPatternColor);
	CGContextStrokeRectWithWidth(context, CGRectMake(120.0, 10.0, 90.0, 90.0), 8.0);
	
	// Since we aren't encapsulating our pattern in a CGColorRef for the uncolored pattern case, setup requires two steps.
	// First you have to set the context's current colorspace (fill or stroke) to a pattern colorspace,
	// indicating to Quartz that you want to draw a pattern.
	CGContextSetFillColorSpace(context, uncoloredPatternColorSpace);
	// Next you set the pattern and the color that you want the pattern to draw with.
	CGFloat color1[] = {1.0, 0.0, 0.0, 1.0};
	CGContextSetFillPattern(context, uncoloredPattern, color1);
	// And finally you draw!
	CGContextFillRect(context, CGRectMake(10.0, 120.0, 90.0, 90.0));
	// As long as the current colorspace is a pattern colorspace, you are free to change the pattern or pattern color
	CGFloat color2[] = {0.0, 1.0, 0.0, 1.0};
	CGContextSetFillPattern(context, uncoloredPattern, color2);
	CGContextFillRect(context, CGRectMake(10.0, 230.0, 90.0, 90.0));
	
	// And of course, just like the colored case, you can stroke with a pattern as well.
	CGContextSetStrokeColorSpace(context, uncoloredPatternColorSpace);
	CGContextSetStrokePattern(context, uncoloredPattern, color1);
	CGContextStrokeRectWithWidth(context, CGRectMake(120.0, 120.0, 90.0, 90.0), 8.0);
	// As long as the current colorspace is a pattern colorspace, you are free to change the pattern or pattern color
	CGContextSetStrokePattern(context, uncoloredPattern, color2);
	CGContextStrokeRectWithWidth(context, CGRectMake(120.0, 230.0, 90.0, 90.0), 8.0);
}

@end