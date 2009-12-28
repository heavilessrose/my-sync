//
//  UIFrameworkAppDelegate.h
//  UIFramework
//
//  Created by luke on 09-12-22.
//  Copyright WeTeam 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIFrameworkAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

