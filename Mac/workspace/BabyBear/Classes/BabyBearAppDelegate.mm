//
//  BabyBearAppDelegate.m
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright Luke 2010. All rights reserved.
//

#import "BabyBearAppDelegate.h"
#import "defines.h"

#import "FeaturedViewController.h"
#import "ProductsViewController.h"
#import "StoresViewController.h"
#import "FavoritesViewController.h"
#import "CartViewController.h"

@interface BabyBearAppDelegate ()
@property (nonatomic, assign) NSInteger	networkingCount;
@end

@implementation BabyBearAppDelegate


+ (void)initialize
// 在此做一些初始化工作
{
	if ([self class] == [BabyBearAppDelegate class]) {
        NSString *      initialDefaultsPath;
        NSDictionary *  initialDefaults;
		
        initialDefaultsPath = [[NSBundle mainBundle] pathForResource:@"InitialDefaults" ofType:@"plist"];
        assert(initialDefaultsPath != nil);
        
        initialDefaults = [NSDictionary dictionaryWithContentsOfFile:initialDefaultsPath];
        assert(initialDefaults != nil);
        
#if ! TARGET_IPHONE_SIMULATOR
        {
            NSMutableDictionary *   initialDefaultsChanged;
            
            initialDefaultsChanged = [initialDefaults mutableCopy];
            assert(initialDefaultsChanged != nil);
            
            [initialDefaultsChanged setObject:@"http://localhost/review" forKey:@"baseReviewUrl"];
            [initialDefaultsChanged setObject:@"http://localhost/rating" forKey:@"baseRatingUrl"];
            
            initialDefaults = initialDefaultsChanged;
        }
#endif
        
        [[NSUserDefaults standardUserDefaults] registerDefaults:initialDefaults];
    }
}

+ (BabyBearAppDelegate *)sharedAppDelegate
{
	return (BabyBearAppDelegate *) [UIApplication sharedApplication].delegate;
}


#pragma mark -
#pragma mark Application lifecycle

@synthesize window, tabs;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
#ifdef USE_XIB
#pragma unused(application)
#pragma unused(launchOptions)
    assert(self.window != nil);
    assert(self.tabs != nil);
	
    [self.window addSubview:self.tabs.view];
    
    self.tabs.selectedIndex = [[NSUserDefaults standardUserDefaults] integerForKey:@"currentTab"];
    
#else
	NSLog(@"%@", self.tabs);
	self.tabs = [[[UITabBarController alloc] init] autorelease];
	FeaturedViewController *featureVC = [[[FeaturedViewController alloc] init] autorelease];
	ProductsViewController *productsVC = [[[ProductsViewController alloc] init] autorelease];
	StoresViewController *storesVC = [[[StoresViewController alloc] init] autorelease];
	FavoritesViewController *favoritesVC = [[[FavoritesViewController alloc] init] autorelease];
	CartViewController *cartVC = [[[CartViewController alloc] init] autorelease];
	
	UINavigationController *featureNV = [[[UINavigationController alloc] initWithRootViewController:featureVC] autorelease];
	UINavigationController *productsNV = [[[UINavigationController alloc] initWithRootViewController:productsVC] autorelease];
	UINavigationController *storesNV = [[[UINavigationController alloc] initWithRootViewController:storesVC] autorelease];
	UINavigationController *favoritesNV = [[[UINavigationController alloc] initWithRootViewController:favoritesVC] autorelease];
	UINavigationController *cartNV = [[[UINavigationController alloc] initWithRootViewController:cartVC] autorelease];
	
	NSArray* controllers = [NSArray arrayWithObjects:featureNV, productsNV, storesNV, favoritesNV, cartNV, nil];
	//NSArray* controllers = [NSArray arrayWithObjects:featureVC, productsVC, storesVC, favoritesVC, cartVC, nil];
	tabs.viewControllers = controllers;
	
	// Add the tab bar controller's current view as a subview of the window
	self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
	NSLog(@"tabs.view: %@", tabs.view);
	[window addSubview:tabs.view];
    [window makeKeyAndVisible];
#endif
	
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
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
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
     See also applicationDidEnterBackground:.
     */
    [[NSUserDefaults standardUserDefaults] setInteger:self.tabs.selectedIndex forKey:@"currentTab"];
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc
{
	[tabs release];
    [window release];
    [super dealloc];
}

#pragma mark -
#pragma mark background tasks

//TODO: 直接调用来开始一个后台任务, 返回该任务ID
//- (UIBackgroundTaskIdentifier)beginBackgroundTaskWithExpirationHandler:(void(^)(void))handler;
//- (void)endBackgroundTask:(UIBackgroundTaskIdentifier)identifier;

@end
