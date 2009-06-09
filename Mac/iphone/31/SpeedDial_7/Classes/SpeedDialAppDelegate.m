//
//  SpeedDialAppDelegate.m
//  SpeedDial
//
//  Created by apple on 10/7/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "SpeedDialAppDelegate.h"
#import "SpeedDialViewController.h"

@implementation SpeedDialAppDelegate

@synthesize window;
@synthesize viewController;

BOOL doesStringContain(NSString* string, NSString* charcter){
	for (int i=0; i<[string length]; i++) {
		NSString* chr = [string substringWithRange:NSMakeRange(i, 1)];
		if([chr isEqualToString:charcter])
			return TRUE;
	}
	return FALSE;
}

BOOL tryDial() {
	NSString* rawNumber = [[NSUserDefaults standardUserDefaults] stringForKey: @"phonenumber_preference"];
	if(rawNumber && ![rawNumber isEqualToString:@""]) {
		NSString* telNumber = @"";
		for (int i=0; i<[rawNumber length]; i++) {
			NSString* chr = [rawNumber substringWithRange:NSMakeRange(i, 1)];
			if(doesStringContain(@"0123456789", chr)) {
				telNumber = [telNumber stringByAppendingFormat:@"%@", chr];
			}
		}
		telNumber = [NSString stringWithFormat:@"tel:%@", telNumber]; 
		[[UIApplication sharedApplication] openURL:[[NSURL alloc] initWithString:telNumber]];	
		return TRUE;
	}	
	return FALSE;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	if(tryDial()) {
		[[NSUserDefaults standardUserDefaults] setValue:"yes" forKey:@"just_dialed"];
		return;
	}
	[window addSubview:viewController.view];
	[window makeKeyAndVisible];
}

- (void)dealloc {
    [viewController release];
	[window release];
	[super dealloc];
}


@end
