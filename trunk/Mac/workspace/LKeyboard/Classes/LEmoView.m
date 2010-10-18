//
//  LEmoView.m
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LEmoView.h"
#import "LKeyboardAppDelegate.h"

@interface LEmoView ()

@property (nonatomic, retain) UIButton *emoButton;

@end


@implementation LEmoView

@synthesize emoButton, emoIndex, delegate;

- (id)initWithFrame:(CGRect)frame emoImg:(UIImage *)emoImg
{
    if ((self = [super initWithFrame:frame])) {
		self.backgroundColor = [UIColor whiteColor];
		self.emoButton = [UIButton buttonWithType:UIButtonTypeCustom];
		
		CGRect emoButtonRect = CGRectMake(0, 0, frame.size.width, frame.size.height);
		[emoButton setFrame:emoButtonRect];
		[emoButton setImage:emoImg forState:UIControlStateNormal];
		[emoButton addTarget:self action:@selector(emoPressed) forControlEvents:UIControlEventTouchDown];
		[self addSubview:emoButton];
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


- (void)layoutSubviews
{
	
}

- (void)dealloc
{
	[emoButton release];
	
    [super dealloc];
}

- (void)emoPressed
{
	if ([self.delegate respondsToSelector:@selector(emoSelected:)]) {
		NSArray *emoStrs = [[[LKeyboardAppDelegate globalAppDelegate].emoStrings objectAtIndex:emoIndex] componentsSeparatedByString:@","];
		NSLog(@"emoStrs: %@", emoStrs);
		[self.delegate emoSelected:emoStrs];
	}
}

@end
