//
//  LKPartStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKPartStyle.h"


@implementation LKPartStyle

@synthesize name, style;

- (void)dealloc {
	
	self.name = nil;
	self.style = nil;
	[super dealloc];
}


#pragma mark -

+ (LKPartStyle *)styleWithName:(NSString *)name style:(LKStyle *)aStyle next:(LKStyle *)next {
	
	LKPartStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.name = name;
	style.style = aStyle;
	return style;	
}

#pragma mark -
#pragma mark LKStyle

- (void)draw:(LKStyleContext *)context {
	[self.next draw:context];
}

- (void)drawPart:(LKStyleContext *)context {
	
	[style draw:context];
}

@end
