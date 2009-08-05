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

#define kFontHeight 18
#define kGreetingRect_x (40)
#define kGreetingRect_y (40)
#define kGreetingRectWidth (320 - kGreetingRect_x * 2)
#define kGreetingRectHeight (kFontHeight + 2)
// 以下两个宏的坐标以该view的左上角为坐标原点
#define kDrawPoint_x (kGreetingRectWidth)
#define kDrawPoint_y 0
static int n = 0;

static void debugFrame(CGRect frame)
{
	printf("X=%f, Y=%f, W=%f, H=%f\n", frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
}

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
		super.backgroundColor = [UIColor colorWithWhite:1.0f alpha:1.0f];
		debugFrame(frame);
        // Initialization code
		CGRect newFrame = CGRectMake(kGreetingRect_x, kGreetingRect_y, kGreetingRectWidth, kGreetingRectHeight);

		
		debugFrame(newFrame);
		self.frame = newFrame;
		
		self.backgroundColor = [UIColor colorWithRed:0.5f green:0.0f blue:0.0f alpha:1.0f];
    }
    return self;
}


- (void)drawRect:(CGRect)rect
{
    // Drawing code
	[self renderGreeting:@"诉讼代理开始的放开了死灵法代理开始的放开了死灵法代理开始的放开了死灵法"];
}


- (void)dealloc
{
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
	//???: 不应放在循环中
	CGFloat greetingWidth = [self getStrWidth:greeting];
	if(greetingWidth > kGreetingRectWidth){
		if(n < (kGreetingRectWidth + greetingWidth))
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
		CGFloat x = (kGreetingRectWidth - greetingWidth) / 2;
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
