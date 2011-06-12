//
//  LKTipView.m
//  SK
//
//  Created by luke on 10-11-17.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKTipView.h"
#import "UIView+LKAddition.h"

@interface LKTipView ()

- (void)adjust;

@end


@implementation LKTipView

@synthesize text, image;

- (id)initWithFrame:(CGRect)frame {
	
    if ((self = [super initWithFrame:frame])) {
		self.backgroundColor = [UIColor clearColor];
		self.userInteractionEnabled = NO;
		keepFit = NO;
		roundedConner = NO;
		messageRect = CGRectInset(self.bounds, 10, 10);
    }
    return self;
}

- (id)init {
	
	CGRect tipFrame = CGRectMake(0, 0, 100, 100);
	if (!(self = [super initWithFrame:tipFrame]))
		return nil;
	
	messageRect = CGRectInset(self.bounds, 10, 10);
	self.backgroundColor = [UIColor clearColor];
	self.userInteractionEnabled = YES;
	keepFit = YES;
	roundedConner = YES;
	
	return self;
}

- (void)adjust {
	
	if (keepFit) {
		CGSize s = [text sizeWithFont:Font_Small 
					constrainedToSize:CGSizeMake(160,200) 
						lineBreakMode:UILineBreakModeWordWrap];
		
		float imageAdjustment = 0;
		if (image) {
			imageAdjustment = 7+image.size.height;
		}
		
		self.bounds = CGRectMake(0, 0, s.width+40, s.height+15+15+imageAdjustment);
		
		messageRect.size = s;
		messageRect.size.height += 5;
		messageRect.origin.x = 20;
		messageRect.origin.y = 15+imageAdjustment;
	}

	[self setNeedsLayout];
	[self setNeedsDisplay];
}

- (void)setText:(NSString *)str {
	if (str) {
		text = [str retain];
		[self adjust];
	}
}

- (void)setImage:(UIImage *)img {
	if (img) {
		image = [img retain];
		[self adjust];
	}
}

- (void)drawRect:(CGRect)rect {
	
	if (roundedConner) {
		[UIView drawRoundRectangleInRect:rect withRadius:10 color:[UIColor colorWithWhite:0.3 alpha:0.7]];
	} else {
		[UIView drawRectangleInRect:rect color:[UIColor colorWithWhite:0.3 alpha:0.7]];
	}

	[[UIColor whiteColor] set];
	[text drawInRect:messageRect withFont:Font_Small lineBreakMode:UILineBreakModeWordWrap alignment:UITextAlignmentCenter];
	
	CGRect r = CGRectZero;
	r.origin.y = 15;
	r.origin.x = (rect.size.width-image.size.width)/2;
	r.size = image.size;
	
	[image drawInRect:r];
}

- (void)dealloc {
	
	MLog(@"");
	[text release];
	[image release];
	[super dealloc];
}

@end
