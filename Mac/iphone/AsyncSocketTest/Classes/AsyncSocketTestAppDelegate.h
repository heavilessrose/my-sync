//
//  AsyncSocketTestAppDelegate.h
//  AsyncSocketTest
//
//  Created by wang luke on 6/18/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class AsyncSocketTestViewController;

@interface AsyncSocketTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    AsyncSocketTestViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet AsyncSocketTestViewController *viewController;

@end

