//
//  QuartzFunAppDelegate.h
//  QuartzFun
//
//  Created by Jeff LaMarche on 7/31/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class QuartzFunViewController;

@interface QuartzFunAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet QuartzFunViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) QuartzFunViewController *viewController;

@end

