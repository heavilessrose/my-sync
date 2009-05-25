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
	
	srandom(time(NULL));
	
	path = [[NSBezierPath alloc] init];
	[path setLineWidth:3.0];
	NSPoint p = [self randomPoint];
	[path moveToPoint:p];
	int i;
    for (i = 0; i < 15; i++) {
		p = [self randomPoint];
		[path lineToPoint:p];
	}
	[path closePath];
	return self;
}

- (void)dealloc
{
	[path release];
	[super dealloc];
}

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

}

@end
