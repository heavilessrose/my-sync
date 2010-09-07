//
//  BabyBearAppDelegate.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright Luke 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface BabyBearAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow			*window;
	UITabBarController	*tabs;
    NSInteger			networkingCount;
}
//
//#ifdef USE_XIB
//@property (nonatomic, retain) UIWindow *window;
//@property (nonatomic, retain) UITabBarController *tabs;
//#else
@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) UITabBarController *tabs;
//#endif

+ (BabyBearAppDelegate *)sharedAppDelegate;

@end

