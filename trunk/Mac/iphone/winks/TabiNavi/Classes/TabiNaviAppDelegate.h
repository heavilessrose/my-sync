//
//  TabiNaviAppDelegate.h
//  TabiNavi
//
//  Created by wang luke on 6/29/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MyNavigationController.h"

@interface TabiNaviAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	IBOutlet UITabBarController *rootController;
	
	UINavigationController *navController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UITabBarController *rootController;

@property (nonatomic, retain) UINavigationController *navController;
@end

