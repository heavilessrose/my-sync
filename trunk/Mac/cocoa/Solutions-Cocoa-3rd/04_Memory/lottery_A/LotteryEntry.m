//
//  LotteryEntry.m
//  lottery
//
//  Created by Aaron Hillegass on 9/12/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "LotteryEntry.h"


@implementation LotteryEntry

- (id)initWithEntryDate:(NSCalendarDate *)theDate
{
	if (![super init])
		return nil;
	
	entryDate = [theDate retain];
	firstNumber = random() % 100 + 1;
	secondNumber = random() % 100 + 1;
	return self;		
}

- (id)init
{
	return [self initWithEntryDate:[NSCalendarDate calendarDate]];
}
- (void)dealloc
{
	NSLog(@"deallocing %@", self);
	[entryDate release];
	[super dealloc];
}

- (NSString *)description
{
	NSString *result;
	result = [[NSString alloc] initWithFormat:@"%@ = %d and %d",
			  [entryDate descriptionWithCalendarFormat:@"%b %d %Y"], firstNumber, secondNumber];
	[result autorelease];
	return result;
}

- (void)setEntryDate:(NSCalendarDate *)date
{
    [date retain];
	[entryDate release];
	entryDate = date;
}
- (NSCalendarDate *)entryDate
{
	return entryDate;
}
- (int)firstNumber
{
	return firstNumber;
}
- (int)secondNumber
{
	return secondNumber;
}


@end
