//
//  LKTipCenter.m
//  SK
//
//  Created by luke on 10-11-17.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKTipCenter.h"
#import "LKTipView.h"
#import "LKTip.h"
#import "LKStatusBar.h"

/*
@protocol LKTipDelegate <NSObject>

@optional
- (void)LKTipPressed:(NSString *)message;

@end
*/
 
@interface LKTipCenter ()

@property (nonatomic, retain) NSMutableDictionary *tips;
@property (nonatomic, retain) LKTipView *curTipView;
@property (nonatomic, assign) LKTip *curTip;
@property (nonatomic, retain) NSMutableDictionary  *fallingContainers;

- (void)disposeTip:(UIView *)container;
- (void)animationFadeout:(NSTimer *)theTimer;

@end


@implementation LKTipCenter

@synthesize tips, curTipView, curTip;
@synthesize fallingContainers;

- (id)init {
	
	if (!(self=[super init]))
		return nil;
	self.fallingContainers = [NSMutableDictionary dictionary];
	self.tips = [NSMutableDictionary dictionary];
	active = NO;
	
	return self;
}

- (void)dealloc {
	
	MLog(@"");
    self.fallingContainers = nil;
	[tips release];
	[statusBar release];
	[curTipView release];
	[super dealloc];
}

- (void)disposeTip:(UIView *)container {
	
	[curTipView removeFromSuperview];
	[tips removeObjectForKey:container];
    if (container) {
        [fallingContainers removeObjectForKey:container];
        self.curTipView = nil;
    }
}

- (void)animationDrawup:(NSTimer *)theTimer inContainer:(UIView *)container {
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDelay:1];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(disposeTip:)];
	
	curTipView.frame = CGRectMake(curTipView.frame.origin.x, curTipView.frame.origin.y - 40, curTipView.frame.size.width, curTipView.frame.size.height);
	curTipView.alpha = 0;
	[UIView commitAnimations];
}

- (void)timedDrawup:(UIView *)container {
	
	LKTip *aTip = [tips objectForKey:container];
	if (aTip && aTip.time) {
		DLog(@"");
		[NSTimer scheduledTimerWithTimeInterval:aTip.time target:self selector:@selector(animationDrawup:) userInfo:nil repeats:NO];
	} else {
		[self animationDrawup:nil inContainer:container];
	}
}

- (void)animationFadeout:(NSTimer *)theTimer {
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDelay:1];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(disposeTip:)];
	
	UIInterfaceOrientation o = [UIApplication sharedApplication].statusBarOrientation;
	CGFloat degrees = 0;
	if (o == UIInterfaceOrientationLandscapeLeft) {
		degrees = -90;
	} else if (o == UIInterfaceOrientationLandscapeRight) {
		degrees = 90;
	} else if (o == UIInterfaceOrientationPortraitUpsideDown) {
		degrees = 180;
	}
	//tipView.transform = CGAffineTransformMakeRotation(degrees*M_PI/180);
	//tipView.transform = CGAffineTransformScale(tipView.transform, 0.5, 0.5);
	
	curTipView.alpha = 0;
	[UIView commitAnimations];
}

- (void)timedFadeout:(UIView *)container {
	
	LKTip *aTip = [tips objectForKey:container];
	if (aTip && aTip.time) {
		[NSTimer scheduledTimerWithTimeInterval:aTip.time target:self selector:@selector(animationFadeout:) userInfo:nil repeats:NO];
	} else {
		[self animationFadeout:nil];
	}
}

