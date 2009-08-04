//
//  greetAppDelegate.m
//  greet
//
//  Created by wang luke on 8/3/09.
//  Copyright luke 2009. All rights reserved.
//

#import "greetAppDelegate.h"

@implementation greetAppDelegate

@synthesize window;
@synthesize greetView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	
	// Create window
	self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Set up content view
	self.greetView = [[[GreetView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease];
	[window addSubview:greetView];
	// Show window
	[window makeKeyAndVisible];
}


- (void)dealloc {
	[greetView release];
    [window release];
    [super dealloc];
}


@end
