//
//  controlFunAppDelegate.m
//  controlFun
//
//  Created by wang luke on 4/23/09.
//  Copyright luke 2009. All rights reserved.
//

#import "controlFunAppDelegate.h"
#import "controlFunViewController.h"

@implementation controlFunAppDelegate

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
