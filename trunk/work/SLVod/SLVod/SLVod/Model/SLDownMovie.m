//
//  SLDownMovie.m
//  SLVod
//
//  Created by luke on 11-7-30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLDownMovie.h"
#import "SLMovie.h"
#import "LKTools.h"
#import "ASIHTTPRequest.h"

@implementation SLDownMovie

@synthesize downloadPercent, hash, size, path, movie, movieReq;

- (void)dealloc
{
    MLog(@"");
    [movie release];
    [hash release];
    [path release];
    [super dealloc];
}

- (id)initWithMovie:(SLMovie *)mov req:(ASIHTTPRequest *)req
{
    self = [super init];
    if (self) {
        self.movie = mov;
        self.movieReq = req;
        self.hash = keyForURL(mov.url);
    }
    
    return self;
}

@end
