//
//  SLVodAppDelegate.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BCTabBarController;

@interface SLVodAppDelegate : NSObject <UIApplicationDelegate> {

    BCTabBarController *tabBarController;
}

@property (nonatomic, retain) BCTabBarController *tabBarController;
@property (nonatomic, retain) IBOutlet UIWindow *window;

@end
