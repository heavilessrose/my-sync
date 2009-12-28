//
//  WeLogTestAppAppDelegate.h
//  WeLogTestApp
//
//  Created by luke on 09-12-28.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class WeLogTestAppViewController;

@interface WeLogTestAppAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    WeLogTestAppViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet WeLogTestAppViewController *viewController;

@end

