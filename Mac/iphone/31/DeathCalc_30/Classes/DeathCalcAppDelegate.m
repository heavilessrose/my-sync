//
//  DeathCalcAppDelegate.m
//  DeathCalc
//
//  Created by apps on 11/1/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "DeathCalcAppDelegate.h"
#import "RootViewController.h"

@implementation DeathCalcAppDelegate


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
