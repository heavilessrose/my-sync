//
//  testsAppDelegate.m
//  tests
//
//  Created by luke on 7/11/09.
//  Copyright luke 2009. All rights reserved.
//

#import "testsAppDelegate.h"
#import "testsViewController.h"

@implementation testsAppDelegate

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
