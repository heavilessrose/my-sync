//
//  LKContacts.m
//  AdreessBookTools
//
//  Created by luke on 5/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "LKContacts.h"

@interface LKContacts (private)
- (int)getSingleProp:(ABPropertyID)aProp from:(ABRecordRef)aRecord outValue:(CFStringRef *)aValue;
- (int)setSingleProp:(ABPropertyID)aProp value:(NSString *)aValue to:(ABRecordRef)aRecord;
- (void)getMultiProp:(ABPropertyID)aProp from:(ABRecordRef)aRecord outValue:(NSMutableDictionary *)values;
- (ABMultiValueIdentifier)setMultiProp:(ABPropertyType)aType label:(CFStringRef)aLabel value:(CFStringRef)aValue to:(ABRecordRef)aRecord;
@end

@implementation LKContacts
@synthesize allPeople;

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
    if(allPeople)
    {
        NSLog(@"allPeople count = %d", [allPeople retainCount]);
        [allPeople release];
    }
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
- (int)getSingleProp:(ABPropertyID)aProp from:(ABRecordRef)aRecord outValue:(CFStringRef *)aValue
{
    //CFStringRef aValue;
    *aValue = (CFStringRef)ABRecordCopyValue(aRecord, aProp);
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
- (void)getMultiProp:(ABPropertyID)aProp from:(ABRecordRef)aRecord outValue:(NSMutableDictionary *)values
{
    // 读取record multi property字段
    CFStringRef propValue = NULL, propLabel = NULL;
    ABMutableMultiValueRef multi = ABRecordCopyValue(aRecord, aProp);
    
    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++)
    {
        propLabel = ABMultiValueCopyLabelAtIndex(multi, i);
        propValue = ABMultiValueCopyValueAtIndex(multi, i);
        
        [values setObject:[NSString stringWithFormat:@"%@", propValue] forKey:[NSString stringWithFormat:@"%@", propLabel]];
        CFRelease(propLabel);
        CFRelease(propValue);
    }
    
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

- (BOOL)getContactShownameCopy:(ABRecordRef)aPerson to:(CFStringRef *)showname
{
    *showname = ABRecordCopyCompositeName(aPerson);
    NSLog(@"showname = %@", *showname);
    
    if(showname){
        return YES;
    } 
    else 
    {
        // 获取email或电话号码作为名字(iphone联系人app的规则)
        
        return YES;
    }

    return NO;
}

- (ABRecordID)getContactUniqueID:(ABRecordRef)aPerson
{
    return ABRecordGetRecordID(aPerson);
}

- (BOOL)getContactEmails:(ABRecordRef)aPerson to:(NSMutableDictionary *)emails
{
    if(emails)
    {
        [self getMultiProp:kABPersonEmailProperty from:aPerson outValue:emails];
        return YES;
    }
    NSLog(@"emails should be alloced");
    return NO;
}

- (BOOL)getContactPhoneNumbers:(ABRecordRef)aPerson to:(NSMutableDictionary *)pnumbers
{
    if(pnumbers)
    {
        [self getMultiProp:kABPersonPhoneProperty from:aPerson outValue:pnumbers];
        
        for (id key in pnumbers) {
            NSLog(@"key: %@, value: %@", key, [pnumbers objectForKey:key]);
        }
        return YES;
    }
    NSLog(@"pnumbers should be alloced");
    return NO;
}

// for test
- (BOOL)getAllContacts
{
    CFIndex count = ABAddressBookGetPersonCount(_addressBook);
	allPeople = (NSArray*)ABAddressBookCopyArrayOfAllPeople(_addressBook);
    
    if(count != allPeople.count)
    {
        NSLog(@"allPeople count not unique");
    }
    
	NSUInteger i;
	for(i = 0; i < allPeople.count; i++){
		ABRecordRef person = [allPeople objectAtIndex:i];
		ABRecordID recordID = ABRecordGetRecordID(person);
		// 依次处理每个联系人
        /*CFStringRef firstname = NULL;
        CFStringRef lastname = NULL;
        CFStringRef midname = NULL;
        CFStringRef prename = NULL;
        CFStringRef suffixname = NULL;
        CFStringRef nickname = NULL;
        CFStringRef firstnamePhonetic = NULL;
        CFStringRef lastnamePhonetic = NULL;
        CFStringRef midnamePhonetic = NULL;*/
        
        CFStringRef org = NULL;
        NSMutableDictionary *emails = [[NSMutableDictionary alloc] init];
        NSMutableDictionary *pnumbers = [[NSMutableDictionary alloc] init];
        NSMutableDictionary *addrs = [[NSMutableDictionary alloc] init];
        //NSMutableData *birthDate = [[NSMutableData alloc] init];
        CFStringRef birthDate = NULL;
        /*
        [self getSingleProp:kABPersonFirstNameProperty from:person outValue:&firstname];
        [self getSingleProp:kABPersonLastNameProperty from:person outValue:&lastname];
        [self getSingleProp:kABPersonMiddleNameProperty from:person outValue:&midname];
        [self getSingleProp:kABPersonPrefixProperty from:person outValue:&prename];
        [self getSingleProp:kABPersonSuffixProperty from:person outValue:&suffixname];
        [self getSingleProp:kABPersonNicknameProperty from:person outValue:&nickname];
        [self getSingleProp:kABPersonFirstNamePhoneticProperty from:person outValue:&firstnamePhonetic];
        [self getSingleProp:kABPersonLastNamePhoneticProperty from:person outValue:&lastnamePhonetic];
        [self getSingleProp:kABPersonMiddleNamePhoneticProperty from:person outValue:&midnamePhonetic];
        
        [self getSingleProp:kABPersonOrganizationProperty from:person outValue:&org];
        [self getSingleProp:kABPersonBirthdayProperty from:person outValue:&birthDate];
		NSLog(@"\r\n recordID = %d\r\n firstName = %@\r\n lastname = %@\r\n \
              midname = %@\r\n \
              prename = %@\r\n \
              suffixname = %@\r\n \
              nickname = %@\r\n \
              firstnamePhonetic = %@\r\n \
              lastnamePhonetic = %@\r\n \
              midnamePhonetic = %@\r\n \
              org = %@\r\n birth = %@", recordID, firstname, lastname, midname, prename, suffixname, nickname, 
              firstnamePhonetic, lastnamePhonetic, midnamePhonetic, org, birthDate);
        */
        NSString *showname = (NSString *)ABRecordCopyCompositeName(person);
        NSLog(@"showname = %@", showname);
        
        [self getMultiProp:kABPersonEmailProperty from:person outValue:emails];
        
        for (id key in emails) {
            NSLog(@"key: %@, value: %@", key, [emails objectForKey:key]);
        }
        
        [self getMultiProp:kABPersonPhoneProperty from:person outValue:pnumbers];
        
        for (id key in pnumbers) {
            NSLog(@"key: %@, value: %@", key, [pnumbers objectForKey:key]);
        }
        
        [self getMultiProp:kABPersonAddressProperty from:person outValue:addrs];
        
        for (id key in addrs) {
            NSLog(@"key: %@, value: %@", key, [addrs objectForKey:key]);
        }
//        [self getSingleProp:kABPersonBirthdayProperty from:person outValue:&lastname];
//        [self getSingleProp:kABPersonLastNameProperty from:person outValue:&lastname];
//        [self getSingleProp:kABPersonLastNameProperty from:person outValue:&lastname];
        //CFRelease(firstname);
	}
    return YES;
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
