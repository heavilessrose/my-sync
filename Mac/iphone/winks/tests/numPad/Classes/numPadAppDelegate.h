//
//  numPadAppDelegate.h
//  numPad
//
//  Created by wang luke on 8/5/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class numPadViewController;

@interface numPadAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    numPadViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet numPadViewController *viewController;

@end

