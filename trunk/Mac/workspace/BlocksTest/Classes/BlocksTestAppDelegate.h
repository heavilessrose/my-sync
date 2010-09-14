//
//  BlocksTestAppDelegate.h
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BlocksTestViewController;

@interface BlocksTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    BlocksTestViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet BlocksTestViewController *viewController;

@end

