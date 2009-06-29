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

@interface ContactsViewController : UIViewController <ABPeoplePickerNavigationControllerDelegate, ABPersonViewControllerDelegate> {
	
}

- (void)showPicker;
- (void)displayContactInfo:(ABRecordRef)person;
- (void)getNumbers:(ABRecordRef)person;
- (void)getAddr:(ABRecordRef)person;
- (void)getAddressBook;


//////////////////////////////////////// 抽象层 ////////////////////////////////////////
void osalTest();

/*取电话本的记录个数*/
//phone_cnt，返回本机电话的条数
//sim_cnt,返回sim卡电话的条数
//返回值：0表示成功，-1表示失败
int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt);
@end
