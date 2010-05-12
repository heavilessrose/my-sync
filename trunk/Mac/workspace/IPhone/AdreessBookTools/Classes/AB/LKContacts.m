//
//  LKContacts.m
//  AdreessBookTools
//
//  Created by luke on 5/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "LKContacts.h"

@interface LKContacts (private)
//- (int)getSingleProp:(ABPropertyID)aProp from:(ABRecordRef)aRecord outValue:(CFStringRef)aValue;
//- (int)setSingleProp:(ABPropertyID)aProp value:(NSString *)aValue to:(ABRecordRef)aRecord;
@end

@implementation LKContacts

- (id)init
{
    self = [super init];
    if(self)
    {
        _addressBook = ABAddressBookCreate();
    } else {
        NSLog(@"LKContacts init err");
        return nil;
    }
    return self;
}

- (void)dealloc
{
    NSLog(@"LKContacts dealloc =====");
    if(_addressBook != NULL)
    {
        CFRelease(_addressBook);
        _addressBook = NULL;
    }
	[super dealloc];
    NSLog(@"LKContacts dealloc over =====");
}

#pragma mark ---private---

// kABPersonFirstNameProperty
- (int)getSingleProp:(ABPropertyID)aProp from:(ABRecordRef)aRecord outValue:(CFStringRef)aValue
{
    //CFStringRef aValue;
    aValue = ABRecordCopyValue(aRecord, aProp);
    //TODO: how to return value?
    //CFRelease(aValue);
    return 0;
}

// kABPersonFirstNameProperty
- (int)setSingleProp:(ABPropertyID)aProp value:(NSString *)aValue to:(ABRecordRef)aRecord
{
    //ABRecordRef aRecord = ABPersonCreate();
    CFErrorRef anError = NULL;
    
    //ABRecordSetValue(aRecord, aProp, CFSTR("Katie"), &anError);
    ABRecordSetValue(aRecord, aProp, aValue, &anError);
    if (anError != NULL)
    {
        CFStringRef errStr = CFErrorCopyDescription(anError);
        NSLog(@"setSingleProp err: %@", errStr);
        return -1;
    }
    return 0;
}

// kABPersonPhoneProperty
- (void)getMultiProp:(ABPropertyID)aProp from:(ABRecordRef)aRecord
{
    // 读取record multi property字段
    CFStringRef propValue, propLabel;
    ABMutableMultiValueRef multi = ABRecordCopyValue(aRecord, aProp);
    
    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++)
    {
        propLabel = ABMultiValueCopyLabelAtIndex(multi, i);
        propValue = ABMultiValueCopyValueAtIndex(multi, i);
        
        NSLog(@"propLabel: %@\r\npropValue: %@\r\n", propLabel, propValue);
        //FIXME: release?
        CFRelease(propLabel);
        CFRelease(propValue);
    }
    
    //FIXME: release?
    CFRelease(aRecord);
    CFRelease(multi);
}

// kABMultiStringPropertyType kABPersonPhoneMobileLabel @"(555) 555-1234"
- (ABMultiValueIdentifier)setMultiProp:(ABPropertyType)aType label:(CFStringRef)aLabel value:(CFStringRef)aValue to:(ABRecordRef)aRecord
{
    ABMutableMultiValueRef multi = ABMultiValueCreateMutable(aType);
    CFErrorRef anError = NULL;
    
    // 联系人增加multi property字段
    ABMultiValueIdentifier *multivalueID;
    bool didAdd = ABMultiValueAddValueAndLabel(multi, aValue, aLabel, multivalueID);
    if (didAdd != YES)
    {
        NSLog(@"set multi value err");
        return -1;
    }
    
    //ABRecordRef aRecord = ABPersonCreate();
    ABRecordSetValue(aRecord, kABPersonPhoneProperty, multi, &anError);
    if (anError != NULL)
    {
        NSLog(@"set multi value to record err");
        return -1;
    }
    CFRelease(multi);
    
    return *multivalueID;
}

#pragma mark ---Founctions---
- (BOOL)submitAddrBookChanges
{
    if (ABAddressBookHasUnsavedChanges(_addressBook))
    {
        if (wantToSaveChanges)
        {
            ABAddressBookSave(_addressBook, &error);
        } 
        else
        {
            ABAddressBookRevert(_addressBook);
        }
    }
    
    if (error != NULL) 
    {
        CFStringRef errStr = CFErrorCopyDescription(error);
        NSLog(@"submit err: %@", errStr);
        return NO;
    }
    
    CFRelease(_addressBook);
    _addressBook = NULL;
    return YES;
}

- (void)getStreetAddr
{

}

- (void)setStreetAddr:(CFStringRef *)keys values:(CFStringRef *)values valueCount:(CFIndex)count to:(ABRecordRef)aPerson
{
    ABMutableMultiValueRef address = ABMultiValueCreateMutable(kABDictionaryPropertyType);
    /*
    // Set up keys and values for the dictionary.
    CFStringRef keys[5];
    CFStringRef values[5];
    keys[0] = kABPersonAddressStreetKey;
    keys[1] = kABPersonAddressCityKey;
    keys[2] = kABPersonAddressStateKey;
    keys[3] = kABPersonAddressZIPKey;
    keys[4] = kABPersonAddressCountryKey;
    values[0] = CFSTR("1234 Laurel Street");
    values[1] = CFSTR("Atlanta");
    values[2] = CFSTR("GA");
    values[3] = CFSTR("30303");
    values[4] = CFSTR("USA");
    */
    CFDictionaryRef aDict = CFDictionaryCreate(
                                               kCFAllocatorDefault,
                                               (void *)keys,
                                               (void *)values,
                                               count,
                                               &kCFCopyStringDictionaryKeyCallBacks,
                                               &kCFTypeDictionaryValueCallBacks);
    
    // Add the street address to the person record.
    ABMultiValueIdentifier identifier;
    ABMultiValueAddValueAndLabel(address, aDict, kABHomeLabel, &identifier);
    CFRelease(aDict);
    
    CFErrorRef anError = NULL;
    ABRecordSetValue(aPerson, kABPersonAddressProperty, address, &anError);
    if (anError != NULL)
    {
        NSLog(@"set street addr to record err");
        //return -1;
    }
    CFRelease(address);
}

- (void)getAllContacts
{
    
}

#pragma mark ---external changes notify---
void addrBookExternalChangeCallback(ABAddressBookRef addressBook, CFDictionaryRef info, void *context)
{
    printf("addrbook external changed\r\n");
    /*
    if (ABAddressBookHasUnsavedChanges(_addressBook) && wantToSaveChanges)
    {
        ABAddressBookSave(_addressBook, &error);
    }
    else
    {
        ABAddressBookRevert(_addressBook);
    }
    
    if (error != NULL) 
    {
        CFStringRef errStr = CFErrorCopyDescription(error);
        NSLog(@"update external changes or merge unsavedChanges err: %@", errStr);
    }
     */
}

- (void)registExternalChange
{
    ABAddressBookRegisterExternalChangeCallback(_addressBook, addrBookExternalChangeCallback, _externalChangeContext);
}

- (void)unregistExternalChange
{
    ABAddressBookUnregisterExternalChangeCallback (_addressBook, addrBookExternalChangeCallback, _externalChangeContext);
}


@end
