//
//  Review.m
//  BabyBear
//
//  Created by luke on 10-8-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Review.h"


@implementation Review

@synthesize rname, rrating, rdate, rtitle, rcatagory, rcomment;


- (void)dealloc
{
	[rname release];
	[rrating release];
	[rdate release];
	[rtitle release];
	[rcatagory release];
	[rcomment release];
	
	[super dealloc];
}

- (NSString *)description
{
	return [NSString stringWithFormat:@"rname = %@, rdate: %@, rtitle: %@", rname, rdate, rtitle];
}

@end
