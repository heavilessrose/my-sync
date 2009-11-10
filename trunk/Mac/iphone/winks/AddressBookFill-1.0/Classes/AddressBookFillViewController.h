//
//  AddressBookFillViewController.h
//  AddressBookFill
//
//  Created by Doug Diego on 4/10/09.
//  Copyright Doug Diego 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface AddressBookFillViewController : UIViewController {

}

- (void) buttonPressed:(id)sender;
- (void) populateAddressBookWithPresidents;
- (void) addPersonWithFirstName:(NSString *) firstName lastName: (NSString *) lastName phone: (NSString *) phone birthDate: (NSDate *) birthDate image: (NSString *) image ;

@end

