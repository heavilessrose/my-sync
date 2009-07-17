/*
 *  wk_contact.c
 *  TabiNavi
 *
 *  Created by wang luke on 7/16/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "wk_contact.h"

ABAddressBookRef _addressBook;
ABRecordID* people_ID_list;

// 得到联系人的所有电话号码
+ (NSMutableArray *)getNumbers:(ABRecordRef)person{
	//FIXME: numbers 如何释放？
	NSMutableArray * numbers = [[NSMutableArray alloc] init];
	CFStringRef phoneNumber, phoneNumberLabel;
	ABMutableMultiValueRef multi = ABRecordCopyValue(person, kABPersonPhoneProperty);
	
	for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
		phoneNumberLabel = ABMultiValueCopyLabelAtIndex(multi, i);
		phoneNumber = ABMultiValueCopyValueAtIndex(multi, i);
		//???: cfStringref 转换NSString
		NSLog(@"%@: %@", (NSString *)phoneNumberLabel, (NSString *)phoneNumber);
		
		[numbers addObject:(NSString *)phoneNumber];
		
		CFRelease(phoneNumberLabel);
		CFRelease(phoneNumber);
	}
	return numbers;
}