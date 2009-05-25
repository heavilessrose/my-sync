//
//  contactSampleViewController.m
//  contactSample
//
//  Created by wang luke on 5/21/09.
//  Copyright luke 2009. All rights reserved.
//

#import "contactSampleViewController.h"

@implementation contactSampleViewController

@synthesize firstName;
@synthesize lastName;
@synthesize fullName;

- (void)dealloc 
{
	[firstName release];
    [lastName release];
    [super dealloc];
    [super dealloc];
}

// 选取器
- (IBAction)showPicker:(id)sender 
{
	// 显示联系人选择
    ABPeoplePickerNavigationController *picker = [[ABPeoplePickerNavigationController alloc] init];
    picker.peoplePickerDelegate = self;
    [self presentModalViewController:picker animated:YES];
    [picker release];
}


- (void)viewDidLoad
{
	// 注册接收电话本修改的通知
	ABAddressBookRegisterExternalChangeCallback (addressBook,
													  MyAddressBookExternalChangeCallback,
													  NULL);
}

// 获得电话本修改通知后的处理函数
void *MyAddressBookExternalChangeCallback (ABAddressBookRef addressBook,
										   CFDictionaryRef info,
										   void *context)
{
	
}

#pragma mark -
#pragma mark Application Delegate

- (void)didReceiveMemoryWarning 
{
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark -
#pragma mark ABPeoplePickerNavigationController

- (void)peoplePickerNavigationControllerDidCancel:(ABPeoplePickerNavigationController *)peoplePicker 
{
    [self dismissModalViewControllerAnimated:YES];
}


- (BOOL)peoplePickerNavigationController:(ABPeoplePickerNavigationController *)peoplePicker
											shouldContinueAfterSelectingPerson:(ABRecordRef)person 
{
//    NSString* name = (NSString *)ABRecordCopyValue(person, kABPersonFirstNameProperty);
//    self.firstName.text = name;
//    [name release];
//	
//    name = (NSString *)ABRecordCopyValue(person, kABPersonLastNameProperty);
//    self.lastName.text = name;
//    [name release];
//	
//	// 得到全名
//	name = (NSString *)ABRecordCopyCompositeName(person);
//	self.fullName.text = name;
//	[name release];
//	
//    [self dismissModalViewControllerAnimated:YES];
	
	[self displayContactInfo:person];
	// 返回yes 才能跳转屏幕
    return YES;
}

- (BOOL)peoplePickerNavigationController:(ABPeoplePickerNavigationController *)peoplePicker
											shouldContinueAfterSelectingPerson:(ABRecordRef)person
											property:(ABPropertyID)property
											identifier:(ABMultiValueIdentifier)identifier
{
    return NO;
}

#pragma mark -
#pragma mark 显示联系人信息
- (void)displayContactInfo:(ABRecordRef)person
{
	ABPersonViewController *personController = [[ABPersonViewController alloc] init];
	personController.displayedPerson = person;
	//personController.addressBook = addrBook;
	personController.allowsEditing = YES;
	personController.personViewDelegate = self;
	
	//[[self navigationController] popViewControllerAnimated:YES];
	[[self navigationController] pushViewController: personController animated: YES];
	[personController release];
}

#pragma mark -
#pragma mark ABPersonViewControllerDelegate

- (BOOL)personViewController:(ABPersonViewController *)personViewController 
shouldPerformDefaultActionForPerson:(ABRecordRef)person 
					property:(ABPropertyID)property 
				  identifier:(ABMultiValueIdentifier)identifier
{

	// 注意这个返回值
	return YES;
}

//#pragma mark -
//// address book 操作
//- (void) addressBookFunction
//{
//	ABAddressBookRef addressBook;
//	CFErrorRef error = NULL;
//	BOOL wantToSaveChanges = YES;
//	// 得到电话本对象
//	addressBook = ABAddressBookCreate();
//	
//	// work with address book
//	if (ABAddressBookHasUnsavedChanges(addressBook)) {
//		if (wantToSaveChanges) {
//			ABAddressBookSave(addressBook, &error);
//		} else {
//			ABAddressBookRevert(addressBook);
//		}
//	}
//	
//	if (error != NULL) {
//		// handle error
//	}
//	
//	CFRelease(addressBook);
//}
//
//#pragma mark -
//#pragma mark single-value properties 操作
//
//// 新建联系人
//- (void) createPerson:(NSString *)firstname lastname:(NSString *)lastname
//{
//	// 姓名：single-value properties
//	char* first = [firstname UTF8String];
//	char* last = [lastname UTF8String];
//	
//	ABRecordRef aRecord = ABPersonCreate();
//	CFErrorRef  anError = NULL;
//	
//	ABRecordSetValue(aRecord, kABPersonFirstNameProperty, CFSTR(first), &anError);
//	ABRecordSetValue(aRecord, kABPersonLastNameProperty, CFSTR(last), &anError);
//	if (anError != NULL) { /* ... handle error ... */ }
//	
//	// 电话：multi-value properties
//	ABMutableMultiValueRef multi = ABMultiValueCreateMutable(kABMultiStringPropertyType);
//	CFErrorRef anError = NULL;
//	
//	bool didAdd = ABMultiValueAddValueAndLabel(multi, @"(555) 555-1234", kABPersonPhoneMobileLabel, NULL)
//	&& ABMultiValueAddValueAndLabel(multi, @"(555) 555-2345", kABPersonPhoneMainLabel, NULL);
//	if (didAdd != YES) { /* ... handle error ... */}
//	
//	ABRecordRef aRecord = ABPersonCreate();
//	ABRecordSetValue(aRecord, kABPersonPhoneProperty, multi, &anError);
//	if (anError != NULL) { /* ... handle error ... */}
//	CFRelease(multi);
//	
//	CFRelease(aRecord);
//}
//
//// 取得联系人的姓，名
//- (void) getName:(ABRecordRef)aRecord
//{	
//	CFStringRef firstName, lastName;
//	firstName = ABRecordCopyValue(aRecord, kABPersonFirstNameProperty);
//	lastName  = ABRecordCopyValue(aRecord, kABPersonLastNameProperty);
//	
//	CFRelease(aRecord);
//	CFRelease(firstName);
//	CFRelease(lastName);
//}
//
//#pragma mark -
//#pragma mark single-value properties 操作
//
//// 修改联系人电话号码
//- (void)modifyPhoneNums
//{
//	CFStringRef phoneNumber, phoneNumberLabel;
//	multi = ABRecordCopyValue(aRecord, kABPersonPhoneProperty);
//	
//	for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
//		phoneNumberLabel = ABMultiValueCopyLabelAtIndex(multi, i);
//		phoneNumber      = ABMultiValueCopyValueAtIndex(multi, i);
//		
//		/* ... do something with phoneNumberLabel and phoneNumber ... */
//		
//		CFRelease(phoneNumberLabel);
//		CFRelease(phoneNumber);
//	}
//}



@end
