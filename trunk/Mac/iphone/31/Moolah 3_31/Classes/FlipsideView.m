//
//  FlipsideView.m
//  DrawTest
//
//  Created by AppsAmuck on 8/26/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "FlipsideView.h"

@implementation FlipsideView

- (IBAction)openLink {
	// open in Safari
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=287545019&mt=8"]];
}

- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)drawRect:(CGRect)rect {
	// Drawing code
}


- (void)dealloc {
	[super dealloc];
}


@end
