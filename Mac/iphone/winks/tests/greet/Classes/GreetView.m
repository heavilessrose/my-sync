//
//  GreetView.m
//  greet
//
//  Created by wang luke on 8/3/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "GreetView.h"


@implementation GreetView
@synthesize greetingTimer;

#define kGreetingRect_x (20)
#define kGreetingRect_y (20)
#define kGreetingRectWidth (self.frame.size.width - kGreetingRect_x * 2)
#define kGreetingRectHeight (18)
#define kDrawPoint_x (kGreetingRect_x + kGreetingRectWidth)
#define kDrawPoint_y kGreetingRect_y
static int n = 0;


static void debugFrame(CGRect frame){
	printf("X=%f, Y=%f, W=%f, H=%f\n", frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
		debugFrame(frame);
        // Initialization code
		CGRect newFrame = frame;
		newFrame.origin.x == kGreetingRect_x;
		newFrame.origin.y == kGreetingRect_y;
		newFrame.size.height == kGreetingRectHeight;
		newFrame.size.width == kGreetingRectWidth;
		
//		newFrame.origin.x == frame.origin.x - 60;
//		newFrame.origin.y == frame.origin.y - 60;
//		newFrame.size.height == frame.size.height - 60;
//		newFrame.size.width == frame.size.width - 60;
		
		debugFrame(newFrame);
		self.frame = newFrame;
		
		self.backgroundColor = [UIColor colorWithWhite:1.0 alpha:1.0];
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
    // Drawing code
	[self renderGreeting:@"诉讼代理开始的放开了死灵法师地理空间防水堵漏份开始ksjkjksdkjsdkjjk1sdfasfasdfasfas拉拉"];
}


- (void)dealloc {
	[self disposeTimer];
    [super dealloc];
}

#pragma mark -

- (CGFloat)getStrWidth:(NSString *)str
{
	CGSize strSize = [str sizeWithFont:[UIFont systemFontOfSize:[UIFont systemFontSize]]];
	return strSize.width;
}

- (void)renderGreeting:(NSString *)greeting
{
	CGFloat greetingWidth = [self getStrWidth:greeting];
	if( greetingWidth > kGreetingRectWidth){
		if(n < (kGreetingRectWidth * 2 + (greetingWidth - kGreetingRectWidth)))
			n++;
		else
			n = 0;
		
		// 滚动画
		if(greetingTimer == nil){
			NSTimeInterval animationInterval = 1.5 / 60.0;
			self.greetingTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval 
																  target:self 
																selector:@selector(timerFireMethod:) 
																userInfo:nil repeats:YES];
		}
		[greeting drawAtPoint:CGPointMake(kDrawPoint_x - n,kDrawPoint_y) withFont:[UIFont systemFontOfSize:[UIFont systemFontSize]]];
	}else{
		// 居中画
		CGFloat x = (kGreetingRectWidth - greetingWidth) / 2 + kGreetingRect_x;
		[greeting drawAtPoint:CGPointMake(x,kDrawPoint_y) withFont:[UIFont systemFontOfSize:[UIFont systemFontSize]]];
	}
}

- (void)timerFireMethod:(NSTimer*)theTimer
{
	// repaint
	[self setNeedsDisplay];
}

- (void)disposeTimer
{
	[greetingTimer invalidate];
	self.greetingTimer = nil;
}
@end
