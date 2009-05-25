//
//  StretchView.m
//  ImageFun
//
//  Created by Aaron Hillegass on 10/17/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "StretchView.h"


@implementation StretchView


- (id)initWithFrame:(NSRect)frame {
    if (![super initWithFrame:frame]) 
		return nil;
	
	NSPoint p;
	srandom(time(NULL));
	
	path = [[NSBezierPath alloc] init];
	[path setLineWidth:3.0];
	p = [self randomPoint];
	[path moveToPoint:p];
	int i;
    for (i = 0; i < 15; i++) {
		p = [self randomPoint];
		[path lineToPoint:p];
	}
	[path closePath];
	
	opacity = 1.0;
	return self;
}

- (void)dealloc
{
	[path release];
	[super dealloc];
}

#pragma mark Drawing

- (NSPoint)randomPoint
{
	NSRect bounds = [self bounds];
	NSPoint result;
	result.x = bounds.origin.x + random() % (int)bounds.size.width;
	result.y = bounds.origin.y + random() % (int)bounds.size.height;
	return result;
}

- (void)drawRect:(NSRect)rect {
	NSRect bounds = [self bounds];
	[[NSColor greenColor] set];
	[NSBezierPath fillRect:bounds];
	
	[[NSColor whiteColor] set];
	[path fill];
	
	if (image) {
		NSRect imageRect;
		imageRect.origin = NSZeroPoint;
		imageRect.size = [image size];
		NSRect drawingRect = [self currentRect];
		[image drawInRect:drawingRect
				 fromRect:imageRect
				operation:NSCompositeSourceOver
				 fraction:opacity];
	}
}

#pragma mark Events

- (void)mouseDown:(NSEvent *)event
{
	NSPoint p = [event locationInWindow];
	downPoint = [self convertPoint:p fromView:nil];
	currentPoint = downPoint;
	[self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)e
{
	NSPoint p = [e locationInWindow];
	currentPoint = [self convertPoint:p fromView:nil];
	[self autoscroll:e];
	[self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent *)e
{
	NSPoint p = [e locationInWindow];
	currentPoint = [self convertPoint:p fromView:nil];
	[self setNeedsDisplay:YES];
}

- (NSRect)currentRect
{
	float minX = MIN(downPoint.x, currentPoint.x);
	float minY = MIN(downPoint.y, currentPoint.y);
	float maxX = MAX(downPoint.x, currentPoint.x);
	float maxY = MAX(downPoint.y, currentPoint.y);
	return NSMakeRect(minX, minY, maxX - minX, maxY - minY);
	
}

#pragma Accessors

- (void)setImage:(NSImage *)i
{
	[i retain];
	[image release];
	image = i;
	NSSize imageSize = [i size];
	downPoint = NSZeroPoint;
	currentPoint.x = downPoint.x + imageSize.width;
	currentPoint.y = downPoint.y + imageSize.height;
	[self setNeedsDisplay:YES];
}

- (float)opacity
{
	return opacity;
}
- (void)setOpacity:(float)f
{
	opacity = f;
	[self setNeedsDisplay:YES];
}

@end
