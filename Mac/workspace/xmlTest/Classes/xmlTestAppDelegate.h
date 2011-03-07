//
//  xmlTestAppDelegate.h
//  xmlTest
//
//  Created by luke on 11-3-2.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class xmlTestViewController;

@interface xmlTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    xmlTestViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet xmlTestViewController *viewController;

@end

