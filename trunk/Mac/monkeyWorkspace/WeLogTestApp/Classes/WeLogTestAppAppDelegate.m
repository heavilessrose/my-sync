//
//  WeLogTestAppAppDelegate.m
//  WeLogTestApp
//
//  Created by luke on 09-12-28.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "WeLogTestAppAppDelegate.h"
#import "WeLogTestAppViewController.h"
#import "SampleClass.h"

@implementation WeLogTestAppAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    
	// tests
	[[NSThread currentThread] setName:@"MainThread"];
	[SampleClass debugExample];
	SampleClass *sample = [[SampleClass alloc] init];
	[sample debugExample];
	[sample release];
	
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
