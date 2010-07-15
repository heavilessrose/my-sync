//
//  pcTwoLineAlertView.m
//  Picoffee
//
//  Created by luke on 10-7-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "pcTwoLineAlertView.h"


@implementation pcTwoLineAlertView


- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		UIColor *color = [UIColor whiteColor];
		/*
		nameLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		nameLabel.text = @"Name";
		*/
		nameField = [[UITextField alloc] initWithFrame:CGRectZero];
		nameField.backgroundColor = color;
		nameField.opaque = YES;
		nameField.placeholder = @"Name";
		
		passwordField = [[UITextField alloc] initWithFrame:CGRectZero];
		passwordField.backgroundColor = color;
		passwordField.opaque = YES;
		passwordField.placeholder = @"Password";
		
		passwordConfirmField = [[UITextField alloc] initWithFrame:CGRectZero];
		passwordConfirmField.backgroundColor = color;
		passwordConfirmField.opaque = YES;
		passwordConfirmField.placeholder = @"Confirm";
		
		[self addSubview:nameField];
		[self addSubview:passwordField];
		[self addSubview:passwordConfirmField];
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (void)dealloc {
    [super dealloc];
}

- (void)setFrame:(CGRect)rect {
	[super setFrame:CGRectMake(0, 0, rect.size.width, 300)];
	self.center = CGPointMake(320/2, 480/2);
}

- (void)layoutSubviews {
	CGFloat buttonTop;
	for (UIView *view in self.subviews) {
		if ([[[view class] description] isEqualToString:@"UIThreePartButton"]) {
			view.frame = CGRectMake(view.frame.origin.x, self.bounds.size.height - view.frame.size.height - 15, view.frame.size.width, view.frame.size.height);
			buttonTop = view.frame.origin.y;
		}
	}
	
	buttonTop -= 7;
	buttonTop -= 60;
	passwordConfirmField.frame = CGRectMake(12, buttonTop, self.frame.size.width - 53, 30);
	
	buttonTop -= 7;
	buttonTop -= 30;
	passwordField.frame = CGRectMake(12, buttonTop, self.frame.size.width - 52, 30);
	
	buttonTop -= 7;
	buttonTop -= 30;
	nameField.frame = CGRectMake(12, buttonTop, self.frame.size.width - 52, 30);
}

@end
