/*
 *  TapImage.m
 *  Picoffee
 *
 *  Created by luke on 8/6/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TapImage.h"

@implementation TapImage

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	NSLog(@"Touches began");
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Hello!" 
													message:@"You tapped me!" 
												   delegate:nil 
										  cancelButtonTitle:@"Oh yes!" 
										  otherButtonTitles:nil];
	
	[alert show];
	[alert release];
}

@end