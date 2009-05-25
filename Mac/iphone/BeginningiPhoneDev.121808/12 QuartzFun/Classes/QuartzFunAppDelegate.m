//
//  QuartzFunAppDelegate.m
//  QuartzFun
//
//  Created by Jeff LaMarche on 7/31/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "QuartzFunAppDelegate.h"
#import "QuartzFunViewController.h"

@implementation QuartzFunAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	
	// Override point for customization after app launch	
    [window addSubview:viewController.view];
	[window makeKeyAndVisible];
}


- (void)dealloc {
    [viewController release];
	[window release];
	[super dealloc];
}


@end
