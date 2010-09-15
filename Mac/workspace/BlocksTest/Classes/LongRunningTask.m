//
//  LongRunningTask.m
//  BlocksTest
//
//  Created by luke on 10-9-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LongRunningTask.h"
#import <unistd.h>
#import "BlocksTestAppDelegate.h"


@implementation LongRunningTask

@synthesize bgTaskID;


- (BOOL)runTask
{
	UIApplication *app = [UIApplication sharedApplication];
	BlocksTestAppDelegate *appDelegate = (BlocksTestAppDelegate *)app.delegate;
	NSLog(@"%@ -------- start runTask --------", appDelegate.appStatus);
	while (app.backgroundTimeRemaining/* > 590*/) {
		NSLog(@"remain = %f %@", +app.backgroundTimeRemaining, appDelegate.appStatus);
		sleep(1);
	}
	return YES;
}

@end
