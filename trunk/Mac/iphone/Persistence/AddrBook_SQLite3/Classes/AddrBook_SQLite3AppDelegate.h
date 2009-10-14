//
//  AddrBook_SQLite3AppDelegate.h
//  AddrBook_SQLite3
//
//  Created by wang luke on 09-10-14.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class AddrBook_SQLite3ViewController;

@interface AddrBook_SQLite3AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    AddrBook_SQLite3ViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet AddrBook_SQLite3ViewController *viewController;

@end

