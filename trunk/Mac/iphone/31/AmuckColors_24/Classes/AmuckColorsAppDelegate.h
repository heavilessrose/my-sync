//
//  AmuckColorsAppDelegate.h
//  AmuckColors
//
//  Created by AppsAmuck on 10/25/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface AmuckColorsAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

