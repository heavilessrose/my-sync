//
//  AddrBook_SQLite3AppDelegate.m
//  AddrBook_SQLite3
//
//  Created by wang luke on 09-10-14.
//  Copyright luke 2009. All rights reserved.
//

#import "AddrBook_SQLite3AppDelegate.h"
#import "AddrBook_SQLite3ViewController.h"

@implementation AddrBook_SQLite3AppDelegate

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
