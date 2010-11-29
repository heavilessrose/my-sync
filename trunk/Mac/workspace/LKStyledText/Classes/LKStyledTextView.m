//
//  LKStyledTextView.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledTextView.h"


@implementation LKStyledTextView

@synthesize stext, contentInset;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		
	}
    return self;
}

// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
	
	
	CGPoint origin = CGPointMake(rect.origin.x + contentInset.left,
								 rect.origin.y + contentInset.top);
	[stext drawAtPoint:origin];
}

- (void)dealloc {
	
	[stext release];
    [super dealloc];
}


@end
