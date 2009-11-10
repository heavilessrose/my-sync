//
//  AddressBookFillAppDelegate.m
//  AddressBookFill
//
//  Created by Doug Diego on 4/10/09.
//  Copyright Doug Diego 2009. All rights reserved.
//

#import "AddressBookFillAppDelegate.h"
#import "AddressBookFillViewController.h"

@implementation AddressBookFillAppDelegate

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
