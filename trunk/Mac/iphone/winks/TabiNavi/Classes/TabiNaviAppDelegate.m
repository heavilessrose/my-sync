//
//  TabiNaviAppDelegate.m
//  TabiNavi
//
//  Created by wang luke on 6/29/09.
//  Copyright luke 2009. All rights reserved.
//

#import "TabiNaviAppDelegate.h"

@implementation TabiNaviAppDelegate

@synthesize window;
@synthesize rootController;
@synthesize navController;

- (void)applicationDidFinishLaunching:(UIApplication *)application {    

    // Override point for customization after application launch
	[window addSubview:rootController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
	[rootController release];
    [super dealloc];
}


@end
