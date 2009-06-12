//
//  FileBrowserAppDelegate.m
//  FileBrowser
//
//  Created by wang luke on 6/12/09.
//  Copyright luke 2009. All rights reserved.
//

#import "FileBrowserAppDelegate.h"

@implementation FileBrowserAppDelegate

@synthesize window;
@synthesize navController;

- (void)applicationDidFinishLaunching:(UIApplication *)application {    

	[window addSubview:navController.view];
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}


- (void)dealloc {
	[navController release];
    [window release];
    [super dealloc];
}


@end
