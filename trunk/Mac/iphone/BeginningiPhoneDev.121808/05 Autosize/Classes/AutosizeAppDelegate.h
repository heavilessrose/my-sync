//
//  AutosizeAppDelegate.h
//  Autosize
//
//  Created by Jeff LaMarche on 6/15/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class AutosizeViewController;

@interface AutosizeAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet AutosizeViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) AutosizeViewController *viewController;

@end

