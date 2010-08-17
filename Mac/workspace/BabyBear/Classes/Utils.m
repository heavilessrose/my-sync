//
//  Utils.m
//  BabyBear
//
//  Created by luke on 10-8-17.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Utils.h"


@implementation Utils

+ (BOOL)isMultitasking
{
	UIDevice* device = [UIDevice currentDevice];
	BOOL backgroundSupported = NO;
	if ([device respondsToSelector:@selector(isMultitaskingSupported)])
		backgroundSupported = device.multitaskingSupported;
	return backgroundSupported;
}

@end
