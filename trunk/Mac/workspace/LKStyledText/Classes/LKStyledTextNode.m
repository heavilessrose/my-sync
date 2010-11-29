//
//  LKStyledTextNode.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledTextNode.h"


@implementation LKStyledTextNode

@synthesize text;

- (id)initWithText:(NSString *)aStr {
	
	if (self = [super init]) {
		self.text = aStr;
	}
	return self;
}

- (void)dealloc {
	
	[text release];
	[super dealloc];
}

- (NSString *)description {
	
	NSString *des = [NSString stringWithFormat:@"[TextNode] : [%@]", text];
	return des;
}

@end
