//
//  CountMeInAppDelegate.h
//  CountMeIn
//
//  Created by acs on 10/5/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface CountMeInAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

