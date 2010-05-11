//
//  pcAudioActionSheet.m
//  textfieldAlertView
//
//  Created by  apple on 09-12-31.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "pcAudioActionSheet.h"


@implementation pcAudioActionSheet
@synthesize progress, leftButton, rightButton;

- (id)initWithTitle: (NSString*) aTitle
		   delegate: (id< UIActionSheetDelegate >) aDelegate
  cancelButtonTitle: (NSString*) aCancelButtonTitle
destructiveButtonTitle: (NSString*) aDestructiveButtonTitle
  otherButtonTitles: (NSString*)anOtherButtonTitles, ...{
	self = [super initWithTitle: aTitle
					   delegate: aDelegate
			  cancelButtonTitle: aCancelButtonTitle
		 destructiveButtonTitle: aDestructiveButtonTitle
			  otherButtonTitles: anOtherButtonTitles, nil];
	if (self != nil){
		progress = [[UIProgressView alloc] initWithProgressViewStyle: UIProgressViewStyleDefault];
		[progress setProgress:0.0f];
		[self addSubview: progress];
		
		label = [[UILabel alloc] initWithFrame: CGRectZero];
		label.lineBreakMode = UILineBreakModeWordWrap;
		label.font = [UIFont systemFontOfSize: 16];
		label.backgroundColor = [UIColor clearColor];
		label.numberOfLines = 1;
		[self addSubview:label];
		
		leftButton = [[UIButton buttonWithType: UIButtonTypeRoundedRect] retain];
		[leftButton setTitle: @"left" forState: UIControlStateNormal];
		[leftButton addTarget: self
					   action: @selector(leftButtonClick:)
			 forControlEvents: UIControlEventTouchUpInside];
		[self addSubview: leftButton];

		rightButton = [[UIButton buttonWithType: UIButtonTypeRoundedRect] retain];
		[rightButton setTitle: @"right" forState: UIControlStateNormal];
		[rightButton addTarget: self
					   action: @selector(rightButtonClick:)
			 forControlEvents: UIControlEventTouchUpInside];
		[self addSubview: rightButton];
	}
	return self;
}


- (void)setFrame: (CGRect)rect {
	NSInteger aHeight = 280;
	CGRect rcScreen = [UIScreen mainScreen].bounds;
	if ([self.title length] == 0)
		aHeight -= 30;
	[super setFrame: CGRectMake(0, rcScreen.size.height - aHeight, rect.size.width, aHeight)];
}

- (void)layoutSubviews {
	[super layoutSubviews];
	CGRect rcBounds = self.bounds;
	progress.frame	= CGRectMake(20, 30, rcBounds.size.width - 40, 30);
	label.frame = progress.frame;
	leftButton.frame	= CGRectMake(20, 70, 120, 50);
	rightButton.frame	= CGRectMake(rcBounds.size.width - 40 - 120, 70, 120, 50);
}

- (void)dealloc {
	[label release];
	[progress release];
	[leftButton release];
	[rightButton release];
    [super dealloc];
}

- (void)setLabelText: (NSString*)aText{
	label.text = aText;
}


- (IBAction)leftButtonClick: (id)sender{
	if ([self.delegate respondsToSelector: @selector(leftButtonClick:)]){
		[self.delegate leftButtonClick: self];
	}
}

- (IBAction)rightButtonClick: (id)sender{
	if ([self.delegate respondsToSelector: @selector(rightButtonClick:)]){
		[self.delegate rightButtonClick: self];
	}
}

@end
