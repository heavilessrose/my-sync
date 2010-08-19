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

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UITabBarController *tabs;

+ (BabyBearAppDelegate *)sharedAppDelegate;

@end

