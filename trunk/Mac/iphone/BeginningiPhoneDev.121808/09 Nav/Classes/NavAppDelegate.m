//
//  NavAppDelegate.m
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "NavAppDelegate.h"

@implementation NavAppDelegate

@synthesize window;
@synthesize navController;
- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	
	// Override point for customization after app launch	
	[window addSubview:navController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
	[navController release];
	[window release];
	[super dealloc];
}


@end
