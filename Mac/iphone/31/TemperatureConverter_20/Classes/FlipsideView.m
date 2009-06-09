//
//  FlipsideView.m
//  TemperatureConverter
//
//  Created by apps on 10/21/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "FlipsideView.h"

@implementation FlipsideView
- (IBAction)showMeTheWay {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.AppsAmuck.com/"]]; 
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
