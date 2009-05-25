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

- (NSString *)description
{
	NSString *result;
	result = [[NSString alloc] initWithFormat:@"%@ = %d and %d",
			  entryDate, firstNumber, secondNumber];
	return result;
}

- (void)setEntryDate:(NSCalendarDate *)date
{
    NSAssert(date == nil, @"argument is nil");

	entryDate = date;
	[entryDate setCalendarFormat:@"%b %d %Y"];
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
