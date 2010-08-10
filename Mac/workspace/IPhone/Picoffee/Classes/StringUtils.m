/*
 *  StringUtils.cpp
 *  Picoffee
 *
 *  Created by luke on 8/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "StringUtils.h"

@implementation StringUtils


- (NSString *)test:(NSString *)format, ...
{
    NSString *localizedStr = [NSString localizedStringWithFormat:@"Your passport is %d", 456];
    NSLog(@"%@", localizedStr);
    return localizedStr;
}

#pragma mark -
#pragma mark System

- (BOOL)isMultitaskingSupported
{
    BOOL backgroundSupported = NO;
    /*
    UIDevice* device = [UIDevice currentDevice];
    if ([device respondsToSelector:@selector(isMultitaskingSupported)])
        backgroundSupported = device.multitaskingSupported;
     */
    return backgroundSupported;
}

@end

