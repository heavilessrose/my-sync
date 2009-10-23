//
//  netTestAppDelegate.m
//  netTest
//
//  Created by wang luke on 09-10-23.
//  Copyright luke 2009. All rights reserved.
//

#import "netTestAppDelegate.h"
#import "netTestViewController.h"

@implementation netTestAppDelegate

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
