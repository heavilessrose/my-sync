//
//  FourLines.m
//  archive
//
//  Created by wang luke on 5/4/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "FourLines.h"

@implementation FourLines

@synthesize field1;
@synthesize field2;
@synthesize field3;
@synthesize field4;

#pragma mark -
#pragma mark NSCoding
// 编码
- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeObject:field1 forKey:kField1key];
	[encoder encodeObject:field2 forKey:kField2key];
	[encoder encodeObject:field3 forKey:kField3key];
	[encoder encodeObject:field4 forKey:kField4key];
}

// 解码
- (id)initWithCoder:(NSCoder *)decoder
{
	if(self = [super init]){
		self.field1 = [decoder decodeObjectForKey:kField1key];
		self.field2 = [decoder decodeObjectForKey:kField2key];
		self.field3 = [decoder decodeObjectForKey:kField3key];
		self.field4 = [decoder decodeObjectForKey:kField4key];
	}
	return self;
}

#pragma mark -
#pragma mark NSCopying
// 对象拷贝
- (id)copyWithZone:(NSZone *)zone
{
	FourLines *copy = [[[self class] allocWithZone:zone] init];
	field1 = [self.field1 copy];
	field2 = [self.field2 copy];
	field3 = [self.field3 copy];
	field4 = [self.field4 copy];
	
	return copy;
}
@end
