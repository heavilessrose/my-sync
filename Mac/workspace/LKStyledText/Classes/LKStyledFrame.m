//
//  LKStyledFrame.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledFrame.h"
#import "LKStyledLinkNode.h"


@implementation LKStyledFrame

@synthesize frame, selected;
@synthesize node, nextFrame, childFrame;

// implement by subclasses
- (void)drawInRect:(CGRect)aRect {
	
}

- (LKStyledFrame *)touchCheck:(CGPoint)aPoint {
	
	DLog(@"");
	if ([node isKindOfClass:[LKStyledLinkNode class]] && CGRectContainsPoint(self.frame, aPoint)) {
		return self;
	}
	return [self.nextFrame touchCheck:aPoint];
}

- (NSString *)description {
	
	return [NSString stringWithFormat:@"(%.0f, %.0f, %.0f, %.0f)", frame.origin.x, frame.origin.y, frame.size.width, frame.size.height];
}

- (void)dealloc {
	
	[childFrame release];
	[nextFrame release];
	[super dealloc];
}

@end
