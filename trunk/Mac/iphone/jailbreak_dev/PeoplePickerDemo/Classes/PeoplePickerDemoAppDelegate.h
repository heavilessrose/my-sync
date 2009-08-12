//
//  PeoplePickerDemoAppDelegate.h
//  PeoplePickerDemo
//
//  Copyright Apple, Inc. 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PeoplePickerDemoAppDelegate : NSObject <UIApplicationDelegate> {
    
    UIWindow *window;
    UINavigationController *navigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;

@end

