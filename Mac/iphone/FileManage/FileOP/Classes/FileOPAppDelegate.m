//
//  FileOPAppDelegate.m
//  FileOP
//
//  Created by wang luke on 5/4/09.
//  Copyright luke 2009. All rights reserved.
//

#import "FileOPAppDelegate.h"
#import "FileOPViewController.h"

@implementation FileOPAppDelegate

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
