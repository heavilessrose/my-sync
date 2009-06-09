//
//  main.m
//  SpeedDial
//
//  Created by apple on 10/7/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int retVal = 0;
	// need this code so the app does not create a recursive dialing loop when the
	// user hangs up the phone.
	NSString* justDialed = [[NSUserDefaults standardUserDefaults] stringForKey: @"just_dialed"];
	if(justDialed && [justDialed isEqualToString:@"yes"]) {
		[[NSUserDefaults standardUserDefaults] setValue:@"no" forKey:@"just_dialed"];
	}
	else {
		retVal = UIApplicationMain(argc, argv, nil, nil);
	}
	[pool release];
	return retVal;
}
