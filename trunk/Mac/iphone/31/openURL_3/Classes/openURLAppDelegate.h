//
//  openURLAppDelegate.h
//  openURL
//
//  Created by apple on 10/3/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface openURLAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

