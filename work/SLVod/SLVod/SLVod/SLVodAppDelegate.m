//
//  SLVodAppDelegate.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLVodAppDelegate.h"
#import "BCTabBarController.h"
#import "LKViewController.h"
#import "SLHotController.h"
#import "SLCategoryController.h"
#import "SLFavController.h"
#import "SLMoreController.h"
#import "SLUserCenterController.h"
#import "SLDownloadManController.h"
#import "SCAppUtils.h"
#import "FFSettings.h"

NSString *kScalingModeKey	= @"scalingMode";
NSString *kControlModeKey	= @"controlMode";
NSString *kBackgroundColorKey	= @"backgroundColor";

@implementation SLVodAppDelegate


@synthesize window=_window;
@synthesize tabBarController, downMan;

- (void)dealloc
{   
	self.tabBarController = nil;
    self.downMan = nil;
    [_window release];
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [FFSettings shareSettings];
    
	self.tabBarController = [[[BCTabBarController alloc] init] autorelease];
    UINavigationController *hotNav = [[[UINavigationController alloc]
                                       initWithRootViewController:[[[SLHotController alloc] initWithNibName:@"SLHotController" bundle:nil] autorelease]]
                                      autorelease];
    [SCAppUtils customizeNavigationController:hotNav withImg:kSCNavBarImgWithLogoName];
    
    UINavigationController *cateNav = [[[UINavigationController alloc]
                                        initWithRootViewController:[[[SLCategoryController alloc] initWithNibName:@"SLCategoryController" bundle:nil] autorelease]]
                                       autorelease];
    [SCAppUtils customizeNavigationController:cateNav withImg:kSCNavBarImgName];
    
    self.downMan = [[[SLDownloadManController alloc] initWithNibName:@"SLDownloadManController" bundle:nil] autorelease];
    UINavigationController *downManNav = [[[UINavigationController alloc]
                                           initWithRootViewController:downMan]
                                          autorelease];
    [SCAppUtils customizeNavigationController:downManNav withImg:kSCNavBarImgName];
    
    UINavigationController *usercenterNav = [[[UINavigationController alloc]
      initWithRootViewController:[[[SLUserCenterController alloc] initWithNibName:@"SLUserCenterController" bundle:nil] autorelease]]
                                             autorelease];
    [SCAppUtils customizeNavigationController:usercenterNav withImg:kSCNavBarImgName];
    
	self.tabBarController.viewControllers = [NSArray arrayWithObjects:
											 hotNav, cateNav, downManNav,
                                             /*
                                             [[[UINavigationController alloc]
                                               initWithRootViewController:[[[SLMoreController alloc] init] autorelease]]
											  autorelease],
                                             [[[UINavigationController alloc]
                                               initWithRootViewController:[[[SLFavController alloc] initWithNibName:@"SLFavController" bundle:nil] autorelease]]
											  autorelease],
                                             [[[UINavigationController alloc]
                                               initWithRootViewController:[[[SLMoreController alloc] init] autorelease]]
											  autorelease],
                                              */
                                             usercenterNav,
											 nil];
	[self.window addSubview:self.tabBarController.view];
    
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
}


@end
