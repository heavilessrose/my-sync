/*
File: Message.m
Abstract: Network messages.

Version: 2.0
*/

#import "Message.h"

@implementation Message

+ (id) messageWithType:(MessageType)type data:(id)data
{
	Message* result;
	
	if ((result = [[self alloc] init])) {
		result->type = type;
		if (data)
		result->data = [data retain];
	}
	
	return [result autorelease];
}

- (void) dealloc
{
	[data release];
	
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder
{
	[aCoder encodeInt:type forKey:@"type"];
	if (data)
	[aCoder encodeObject:data forKey:@"data"];	
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
	if ((self = [super init])) {
		type = (MessageType)[aDecoder decodeIntForKey:@"type"];
		data = [[aDecoder decodeObjectForKey:@"data"] retain];
	}
	return self;
}

@end
