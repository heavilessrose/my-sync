//
//  AddressBookFillAppDelegate.h
//  AddressBookFill
//
//  Created by Doug Diego on 4/10/09.
//  Copyright Doug Diego 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class AddressBookFillViewController;

@interface AddressBookFillAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    AddressBookFillViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet AddressBookFillViewController *viewController;

@end

