//
//  ContactsViewController.m
//  TabiNavi
//
//  Created by wang luke on 6/29/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "ContactsViewController.h"
//#import "Utils.h"


@implementation ContactsViewController
ABAddressBookRef _addressBook;
ABRecordID* people_ID_list;
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
	

	[ContactsViewController getNumbers:person];
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
@end
