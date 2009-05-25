//
//  LotteryEntry.m
//  lottery
//
//  Created by Aaron Hillegass on 9/12/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "LotteryEntry.h"


@implementation LotteryEntry

- (void)prepareRandomNumbers
{
	firstNumber = random() % 100 + 1;
	secondNumber = random() % 100 + 1;
}
- (void)setEntryDate:(NSCalendarDate *)date
{
	entryDate = date;
	[date setCalendarFormat:@"%b %d %Y"];
}
- (NSCalendarDate *)entryDate
{
	return entryDate;
}
- (int)firstNumber
{
	return firstNumber;
}
- (int)secondNumbjer
{
	return secondNumber;
}


@end
