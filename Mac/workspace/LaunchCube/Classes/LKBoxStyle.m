//
//  LKBoxStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKBoxStyle.h"
#import "LKRects.h"
#import "LKStyleContext.h"


@implementation LKBoxStyle

@synthesize margin, padding, minSize, position;

- (id)initWithNext:(LKStyle *)aNext {
	if (self = [super initWithNext:aNext]) {
		margin = UIEdgeInsetsZero;
		padding = UIEdgeInsetsZero;
		minSize = CGSizeZero;
		position = LKPosition_Static;
	}
	
	return self;
}
#pragma mark -
#pragma mark Class public

+ (LKBoxStyle *)styleWithMargin:(UIEdgeInsets)margin next:(LKStyle *)next {
	LKBoxStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.margin = margin;
	return style;
}

+ (LKBoxStyle *)styleWithPadding:(UIEdgeInsets)padding next:(LKStyle *)next {
	LKBoxStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.padding = padding;
	return style;
}

+ (LKBoxStyle *)styleWithFloats:(LKPosition)position next:(LKStyle *)next {
	LKBoxStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.position = position;
	return style;
}

+ (LKBoxStyle *)styleWithMargin:(UIEdgeInsets)margin padding:(UIEdgeInsets)padding
                          next:(LKStyle *)next {
	LKBoxStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.margin = margin;
	style.padding = padding;
	return style;
}

+ (LKBoxStyle *)styleWithMargin:(UIEdgeInsets)margin padding:(UIEdgeInsets)padding
                       minSize:(CGSize)minSize position:(LKPosition)position next:(LKStyle *)next {
	LKBoxStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.margin = margin;
	style.padding = padding;
	style.minSize = minSize;
	style.position = position;
	return style;
}

#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	context.contentFrame = LKRectInset(context.contentFrame, padding);
	[self.next draw:context];
}

- (CGSize)addToSize:(CGSize)size context:(LKStyleContext *)context {
	size.width += padding.left + padding.right;
	size.height += padding.top + padding.bottom;
	
	if (next) {
		return [self.next addToSize:size context:context];
		
	} else {
		return size;
	}
}


@end
