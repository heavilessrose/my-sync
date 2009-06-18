//
//  AsyncSocketTestAppDelegate.m
//  AsyncSocketTest
//
//  Created by wang luke on 6/18/09.
//  Copyright luke 2009. All rights reserved.
//

#import "AsyncSocketTestAppDelegate.h"
#import "AsyncSocketTestViewController.h"

@implementation AsyncSocketTestAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    
    // Override point for customization after app launch    
    [viewController init];
	[window addSubview:viewController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
