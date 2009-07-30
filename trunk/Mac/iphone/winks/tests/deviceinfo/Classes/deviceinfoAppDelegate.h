//
//  deviceinfoAppDelegate.h
//  deviceinfo
//
//  Created by wang luke on 7/24/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class deviceinfoViewController;

@interface deviceinfoAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    deviceinfoViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet deviceinfoViewController *viewController;

@end

