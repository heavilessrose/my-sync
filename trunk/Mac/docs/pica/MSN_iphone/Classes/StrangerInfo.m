//
//  Stranger.m
//  MSN
//
//  Created by  apple on 08-10-20.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "StrangerInfo.h"


@implementation StrangerInfo
@synthesize imid,nickname,realname,sex,age,province,desc, group;
- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		imid = nil;
		nickname = nil;
		realname = nil;
		sex = nil;
		age = nil;
		province = nil;
		desc = nil;
		group = nil;
	}
	return self;
}

- (id)copyWithZone:(NSZone *)zone
{
    StrangerInfo *copy = [[StrangerInfo allocWithZone: zone] init];
	copy.imid = self.imid;
	copy.nickname = self.nickname;
	copy.realname = self.realname;
	copy.sex = self.sex;
	copy.age = self.age;
	copy.province = self.province;
	copy.desc = self.desc;
	copy.group = self.group;
    return copy;
}

- (void) dealloc
{
	[imid release];
	[nickname release];
	[realname release];
	[sex release];
	[age release];
	[province release];
	[desc release];
	[group release];
	[super dealloc];
}

@end
