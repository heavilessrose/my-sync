//
//  LKStyledTextFrame.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledTextFrame.h"
#import "LKStyledTextNode.h"


@implementation LKStyledTextFrame

@synthesize tnode;
@synthesize text, font;

- (id)initWithText:(NSString *)atext node:(LKStyledTextNode *)aTNode {
	
	if (self = [super init]) {
		self.text = atext;
		tnode = aTNode;
	}
	return self;
}

- (void)drawInRect:(CGRect)aRect {
	
	[text drawInRect:aRect withFont:font lineBreakMode:UILineBreakModeClip];
}

- (void)dealloc {
	
	[text release];
	[font release];
	[super dealloc];
}

@end
