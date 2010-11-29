//
//  LKStyledTextAppDelegate.h
//  LKStyledText
//
//  Created by luke on 10-11-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LKStyledTextViewController;

@interface LKStyledTextAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    LKStyledTextViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet LKStyledTextViewController *viewController;

@end

