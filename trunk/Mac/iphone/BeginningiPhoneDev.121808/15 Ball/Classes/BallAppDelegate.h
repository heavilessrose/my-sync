//
//  BallAppDelegate.h
//  Ball
//
//  Created by Jeff LaMarche on 8/6/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BallViewController;

@interface BallAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet BallViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) BallViewController *viewController;

@end

