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
// The implementation of copyWithZone on page 346 is incorrect. Either this one,
// or the one commented out below are correct.
- (id)copyWithZone:(NSZone *)zone
{
	FourLines *copy = [[[self class] allocWithZone: zone] init];

	NSString *field1Copy = [self.field1 copy];
	copy.field1 = field1Copy;
	[field1Copy release];
	
	NSString *field2Copy = [self.field2 copy];
	copy.field2 = field2Copy;
	[field2Copy release];
	
	NSString *field3Copy = [self.field3 copy];
	copy.field3 = field3Copy;
	[field3Copy release];
	
	NSString *field4Copy = [self.field4 copy];
	copy.field4 = field4Copy;
	[field4Copy release];

	return copy;
}
//- (id)copyWithZone:(NSZone *)zone
//{
//	FourLines *copy = [[[self class] allocWithZone: zone] init];
//	copy.field1 = [[self.field1 copy] autorelease];
//	copy.field2 = [[self.field2 copy] autorelease];
//	copy.field3 = [[self.field3 copy] autorelease];
//	copy.field4 = [[self.field4 copy] autorelease];
//	return copy;
//}
@end
