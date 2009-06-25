//
//  Navi1AppDelegate.m
//  Navi1
//
//  Created by wang luke on 6/25/09.
//  Copyright luke 2009. All rights reserved.
//

#import "Navi1AppDelegate.h"

@implementation Navi1AppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    

    // Override point for customization after application launch
	[window addSubview:navController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
	[navController release];
    [window release];
    [super dealloc];
}


@end
