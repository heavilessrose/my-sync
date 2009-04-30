//
//  SQLiteAppDelegate.h
//  SQLite
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SQLiteViewController;

@interface SQLiteAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    SQLiteViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet SQLiteViewController *viewController;

@end

