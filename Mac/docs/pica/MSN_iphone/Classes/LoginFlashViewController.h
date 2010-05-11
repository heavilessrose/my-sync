//
//  LoginFlashViewController.h
//  MSN
//
//  Created by  apple on 08-11-3.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Advertisement;
@interface LoginFlashViewController : UIViewController 
{
	UIImageView *imageView;
	UILabel *statusLabel;
	UILabel *tipsLabel;
	Advertisement *advt;
	NSTimer *timer;
	int currentIndex;
}

- (IBAction)logout:(id)sender;

- (void)notifyFindDispatch: (NSNotification*)note;
- (void)notifyConnectingServer: (NSNotification*)note;
- (void)notifyLoggingIn: (NSNotification*)note;
- (void)notifygetList: (NSNotification*)note;
- (void)notifyH3G: (NSNotification*)note;
@end
