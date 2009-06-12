//
//  FileBrowserAppDelegate.h
//  FileBrowser
//
//  Created by wang luke on 6/12/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FileBrowserAppDelegate : NSObject <UIApplicationDelegate> {
    IBOutlet UIWindow *window;
	IBOutlet UINavigationController *navController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) UINavigationController *navController;

@end

