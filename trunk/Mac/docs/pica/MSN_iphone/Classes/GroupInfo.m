//
//  GroupInfo.m
//  msn_for_mac
//
//  Created by  apple on 08-9-19.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "GroupInfo.h"
#import "ListHeaderView.h"

@implementation GroupInfo
@synthesize name, stringId, gbcode, sync, persons,cell;

- (id) init
{
	self = [super init];
	if (self != nil)
	{
		name = nil;
		stringId = nil;
		gbcode = nil;
		sync = nil;
		persons = [[NSMutableArray alloc] init];
	}
	return self;
}

- (id)initWithDictionary:(NSDictionary*)dict
{
	self = [super init];
	if (self != nil)
	{
		NSString *value = [dict objectForKey:@"stringId"];
		if (value != nil)
			self.stringId = value;
		
		value = [dict objectForKey:@"name"];
		if (value != nil)
			self.name = value;
		
		value = [dict objectForKey:@"sync"];
		if (value != nil)
			self.sync = value;

		value = [dict objectForKey:@"gbcode"];
		if (value != nil)
			self.gbcode = value;
		
		persons = [[NSMutableArray alloc] init];

	}
	return self;
}

- (id)copyWithZone:(NSZone *)zone
{
    GroupInfo *copy = [[GroupInfo allocWithZone: zone] init];
	copy.name = [[name copy] autorelease];
	copy.stringId = [[stringId copy] autorelease];
	copy.gbcode = [[gbcode copy] autorelease];
	copy.sync = [[sync copy] autorelease];
	copy.persons = [[persons mutableCopy] autorelease];
    return copy;
}

- (void) dealloc
{
	[name release];
	[stringId release];
	[gbcode release];
	[sync release];
	[persons release];
	[super dealloc];
}

- (void)updateInfo:(GroupInfo*)theInfo
{
	BOOL needNotify = NO;
	
	if (theInfo.name != nil)
	{
		if (self.name != nil)
		{
			if ([self.name compare:theInfo.name] != NSOrderedSame)
			{
				needNotify = YES;
				self.name = theInfo.name;
			}
		}
		else
		{
			needNotify = YES;
			self.name = theInfo.name;
		}
	}
	
	if (theInfo.gbcode != nil)
		self.gbcode = theInfo.gbcode;
	
	if (theInfo.sync != nil)
		self.sync = theInfo.sync;
}


- (NSDictionary*)dictionaryRepresentation;
{
	NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:0];
	
	if (stringId != nil)
		[dict setObject:stringId forKey:@"stringId"];
	
	if (name != nil)
		[dict setObject:name forKey:@"name"];
	
	if (gbcode != nil)
		[dict setObject:gbcode forKey:@"gbcode"];
	
	if (sync != nil)
		[dict setObject:sync forKey:@"sync"];
	
	return dict;
}

@end
