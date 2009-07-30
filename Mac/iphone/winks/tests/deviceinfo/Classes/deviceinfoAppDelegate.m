//
//  deviceinfoAppDelegate.m
//  deviceinfo
//
//  Created by wang luke on 7/24/09.
//  Copyright luke 2009. All rights reserved.
//

#import "deviceinfoAppDelegate.h"
#import "deviceinfoViewController.h"

@implementation deviceinfoAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    
    // Override point for customization after app launch    
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
