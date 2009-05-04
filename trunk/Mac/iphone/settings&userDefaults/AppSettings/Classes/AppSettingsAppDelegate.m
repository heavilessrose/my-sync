//
//  AppSettingsAppDelegate.m
//  AppSettings
//
//  Created by luke on 5/2/09.
//  Copyright luke 2009. All rights reserved.
//

#import "AppSettingsAppDelegate.h"
#import "RootViewController.h"

@implementation AppSettingsAppDelegate


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
