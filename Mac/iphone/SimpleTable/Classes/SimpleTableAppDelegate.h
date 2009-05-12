//
//  SimpleTableAppDelegate.h
//  SimpleTable
//
//  Created by wang luke on 5/11/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SimpleTableViewController;

@interface SimpleTableAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    SimpleTableViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet SimpleTableViewController *viewController;

@end

