//
//  LKStyleContext.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKStyleContext.h"


@implementation LKStyleContext

@synthesize frame, contentFrame, shape, font, didDrawContent, delegate;

- (void)dealloc {
	
	[shape release];
	[font release];
	[super dealloc];
}

@end
