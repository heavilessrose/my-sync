//
//  controlFunAppDelegate.h
//  controlFun
//
//  Created by wang luke on 4/23/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class controlFunViewController;

@interface controlFunAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    controlFunViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet controlFunViewController *viewController;

@end

