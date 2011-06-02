//
//  siluVodAppDelegate.m
//  siluVod
//
//  Created by luke on 11-5-15.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "siluVodAppDelegate.h"
#import "HomeViewController.h"
#import "FavViewController.h"
#import "MoreViewController.h"

@implementation siluVodAppDelegate


@synthesize window=_window;

@synthesize navigationController=_navigationController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    // Add the navigation controller's view to the window and display.
    self.window.rootViewController = self.navigationController;
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

- (void)dealloc
{
    [_window release];
    [_navigationController release];
    [super dealloc];
}


#pragma mark -
#pragma mark LKTabBarControllerDelegate

- (NSInteger)numberOfTabsInTabBar:(LKTabBarController *)tabBarCtrl
{
    return 3;
}

- (NSDictionary *)tabBar:(LKTabBarController *)tabBarCtrl ctrlDicForIndexOfTab:(NSInteger)index
{
    NSDictionary *ctrlDic = nil;
    switch (index) {
        case 0:{
            HomeViewController *homeVC = [[[HomeViewController alloc] init] autorelease];
            homeVC.view.backgroundColor = [UIColor redColor];
            UINavigationController *homeNav = [[[UINavigationController alloc] initWithRootViewController:homeVC] autorelease];
            ctrlDic = [NSDictionary dictionaryWithObjectsAndKeys:@"chat.png", @"image", homeVC, @"viewController", homeNav, @"nav", nil];
        }
            break;
        case 1:{
            FavViewController *favVC = [[[FavViewController alloc] init] autorelease];
            favVC.view.backgroundColor = [UIColor blueColor];
            UINavigationController *favNav = [[[UINavigationController alloc] initWithRootViewController:favVC] autorelease];
            ctrlDic = [NSDictionary dictionaryWithObjectsAndKeys:@"messages.png", @"image", favVC, @"viewController", favNav, @"nav", nil];
        }
            break;
        case 2:{
            MoreViewController *moreVC = [[[MoreViewController alloc] init] autorelease];
            moreVC.view.backgroundColor = [UIColor yellowColor];
            UINavigationController *moreNav = [[[UINavigationController alloc] initWithRootViewController:moreVC] autorelease];
            ctrlDic = [NSDictionary dictionaryWithObjectsAndKeys:@"more.png", @"image", moreVC, @"viewController", moreNav, @"nav", nil];
        }
            break;
        case 3:{
            
        }
            break;
        case 4:{
            
        }
            break;
        default:
            break;
    }
    
    return ctrlDic;
}

@end
