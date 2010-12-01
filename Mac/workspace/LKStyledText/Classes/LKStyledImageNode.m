//
//  LKStyledImageNode.m
//  LKStyledText
//
//  Created by luke on 10-12-1.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledImageNode.h"


@implementation LKStyledImageNode

@synthesize image;

- (id)initWithImage:(UIImage *)aImg {
	
	if (self = [super init]) {
		self.image = aImg;
	}
	return self;
}

- (void)dealloc {
	
	[image release];
	[super dealloc];
}

@end
