//
//  wk_contacts.m
//  TabiNavi
//
//  Created by wang luke on 7/23/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "wk_contacts.h"


@implementation wk_contacts

ABAddressBookRef _addressBook;
ABRecordID* people_ID_list;

#pragma mark -
#pragma mark 抽象层
void osalTest(){
	unsigned long *phone_cnt = malloc(sizeof(unsigned long));
	unsigned long *sim_cnt = malloc(sizeof(unsigned long));
	
	Winks_GetPhonebookCount(phone_cnt, sim_cnt);
	printf("phone_cnt: %u\n", *phone_cnt);
	
	const char friend_no[256] = "555-522-8243";
	char friend_name[256];
	Winks_GetPhonebookName(friend_no, friend_name, 256);
	//printf("friend_name: %s", friend_name);
}

int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt){
	CFIndex count = ABAddressBookGetPersonCount(_addressBook);
	*phone_cnt = (unsigned long)count;
	
	people_ID_list = calloc((size_t) count, sizeof(ABRecordID));
	setIdList();
	return 0;
}

// 设置id列表
void setIdList(){
	if(_addressBook == nil)
		_addressBook = ABAddressBookCreate();
	NSArray* allPeople = (NSArray*)ABAddressBookCopyArrayOfAllPeople(_addressBook);
	
	for(NSUInteger i = 0; i < allPeople.count; i++){
		ABRecordRef person = [allPeople objectAtIndex:i];
		*(people_ID_list + i) = ABRecordGetRecordID(person);
	}
}

// ABRecordID 与 index转换
//unsigned int getIndexFromID(ABRecordID pid){
//	int i;
//	unsigned long len = 0;
//	Winks_GetPhonebookCount(&len, NULL);
//	for(i = 0; i < len; i++){
//		if(pid == people_ID_list[i])
//			break;
//	}
//	return i;
//} 


/*取电话本中指定的一条记录（一条记录可能含有多个电话号码）*/
//type，1表示电话本机，2表示sim卡
//index，表示第几条，从0开始 
//numberItme，返回的电话号码放在此参数中
//返回值，0表示成功，-1表示失败
int Winks_ReadPhonebook(unsigned int type, unsigned int index, Winks_PhoneBookItem_s* numberItem){
	if (!numberItem)
		return -1;
	
	if (type == 2)
		return -1;
	
	ABRecordID recordid = *(people_ID_list + index);
	ABRecordRef thePeople = ABAddressBookGetPersonWithRecordID (_addressBook, recordid);
	NSMutableArray * numArr = [ContactsViewController getNumbers:thePeople];
	int count = [numArr count];
	int i;
	for (i = 0; i < count; i++) {
		numberItem->number[i] = [(NSString *)[numArr objectAtIndex:(NSUInteger)i] UTF8String];
	}
	return 0;
}

void Winks_GetPhonebookName(const char* friend_no, char *friend_name, unsigned long len){
	if (!friend_no || !friend_name || len <= 0 || !*friend_no){
		return;
	}
	int iLen = 0;
	iLen = strlen(friend_no);
	NSString *ns_friend_no = [[NSString alloc] initWithCString:friend_no];
	
	if(_addressBook == nil)
		_addressBook = ABAddressBookCreate();
	NSArray* allPeople = (NSArray*)ABAddressBookCopyArrayOfAllPeople(_addressBook);
	//CFArrayRef people = ABAddressBookCopyPeopleWithName (ABAddressBookRef addressBook, CFStringRef name);
	
	NSArray* numbers = nil;
	for(NSUInteger i = 0; i < allPeople.count; i++){
		ABRecordRef person = [allPeople objectAtIndex:i];
		numbers = [ContactsViewController getNumbers:person];
		if([numbers containsObject:ns_friend_no]){
			NSString* name = (NSString *)ABRecordCopyCompositeName(person);
			NSLog(@"friend_name: %@", name);
			memcpy(friend_name, [name UTF8String], sizeof([name UTF8String]));
			break;
		}
	}
	[ns_friend_no release];
	//CFRelease(_addressBook);
}

#pragma mark -
#pragma mark 内部函数
- (void)getAddressBook{
	if(_addressBook == nil)
		//???: 得到电话本对象.什么时候释放？
		_addressBook = ABAddressBookCreate();
	//	//CFRelease(_addressBook);
}

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
@end
