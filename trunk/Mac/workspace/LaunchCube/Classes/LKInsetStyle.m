//
//  LKInsetStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKInsetStyle.h"
#import "LKStyleContext.h"


@implementation LKInsetStyle

@synthesize inset;

- (id)initWithNext:(LKStyle *)aNext {
	if (self = [super initWithNext:aNext]) {
		inset = UIEdgeInsetsZero;
	}
	
	return self;
}


#pragma mark -
#pragma mark Class public

+ (LKInsetStyle *)styleWithInset:(UIEdgeInsets)inset next:(LKStyle *)next {
	LKInsetStyle * style = [[[self alloc] initWithNext:next] autorelease];
	style.inset = inset;
	return style;
}


#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	CGRect rect = context.frame;
	context.frame = CGRectMake(rect.origin.x+inset.left, rect.origin.y+inset.top,
							   rect.size.width - (inset.left + inset.right),
							   rect.size.height - (inset.top + inset.bottom));
	[self.next draw:context];
}

- (UIEdgeInsets)addToInsets:(UIEdgeInsets)insets forSize:(CGSize)size {
	insets.top += inset.top;
	insets.right += inset.right;
	insets.bottom += inset.bottom;
	insets.left += inset.left;
	if (self.next) {
		return [self.next addToInsets:insets forSize:size];
	} else {
		return insets;
	}
}

@end
