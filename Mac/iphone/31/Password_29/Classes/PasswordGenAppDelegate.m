//
//  PasswordGenAppDelegate.m
//  PasswordGen
//
//  Created by amuck on 8/18/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "PasswordGenAppDelegate.h"
#import "RootViewController.h"

@implementation PasswordGenAppDelegate


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
