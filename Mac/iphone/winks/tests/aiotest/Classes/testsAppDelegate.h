//
//  testsAppDelegate.h
//  tests
//
//  Created by luke on 7/11/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class testsViewController;

@interface testsAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    testsViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet testsViewController *viewController;

@end

