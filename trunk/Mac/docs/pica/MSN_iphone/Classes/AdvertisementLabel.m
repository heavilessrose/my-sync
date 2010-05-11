//
//  AdvertisementLabel.m
//  MSN
//
//  Created by  apple on 09-3-11.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "AdvertisementLabel.h"


@implementation AdvertisementLabel
@synthesize target,callbackFun;
@synthesize sLink;
@synthesize label;

- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame])
	{
		self.tintColor = [UIColor whiteColor];
		label = [[UILabel alloc] initWithFrame:frame];
		label.adjustsFontSizeToFitWidth = YES;
		[self addSubview:label];
		// Initialization code
	}
	return self;
}

- (void) dealloc
{
	[label release];
	[target release];
	[super dealloc];
}

- (NSString *)text
{
	return label.text;
}

- (void)setFrame:(CGRect)frame
{
	[super setFrame:frame];
	frame.origin = CGPointZero;
	label.frame = frame;
}

- (void)setText:(NSString *)theText
{
	label.text = theText;
}

- (UITextAlignment)textAlignment
{
	return label.textAlignment;
}

- (void)setTextAlignment:(UITextAlignment)theTextAlignment
{
	label.textAlignment = theTextAlignment;
}

- (UIColor*)textColor
{
	return label.textColor;
}

- (void)setTextColor:(UIColor*)theColor
{
	label.textColor = theColor;
}

- (CGRect)textRectForBounds:(CGRect)bounds limitedToNumberOfLines:(NSInteger)numberOfLines
{
	return [label textRectForBounds:bounds limitedToNumberOfLines:numberOfLines];
}

- (void)drawRect:(CGRect)rect 
{
	[super drawRect:rect];
	// Drawing code
}

- (void)addTarget:(id)theTarget action:(SEL)theFun
{
	self.target = theTarget;
	self.callbackFun = theFun;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[target performSelector:callbackFun withObject:self];
	[super touchesBegan:touches withEvent:event];
}

@end
