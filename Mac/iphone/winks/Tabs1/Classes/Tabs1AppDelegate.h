//
//  Tabs1AppDelegate.h
//  Tabs1
//
//  Created by wang luke on 6/25/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface Tabs1AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	IBOutlet UITabBarController *rootController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UITabBarController *rootController;
@end

