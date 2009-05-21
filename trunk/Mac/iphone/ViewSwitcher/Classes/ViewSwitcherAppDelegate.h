//
//  ViewSwitcherAppDelegate.h
//  ViewSwitcher
//
//  Created by wang luke on 5/21/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SwitchViewController;

@interface ViewSwitcherAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	
	IBOutlet SwitchViewController *switchViewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet SwitchViewController *switchViewController;

@end

