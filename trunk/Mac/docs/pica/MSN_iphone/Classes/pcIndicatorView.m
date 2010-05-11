//
//  pcIndicatorView.m
//  mBackground
//
//  Created by ancle on 4/28/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "pcIndicatorView.h"

#define INDICATOR_SIZE			30
#define LEFT_INTERVAL			10
@implementation pcIndicatorView


- (id)initWithFrame: (CGRect)frame
			  title: (NSString*)aTitle
		 background: (NSString*)aFilename {
    if (self = [super initWithFrame:frame]) {
		indicator = [[UIActivityIndicatorView alloc] initWithFrame: CGRectMake(0, 0, INDICATOR_SIZE, INDICATOR_SIZE)];
		indicator.hidesWhenStopped = YES;
		indicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyleGray;
		indicator.autoresizingMask = (UIViewAutoresizingFlexibleLeftMargin |
									  UIViewAutoresizingFlexibleRightMargin |
									  UIViewAutoresizingFlexibleTopMargin |
									  UIViewAutoresizingFlexibleBottomMargin);
		[self addSubview: indicator];
		
		label = [[UILabel alloc] initWithFrame: CGRectZero];
		label.text = aTitle;
		label.textColor = [UIColor whiteColor];
		label.backgroundColor = [UIColor clearColor];
		[self addSubview: label];
		
		UIImage* anImage = nil;
		if (aFilename != nil && (anImage = [UIImage imageNamed: aFilename]))
			self.image = anImage;
		else 
			self.backgroundColor = [UIColor clearColor];
    }
    return self;
}

- (void)layoutSubviews{
	[super layoutSubviews];
	CGRect bounds = self.bounds;
	CGRect rcIndicator = bounds;
	CGRect rcLabel = CGRectZero;
	if ([label.text length] > 0){
		label.hidden = NO;
		CGSize fontSize = [label.text sizeWithFont: label.font]; 
		rcLabel.origin.x = LEFT_INTERVAL;
		rcLabel.origin.y = bounds.size.height - fontSize.height - LEFT_INTERVAL / 2;
		if (fontSize.width > (bounds.size.width - LEFT_INTERVAL * 2))
			rcLabel.size.width = bounds.size.width - LEFT_INTERVAL * 2;
		else
			rcLabel.size.width = fontSize.width;
		rcLabel.size.height = fontSize.height;
	}
	else {
		label.hidden = YES;
	}
	label.frame = rcLabel;
	
	rcIndicator.origin.x = (bounds.size.width - INDICATOR_SIZE) / 2;
	if (label.hidden)
		rcIndicator.origin.y = (bounds.size.height - INDICATOR_SIZE) / 2;
	else
		rcIndicator.origin.y = (bounds.size.height - rcLabel.origin.y) / 2;
	rcIndicator.size = CGSizeMake(INDICATOR_SIZE, INDICATOR_SIZE);
	indicator.frame = rcIndicator;
}


- (void)dealloc {
	[label release];
	[indicator release];
    [super dealloc];
}

- (void)setTitle: (NSString*)aTitle{
	label.text = aTitle;
	[self layoutSubviews];
}

- (void)startAnimation{
	[indicator startAnimating];
}

- (void)stopAnimating{
	[indicator stopAnimating];
}

@end
