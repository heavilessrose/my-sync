//
//  LKStyledImageFrame.m
//  LKStyledText
//
//  Created by luke on 10-12-1.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledImageFrame.h"
#import "LKStyledImageNode.h"


@implementation LKStyledImageFrame

- (void)drawInRect:(CGRect)aRect {
	
	if ([self.node isKindOfClass:[LKStyledImageNode class]]) {
		LKStyledImageNode *imgNode = (LKStyledImageNode *)node;
		[imgNode.image drawInRect:aRect];
	}
}

@end
