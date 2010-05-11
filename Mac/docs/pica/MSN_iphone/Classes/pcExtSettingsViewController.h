//
//  pcExtSettingsViewController.h
//  MSN
//
//  Created by  apple on 10-1-21.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MyUITextSwitchTableCell;
@class MyUITextAndValueTableCell;
@class LoginSelectViewController;

@interface pcExtSettingsViewController : UITableViewController {
	MyUITextAndValueTableCell*		apsDurationCell;
	
	MyUITextSwitchTableCell*		ringOfContactOnLineCell;
	MyUITextSwitchTableCell*		ringOfMsgRecvCell;
	MyUITextSwitchTableCell*		ringOfShakeCell;
	
	MyUITextSwitchTableCell*		showBigImageCell;
	MyUITextSwitchTableCell*		showOnLineCell;
	
	MyUITextSwitchTableCell*		persistentHistoryCell;
	MyUITextSwitchTableCell*		pwdCell;
	MyUITextSwitchTableCell*		autoLoginCell;
	
	MyUITextAndValueTableCell*		LoginStatusCell;
	
	LoginSelectViewController*		viewController;
	
	NSInteger						alertReason;
	
	UIAlertView*					alertView;
	
	NSInteger						apsDuration;
}
@property (nonatomic, retain) UIAlertView* alertView;

- (IBAction)switchAction: (id)sender;

- (void)forseDismissAlertview;
@end
