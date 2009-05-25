//
//  PinchMeAppDelegate.h
//  PinchMe
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PinchMeViewController;

@interface PinchMeAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet PinchMeViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) PinchMeViewController *viewController;

@end

