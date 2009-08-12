//
//  PeoplePickerDemoAppDelegate.m
//  PeoplePickerDemo
//
//  Copyright Apple, Inc. 2008. All rights reserved.
//

#import "PeoplePickerDemoAppDelegate.h"
#import "RootViewController.h"


@implementation PeoplePickerDemoAppDelegate

@synthesize window;
@synthesize navigationController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	
	[window addSubview:[navigationController view]];
	[window makeKeyAndVisible];
}


- (void)applicationWillTerminate:(UIApplication *)application {
}


- (void)dealloc {
	[navigationController release];
	[window release];
	[super dealloc];
}

@end
