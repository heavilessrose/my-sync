//
//  WaterfallAppDelegate.h
//  Waterfall
//
//  Created by acs on 9/7/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface WaterfallAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

