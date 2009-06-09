//
//  CountMeInAppDelegate.m
//  CountMeIn
//
//  Created by acs on 10/5/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "CountMeInAppDelegate.h"
#import "RootViewController.h"

@implementation CountMeInAppDelegate


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
