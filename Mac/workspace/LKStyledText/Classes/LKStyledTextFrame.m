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
@synthesize text, font, textColor;

- (id)initWithText:(NSString *)atext node:(LKStyledTextNode *)aTNode {
	
	if (self = [super init]) {
		self.text = atext;
		tnode = aTNode;
	}
	return self;
}

- (void)drawInRect:(CGRect)aRect {
	
	CGContextRef _context = UIGraphicsGetCurrentContext();
	CGContextSaveGState(_context);
	CGContextSetStrokeColorWithColor(_context, [textColor CGColor]);
	CGContextSetFillColorWithColor(_context, [textColor CGColor]);
	
	//[self.textColor set];
	DLog(@"color: %@", textColor);
	[text drawInRect:aRect withFont:font lineBreakMode:UILineBreakModeClip];
	CGContextRestoreGState(_context);
}

- (void)dealloc {
	
	[text release];
	[font release];
	[textColor release];
	[super dealloc];
}

@end
