/*
 *  TapImage.m
 *  Picoffee
 *
 *  Created by luke on 8/6/10.
 *  Copyright 2010 Luke. All rights reserved.
 *
 */

#include "TapImage.h"

@implementation TapImage
@synthesize product_id, rank, name, price, briefInfo, contentInfo, avatarUrl, index;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	NSLog(@"Touches began");
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Hello!" 
													message:[NSString stringWithFormat:@"You tapped img %d!", index]
												   delegate:nil 
										  cancelButtonTitle:@"Oh yes!" 
										  otherButtonTitles:nil];
	
	[alert show];
	[alert release];
}

- (NSString *)description
{
	return [NSString stringWithFormat:@"TapImage index: %d", index];
}

- (void)dealloc
{
    [name release];
    [price release];
    [briefInfo release];
    [contentInfo release];
    [avatarUrl release];
    [super dealloc];
}

@end