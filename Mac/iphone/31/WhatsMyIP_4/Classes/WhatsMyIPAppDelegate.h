//
//  WhatsMyIPAppDelegate.h
//  WhatsMyIP
//
//  Created by apple on 10/4/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface WhatsMyIPAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

