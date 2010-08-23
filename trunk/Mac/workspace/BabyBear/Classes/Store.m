//
//  Store.m
//  BabyBear
//
//  Created by luke on 10-8-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Store.h"


@implementation Store

@synthesize sid, sname, saddr, sreserves, slongitude, slatitude;

- (void)dealloc
{
	[sid release];
	[sname release];
	[saddr release];
	[sreserves release];
	[slongitude release];
	[slatitude release];
	
	[super dealloc];
}

- (NSString *)description
{
	return [NSString stringWithFormat:@"sid = %@, sname: %@", sid, sname];
}

@end
