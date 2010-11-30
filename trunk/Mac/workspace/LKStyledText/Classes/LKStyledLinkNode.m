//
//  LKStyledLinkNode.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledLinkNode.h"


@implementation LKStyledLinkNode

@synthesize URL;

- (void)dealloc {
	
	[URL release];
	[super dealloc];
}

- (NSString *)description {
	
	return [NSString stringWithFormat:@"[LinkNode]: text= [%@], URL= [%@]", text, URL];
}

@end
