//
//  BCTabBarView.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

@class BCTabBar;

@interface BCTabBarView : UIView {
	UIView *contentView;
	BCTabBar *tabBar;
}

@property (nonatomic, assign) UIView *contentView;
@property (nonatomic, assign) BCTabBar *tabBar;


@end
