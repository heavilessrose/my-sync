//
//  FlipsideView.m
//  PasswordGen
//
//  Created by amuck on 8/18/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "FlipsideView.h"

@implementation FlipsideView


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

-(IBAction) openLink {
	// open in Safari
	//[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.apple.com/"]];
	
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=287545019&mt=8"]];

}


@end
