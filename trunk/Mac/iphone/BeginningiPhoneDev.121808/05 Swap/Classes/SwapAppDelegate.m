//
//  SwapAppDelegate.m
//  Swap
//
//  Created by Dave Mark on 7/5/08.
//  Copyright SpiderWorks 2008. All rights reserved.
//

#import "SwapAppDelegate.h"
#import "SwapViewController.h"

@implementation SwapAppDelegate

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
