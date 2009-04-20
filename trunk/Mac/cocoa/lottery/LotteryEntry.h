//
//  LotteryEntry.h
//  lottery
//
//  Created by wang luke on 4/17/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>


@interface LotteryEntry : NSObject {
	NSCalendarDate *entryDate;
	int firstNumber;
	int secondNumber;
}

//- (id)init;
- (id)initWithEntryDate:(NSCalendarDate *)theDate;	
//- (void)prepareRandomNumbers;
- (void)setEntryDate:(NSCalendarDate *)date;
- (NSCalendarDate *)entryDate;
- (int)firstNumber;
- (int)secondNumber;

@end
