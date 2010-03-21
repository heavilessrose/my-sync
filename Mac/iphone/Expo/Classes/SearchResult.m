//
//  SearchResult.m
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "SearchResult.h"

@implementation SearchResult

@synthesize title, bigImageURL, thumbnailURL, txtInfo, bigImageSize;

- (void)dealloc
{
    [title release];
    [bigImageURL release];
    [thumbnailURL release];
	[txtInfo release];
    [super dealloc];
}

@end
