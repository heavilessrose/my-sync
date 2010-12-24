//
//  LKShape.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKShape.h"


@implementation LKShape

- (void)openPath:(CGRect)rect {
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSaveGState(context);
	CGContextTranslateCTM(context, CGRectGetMinX(rect), CGRectGetMinY(rect));
	CGContextBeginPath(context);
}

- (void)closePath:(CGRect)rect {
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextClosePath(context);
	CGContextRestoreGState(context);
}

- (UIEdgeInsets)insetsForSize:(CGSize)size {
	return UIEdgeInsetsZero;
}

- (void)addTopEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource {
}

- (void)addRightEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource {
}

- (void)addBottomEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource {
}

- (void)addLeftEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource {
}

- (void)addToPath:(CGRect)rect {
}

- (void)addInverseToPath:(CGRect)rect {
}

@end
