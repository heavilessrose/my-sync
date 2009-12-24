//
//  UIFrameworkAppDelegate.m
//  UIFramework
//
//  Created by luke on 09-12-22.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "UIFrameworkAppDelegate.h"

@implementation UIFrameworkAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    

    // Override point for customization after application launch
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
