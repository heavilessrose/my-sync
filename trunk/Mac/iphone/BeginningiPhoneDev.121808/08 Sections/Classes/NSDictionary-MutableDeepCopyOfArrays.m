#import "NSDictionary-MutableDeepCopyOfArrays.h"

@implementation NSDictionary (MutableDeepCopyOfArrays)

- (NSMutableDictionary *)mutableDeepCopyOfArrays
{
	NSMutableDictionary *ret = [NSMutableDictionary dictionaryWithCapacity:[self count]];
	NSArray *keys = [self allKeys];
	for (id key in keys)
	{
		NSArray *oneArray = [self valueForKey:key];
		NSMutableArray *arrayCopy = [[NSMutableArray alloc] initWithArray:oneArray];
		[ret setValue:arrayCopy forKey:key];
		[arrayCopy release];
	}
	return ret;
}
- (NSMutableDictionary *) mutableDeepCopy
{
	NSMutableDictionary *ret = [NSMutableDictionary dictionaryWithCapacity:[self count]];
	NSArray *keys = [self allKeys];
	for (id key in keys)
	{
		id oneValue = [self valueForKey:key];
		id oneCopy = nil;
		
		if ([oneValue respondsToSelector:@selector(mutableDeepCopy)])
			oneCopy = [oneValue mutableDeepCopy];
		else if ([oneValue respondsToSelector:@selector(mutableCopy)])
			oneCopy = [oneValue mutableCopy];
		
		if (oneCopy == nil)
			oneCopy = [oneValue copy];
		[ret setValue:oneCopy forKey:key];
	}
	return ret;
}

@end

