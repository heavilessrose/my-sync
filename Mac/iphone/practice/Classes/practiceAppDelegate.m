//
//  practiceAppDelegate.m
//  practice
//
//  Created by wang luke on 4/23/09.
//  Copyright luke 2009. All rights reserved.
//

#import "practiceAppDelegate.h"
#import "practiceViewController.h"

@implementation practiceAppDelegate

@synthesize window;
@synthesize viewController;

// 入口
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
