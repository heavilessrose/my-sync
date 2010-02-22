//
//  DrawTemplateAppDelegate.m
//  DrawTemplate
//
//  Created by luke on 10-1-28.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "DrawTemplateAppDelegate.h"
#import "DrawTemplateViewController.h"

@implementation DrawTemplateAppDelegate

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
