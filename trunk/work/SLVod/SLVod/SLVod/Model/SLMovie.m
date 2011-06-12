//
//  SLMovie.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLMovie.h"


@implementation SLMovie

@synthesize title;
@synthesize imageUrl;
@synthesize url;
@synthesize content;
@synthesize actor;
@synthesize genre;

- (void)dealloc
{
    self.title = nil;
    self.imageUrl = nil;
    self.url = nil;
    self.content = nil;
    self.actor = nil;
    self.genre = nil;
    [super dealloc];
}

- (id)initWithDic:(NSDictionary *)aDic
{
    if ((self = [super init])) {
        if (aDic && [aDic count] > 0) {
            self.title = [aDic objectForKey:@"title"];
            self.imageUrl = [aDic objectForKey:@"image"];
            self.url = [NSURL URLWithString:[aDic objectForKey:@"url"]];
            self.content = [aDic objectForKey:@"content"];
            self.actor = [aDic objectForKey:@"actor"];
            self.genre = [aDic objectForKey:@"typename"];
            return self;
        } else {
            self = nil;
            return nil;
        }
    }
    return nil;
}

@end
