//
//  ContactsViewController.h
//  TabiNavi
//
//  Created by wang luke on 6/29/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AddressBook/AddressBook.h>
#import <AddressBookUI/AddressBookUI.h>

//#include "wk_osfnc.h"

@interface ContactsViewController : UIViewController <ABPeoplePickerNavigationControllerDelegate, ABPersonViewControllerDelegate> {
	
}

- (void)showPicker;
- (void)displayContactInfo:(ABRecordRef)person;
+ (NSMutableArray *)getNumbers:(ABRecordRef)person;
- (void)getAddr:(ABRecordRef)person;
- (void)getAddressBook;


void osalTest();

/*取电话本的记录个数*/
//phone_cnt，返回本机电话的条数
//sim_cnt,返回sim卡电话的条数
//返回值：0表示成功，-1表示失败
int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt);

/*取指定电话本记录中的所有电话号码*/
//type，1表示电话本机，2表示sim卡
//index，表示第几条，从0开始 
//numberItme，返回的电话号码放在此参数中
//返回值，0表示成功，-1表示失败
//int Winks_ReadPhonebook(unsigned int type, unsigned int index, Winks_PhoneBookItem_s* numberItem);

/* 从电话本中取号码对应的名字(UTF-8 编码) */
void Winks_GetPhonebookName(const char* friend_no, char *friend_name, unsigned long len);
@end
