//
//  TouchTestsAppDelegate.h
//  TouchTests
//
//  Created by luke on 09-11-10.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TouchTestsViewController;

@interface TouchTestsAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    TouchTestsViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet TouchTestsViewController *viewController;

@end

