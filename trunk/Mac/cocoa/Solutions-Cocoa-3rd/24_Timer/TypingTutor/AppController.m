//
//  AppController.m
//  TypingTutor
//
//  Created by Aaron Hillegass on 10/22/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "AppController.h"
#import "BigLetterView.h"

#define MAX_COUNT (100)
#define COUNT_STEP (5)

@implementation AppController

- (id)init
{
	[super init];
	
	letters = [[NSArray alloc] initWithObjects:@"a", @"s", @"d", @"f", @"j", @"k", @"l", @";", nil];
	
	lastIndex = 0;
	
	return self;
}

- (void)awakeFromNib
{
	[self showAnotherLetter];
}

- (void)resetCount
{
	[self willChangeValueForKey:@"count"];
	count = 0;
	[self didChangeValueForKey:@"count"];
}

- (void)incrementCount
{
	[self willChangeValueForKey:@"count"];
	count = count + COUNT_STEP;
	if (count > MAX_COUNT) {
		count = MAX_COUNT;
	}
	[self didChangeValueForKey:@"count"];
}

- (void)showAnotherLetter
{
	int x = lastIndex;
	while (x == lastIndex) {
		x = random() % [letters count];
	}
	lastIndex = x;
	[outLetterView setString:[letters objectAtIndex:lastIndex]];
	[self resetCount];
}

- (IBAction)stopGo:(id)sender
{
	if (timer == nil) {
		timer = [NSTimer scheduledTimerWithTimeInterval:0.1
												 target:self
											   selector:@selector(checkThem:)
											   userInfo:nil
												repeats:YES];
		[timer retain];
	} else {
		[timer invalidate];
		[timer release];
		timer = nil;
	}
}

- (void)checkThem:(NSTimer *)t
{
	if ([[inLetterView string] isEqual:[outLetterView string]])  {
		[self showAnotherLetter];
	}
	if (count == MAX_COUNT) {
		NSBeep();
		[self resetCount];
	} else {
		[self incrementCount];
	}
}

@end
