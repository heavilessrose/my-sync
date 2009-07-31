//
//  customUIAppDelegate.h
//  customUI
//
//  Created by wang luke on 7/31/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class customUIViewController;

@interface customUIAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    MessageTestController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MessageTestController *viewController;

@end

