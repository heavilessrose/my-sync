//
//  LKeyboardAppDelegate.h
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LKeyboardViewController;

@interface LKeyboardAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    LKeyboardViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet LKeyboardViewController *viewController;


+ (LKeyboardAppDelegate *)globalAppDelegate;
@end

