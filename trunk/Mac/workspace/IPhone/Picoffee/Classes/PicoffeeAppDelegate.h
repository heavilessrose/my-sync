//
//  PicoffeeAppDelegate.h
//  Picoffee
//
//  Created by luke on 6/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TestViewController;
@interface PicoffeeAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	TestViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

