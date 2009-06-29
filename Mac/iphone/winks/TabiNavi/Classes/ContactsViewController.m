//
//  ContactsViewController.m
//  TabiNavi
//
//  Created by wang luke on 6/29/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "ContactsViewController.h"


@implementation ContactsViewController
ABAddressBookRef _addressBook;
/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	[self showPicker];
	[self getAddressBook];
	osalTest();
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc {
	CFRelease(_addressBook);
    [super dealloc];
}

#pragma mark -
// 显示选取器
//- (IBAction)showPicker:(id)sender{
- (void)showPicker{
    ABPeoplePickerNavigationController *picker = [[ABPeoplePickerNavigationController alloc] init];
    picker.peoplePickerDelegate = self;
    [self presentModalViewController:picker animated:YES];
    [picker release];
}

- (void)peoplePickerNavigationControllerDidCancel:(ABPeoplePickerNavigationController *)peoplePicker 
{
    [self dismissModalViewControllerAnimated:YES];
}

// 得到联系人的所有电话号码
- (void)getNumbers:(ABRecordRef)person{
	CFStringRef phoneNumber, phoneNumberLabel;
	ABMutableMultiValueRef multi = ABRecordCopyValue(person, kABPersonPhoneProperty);
	
	for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
		phoneNumberLabel = ABMultiValueCopyLabelAtIndex(multi, i);
		phoneNumber = ABMultiValueCopyValueAtIndex(multi, i);
		//???: cfStringref 转换NSString
		NSLog(@"%@: %@", (NSString *)phoneNumberLabel, (NSString *)phoneNumber);
				
		CFRelease(phoneNumberLabel);
		CFRelease(phoneNumber);
	}
}

// 联系人地址
- (void)getAddr:(ABRecordRef)person{
	CFStringRef addrLabel, addr;
	ABMutableMultiValueRef multi = ABRecordCopyValue(person, kABPersonAddressProperty);
	for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
		addrLabel = ABMultiValueCopyLabelAtIndex(multi, i);
		addr = ABMultiValueCopyValueAtIndex(multi, i);
		//???: cfStringref 转换NSString
		NSLog(@"%@: %@", (NSString *)addrLabel, (NSString *)addr);
		
		CFRelease(addrLabel);
		CFRelease(addr);
	}
}

- (BOOL)peoplePickerNavigationController:(ABPeoplePickerNavigationController *)peoplePicker
	  shouldContinueAfterSelectingPerson:(ABRecordRef)person 
{
	NSString* name = (NSString *)ABRecordCopyValue(person, kABPersonFirstNameProperty);
	//self.firstName.text = name;
	NSLog(name);
	[name release];
	name = (NSString *)ABRecordCopyValue(person, kABPersonLastNameProperty);
	//self.lastName.text = name;
	NSLog(name);
	[name release];
	// 得到全名
	name = (NSString *)ABRecordCopyCompositeName(person);
	//self.fullName.text = name;
	NSLog(name);
	[name release];
	
	NSString* org = (NSString *)ABRecordCopyValue(person, kABPersonOrganizationProperty);
	if(org != nil)
		NSLog(@"org: %@", org);	
	NSString* depart = (NSString *)ABRecordCopyValue(person, kABPersonDepartmentProperty);
	if(depart != nil)
		NSLog(@"depart: %@", depart);	
	NSString* email = (NSString *)ABRecordCopyValue(person, kABPersonEmailProperty);
	if(email != nil)
		NSLog(@"email: %@", email);	
	NSString* birth = (NSString *)ABRecordCopyValue(person, kABPersonBirthdayProperty);
	if(birth != nil)
		NSLog(@"birth: %@", birth);	
	NSString* note = (NSString *)ABRecordCopyValue(person, kABPersonNoteProperty);
	if(note != nil)
		NSLog(@"note: %@", note);	
	NSString* createDate = (NSString *)ABRecordCopyValue(person, kABPersonCreationDateProperty);
	if(createDate != nil)
		NSLog(@"createDate: %@", createDate);
	NSString* modifyDate = (NSString *)ABRecordCopyValue(person, kABPersonModificationDateProperty);
	if(modifyDate != nil)
		NSLog(@"modifyDate: %@", modifyDate);
	

	[self getNumbers:person];
	[self getAddr:person];
	// 退出联系人列表
	//[self dismissModalViewControllerAnimated:YES];
////////
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

- (void)displayContactInfo:(ABRecordRef)person{
	ABPersonViewController *personController = [[ABPersonViewController alloc] init];
	personController.displayedPerson = person;
	//personController.addressBook = addrBook;
	personController.allowsEditing = YES;
	personController.personViewDelegate = self;
	
	//[[self navigationController] popViewControllerAnimated:YES];
	[[self navigationController] pushViewController: personController animated: YES];
	[personController release];
}

- (BOOL)personViewController:(ABPersonViewController *)personViewController 
shouldPerformDefaultActionForPerson:(ABRecordRef)person 
					property:(ABPropertyID)property 
				  identifier:(ABMultiValueIdentifier)identifier
{
	
	// 注意这个返回值
	return YES;
}

#pragma mark -
- (void)getAddressBook{
	if(_addressBook == nil)
		//???: 得到电话本对象.什么时候释放？
		_addressBook = ABAddressBookCreate();
//	CFErrorRef error = NULL;
//	BOOL wantToSaveChanges = YES;
//	
//	// work with address book
//	if (ABAddressBookHasUnsavedChanges(_addressBook)) {
//		if (wantToSaveChanges) {
//			ABAddressBookSave(_addressBook, &error);
//		} else {
//			ABAddressBookRevert(_addressBook);
//		}
//	}
//	
//	if (error != NULL) {
//		// handle error
//	}
//	
//	//CFRelease(_addressBook);
}
#pragma mark -
#pragma mark 抽象层
void osalTest(){
	unsigned long *phone_cnt = malloc(sizeof(unsigned long));
	unsigned long *sim_cnt = malloc(sizeof(unsigned long));

	Winks_GetPhonebookCount(phone_cnt, sim_cnt);
	printf("phone_cnt: %u\n", *phone_cnt);
}

int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt){
	CFIndex count = ABAddressBookGetPersonCount(_addressBook);
	*phone_cnt = (unsigned long)count;
	return 0;
}
@end
