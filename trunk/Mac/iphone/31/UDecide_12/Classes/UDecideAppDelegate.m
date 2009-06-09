//
//  UDecideAppDelegate.m
//  UDecide
//
//  Created by acs on 10/12/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "UDecideAppDelegate.h"
#import "RootViewController.h"

@implementation UDecideAppDelegate


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
