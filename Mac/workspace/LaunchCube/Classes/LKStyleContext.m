//
//  LKStyleContext.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKStyleContext.h"
#import "LKRectangleShape.h"


@implementation LKStyleContext

@synthesize frame, contentFrame, shape, font, didDrawContent, delegate;

- (id)init {
	
	if (self = [super init]) {
		self.frame = CGRectZero;
		self.contentFrame = CGRectZero;
	}
	
	return self;
}

- (void)dealloc {
	
	self.shape = nil;
	self.font = nil;
	[super dealloc];
}

#pragma mark -

- (LKShape *)shape {
	
	if (!self.shape) {
		self.shape = [LKRectangleShape shape];
	}
	
	return shape;
}

@end
