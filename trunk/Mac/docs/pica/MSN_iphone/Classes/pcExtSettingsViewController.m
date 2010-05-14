//
//  pcExtSettingsViewController.m
//  MSN
//
//  Created by  apple on 10-1-21.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "pcExtSettingsViewController.h"
#import "MyUITextAndValueTableCell.h"
#import "MyUITextSwitchTableCell.h"
#import "LoginSelectViewController.h"
#import "ListSelectionViewController.h"
#import "ClientNetWorkController.h"
#import "StatusViewDatasource.h"
#import "MSNAppDelegate.h"
#import "define.h"
#import "API.h"

#define GROUP_COUNTS			5
#define GROUP_ITEM1_COUNTS		3
#define GROUP_ITEM2_COUNTS		1
#define GROUP_ITEM3_COUNTS		3
#define GROUP_ITEM4_COUNTS		2
#define GROUP_ITEM5_COUNTS		1

enum {
	INDEX_RING						= 0,
	INDEX_RING_OF_CONTACT_ONLINE	= INDEX_RING,
	INDEX_RING_OF_RECV_MSG,
	INDEX_RING_OF_SHAKE,

	INDEX_DISPLAY_MODE,
	INDEX_DISPLAY_OF_SHOW_BIG_IMAGE	= INDEX_DISPLAY_MODE,
	INDEX_DISPLAY_OF_ONLINE_CONTACT,

	INDEX_SAVE_HISTORY,
	INDEX_SAVE_OF_CONVERSATION		= INDEX_SAVE_HISTORY,

	INDEX_LOGIN_INFO,
	INDEX_LOGIN_INFO_OF_SAVE_PWD	= INDEX_LOGIN_INFO,
	INDEX_LOGIN_INFO_OF_AUTO_LOGIN,
	INDEX_LOGIN_INFO_OF_LOGIN_STATUS,

	INDEX_APS,
	INDEX_APS_DURATION				= INDEX_APS,
};

enum {
	ALERT_REASON_FOR_NONE,
	ALERT_REASON_FOR_CONTACT_ONLINE,
	ALERT_REASON_FOR_RECV_MSG,
	ALERT_REASON_FOR_SHAKE,
	ALERT_REASON_FOR_CHANG_SHOW_BIG_IMAGE,
	ALERT_REASON_FOR_CHANG_DISPLAY_ONLINE_CONTACTS,
	ALERT_REASON_FOR_SAVE_CONVERSATIONS,
	ALERT_REASON_FOR_REMEMBER_PWD,
	ALERT_REASON_FOR_AUTO_LOGIN,
	ALERT_REASON_FOR_LOGIN_STATUS,
};


@implementation pcExtSettingsViewController
@synthesize alertView;


- (NSInteger)rowFromIndexPath: (NSIndexPath*)anIndexPath{
	
	NSInteger aRow = anIndexPath.row;
	switch (anIndexPath.section){
		case 0:
			aRow += INDEX_LOGIN_INFO;
			break;
		case 1:
			aRow += INDEX_APS;
			break;
		case 2:
			aRow += INDEX_RING;
			break;
		case 3:
			aRow += INDEX_DISPLAY_MODE;
			break;
		case 4:
			aRow += INDEX_SAVE_HISTORY;
			break;
	}
	return aRow;
}

