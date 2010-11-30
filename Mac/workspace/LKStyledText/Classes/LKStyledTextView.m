//
//  LKStyledTextView.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledTextView.h"
#import "LKStyledTextFrame.h"


@implementation LKStyledTextView

@synthesize stext, contentInset, selectedFrame;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		CGFloat top = 100, left = 0, bottom = 20, right = 0;
		contentInset = UIEdgeInsetsMake(top, left, bottom, right);
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

#pragma mark -
#pragma mark touch handle

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	UITouch *touch = [touches anyObject];
	CGPoint point = [touch locationInView:self];
	point.x -= contentInset.left;
	point.y -= contentInset.top;
	
	self.selectedFrame = [stext touchCheck:point];
	selectedFrame.selected = YES;
	if ([selectedFrame isKindOfClass:[LKStyledTextFrame class]]) {
		NSArray *siblings = ((LKStyledTextFrame *)selectedFrame).siblings;
		if (siblings) {
			for (LKStyledFrame *aFrame in siblings) {
				aFrame.selected = YES;
			}
		}
	}
	DLog(@"touch: (%.0f, %.0f), selectedFrame: %@", point.x, point.y, self.selectedFrame);
	[self setNeedsDisplay];
	
	[super touchesBegan:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	UITouch *touch = [touches anyObject];
	CGPoint point = [touch locationInView:self];
	point.x -= contentInset.left;
	point.y -= contentInset.top;
	selectedFrame.selected = NO;
	if ([selectedFrame isKindOfClass:[LKStyledTextFrame class]]) {
		NSArray *siblings = ((LKStyledTextFrame *)selectedFrame).siblings;
		if (siblings) {
			for (LKStyledFrame *aFrame in siblings) {
				aFrame.selected = NO;
			}
		}
	}
	DLog(@"touch: (%.0f, %.0f), selectedFrame: %@", point.x, point.y, self.selectedFrame);
	[self setNeedsDisplay];
	
	[super touchesEnded:touches withEvent:event];
}

@end
