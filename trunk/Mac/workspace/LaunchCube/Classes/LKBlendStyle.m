//
//  LKBlendStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKBlendStyle.h"
#import "LKStyleContext.h"


@implementation LKBlendStyle

@synthesize blendMode;

- (id)initWithNext:(LKStyle *)aNext {
	if (self = [super initWithNext:aNext]) {
		blendMode = kCGBlendModeNormal;
	}
	
	return self;
}


#pragma mark -
#pragma mark Class public

+ (LKBlendStyle *)styleWithBlend:(CGBlendMode)blendMode next:(LKStyle *)next {
	LKBlendStyle * style = [[[self alloc] initWithNext:next] autorelease];
	style.blendMode = blendMode;
	return style;
}


#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	if (blendMode) {
		CGContextRef ctx = UIGraphicsGetCurrentContext();
		CGContextSaveGState(ctx);
		CGContextSetBlendMode(ctx, blendMode);
		
		[self.next draw:context];
		CGContextRestoreGState(ctx);
		
	} else {
		return [self.next draw:context];
	}
}

@end
