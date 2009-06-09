//
//  WhereAmIAppDelegate.m
//  WhereAmI
//
//  Created by acs on 10/10/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "WhereAmIAppDelegate.h"
#import "RootViewController.h"

@implementation WhereAmIAppDelegate


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
