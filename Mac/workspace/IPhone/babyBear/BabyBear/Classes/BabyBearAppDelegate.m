//
//  BabyBearAppDelegate.m
//  BabyBear
//
//  Created by luke on 8/3/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "BabyBearAppDelegate.h"

@implementation BabyBearAppDelegate

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
