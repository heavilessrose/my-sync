//
//  FileOPAppDelegate.h
//  FileOP
//
//  Created by wang luke on 5/4/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class FileOPViewController;

@interface FileOPAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    FileOPViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet FileOPViewController *viewController;

@end

