//
//  Simple_TableAppDelegate.h
//  Simple Table
//
//  Created by markd on 8/2/08.
//  Copyright Length-O-Words.com 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Simple_TableViewController;

@interface Simple_TableAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet Simple_TableViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) Simple_TableViewController *viewController;

@end

