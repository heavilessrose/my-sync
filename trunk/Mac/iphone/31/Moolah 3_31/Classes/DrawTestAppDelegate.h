//
//  DrawTestAppDelegate.h
//  DrawTest
//
//  Created by AppsAmuck on 8/26/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface DrawTestAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

