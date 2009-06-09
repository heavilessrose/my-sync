//
//  WhatsMyIPAppDelegate.m
//  WhatsMyIP
//
//  Created by apple on 10/4/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "WhatsMyIPAppDelegate.h"
#import "RootViewController.h"

@implementation WhatsMyIPAppDelegate


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
