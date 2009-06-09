//
//  SpeedDialAppDelegate.h
//  SpeedDial
//
//  Created by apple on 10/7/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SpeedDialViewController;

@interface SpeedDialAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet SpeedDialViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) SpeedDialViewController *viewController;

@end

