//
//  propertyListAppDelegate.h
//  propertyList
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class propertyListViewController;

@interface propertyListAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    propertyListViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet propertyListViewController *viewController;

@end

