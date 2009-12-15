//
//  weHelp_UIAppDelegate.m
//  weHelp_UI
//
//  Created by luke on 09-12-10.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "weHelp_UIAppDelegate.h"
#import "weHelp_UIViewController.h"

@implementation weHelp_UIAppDelegate

@synthesize window;
@synthesize rootViewControl;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    
    // Override point for customization after app launch    
    [window addSubview:rootViewControl.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [rootViewControl release];
    [window release];
    [super dealloc];
}


@end
