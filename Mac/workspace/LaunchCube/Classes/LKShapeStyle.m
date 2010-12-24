//
//  LKShapeStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKShapeStyle.h"
#import "LKShape.h"
#import "LKStyleContext.h"
#import "LKRects.h"


@implementation LKShapeStyle

@synthesize shape;

- (void)dealloc {
	self.shape =nil;
	
	[super dealloc];
}


#pragma mark -
#pragma mark Class public

+ (LKShapeStyle *)styleWithShape:(LKShape *)shape next:(LKStyle *)next {
	LKShapeStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.shape = shape;
	return style;
}


#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	UIEdgeInsets shapeInsets = [shape insetsForSize:context.frame.size];
	context.contentFrame = LKRectInset(context.contentFrame, shapeInsets);
	context.shape = shape;
	[self.next draw:context];
}

- (UIEdgeInsets)addToInsets:(UIEdgeInsets)insets forSize:(CGSize)size {
	UIEdgeInsets shapeInsets = [shape insetsForSize:size];
	insets.top += shapeInsets.top;
	insets.right += shapeInsets.right;
	insets.bottom += shapeInsets.bottom;
	insets.left += shapeInsets.left;
	
	if (self.next) {
		return [self.next addToInsets:insets forSize:size];
		
	} else {
		return insets;
	}
}

- (CGSize)addToSize:(CGSize)size context:(LKStyleContext *)context {
	CGSize innerSize = [self.next addToSize:size context:context];
	UIEdgeInsets shapeInsets = [shape insetsForSize:innerSize];
	innerSize.width += shapeInsets.left + shapeInsets.right;
	innerSize.height += shapeInsets.top + shapeInsets.bottom;
	
	return innerSize;
}

@end
