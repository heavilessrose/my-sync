//
//  numPadAppDelegate.m
//  numPad
//
//  Created by wang luke on 8/5/09.
//  Copyright luke 2009. All rights reserved.
//

#import "numPadAppDelegate.h"
#import "numPadViewController.h"

@implementation numPadAppDelegate

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
