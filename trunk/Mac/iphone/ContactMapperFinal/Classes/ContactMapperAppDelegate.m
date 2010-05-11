//
//  ContactMapperAppDelegate.m
//  ContactMapper
//
//  Created by Bill Dudney on 5/15/09.
//  Copyright Gala Factory Software LLC 2009. All rights reserved.
//
//
//  Licensed with the Apache 2.0 License
//  http://apache.org/licenses/LICENSE-2.0
//

#import "ContactMapperAppDelegate.h"
#import "ContactMapperViewController.h"

@implementation ContactMapperAppDelegate

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