- (void)showViewController: (NSIndexPath*)anIndexPath{

	if (viewController != nil){
		[viewController release];
		viewController = nil;
	}
	NSInteger aRow = [self rowFromIndexPath: anIndexPath];
	if (aRow == INDEX_LOGIN_INFO_OF_LOGIN_STATUS){
		NSString* aString = [[MSNAppDelegate globalAppDelegate].infoDict objectForKey: kLoginStatusKey];
		if (aString == nil)
			aString = @"online";
		viewController = [[LoginSelectViewController alloc] initWithStyle:UITableViewStyleGrouped];
		viewController.type = StatusView;
		[self.navigationController pushViewController: viewController animated:YES];
		[viewController setStatus: getStatusString(aString, NO)];
	}
	else if (aRow == INDEX_APS_DURATION){
		UIApplication* anApp = [UIApplication sharedApplication];
		if ([anApp respondsToSelector: @selector(enabledRemoteNotificationTypes)] &&
			[anApp enabledRemoteNotificationTypes]){
			ListSelectionData* aListData = [[ListSelectionData alloc] init];
			aListData.data = [apsDurationStrings() autorelease];
			aListData.title= NSLocalizedString(@"Duration", nil);
			aListData.selected = apsDuration;
			viewController = [[ListSelectionViewController alloc] initWithNibName: @"ListSelectionViewController"
																		   bundle: nil];
			((ListSelectionViewController*)viewController).data = aListData;
			[self.navigationController pushViewController: viewController animated: YES];
			[aListData release];
		}
	}
}

- (void)viewDidLoad {
    [super viewDidLoad];
	alertReason = ALERT_REASON_FOR_NONE;
	apsDuration = APS_DURATION_NONE;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
	[self.navigationController setNavigationBarHidden: NO animated: NO];
	apsDuration = [[NSUserDefaults standardUserDefaults] integerForKey: kAPSDurationKey];
	if (viewController != nil){
		if ([viewController isKindOfClass: [LoginSelectViewController class]]){
			NSString* aStatus = [viewController status];
			MSNAppDelegate* anAppDelegate = [MSNAppDelegate globalAppDelegate];
			[anAppDelegate.infoDict setObject: getStatus(aStatus) forKey: kLoginStatusKey];
			[anAppDelegate saveAccountInfo];
			[LoginStatusCell setValue: aStatus];
		}
		else if ([viewController isKindOfClass: [ListSelectionViewController class]]){
			apsDuration = ((ListSelectionViewController*)viewController).data.selected;
			[[NSUserDefaults standardUserDefaults] setInteger: apsDuration forKey: kAPSDurationKey];
			[[NSUserDefaults standardUserDefaults] synchronize];
			//maybe send to server
			[apsDurationCell setValue: stringAPSDuration(apsDuration)];
			[[ClientNetWorkController getInstance] sendAPSInfoToServer];
		}
		[viewController release];
		viewController = nil;
	}
}

- (void)viewDidAppear: (BOOL)animated{
	[super viewDidAppear: animated];
	CGSize size = self.tableView.contentSize;
	size.height += 64;
	[self.tableView setContentSize: size];
}

