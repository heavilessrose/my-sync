//
//  DigiClockAppDelegate.m
//  DigiClock
//
//  Created by amuck on 10/29/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "DigiClockAppDelegate.h"
#import "RootViewController.h"

@implementation DigiClockAppDelegate


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
