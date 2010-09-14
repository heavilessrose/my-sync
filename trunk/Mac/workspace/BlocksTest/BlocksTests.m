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
		return self;
	}
	return nil;
}

- (void)dealloc
{
	[instanceVariable release];
	
	[super dealloc];
}

#pragma mark -
- (void)runTest
{
#pragma mark using in reference-counted environment (OC)
	dispatch_queue_t queue = dispatch_queue_create(NULL, NULL);
	
#if 1
	dispatch_async(queue, ^{
		// instanceVariable is used by reference, self is retained
		NSLog(@"instanceVariable: %@, count = %u", instanceVariable, [instanceVariable retainCount]);
	});
#else
	NSString *localVariable = instanceVariable;
	dispatch_async(queue, ^{
		// localVariable is used by value, localVariable is retained (not self)
		NSLog(@"localVariable: %@, count = %u", localVariable, [localVariable retainCount]);
	});
#endif
	
}

@end
