//
//  AppSettingsAppDelegate.h
//  AppSettings
//
//  Created by luke on 5/2/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppSettingsAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    RootViewController *rootViewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet RootViewController *rootViewController;

@end

