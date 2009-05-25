//
//  TapTapsAppDelegate.h
//  TapTaps
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TapTapsViewController;

@interface TapTapsAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet TapTapsViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) TapTapsViewController *viewController;

@end

