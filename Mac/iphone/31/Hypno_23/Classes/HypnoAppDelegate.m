//
//  HypnoAppDelegate.m
//  Hypno
//
//  Created by apps on 10/23/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "HypnoAppDelegate.h"
#import "RootViewController.h"

@implementation HypnoAppDelegate


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
