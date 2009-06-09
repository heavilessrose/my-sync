//
//  DrawTestAppDelegate.m
//  DrawTest
//
//  Created by AppsAmuck on 8/26/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "DrawTestAppDelegate.h"
#import "RootViewController.h"

@implementation DrawTestAppDelegate


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
