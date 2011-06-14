//
//  BCTabBarController.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "BCTabBar.h"
@class BCTabBarView;

@interface BCTabBarController : UIViewController <BCTabBarDelegate> {
	NSArray *viewControllers;
	UIViewController *selectedViewController;
	BCTabBar *tabBar;
	BCTabBarView *tabBarView;
	BOOL visible;
}

@property (nonatomic, retain) NSArray *viewControllers;
@property (nonatomic, retain) BCTabBar *tabBar;
@property (nonatomic, retain) UIViewController *selectedViewController;
@property (nonatomic, retain) BCTabBarView *tabBarView;
@property (nonatomic) NSUInteger selectedIndex;

@end
