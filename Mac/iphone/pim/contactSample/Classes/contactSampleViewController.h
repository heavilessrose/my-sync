//
//  contactSampleViewController.h
//  contactSample
//
//  Created by wang luke on 5/21/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AddressBook/AddressBook.h>
#import <AddressBookUI/AddressBookUI.h>

@interface contactSampleViewController : UIViewController <ABPeoplePickerNavigationControllerDelegate> {
    IBOutlet UILabel *firstName;
    IBOutlet UILabel *lastName;
    IBOutlet UILabel *fullName;
}
@property (nonatomic, retain) UILabel *firstName;
@property (nonatomic, retain) UILabel *lastName;
@property (nonatomic, retain) UILabel *fullName;

- (IBAction)showPicker:(id)sender;
@end

