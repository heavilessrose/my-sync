//
//  Multitasking.m
//  BlocksTest
//
//  Created by luke on 10-9-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Multitasking.h"


@implementation Multitasking

+ (BOOL)isMultitasking
{
	UIDevice* device = [UIDevice currentDevice];
	BOOL backgroundSupported = NO;
	if ([device respondsToSelector:@selector(isMultitaskingSupported)])
		backgroundSupported = device.multitaskingSupported;
	return backgroundSupported;
}

- (void)backgroundTask
{
	
}

@end