- (void)fallingTipInContainer:(UIView *)container {
	
	if ([self.tips count] < 1) {
		active = NO;
		return;
	}
	
	active = YES;    
	curTipView.transform = CGAffineTransformIdentity;
	//[UIApplication sharedApplication].keyWindow.center;
	
	UIInterfaceOrientation o = [UIApplication sharedApplication].statusBarOrientation;
	CGFloat degrees = 0;
	CGRect rc = [[UIScreen mainScreen] bounds];

	if (o == UIInterfaceOrientationLandscapeLeft) {
		CGRect f = CGRectMake(0, -20, rc.size.height, 40);
		self.curTipView = [[[LKTipView alloc] initWithFrame:f] autorelease];
		float h = rc.size.height/2;
		
		curTipView.center = CGPointMake(0, h);//  240); 
		degrees = -90;
	} else if (o == UIInterfaceOrientationLandscapeRight) {
		CGRect f = CGRectMake(0, -20, rc.size.height, 40);
		self.curTipView = [[[LKTipView alloc] initWithFrame:f] autorelease];
		float h =  rc.size.height/2;
		
		float w =  rc.size.width;
		curTipView.center = CGPointMake(w,h); //(320, 240); 

		degrees = 90;
	} else if (o == UIInterfaceOrientationPortraitUpsideDown) {
		CGRect f = CGRectMake(0, -20, rc.size.width, 40);
		self.curTipView = [[[LKTipView alloc] initWithFrame:f] autorelease];
		float h = rc.size.height;
		curTipView.center = CGPointMake(rc.size.width/2, h);////480);
		degrees = 180;
	} else {
		CGRect f = CGRectMake(0, 0, rc.size.width, 40);
		self.curTipView = [[[LKTipView alloc] initWithFrame:f] autorelease];
//		curTipView.center = CGPointMake(rc.size.width/2, 0); 
	}

	curTipView.transform = CGAffineTransformMakeRotation(degrees*M_PI/180);
	//tipView.transform = CGAffineTransformScale(tipView.transform, 1, 1);
	curTipView.alpha = 1;
    if (container) {
        [container addSubview:curTipView];
    } else {
        [[UIApplication sharedApplication].keyWindow addSubview:curTipView];
    }
	
	LKTip *aTip = [self.tips objectForKey:container];
	
	if(aTip.message)
		[curTipView setText:aTip.message];
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:0.15];
	[UIView setAnimationDelegate:self];
    if (aTip.time > 0) {
        [UIView setAnimationDidStopSelector:@selector(timedDrawup)];
    }
	
	curTipView.alpha = 1;
	if (o == UIInterfaceOrientationLandscapeLeft) {
	  curTipView.frame = CGRectMake(curTipView.frame.origin.x+ 40, curTipView.frame.origin.y , curTipView.frame.size.width, curTipView.frame.size.height);
		
	} else if (o == UIInterfaceOrientationLandscapeRight) {
		 curTipView.frame = CGRectMake(curTipView.frame.origin.x - 40, curTipView.frame.origin.y , curTipView.frame.size.width, curTipView.frame.size.height);
		//tipView.frame = CGRectMake(0, 20, rc.size.height, 40);
	} else if (o == UIInterfaceOrientationPortraitUpsideDown) {
		curTipView.frame = CGRectMake(curTipView.frame.origin.x, curTipView.frame.origin.y - 40, curTipView.frame.size.width, curTipView.frame.size.height);
		
	} else {
		curTipView.frame = CGRectMake(curTipView.frame.origin.x, curTipView.frame.origin.y + 40, curTipView.frame.size.width, curTipView.frame.size.height);
		
		//tipView.frame = CGRectMake(0, 20, rc.size.width, 40);
	}
	
	[UIView commitAnimations];
    
    
    [fallingContainers setObject:curTipView forKey:container];
}

#pragma mark -
+ (LKTipCenter *)defaultCenter {
	
	static LKTipCenter *defaultCenter = nil;
	if (!defaultCenter) {
		defaultCenter = [[LKTipCenter alloc] init];
	}
	return defaultCenter;
}

- (void)postFallingTipWithMessage:(NSString *)message inContainer:(UIView *)container time:(NSTimeInterval)dTime {
	
	LKTip *aTip = [[LKTip alloc] initWithMsg:message img:nil durationTime:dTime];
	if (aTip) {
		self.curTip = aTip;
		[self.tips setObject:aTip forKey:container];
		[aTip release];
		if (!active)
			[self fallingTipInContainer:container];
	}
}

- (void)disposeFallingTip:(UIView *)container
{
    [self animationDrawup:nil inContainer:container];
}

- (void)changeFallingTip:(UIView *)container withText:(NSString *)errStr
{
    LKTipView *theView = [fallingContainers objectForKey:container];
    if (theView) {
        theView.text = errStr;
    }
}

#pragma mark touch handle

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	DLog(@"");
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	DLog(@"");
}

@end
