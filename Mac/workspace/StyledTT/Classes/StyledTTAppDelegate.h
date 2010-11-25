//
//  StyledTTAppDelegate.h
//  StyledTT
//
//  Created by luke on 10-11-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class StyledTTViewController;

@interface StyledTTAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    StyledTTViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet StyledTTViewController *viewController;

@end

