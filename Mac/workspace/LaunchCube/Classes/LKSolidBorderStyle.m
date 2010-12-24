//
//  LKSolidBorderStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKSolidBorderStyle.h"
#import "LKStyle.h"
#import "LKStyleContext.h"
#import "LKShape.h"


@implementation LKSolidBorderStyle

@synthesize color, width;

- (id)initWithNext:(LKStyle *)aNext {
	if (self = [super initWithNext:aNext]) {
		width = 1;
	}
	
	return self;
}

- (void)dealloc {
	self.color = nil;
	
	[super dealloc];
}


#pragma mark -
#pragma mark Class public

+ (LKSolidBorderStyle *)styleWithColor:(UIColor*)color width:(CGFloat)width next:(LKStyle *)next {
	LKSolidBorderStyle * style = [[[self alloc] initWithNext:next] autorelease];
	style.color = color;
	style.width = width;
	return style;
}


#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSaveGState(ctx);
	
	CGRect strokeRect = CGRectInset(context.frame, width/2, width/2);
	[context.shape addToPath:strokeRect];
	
	[color setStroke];
	CGContextSetLineWidth(ctx, width);
	CGContextStrokePath(ctx);
	
	CGContextRestoreGState(ctx);
	
	context.frame = CGRectInset(context.frame, width, width);
	return [self.next draw:context];
}

@end
