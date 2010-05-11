//
//  ReceivedMessageView.m
//  MSN
//
//  Created by  apple on 08-11-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ReceivedMessageView.h"

NSString *TouchInReceivedMessageViewNotification = @"TouchInReceivedMessageView";
@implementation ReceivedMessageView

- (id)initWithFrame:(CGRect)frame style:(UITableViewStyle)style;
{
	if (self = [super initWithFrame:frame style:style])
	{
		// Initialization code
	}
	return self;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesBegan:touches withEvent:event];
	[[NSNotificationCenter defaultCenter] postNotificationName:TouchInReceivedMessageViewNotification object:self];
}

- (void)drawRect:(CGRect)rect 
{
	[super drawRect:rect];
	// Drawing code
}


- (void)dealloc 
{
	[super dealloc];
}


@end
