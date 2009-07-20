//
//  calendarAppDelegate.h
//  calendar
//
//  Created by wang luke on 7/20/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class calendarViewController;

@interface calendarAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    calendarViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet calendarViewController *viewController;

@end

