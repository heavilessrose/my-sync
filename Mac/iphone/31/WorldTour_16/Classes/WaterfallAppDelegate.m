//
//  WaterfallAppDelegate.m
//  Waterfall
//
//  Created by acs on 9/7/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "WaterfallAppDelegate.h"
#import "RootViewController.h"

@implementation WaterfallAppDelegate


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
