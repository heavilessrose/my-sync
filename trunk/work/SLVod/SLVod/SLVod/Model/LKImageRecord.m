//
//  WBImageRecord.m
//  WBNetwork
//
//  Created by luke on 11-2-23.
//  Copyright 2011 58.com. All rights reserved.
//

#import "LKImageRecord.h"


@implementation LKImageRecord

@synthesize picImage;
@synthesize imageURLString;

- (void)dealloc
{
	MLog(@"");
	[picImage release];
	[imageURLString release];
	[super dealloc];
}

@end
