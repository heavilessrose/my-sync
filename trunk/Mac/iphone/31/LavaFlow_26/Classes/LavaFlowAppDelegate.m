//
//  LavaFlowAppDelegate.m
//  LavaFlow
//
//  Created by apps on 8/26/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "LavaFlowAppDelegate.h"
#import "RootViewController.h"

@implementation LavaFlowAppDelegate


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
