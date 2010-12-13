//
//  LEmoView.m
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LEmoView.h"

@interface LEmoView ()

@property (nonatomic, retain) UIButton *emoButton;

@end


@implementation LEmoView

@synthesize emoStrings, emoButton, emoIndex, delegate;

- (id)initWithFrame:(CGRect)frame emoImg:(UIImage *)emoImg {
	
    if ((self = [super initWithFrame:frame])) {
		self.backgroundColor = [UIColor whiteColor];
		self.emoButton = [UIButton buttonWithType:UIButtonTypeCustom];
		
		CGRect emoButtonRect = CGRectMake(0, 0, frame.size.width, frame.size.height);
		[emoButton setFrame:emoButtonRect];
		[emoButton setImage:emoImg forState:UIControlStateNormal];
		[emoButton addTarget:self action:@selector(emoPressed) forControlEvents:UIControlEventTouchDown];
		[self addSubview:emoButton];
		
		self.emoStrings = [NSArray arrayWithObjects:
						   @":),:-)",@":d,:D,:-d,:-D",@";),;-)",@":o,:O,:-o,:-O",@":p,:P,:-p,:-P",@"(h),(H)",@":@,:-@",@":s,:S,:-s,:-S",@":$,:-$",@":(,:-(",@":'(",
						   @":|,:-|",@"(a),(A)",@"8o|",@"8-|",@"+o(",@"<:o)",@"|-)",@"*-)",@":-#",@":-*",@"^o)",@"8-)",@"(l),(L)",
						   @"(u),(U)",@"(m),(M),*9mil",/*,*red+u,*unicef,*wwf,*oxfam,*one,*care,*mssoc,*help,*hsus,*bgca,*sierra,*acs,*mod,*naf*/
						   @"(@)",@"(&)",@"(sn)",@"(bah)",@"(S)",@"(*)",@"(#)",@"(r),(R)",@"({)",@"(})",@"(k),(K)",@"(f),(F)",@"(w),(W)",@"(o),(O),(0)",nil];
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


- (void)layoutSubviews {
	
}

- (void)dealloc {
	
	[emoButton release];
	[emoStrings release];
    [super dealloc];
}

- (void)emoPressed {
	
	if ([self.delegate respondsToSelector:@selector(emoSelected:)]) {
		NSArray *emoStrs = [[self.emoStrings objectAtIndex:emoIndex] componentsSeparatedByString:@","];
		NSLog(@"emoStrs: %@", emoStrs);
		[self.delegate emoSelected:emoStrs];
	}
}

@end
