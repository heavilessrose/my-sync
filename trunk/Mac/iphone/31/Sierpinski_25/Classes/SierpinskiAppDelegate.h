//
//  SierpinskiAppDelegate.h
//  Sierpinski
//
//  Created by apps on 10/25/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface SierpinskiAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

