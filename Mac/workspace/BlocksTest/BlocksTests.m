//
//  BlocksTests.m
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "BlocksTests.h"


@implementation BlocksTests

@synthesize instanceVariable;

- (id)init
{
	if (self = [super init]) {
		self.instanceVariable = [NSString stringWithCString:"实例变量" encoding:NSUTF8StringEncoding];
		[self retainTest];
		return self;
	}
	return nil;
}

- (void)dealloc
{
	//[instanceVariable release];
	
	[super dealloc];
}

#pragma mark -
- (void)runTest
{
#pragma mark using in reference-counted environment (OC)
	dispatch_queue_t queue = dispatch_queue_create(NULL, NULL);
	
#if 0
	NSLog(@"1 - self count = %u", [self retainCount]);
	dispatch_async(queue, ^{
		//???: self没有retain阿
		// instanceVariable is used by reference, self is retained
		NSLog(@"instanceVariable: %@, count = %u", instanceVariable, [instanceVariable retainCount]);
	});
	NSLog(@"2 - self count = %u", [self retainCount]);
#else
	NSLog(@"3 - self count = %u", [self retainCount]);
	//TODO: __block修饰就不会增加计数了, 如果没有修饰block会把localVariable+1
	__block NSString *localVariable = instanceVariable;
	NSLog(@"1 - localVariable: %@, count = %u", localVariable, [localVariable retainCount]);
	dispatch_async(queue, ^{
		// localVariable is used by value, localVariable is retained (not self)
		NSLog(@"2 - localVariable: %@, count = %u", localVariable, [localVariable retainCount]);
	});
	NSLog(@"4 - self count = %u", [self retainCount]);
#endif
	//TODO: 正常使用
	[self retainTest];
}

- (void)retainTest
{
	NSString *localVariable = instanceVariable;
	NSLog(@"4 - localVariable: %@, count = %u", localVariable, [localVariable retainCount]);
}

@end
