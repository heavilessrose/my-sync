//
//  SwapAppDelegate.h
//  Swap
//
//  Created by Dave Mark on 7/5/08.
//  Copyright SpiderWorks 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SwapViewController;

@interface SwapAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet SwapViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) SwapViewController *viewController;

@end

