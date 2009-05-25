//
//  GLFunAppDelegate.h
//  GLFun
//
//  Created by Jeff LaMarche on 7/31/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GLFunViewController;

@interface GLFunAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet GLFunViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) GLFunViewController *viewController;

@end

