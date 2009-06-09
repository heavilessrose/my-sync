//
//  FireballAppDelegate.m
//  Fireball
//
//  Created by AppsAmuck on 10/17/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "FireballAppDelegate.h"
#import "RootViewController.h"

@implementation FireballAppDelegate


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
