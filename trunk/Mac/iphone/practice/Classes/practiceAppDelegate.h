//
//  practiceAppDelegate.h
//  practice
//
//  Created by wang luke on 4/23/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class practiceViewController;

// 符合UIApplicationDelegate协议， 为UIApplication的委托（应用程序委托），替UIApplication完成一些特定功能。
@interface practiceAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    practiceViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet practiceViewController *viewController;

@end

