//
//  LKStyledNode.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledNode.h"


@implementation LKStyledNode

@synthesize nextNode;

- (void)dealloc {
	
	[nextNode release];
	[super dealloc];
}

@end
