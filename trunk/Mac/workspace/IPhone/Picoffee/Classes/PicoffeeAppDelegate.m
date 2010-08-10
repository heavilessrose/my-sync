//
//  PicoffeeAppDelegate.m
//  Picoffee
//
//  Created by luke on 6/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "PicoffeeAppDelegate.h"
#import "TestViewController.h"

@implementation PicoffeeAppDelegate

@synthesize window;

/*
- (void)applicationDidFinishLaunching:(UIApplication *)application {    

}
*/

- (void)dealloc {
	[viewController release];
    [window release];
    [super dealloc];
}

#pragma mark -
#pragma mark 生命周期管理
/* Launch Options Keys
 NSString *const UIApplicationLaunchOptionsURLKey;
 NSString *const UIApplicationLaunchOptionsSourceApplicationKey;
 NSString *const UIApplicationLaunchOptionsRemoteNotificationKey;
 NSString *const UIApplicationLaunchOptionsAnnotationKey;
 NSString *const UIApplicationLaunchOptionsLocalNotificationKey;
 NSString *const UIApplicationLaunchOptionsLocationKey;
 */
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // right now application is in the inactive state
    
	viewController = [[TestViewController alloc] init];
	[window addSubview:viewController.view];
    // Override point for customization after application launch
    [window makeKeyAndVisible];
    return YES;
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // this is called let the application know it moved from the inactive to active state
    // should use this method to restart any tasks that were paused (or not yet started) while the application was inactive.
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // This method is called to let your application know that it is about to move from the active to inactive state.
    // should use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates.
}

// Tells the delegate when the application is about to terminate.
- (void)applicationWillTerminate:(UIApplication *)application
{
    // Your implementation of this method has approximately five seconds to perform any tasks and return.
    // 3.0SDK 肯定会调, 4.0不一定会调
}

#pragma mark 4.0 only
// Tells the delegate that the application is now in the background.
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Your implementation of this method has approximately five seconds to perform any tasks and return. 
    // If you need additional time to perform any final tasks, you can request additional execution time from the system.
}

// Tells the delegate that the application is about to enter the foreground.
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // The call to this method is invariably followed by a call to the applicationDidBecomeActive: 
    // method, which then moves the application from the inactive to the active state.
}

#pragma mark -
#pragma mark background
/*
// Marks the beginning of a new long-running background task. ask the system for extra time to complete some long-running task in the background.
- (UIBackgroundTaskIdentifier)beginBackgroundTaskWithExpirationHandler:(void(^)(void))handler
{
    UIApplication *app = [UIApplication sharedApplication];
    NSTimeInterval timeRemain = app.backgroundTimeRemaining;
}
 
 // Marks the end of a specific long-running background task.
 - (void)endBackgroundTask:(UIBackgroundTaskIdentifier)identifier
 {
 }
 */
@end
