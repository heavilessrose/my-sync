//
//  LKTip.m
//  SK
//
//  Created by luke on 10-11-17.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKTip.h"


@implementation LKTip

@synthesize image, message, time;

- (id)initWithMsg:(NSString *)msg img:(UIImage *)img durationTime:(NSTimeInterval)dTime {
	
	if ((self = [super init])) {
		self.message = msg;
		self.image = img;
		self.time = dTime;
	}
	return self;
}

- (void)dealloc {
	
	MLog(@"");
	[image release];
	[message release];
	[super dealloc];
}
@end
