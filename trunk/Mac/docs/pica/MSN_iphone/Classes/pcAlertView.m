//
//  pcAlertView.m
//  textfieldAlertView
//
//  Created by  apple on 09-12-31.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "pcAlertView.h"


@implementation pcAlertView
@synthesize textField;


- (id)initWithTitle: (NSString *)aTitle
			message: (NSString *)aMessage
		   delegate: (id)aDelegate
  cancelButtonTitle: (NSString*)aCancelButtonTitle
  otherButtonTitles: (NSString*)anOtherButtonTitles, ...{
    if (self = [super initWithTitle: aTitle
							message: aMessage
						   delegate: aDelegate
				  cancelButtonTitle: aCancelButtonTitle
				  otherButtonTitles: anOtherButtonTitles, nil]) {
		textField = [[UITextField alloc] initWithFrame: CGRectZero];
		[self addSubview: textField];
		textField.borderStyle = UITextBorderStyleRoundedRect;
		textField.clearButtonMode = UITextFieldViewModeWhileEditing;
		textField.font = [UIFont systemFontOfSize: 16];
		textField.keyboardAppearance = UIKeyboardAppearanceAlert;
		textField.autocapitalizationType = UITextAutocapitalizationTypeNone;
		textField.autocorrectionType = UITextAutocorrectionTypeNo;
		textField.returnKeyType = UIReturnKeyDefault;
		textField.delegate = aDelegate;
    }
    return self;
}

- (void)setFrame: (CGRect)rect {
	NSInteger aHeight = 180;
	if ([self.title length] == 0)
		aHeight -= 30;
	if ([self.message length] == 0)
		aHeight -= 30;
	[super setFrame: CGRectMake(20, 80, rect.size.width, aHeight)];
}

- (void)layoutSubviews {
	[super layoutSubviews];
	CGRect aFrame;
	CGFloat aTop= 0.0;
	CGRect aBounds = self.bounds;
	for (UIView *view in self.subviews) {
		if ([[[view class] description] isEqualToString:@"UIThreePartButton"]) {
			aFrame = view.frame;
			aFrame = CGRectMake(aFrame.origin.x, aBounds.size.height - aFrame.size.height - 15, aFrame.size.width, aFrame.size.height);
			view.frame = aFrame;
			aTop = aFrame.origin.y - 45;
		}
	}
	if (aTop <= 0) aTop = 10;
	aFrame = CGRectMake(20, aTop, aBounds.size.width - 40, 33);
	textField.frame = aFrame;
}

- (void)dealloc {
	[textField release];
    [super dealloc];
}

- (void)setKeyboardType: (NSInteger)aType{
	textField.keyboardType = aType;
}

@end
