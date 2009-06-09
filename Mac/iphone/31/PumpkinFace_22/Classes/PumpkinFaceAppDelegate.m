//
//  PumpkinFaceAppDelegate.m
//  PumpkinFace
//
//  Created by AppsAmuck on 10/23/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "PumpkinFaceAppDelegate.h"
#import "RootViewController.h"

@implementation PumpkinFaceAppDelegate


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
