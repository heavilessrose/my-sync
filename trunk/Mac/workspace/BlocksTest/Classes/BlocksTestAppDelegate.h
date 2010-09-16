//
//  BlocksTestAppDelegate.h
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

#define USE_INSTANCE_VAR

#ifdef USE_INSTANCE_VAR
@class LongRunningTask;
#endif
//@class BlocksTestViewController;

@interface BlocksTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    UITabBarController *tabs;
	
	NSString *appStatus;
	
#ifdef USE_INSTANCE_VAR
	LongRunningTask *aTask;
#endif
	BOOL isLongRunningTaskRunning;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UITabBarController *tabs;

@property (nonatomic, retain) NSString *appStatus;

#ifdef USE_INSTANCE_VAR
@property (nonatomic, assign) LongRunningTask *aTask;
#endif

@end

