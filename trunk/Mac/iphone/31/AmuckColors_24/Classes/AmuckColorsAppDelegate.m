//
//  AmuckColorsAppDelegate.m
//  AmuckColors
//
//  Created by AppsAmuck on 10/25/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "AmuckColorsAppDelegate.h"
#import "RootViewController.h"

@implementation AmuckColorsAppDelegate


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
