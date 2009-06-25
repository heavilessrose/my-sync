//
//  Tabs1AppDelegate.m
//  Tabs1
//
//  Created by wang luke on 6/25/09.
//  Copyright luke 2009. All rights reserved.
//

#import "Tabs1AppDelegate.h"

@implementation Tabs1AppDelegate

@synthesize window;
@synthesize rootController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    

    // Override point for customization after application launch
    [window addSubview:rootController.view];
	[window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
