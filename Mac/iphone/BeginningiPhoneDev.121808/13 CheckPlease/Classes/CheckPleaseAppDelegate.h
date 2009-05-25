//
//  CheckPleaseAppDelegate.h
//  CheckPlease
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CheckPleaseViewController;

@interface CheckPleaseAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet CheckPleaseViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) CheckPleaseViewController *viewController;

@end

