//
//  SLDownMovie.m
//  SLVod
//
//  Created by luke on 11-7-30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLDownMovie.h"

@implementation SLDownMovie

@synthesize downloadPercent, hash, size, path;

- (void)dealloc
{
    MLog(@"");
    [hash release];
    [path release];
    [super dealloc];
}

- (id)init
{
    self = [super init];
    if (self) {
        
    }
    
    return self;
}

@end
