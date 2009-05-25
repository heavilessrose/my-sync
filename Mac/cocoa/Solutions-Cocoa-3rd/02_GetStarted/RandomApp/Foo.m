//
//  Foo.m
//  RandomApp
//
//  Created by Aaron Hillegass on 9/11/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "Foo.h"

@implementation Foo

- (IBAction)generate:(id)sender
{
	// Generate a number between 1 and 100 inclusive
	int generated;
	generated = (random() % 100) + 1;
	
	// Ask the text field to change what it is displaying
	[textField setIntValue:generated];
}

- (IBAction)seed:(id)sender
{
	// Seed the random number generator with the time
	srandom(time(NULL));
	[textField setStringValue:@"Generator seeded"];
}

- (void)awakeFromNib
{
	NSCalendarDate *now;
	now = [NSCalendarDate calendarDate];
	[textField setObjectValue:now];
}

@end
