//
//  TemperatureConverterAppDelegate.m
//  TemperatureConverter
//
//  Created by apps on 10/21/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "TemperatureConverterAppDelegate.h"
#import "RootViewController.h"

@implementation TemperatureConverterAppDelegate


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
