//
//  AddressBookFillViewController.m
//  AddressBookFill
//
//  Created by Doug Diego on 4/10/09.
//  Copyright Doug Diego 2009. All rights reserved.
//

#import "AddressBookFillViewController.h"
#import <AddressBook/AddressBook.h>

@implementation AddressBookFillViewController

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

- (void) buttonPressed:(id)sender {
	NSLog(@"button pressed");	
	[self populateAddressBookWithPresidents];
	
	
	UIAlertView* dialog = [[[UIAlertView alloc] init] retain]; 
	[dialog setDelegate:self]; 
	[dialog setTitle:@"Done"]; 
	[dialog addButtonWithTitle:@"OK"]; 
	[dialog show]; 
	[dialog release]; 
	
}


- (void) populateAddressBookWithPresidents {
	NSLog(@"PopulateAddressBook.populateAddressBookWithPresidents");
	
	//NSDate *date1 = [NSDate dateWithTimeIntervalSince1970:-7506043200];
	NSDate *date1 = [NSDate dateWithString:@"1732-02-22 00:00:00 -0800"];
	[self addPersonWithFirstName:@"George" lastName:@"Washington" phone:@"212-212-2112" birthDate:date1
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/b/b6/Gilbert_Stuart_Williamstown_Portrait_of_George_Washington.jpg/225px-Gilbert_Stuart_Williamstown_Portrait_of_George_Washington.jpg"];
	
	
	NSDate *date2 = [NSDate dateWithString:@"1732-10-30 00:00:00 -0800"];
	[self addPersonWithFirstName:@"John" lastName:@"Adams" phone:@"222-222-2222" birthDate:date2
						   image:@"http://upload.wikimedia.org/wikipedia/commons/9/9e/Johnadamsvp.flipped.jpg"];
	
	NSDate *date3 = [NSDate dateWithString:@"1743-04-13 00:00:00 -0800"];
	[self addPersonWithFirstName:@"Thomas" lastName:@"Jefferson" phone:@"333-333-3333" birthDate:date3
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/4/46/T_Jefferson_by_Charles_Willson_Peale_1791_2.jpg/225px-T_Jefferson_by_Charles_Willson_Peale_1791_2.jpg"];
	
	NSDate *date4 = [NSDate dateWithString:@"1751-03-16 00:00:00 -0800"];
	[self addPersonWithFirstName:@"James" lastName:@"Madison" phone:@"444-444-4444" birthDate:date4
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/1/1d/James_Madison.jpg/225px-James_Madison.jpg"];
	
	NSDate *date5 = [NSDate dateWithString:@"1758-04-28 00:00:00 -0800"];
	[self addPersonWithFirstName:@"James" lastName:@"Monroe" phone:@"555-555-5555" birthDate:date5
						   image:@"http://upload.wikimedia.org/wikipedia/commons/f/f2/Jm5.gif"];
	
	
	NSDate *date16 = [NSDate dateWithString:@"1865-02-12 00:00:00 -0800"];
	[self addPersonWithFirstName:@"Abraham" lastName:@"Lincoln" phone:@"333-333-3333" birthDate:date16
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/4/44/Abraham_Lincoln_head_on_shoulders_photo_portrait.jpg/225px-Abraham_Lincoln_head_on_shoulders_photo_portrait.jpg"];

	NSDate *date40 = [NSDate dateWithString:@"1911-02-06 00:00:00 -0800"];
	[self addPersonWithFirstName:@"Ronald" lastName:@"Reagan" phone:@"044-244-0344" birthDate:date40
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/1/16/Official_Portrait_of_President_Reagan_1981.jpg/219px-Official_Portrait_of_President_Reagan_1981.jpg"];
	
	NSDate *date41 = [NSDate dateWithString:@"1924-06-12 00:00:00 -0800"];
	[self addPersonWithFirstName:@"George H. W." lastName:@"Bush" phone:@"345-234-0444" birthDate:date41
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/0/0f/George_H._W._Bush%2C_President_of_the_United_States%2C_1989_official_portrait.jpg/225px-George_H._W._Bush%2C_President_of_the_United_States%2C_1989_official_portrait.jpg"];
	
	NSDate *date42 = [NSDate dateWithString:@"1946-08-19 00:00:00 -0800"];
	[self addPersonWithFirstName:@"Bill" lastName:@"Clinton" phone:@"144-344-0674" birthDate:date42
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/d/d3/Bill_Clinton.jpg/225px-Bill_Clinton.jpg"];
	
	NSDate *date43 = [NSDate dateWithString:@"1946-07-06 00:00:00 -0800"];
	[self addPersonWithFirstName:@"George W." lastName:@"Bush" phone:@"454-054-0033" birthDate:date43
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/d/d4/George-W-Bush.jpeg/225px-George-W-Bush.jpeg"];
		
	NSDate *date44 = [NSDate dateWithString:@"1961-08-04 00:00:00 -0800"];
	[self addPersonWithFirstName:@"Barack" lastName:@"Obama" phone:@"444-144-0554" birthDate:date44
						   image:@"http://upload.wikimedia.org/wikipedia/commons/thumb/e/e9/Official_portrait_of_Barack_Obama.jpg/225px-Official_portrait_of_Barack_Obama.jpg"];

	 
}

-(void) addPersonWithFirstName:(NSString *) firstName lastName: (NSString *) lastName phone: (NSString *) phone birthDate: (NSDate *) birthDate image: (NSString *) image  {
	
	NSLog(@"PopulateAddressBook.addPersonWithFirstName");
	
	CFErrorRef error = NULL; 
	ABAddressBookRef addressBook = ABAddressBookCreate();
	ABRecordRef person = ABPersonCreate();
	
	ABRecordSetValue(person, kABPersonFirstNameProperty, firstName , nil);
	ABRecordSetValue(person, kABPersonLastNameProperty, lastName, nil);
	
	ABMutableMultiValueRef multiPhone = ABMultiValueCreateMutable(kABMultiStringPropertyType);
	ABMultiValueAddValueAndLabel(multiPhone, phone, kABPersonPhoneMainLabel, NULL);
	//ABMultiValueAddValueAndLabel(multiPhone, @"1-123-456-7890", kABPersonPhoneMobileLabel, NULL);            
	//ABMultiValueAddValueAndLabel(multiPhone, @"1-987-654-3210", kABOtherLabel, NULL);        
	ABRecordSetValue(person, kABPersonPhoneProperty, multiPhone,nil);
	CFRelease(multiPhone);
	
	// Add Image
	if(image != nil){
		UIImage *myImage = [[UIImage alloc] initWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:image]]];
		ABPersonSetImageData(person, (CFDataRef) (UIImageJPEGRepresentation(myImage, 1.0f)), &error);
	}
	
	// Add Birthday
	if(birthDate != nil){
		ABRecordSetValue(person, kABPersonBirthdayProperty, birthDate , nil);
	}
	
	ABAddressBookAddRecord(addressBook, person, &error);
	ABAddressBookSave(addressBook, &error);
	
	CFRelease(person);
	
	if (error != NULL) {
		NSLog(@"Danger Will Robinson! Danger!");		
	}
}

 

@end
