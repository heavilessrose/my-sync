//
//  WidgetsAppDelegate.m
//  Widgets
//
//  Created by luke on 10-1-11.
//  Copyright WeTeam 2010. All rights reserved.
//

#import "WidgetsAppDelegate.h"
#import "WidgetsViewController.h"

@implementation WidgetsAppDelegate

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
