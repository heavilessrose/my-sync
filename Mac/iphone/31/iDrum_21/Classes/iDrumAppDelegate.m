//
//  iDrumAppDelegate.m
//  iDrum
//
//  Created by apps on 10/22/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "iDrumAppDelegate.h"
#import "RootViewController.h"

@implementation iDrumAppDelegate


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
