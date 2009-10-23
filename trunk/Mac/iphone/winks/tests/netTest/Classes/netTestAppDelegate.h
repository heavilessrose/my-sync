//
//  netTestAppDelegate.h
//  netTest
//
//  Created by wang luke on 09-10-23.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class netTestViewController;

@interface netTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    netTestViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet netTestViewController *viewController;

@end

