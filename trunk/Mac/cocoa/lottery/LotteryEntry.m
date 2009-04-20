//
//  LotteryEntry.m
//  lottery
//
//  Created by wang luke on 4/17/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "LotteryEntry.h"


@implementation LotteryEntry



///*!
// @method     
// @abstract   为firstNumber和secondNumber设置1到100的随机数.不接受参数,也没有返回值
// @discussion 使用random函数产生1-100的随机数
// */
//- (void)prepareRandomNumbers
//{
//	firstNumber = random() % 100 + 1;
//	secondNumber = random() % 100 + 1;
//}



- (id)initWithEntryDate:(NSCalendarDate *)theDate
{
	if(![super init])
		return nil;
	
	NSAssert(theDate != nil, @"Argument must be non-nil !!");
	
	entryDate = [theDate retain];
	firstNumber = random() % 100 + 1;
	secondNumber = random() % 100 + 1;
	return self;
}

- (id)init
{
	return [self initWithEntryDate:[NSCalendarDate calendarDate]];
}


/*!
    @method     
    @abstract   必须释放自己所使用的对象并且调用父类的dealloc方法
    @discussion <#(comprehensive description)#>
*/
- (void)dealloc
{
	NSLog(@"deallocing %@", self);
	[entryDate release];
	[super dealloc];
}

/*!
    @method     
    @abstract   给entryDate设置新值
    @discussion 先释放原来的retain计数，再使新的date的计数加一并持有该date
*/
- (void)setEntryDate:(NSCalendarDate *)date
{
	if(entryDate != date){
		[entryDate release];
		entryDate = [date retain];
	}
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

- (NSString *)description
{
	NSString *result;
	result = [[NSString alloc] initWithFormat:@"%@ = %d and %d", [entryDate descriptionWithCalendarFormat:@"%b %d %Y"], firstNumber, secondNumber];
	[result autorelease];
	return result;
}

@end
