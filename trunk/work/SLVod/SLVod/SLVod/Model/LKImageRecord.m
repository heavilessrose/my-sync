//
//  WBImageRecord.m
//  WBNetwork
//
//  Created by luke on 11-2-23.
//  Copyright 2011 58.com. All rights reserved.
//

#import "LKImageRecord.h"


@implementation LKImageRecord

@synthesize img, show;
@synthesize url;

- (void)dealloc
{
	MLog(@"");
	[img release];
	[url release];
	[super dealloc];
}

- (id)initWithUrl:(NSString *)urlStr
{
    if ((self = [super init])) {
        self.show = YES;
        self.url = [NSURL URLWithString:urlStr];
    }
    return self;
}

@end
