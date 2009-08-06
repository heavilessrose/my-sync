//
//  MultiLineTextFieldAppDelegate.h
//  MultiLineTextField
//
//  Created by wang luke on 8/6/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MultiLineTextFieldViewController;

@interface MultiLineTextFieldAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    MultiLineTextFieldViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MultiLineTextFieldViewController *viewController;

@end

