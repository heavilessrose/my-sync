//
//  PhotoFrameAppDelegate.m
//  PhotoFrame
//
//  Created by amuck on 10/28/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "PhotoFrameAppDelegate.h"
#import "RootViewController.h"

@implementation PhotoFrameAppDelegate


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
