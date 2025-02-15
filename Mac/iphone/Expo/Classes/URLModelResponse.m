//
//  URLModelResponse.m
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "URLModelResponse.h"


@implementation URLModelResponse

@synthesize objects, totalObjectsAvailableOnServer;

+ (id)response
{
    return [[[[self class] alloc] init] autorelease];
}

- (id)init
{
    if ((self = [super init])) {
        objects = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc
{
    [objects release];
    [super dealloc];
}

//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark TTURLResponse protocol

- (NSError*)request:(TTURLRequest*)request processResponse:(NSHTTPURLResponse*)response data:(id)data
{
    NSAssert(NO, @"URLModelResponse is an abstract class. Sub-classes must implement request:processResponse:data:");
    return nil;
}

- (NSString *)format
{
    NSAssert(NO, @"URLModelResponse is an abstract class. Sub-classes must implement format");
    return nil;
}


@end
