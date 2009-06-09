//
//  HypnoAppDelegate.h
//  Hypno
//
//  Created by apps on 10/23/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface HypnoAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

