//
//  ShakeAndBreakAppDelegate.h
//  ShakeAndBreak
//
//  Created by Jeff LaMarche on 8/6/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ShakeAndBreakViewController;

@interface ShakeAndBreakAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet ShakeAndBreakViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) ShakeAndBreakViewController *viewController;

@end

