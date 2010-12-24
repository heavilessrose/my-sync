//
//  LKLauncherView.m
//  LaunchCube
//
//  Created by luke on 10-12-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKLauncherView.h"
#import "LKLauncherScrollView.h"

#define kPagerHeight (20)

@implementation LKLauncherView

@synthesize scrollView, columnCount, pages;

- (id)initWithFrame:(CGRect)frame {
    
    if (self = [super initWithFrame:frame]) {
		self.scrollView = [[LKLauncherScrollView alloc] initWithFrame:CGRectMake(0, 0, self.bounds.size.width, self.bounds.size.height - kPagerHeight)];
		scrollView.delegate = self;
		scrollView.scrollsToTop = NO;
		scrollView.showsVerticalScrollIndicator = NO;
		scrollView.showsHorizontalScrollIndicator = NO;
		scrollView.alwaysBounceHorizontal = YES;
		scrollView.pagingEnabled = YES;
		scrollView.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
		scrollView.delaysContentTouches = NO;
		scrollView.multipleTouchEnabled = NO;
		[self addSubview:scrollView];
	}
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code.
}
*/

- (void)dealloc {
	
	self.scrollView = nil;
	self.pages = nil;
    [super dealloc];
}


@end