#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return [[UIApplication sharedApplication] respondsToSelector: @selector(enabledRemoteNotificationTypes)] ? GROUP_COUNTS : (GROUP_COUNTS - 1);
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	switch (section) {
		case 0:
			return GROUP_ITEM1_COUNTS;
		case 1:
			return GROUP_ITEM2_COUNTS;
		case 2:
			return GROUP_ITEM3_COUNTS;
		case 3:
			return GROUP_ITEM4_COUNTS;
		case 4:
			return GROUP_ITEM5_COUNTS;
	}
    return 0;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section{
	switch (section) {
		case 0:
			return NSLocalizedString(@"Sign-in", nil);
		case 1:
			return NSLocalizedString(@"When pressing Home", nil);
		case 2:
			return NSLocalizedString(@"Play Sound", nil);
		case 3:
			return NSLocalizedString(@"Display", nil);
		case 4:
			return NSLocalizedString(@"Conversations",nil);
	}
    return nil;
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	NSString * aString = nil;
	NSInteger aRow = [self rowFromIndexPath: indexPath];
	NSString* anId = [NSString stringWithFormat: @"%d", aRow];
	MSNAppDelegate* anAppDelegate = [MSNAppDelegate globalAppDelegate];
    UITableViewCell *aCell = [tableView dequeueReusableCellWithIdentifier: anId];
	
	if (aCell == nil){
		switch (aRow) {
			case INDEX_RING_OF_CONTACT_ONLINE:
				if (ringOfContactOnLineCell == nil){
					ringOfContactOnLineCell = [[MyUITextSwitchTableCell alloc] initWithFrame: CGRectZero
																			 reuseIdentifier: anId
																					delegate: self];
					[ringOfContactOnLineCell setTitle: NSLocalizedString(@"Contacts come online", nil)];
				}
				[ringOfContactOnLineCell setValue: [[anAppDelegate.infoDict objectForKey: kOnLineAlertKey] boolValue]];
				aCell = ringOfContactOnLineCell;
				break;
			case INDEX_RING_OF_RECV_MSG:
				if (ringOfMsgRecvCell == nil){
					ringOfMsgRecvCell = [[MyUITextSwitchTableCell alloc] initWithFrame: CGRectZero
																	   reuseIdentifier: anId
																			  delegate: self];
					[ringOfMsgRecvCell setTitle: NSLocalizedString(@"Messsage received", nil)];
				}
				[ringOfMsgRecvCell setValue: [[anAppDelegate.infoDict objectForKey: kMessageAlertKey] boolValue]];
				aCell = ringOfMsgRecvCell;
				break;
			case INDEX_RING_OF_SHAKE:
				if (ringOfShakeCell == nil){
					ringOfShakeCell = [[MyUITextSwitchTableCell alloc] initWithFrame: CGRectZero
																	 reuseIdentifier: anId
																			delegate: self];
					[ringOfShakeCell setTitle: NSLocalizedString(@"Notify me nudge", nil)];
				}
				[ringOfShakeCell setValue: [[anAppDelegate.infoDict objectForKey: kNotifyNudgeKey] boolValue]];
				aCell = ringOfShakeCell;
				break;
			case INDEX_DISPLAY_OF_SHOW_BIG_IMAGE:
				if (showBigImageCell == nil){
					showBigImageCell = [[MyUITextSwitchTableCell alloc] initWithFrame: CGRectZero
																	  reuseIdentifier: anId
																			 delegate: self];
					[showBigImageCell setTitle: NSLocalizedString(@"Show profile picture", nil)];
				}
				[showBigImageCell setValue: [[anAppDelegate.infoDict objectForKey: kShowBigImageKey] boolValue]];
				aCell = showBigImageCell;
				break;
			case INDEX_DISPLAY_OF_ONLINE_CONTACT:
				if (showOnLineCell == nil){
					showOnLineCell = [[MyUITextSwitchTableCell alloc] initWithFrame: CGRectZero
																	reuseIdentifier: anId
																		   delegate: self];
					[showOnLineCell setTitle: NSLocalizedString(@"Show onlines only", nil)];
				}
				[showOnLineCell setValue: [[anAppDelegate.infoDict objectForKey: kDisplayOnLinesKey] boolValue]];
				aCell = showOnLineCell;
				break;
			case INDEX_SAVE_OF_CONVERSATION:
				if (persistentHistoryCell == nil){
					persistentHistoryCell = [[MyUITextSwitchTableCell alloc] initWithFrame: CGRectZero
																		   reuseIdentifier: anId
																				  delegate: self];
					[persistentHistoryCell setTitle: NSLocalizedString(@"Save", nil)];
				}
				[persistentHistoryCell setValue: [[anAppDelegate.infoDict objectForKey: kSaveConversationKey] boolValue]];
				aCell = persistentHistoryCell;
				break;
			case INDEX_LOGIN_INFO_OF_SAVE_PWD:
				if (pwdCell == nil){
					pwdCell = [[MyUITextSwitchTableCell alloc] initWithFrame: CGRectZero
															 reuseIdentifier: anId
																	delegate: self];
					[pwdCell setTitle: NSLocalizedString(@"pica_str_label_login_insivibly", nil)];
				}
				[pwdCell setValue: [[anAppDelegate.infoDict objectForKey: kSavePwdFlagKey] boolValue]];
				aCell = pwdCell;
				break;
			case INDEX_LOGIN_INFO_OF_AUTO_LOGIN:
				if (autoLoginCell == nil){
					autoLoginCell = [[MyUITextSwitchTableCell alloc] initWithFrame: CGRectZero
																   reuseIdentifier: anId
																		  delegate: self];
					[autoLoginCell setTitle: NSLocalizedString(@"pica_str_label_auto_login", nil)];
				}
				[autoLoginCell setValue: [[anAppDelegate.infoDict objectForKey:kAutoLoginKey] boolValue]];
				aCell = autoLoginCell;
				break;
			case INDEX_LOGIN_INFO_OF_LOGIN_STATUS:
				if (LoginStatusCell == nil){
					LoginStatusCell = [[MyUITextAndValueTableCell alloc] initWithFrame: CGRectZero
																	   reuseIdentifier: anId];
					LoginStatusCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
					[LoginStatusCell setTitle: NSLocalizedString(@"qtn_msn_status_type", nil)];
					
					LoginStatusCell.titleLabel.font = [UIFont systemFontOfSize: 16];
					LoginStatusCell.valueLabel.font = [UIFont systemFontOfSize: 16];
					LoginStatusCell.valueLabel.textColor = [UIColor grayColor];
				}
				aString = [anAppDelegate.infoDict objectForKey: kLoginStatusKey];
				if (aString == nil)
					aString = @"online";
				[LoginStatusCell setValue: getStatusString(aString, NO)];
				aCell = LoginStatusCell;
				break;
			case INDEX_APS_DURATION:
				if (apsDurationCell == nil){
					apsDurationCell = [[MyUITextAndValueTableCell alloc] initWithFrame: CGRectZero
																	   reuseIdentifier: anId];
					apsDurationCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
					[apsDurationCell setTitle: NSLocalizedString(@"qtn_main_buddy_view_exit", nil)];
					
					apsDurationCell.titleLabel.font = [UIFont systemFontOfSize: 16];
					apsDurationCell.valueLabel.font = [UIFont systemFontOfSize: 16];
				}
				[apsDurationCell setValue: stringAPSDuration(apsDuration)];
				aCell = apsDurationCell;
				break;
		}
	}
    
	if (aCell == apsDurationCell){
		UIApplication* anApp = [UIApplication sharedApplication];
		if ([anApp respondsToSelector: @selector(enabledRemoteNotificationTypes)] &&
			[anApp enabledRemoteNotificationTypes]){
			apsDurationCell.titleLabel.textColor = [UIColor blackColor];;
			apsDurationCell.valueLabel.textColor = [UIColor blueColor];;
		}
		else {
			apsDurationCell.titleLabel.textColor = [UIColor lightGrayColor];;
			apsDurationCell.valueLabel.textColor = [UIColor lightGrayColor];;
		}
	}
    return aCell;
}

- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	BOOL anEnable = [[UIApplication sharedApplication] respondsToSelector: @selector(enabledRemoteNotificationTypes)] &&
					([[UIApplication sharedApplication] enabledRemoteNotificationTypes] > 0);
	return (indexPath.section == 4) ? (anEnable ? indexPath : nil) : indexPath;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	[self showViewController: indexPath];
}


- (void)dealloc {
	[viewController release];
	[ringOfContactOnLineCell release];
	[ringOfMsgRecvCell release];
	[ringOfShakeCell release];
	[showBigImageCell release];
	[showOnLineCell release];
	[pwdCell release];
	[autoLoginCell release];
	[LoginStatusCell release];
	[apsDurationCell release];
	self.alertView = nil;
    [super dealloc];
}

- (IBAction)switchAction: (id)sender{
	id aValue = nil;
	NSString* aKey = nil;
	id anObject = nil;
	NSArray* anObjects = nil;
	UIAlertView* anAlertView = nil;
	NSMutableDictionary* aChatViewDatas = nil;
	MSNAppDelegate* anAppDelegate = [MSNAppDelegate globalAppDelegate];
	
	if (sender == ringOfContactOnLineCell){
		alertReason = ALERT_REASON_FOR_CONTACT_ONLINE;
		aValue = [NSNumber numberWithBool: [ringOfContactOnLineCell getValue]];
		aKey = kOnLineAlertKey;
		[anAppDelegate.infoDict setObject: aValue forKey: aKey];
		[anAppDelegate saveAccountInfo];
	}
	else if (sender == ringOfMsgRecvCell){
		alertReason = ALERT_REASON_FOR_RECV_MSG;
		aValue = [NSNumber numberWithBool: [ringOfMsgRecvCell getValue]];
		aKey = kMessageAlertKey;
		[anAppDelegate.infoDict setObject: aValue forKey: aKey];
		[anAppDelegate saveAccountInfo];
		
		aChatViewDatas = (NSMutableDictionary*)[anAppDelegate viewControllers];
		anObjects = [aChatViewDatas allValues];
		for (anObject in anObjects){
			if ([anObject respondsToSelector: @selector(setIsSound:)]){
				[anObject setIsSound: [ringOfMsgRecvCell getValue]];
			}
		}
	}
	else if (sender == ringOfShakeCell){
		alertReason = ALERT_REASON_FOR_SHAKE;
		aValue = [NSNumber numberWithBool: [ringOfShakeCell getValue]];
		aKey = kNotifyNudgeKey;
		[anAppDelegate.infoDict setObject: aValue forKey: aKey];
		[anAppDelegate saveAccountInfo];
	}
	else if (sender == showBigImageCell){
		anAlertView = [[UIAlertView alloc] initWithTitle: nil
												 message: NSLocalizedString(@"qtn_msn_update_buddy_list_confirm",nil)  
												delegate: self 
									   cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil)
									   otherButtonTitles: NSLocalizedString(@"qtn_ok",nil),nil];
		alertReason = ALERT_REASON_FOR_CHANG_SHOW_BIG_IMAGE;
		self.alertView = anAlertView;
	}
	else if (sender == showOnLineCell){
		anAlertView = [[UIAlertView alloc] initWithTitle: nil
												 message: NSLocalizedString(@"qtn_msn_update_buddy_list_confirm",nil)  
												delegate: self 
									   cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil)
									   otherButtonTitles: NSLocalizedString(@"qtn_ok",nil),nil];
		alertReason = ALERT_REASON_FOR_CHANG_DISPLAY_ONLINE_CONTACTS;
		self.alertView = anAlertView;
	}
	else if (sender == persistentHistoryCell){
		if (![persistentHistoryCell getValue]){
			aValue = [NSNumber numberWithBool: NO];
			aKey = kSaveConversationKey;
			[anAppDelegate.infoDict setObject: aValue forKey: aKey];
			[anAppDelegate saveAccountInfo];
		}
		else {
			alertReason = ALERT_REASON_FOR_SAVE_CONVERSATIONS;
			anAlertView = [[UIAlertView alloc] initWithTitle: nil
													 message: NSLocalizedString(@"You have chosen to save your conversations. This may not be permitted under local law. You should confirm that this is permitted before turning it on.",nil)
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_ok", nil)
										   otherButtonTitles: nil];
			self.alertView = anAlertView;
		}
	}
	else if (sender == pwdCell){
		if (![pwdCell getValue]){
			aValue = [NSNumber numberWithBool: [pwdCell getValue]];
			aKey = kSavePwdFlagKey;
			[anAppDelegate.infoDict setObject: aValue forKey: aKey];
			[anAppDelegate saveAccountInfo];
			[autoLoginCell.uiSwitch setOn: NO animated: YES];
			[self switchAction: autoLoginCell];
		}
		else {
			alertReason = ALERT_REASON_FOR_REMEMBER_PWD;
			anAlertView = [[UIAlertView alloc] initWithTitle: nil
													 message: NSLocalizedString(@"msg_safe_prompt",nil)
													delegate: self
										   cancelButtonTitle: nil
										   otherButtonTitles: NSLocalizedString(@"qtn_yes",nil),nil];
			self.alertView = anAlertView;
		}
	}
	else if (sender == autoLoginCell){
		if (![autoLoginCell getValue]){
			aValue = [NSNumber numberWithBool: [autoLoginCell getValue]];
			aKey = kAutoLoginKey;
			[anAppDelegate.infoDict setObject: aValue forKey: aKey];
			[anAppDelegate saveAccountInfo];
		}
		else {
			alertReason = ALERT_REASON_FOR_AUTO_LOGIN;
			anAlertView = [[UIAlertView alloc] initWithTitle: nil
													 message: NSLocalizedString(@"qtn_msn_auto_login_confirm",nil)
													delegate: self
										   cancelButtonTitle: nil
										   otherButtonTitles: NSLocalizedString(@"qtn_yes",nil),nil];
			self.alertView = anAlertView;
		}
	}
	[anAlertView show];
	[anAlertView release];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
	if (alertReason == ALERT_REASON_FOR_CONTACT_ONLINE){
	}
	else if (alertReason == ALERT_REASON_FOR_RECV_MSG){
	}
	else if (alertReason == ALERT_REASON_FOR_SHAKE){
	}
	else if (alertReason == ALERT_REASON_FOR_CHANG_SHOW_BIG_IMAGE){
		alertReason = ALERT_REASON_FOR_NONE;
		self.alertView = nil;
		if (buttonIndex == 0){
			[showBigImageCell.uiSwitch setOn: ![showBigImageCell getValue] animated: YES];
		}
		else {
			[[MSNAppDelegate globalAppDelegate].infoDict setObject: [NSNumber numberWithBool: [showBigImageCell getValue]]
															forKey: kShowBigImageKey];
			[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
			[[MSNAppDelegate globalAppDelegate] restart];
		}
	}
	else if (alertReason == ALERT_REASON_FOR_CHANG_DISPLAY_ONLINE_CONTACTS){
		alertReason = ALERT_REASON_FOR_NONE;
		self.alertView = nil;
		if (buttonIndex == 0){
			[showOnLineCell.uiSwitch setOn: ![showOnLineCell getValue] animated: YES];
		}
		else {
			[[MSNAppDelegate globalAppDelegate].infoDict setObject: [NSNumber numberWithBool: [showOnLineCell getValue]]
															forKey: kDisplayOnLinesKey];
			[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
			[[MSNAppDelegate globalAppDelegate] restart];
		}
	}
	else if (alertReason == ALERT_REASON_FOR_SAVE_CONVERSATIONS){
		[[MSNAppDelegate globalAppDelegate].infoDict setObject: [NSNumber numberWithBool: [persistentHistoryCell getValue]]
														forKey: kSaveConversationKey];
		[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
		alertReason = ALERT_REASON_FOR_NONE;
		self.alertView = nil;
	}
	else if (alertReason == ALERT_REASON_FOR_REMEMBER_PWD){
		[[MSNAppDelegate globalAppDelegate].infoDict setObject: [NSNumber numberWithBool: [pwdCell getValue]]
														forKey: kSavePwdFlagKey];
		[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
		alertReason = ALERT_REASON_FOR_NONE;
		self.alertView = nil;
	}
	else if (alertReason == ALERT_REASON_FOR_AUTO_LOGIN){
		[[MSNAppDelegate globalAppDelegate].infoDict setObject: [NSNumber numberWithBool: [autoLoginCell getValue]]
														forKey: kAutoLoginKey];
		[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
		if (![pwdCell getValue]){
			[pwdCell.uiSwitch setOn: YES animated: YES];
			[self switchAction: pwdCell];
		}
		else {
			alertReason = ALERT_REASON_FOR_NONE;
			self.alertView = nil;
		}
	}
}

- (void)forseDismissAlertview{
	if (alertReason == ALERT_REASON_FOR_CHANG_SHOW_BIG_IMAGE ||
		alertReason == ALERT_REASON_FOR_CHANG_DISPLAY_ONLINE_CONTACTS ||
		alertReason == ALERT_REASON_FOR_SAVE_CONVERSATIONS ||
		alertReason == ALERT_REASON_FOR_REMEMBER_PWD ||
		alertReason == ALERT_REASON_FOR_AUTO_LOGIN){
		[alertView dismissWithClickedButtonIndex: 0 animated: NO];
		self.alertView = nil;
	}
	alertReason = ALERT_REASON_FOR_NONE;
}

@end

