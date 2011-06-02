//
//  LKTabBarController.h
//  siluVod
//
//  Created by luke on 11-5-15.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKTabBar.h"

@class LKTabBarController;

@protocol LKTabBarControllerDelegate <NSObject>

@required
- (NSInteger)numberOfTabsInTabBar:(LKTabBarController *)tabBarCtrl;
- (NSDictionary *)tabBar:(LKTabBarController *)tabBarCtrl ctrlDicForIndexOfTab:(NSInteger)index;

@end

@interface LKTabBarController : UIViewController <LKTabBarDelegate>
{
    LKTabBar *tabBar;
    id <LKTabBarControllerDelegate> delegate;
}

@property (nonatomic, assign) id <LKTabBarControllerDelegate> delegate;
@property (nonatomic, retain) LKTabBar *tabBar;

@end
