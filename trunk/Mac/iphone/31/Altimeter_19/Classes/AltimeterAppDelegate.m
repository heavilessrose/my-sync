//
//  AltimeterAppDelegate.m
//  Altimeter
//
//  Created by apps on 10/19/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "AltimeterAppDelegate.h"
#import "RootViewController.h"

@implementation AltimeterAppDelegate


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
