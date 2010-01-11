//
//  WidgetsAppDelegate.h
//  Widgets
//
//  Created by luke on 10-1-11.
//  Copyright WeTeam 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class WidgetsViewController;

@interface WidgetsAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    WidgetsViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet WidgetsViewController *viewController;

@end

