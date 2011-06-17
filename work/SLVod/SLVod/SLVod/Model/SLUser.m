//
//  SLUser.m
//  SLVod
//
//  Created by luke on 11-6-17.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLUser.h"


@implementation SLUser

@synthesize username, avatar;

- (void)dealloc
{
    MLog(@"");
    [username release];
    [avatar release];
    [super dealloc];
}

- (id)initWithDic:(NSDictionary *)usrInfo
{
    if ((self = [super init])) {
        
    }
    return self;
}

@end
