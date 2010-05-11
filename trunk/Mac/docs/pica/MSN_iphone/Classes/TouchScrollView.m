//
//  TouchScrollView.m
//  MSN
//
//  Created by  apple on 08-10-9.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "TouchScrollView.h"
#import "ChatViewsController.h"
#import "ChatViewData.h"
#import "define.h"

@implementation TouchScrollView

- (id)initWithFrame:(CGRect)frame 
{
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)drawRect:(CGRect)rect 
{
	[super drawRect:rect];
	// Drawing code
}


- (void)dealloc 
{
	MYLog(@"-----------------%@ dealloc----------------", self);
	[super dealloc];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesBegan:touches withEvent:event];
	isTouchMoved = NO;
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesMoved:touches withEvent:event];
	isTouchMoved = YES;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesEnded:touches withEvent:event];
	if (!isTouchMoved)
	{
		UITouch *touch = nil;
		for (touch in touches) 
		{
			int x = floor([touch locationInView:self].x);
			int y = floor([touch locationInView:self].y);
			int with = floor(self.frame.size.width);
			if (!((x % with) < 30 &&  y  < 30))
			{
				ChatViewData *chatViewData = [self.delegate performSelector:@selector(getControllerByTouchPoint:) withObject:[NSNumber numberWithInt:x]];
				if (chatViewData != nil)
				{
					NSDictionary *info = [NSDictionary dictionaryWithObject:chatViewData forKey:@"key"];
					[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];
				}
				break;	
			}
		}	
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesCancelled:touches withEvent:event];
}


@end
