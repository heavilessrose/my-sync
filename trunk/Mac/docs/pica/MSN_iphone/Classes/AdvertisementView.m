//
//  AdvertisementView.m
//  MSN
//
//  Created by  apple on 09-3-11.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "AdvertisementView.h"


@implementation AdvertisementView
@synthesize target,callbackFun;

- (id)initWithFrame:(CGRect)frame 
{
	if (self = [super initWithFrame:frame]) 
	{
		imageView = [[UIImageView alloc] initWithFrame:CGRectZero];
		imageView.backgroundColor = [UIColor whiteColor];
		imageView.contentMode = UIViewContentModeCenter;
		[self addSubview:imageView];
	}
	return self;
}


- (void)drawRect:(CGRect)rect
{
	// Drawing code
}


- (void)dealloc 
{
	[imageView release];
	[target release];
	[super dealloc];
}

- (void)setFrame:(CGRect)frame
{
	[super setFrame:frame];
	frame.origin = CGPointZero;
	imageView.frame = frame;
}

- (UIImage*)image
{
	return imageView.image;
}

- (void)setImage:(UIImage*)theImage
{
	imageView.image = theImage;
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
