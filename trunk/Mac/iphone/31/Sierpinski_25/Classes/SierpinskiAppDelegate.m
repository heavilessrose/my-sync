//
//  SierpinskiAppDelegate.m
//  Sierpinski
//
//  Created by apps on 10/25/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "SierpinskiAppDelegate.h"
#import "RootViewController.h"

@implementation SierpinskiAppDelegate


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
