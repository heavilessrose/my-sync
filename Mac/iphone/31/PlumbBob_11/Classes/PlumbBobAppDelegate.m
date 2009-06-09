//
//  PlumbBobAppDelegate.m
//  PlumbBob
//
//  Created by apple on 10/11/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "PlumbBobAppDelegate.h"
#import "RootViewController.h"

@implementation PlumbBobAppDelegate


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
