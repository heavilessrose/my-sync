//
//  TouchTestsAppDelegate.m
//  TouchTests
//
//  Created by luke on 09-11-10.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "TouchTestsAppDelegate.h"
#import "TouchTestsViewController.h"

@implementation TouchTestsAppDelegate

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
