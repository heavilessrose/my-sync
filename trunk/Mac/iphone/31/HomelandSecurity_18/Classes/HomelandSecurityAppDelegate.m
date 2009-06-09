//
//  HomelandSecurityAppDelegate.m
//  HomelandSecurity
//
//  Created by apps on 10/18/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "HomelandSecurityAppDelegate.h"
#import "RootViewController.h"

@implementation HomelandSecurityAppDelegate


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
