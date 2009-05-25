//
//  FourLines.m
//  Persistence
//
//  Created by Jeff LaMarche on 7/29/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "FourLines.h"


@implementation FourLines
@synthesize field1;
@synthesize field2;
@synthesize field3;
@synthesize field4;

#pragma mark NSCoding
- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeObject:field1 forKey:kField1Key];
	[encoder encodeObject:field2 forKey:kField2Key];
	[encoder encodeObject:field3 forKey:kField3Key];
	[encoder encodeObject:field4 forKey:kField4Key];
}
- (id)initWithCoder:(NSCoder *)decoder
{
	if (self = [super init])
	{
		self.field1 = [decoder decodeObjectForKey:kField1Key];
		self.field2 = [decoder decodeObjectForKey:kField2Key];
		self.field3 = [decoder decodeObjectForKey:kField3Key];
		self.field4 = [decoder decodeObjectForKey:kField4Key];
	}
	return self;
}

#pragma mark -
#pragma mark NSCopying
- (id)copyWithZone:(NSZone *)zone
{
	FourLines *copy = [[[self class] allocWithZone: zone] init];
	copy.field1 = [self.field1 copy];
	copy.field2 = [self.field2 copy];
	copy.field3 = [self.field3 copy];
	copy.field4 = [self.field4 copy];
	return copy;
}
@end
