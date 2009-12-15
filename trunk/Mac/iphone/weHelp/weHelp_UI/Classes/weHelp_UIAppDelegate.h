//
//  weHelp_UIAppDelegate.h
//  weHelp_UI
//
//  Created by luke on 09-12-10.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RootViewController.h"

//@class weHelp_UIViewController;

@interface weHelp_UIAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    //weHelp_UIViewController *viewController;
	RootViewController *rootViewControl;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
//@property (nonatomic, retain) IBOutlet weHelp_UIViewController *viewController;
@property (nonatomic, retain) IBOutlet RootViewController *rootViewControl;
@end

