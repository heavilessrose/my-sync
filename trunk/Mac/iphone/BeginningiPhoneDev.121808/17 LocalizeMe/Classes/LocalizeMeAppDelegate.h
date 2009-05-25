//
//  LocalizeMeAppDelegate.h
//  LocalizeMe
//
//  Created by Jeff LaMarche on 8/11/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LocalizeMeViewController;

@interface LocalizeMeAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet LocalizeMeViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) LocalizeMeViewController *viewController;

@end

