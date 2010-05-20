//
//  LKContacts.h
//  AdreessBookTools
//
//  Created by luke on 5/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//
#import <AddressBook/AddressBook.h>

@interface LKContacts : NSObject {
    CFErrorRef error;
    BOOL wantToSaveChanges;
    NSInteger allCount; // 所有联系人个数
    NSArray* allPeople; // 所有联系人数组
    
    @private
    // 每个线程都需要创建自己的ABAddressBookRef, 不能共用一个.
    ABAddressBookRef _addressBook;
    void *_externalChangeContext;
}

@property(nonatomic, retain) NSArray* allPeople;

// 获取联系人名字的拷贝, 用完后需调用者CFRelease释放showname
- (BOOL)getContactShownameCopy:(ABRecordRef)aPerson to:(CFStringRef *)showname;
// 得到联系人唯一的ID
- (ABRecordID)getContactUniqueID:(ABRecordRef)aPerson;
// 得到联系人所有email
- (BOOL)getContactEmails:(ABRecordRef)aPerson to:(NSMutableDictionary *)emails;
// 得到联系人所有电话号码
- (BOOL)getContactPhoneNumbers:(ABRecordRef)aPerson to:(NSMutableDictionary *)pnumbers;
// 得到联系人图片
- (UIImage *)getContactAvatar:(ABRecordRef)aPerson;
@end
