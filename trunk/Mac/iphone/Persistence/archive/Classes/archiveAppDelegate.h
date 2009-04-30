//
//  archiveAppDelegate.h
//  archive
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class archiveViewController;

@interface archiveAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    archiveViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet archiveViewController *viewController;

@end

