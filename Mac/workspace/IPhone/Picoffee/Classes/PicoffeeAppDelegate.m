//
//  PicoffeeAppDelegate.m
//  Picoffee
//
//  Created by luke on 6/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "PicoffeeAppDelegate.h"

@implementation PicoffeeAppDelegate

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
