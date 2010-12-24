//
//  LKSolidFillStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKSolidFillStyle.h"
#import "LKStyleContext.h"
#import "LKShape.h"


@implementation LKSolidFillStyle

@synthesize color;

- (void)dealloc {
	self.color = nil;
	
	[super dealloc];
}


#pragma mark -
#pragma mark Class public

+ (LKSolidFillStyle *)styleWithColor:(UIColor *)color next:(LKStyle *)aNext {
	LKSolidFillStyle *style = [[[self alloc] initWithNext:aNext] autorelease];
	style.color = color;
	return style;
}


#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	
	CGContextSaveGState(ctx);
	[context.shape addToPath:context.frame];
	
	[color setFill];
	CGContextFillPath(ctx);
	CGContextRestoreGState(ctx);
	
	return [self.next draw:context];
}

@end
