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

- (IBAction)showPicker:(id)sender 
{
    ABPeoplePickerNavigationController *picker = [[ABPeoplePickerNavigationController alloc] init];
    picker.peoplePickerDelegate = self;
	
    [self presentModalViewController:picker animated:YES];
    [picker release];
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
#pragma mark navigation controller Delegate

- (void)peoplePickerNavigationControllerDidCancel:(ABPeoplePickerNavigationController *)peoplePicker 
{
    [self dismissModalViewControllerAnimated:YES];
}


- (BOOL)peoplePickerNavigationController:(ABPeoplePickerNavigationController *)peoplePicker
											shouldContinueAfterSelectingPerson:(ABRecordRef)person 
{
    NSString* name = (NSString *)ABRecordCopyValue(person, kABPersonFirstNameProperty);
    self.firstName.text = name;
    [name release];
	
    name = (NSString *)ABRecordCopyValue(person, kABPersonLastNameProperty);
    self.lastName.text = name;
    [name release];
	
	
	name = (NSString *)ABRecordCopyCompositeName(person);
	self.fullName.text = name;
	[name release];
	
    [self dismissModalViewControllerAnimated:YES];
	
    return NO;
}

- (BOOL)peoplePickerNavigationController:(ABPeoplePickerNavigationController *)peoplePicker
											shouldContinueAfterSelectingPerson:(ABRecordRef)person
											property:(ABPropertyID)property
											identifier:(ABMultiValueIdentifier)identifier
{
    return NO;
}


- (void) addressBookFunction
{
	ABAddressBookRef addressBook;
	CFErrorRef error = NULL;
	BOOL wantToSaveChanges = YES;
	
	addressBook = ABAddressBookCreate();
	
	// work with address book
	if (ABAddressBookHasUnsavedChanges(addressBook)) {
		if (wantToSaveChanges) {
			ABAddressBookSave(addressBook, &error);
		} else {
			ABAddressBookRevert(addressBook);
		}
	}
	
	if (error != NULL) {
		// handle error
	}
	
	CFRelease(addressBook);
}
@end
