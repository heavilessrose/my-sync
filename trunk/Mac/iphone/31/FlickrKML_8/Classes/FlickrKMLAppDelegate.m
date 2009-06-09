//
//  FlickrKMLAppDelegate.m
//  FlickrKML
//
//  Created by acs on 10/7/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "FlickrKMLAppDelegate.h"
#import "RootViewController.h"

@implementation FlickrKMLAppDelegate


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
