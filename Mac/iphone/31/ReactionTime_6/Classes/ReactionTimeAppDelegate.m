//
//  ReactionTimeAppDelegate.m
//  ReactionTime
//
//  Created by acs on 10/6/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "ReactionTimeAppDelegate.h"
#import "RootViewController.h"

@implementation ReactionTimeAppDelegate


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
