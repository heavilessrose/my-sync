//
//  CameraAppDelegate.m
//  Camera
//
//  Created by Jeff LaMarche on 8/8/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "CameraAppDelegate.h"
#import "CameraViewController.h"

@implementation CameraAppDelegate

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
