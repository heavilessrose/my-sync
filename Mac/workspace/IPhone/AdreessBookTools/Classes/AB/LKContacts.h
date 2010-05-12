//
//  LKContacts.h
//  AdreessBookTools
//
//  Created by luke on 5/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//
#import <AddressBook/AddressBook.h>
/*
typedef enum PersonPropLabel_tag {
    
} PersonPropLabel
*/
@interface LKContacts : NSObject {
    CFErrorRef error;
    BOOL wantToSaveChanges;
    
    @private
    // 每个线程都需要创建自己的ABAddressBookRef, 不能共用一个.
    ABAddressBookRef _addressBook;
    void *_externalChangeContext;
}


- (int)getSingleProp:(ABPropertyID)aProp from:(ABRecordRef)aRecord outValue:(CFStringRef)aValue;
- (int)setSingleProp:(ABPropertyID)aProp value:(NSString *)aValue to:(ABRecordRef)aRecord;

@end
