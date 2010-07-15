//
//  PicoffeeAppDelegate.m
//  Picoffee
//
//  Created by luke on 6/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "PicoffeeAppDelegate.h"
#import "TestViewController.h"

@implementation PicoffeeAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    

	viewController = [[TestViewController alloc] init];
	[window addSubview:viewController.view];
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}


- (void)dealloc {
	[viewController release];
    [window release];
    [super dealloc];
}


@end
