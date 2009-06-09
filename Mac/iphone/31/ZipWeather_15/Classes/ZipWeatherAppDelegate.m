//
//  ZipWeatherAppDelegate.m
//  ZipWeather
//
//  Created by AppsAmuck on 10/15/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "ZipWeatherAppDelegate.h"
#import "RootViewController.h"

@implementation ZipWeatherAppDelegate


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
