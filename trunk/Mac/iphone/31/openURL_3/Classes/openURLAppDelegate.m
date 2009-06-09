//
//  openURLAppDelegate.m
//  openURL
//
//  Created by apple on 10/3/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "openURLAppDelegate.h"
#import "RootViewController.h"

@implementation openURLAppDelegate


@synthesize window;
@synthesize rootViewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	
	[window addSubview:[rootViewController view]];
	[window makeKeyAndVisible];
}


- (void)dealloc {
	[rootViewController release];
	[window release];
	[super dealloc];
}

@end
