//
//  Person.m
//  RaiseMan
//
//  Created by Aaron Hillegass on 9/24/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "Person.h"


@implementation Person

@synthesize expectedRaise;
@synthesize personName;

- (id)init
{
	[super init];
	expectedRaise = 0.05;
	personName = @"New Person";
	return self;
}

- (void)dealloc
{
	[personName release];
	[super dealloc];
}

#pragma mark NSCoding

- (id)initWithCoder:(NSCoder *)c
{
	[super init];
	personName = [[c decodeObjectForKey:@"personName"] retain];
	expectedRaise = [c decodeFloatForKey:@"expectedRaise"];
	return self;
}

- (void)encodeWithCoder:(NSCoder *)c
{
	[c encodeObject:personName forKey:@"personName"];
	[c encodeFloat:expectedRaise forKey:@"expectedRaise"];
}

@end

