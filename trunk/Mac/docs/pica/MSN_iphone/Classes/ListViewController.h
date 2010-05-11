//
//  ListViewController.h
//  msn_for_mac
//
//  Created by  apple on 08-9-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ContactList;
@class ImageButton;
@class ImageLabel;
@class pcAlertView;

typedef enum _ContactType
	{
		FIND_CONTACT = 0,
		ADD_CONTACT,
		SEND_CONTACT,
	} ContactType;

@interface ListViewController : UIViewController <UITableViewDelegate,UITextFieldDelegate,UIActionSheetDelegate,UIAlertViewDelegate>
{
	ImageLabel *nickNameLabel;
	ImageLabel *impresaLabel;
	UITableView *contactTableView;
	UIImageView *statusView;
	ImageButton *imageButton;
	UIBarButtonItem *sortButton;
	UIBarButtonItem *showViewsButton;
	UIBarButtonItem *actionButton;
	UIBarButtonItem *findButton;
	UIBarButtonItem *addButton;
	
	UITextField *contactTextField;
	pcAlertView *myAlertView;
	UIActionSheet *myActionSheet;

	BOOL sysDismiss;
	BOOL showBigImage;
	BOOL isActionSheet;

	ContactType contactType;
	NSTimer	*timer;	
	NSIndexPath *selectedIndex;	

	ContactList *contactList;
	NSMutableDictionary *infoDictionary;
	
	UILabel*		toolbarTitle;
	UIButton*		statusText;
}

- (void)block;
- (void)showBlockAlert;
- (void)chat;
- (void)refreshList;
- (IBAction)sort:(id)sender;
- (IBAction)find:(id)sender;
- (IBAction)showChatViews:(id)sender;
- (IBAction)addContact:(id)sender;
- (IBAction)action:(id)sender;
- (IBAction)changeStatus: (id)sender;
- (void)logout;
- (void)presentSheet;
- (void)getPersonPhoto:(NSTimer*)theTimer;
- (void)showConversation;
- (void)showSendAlert;
- (void)sendToImid:(NSString*)theImid;
- (void)notifyAuthInfoChanged: (NSNotification*)note;
- (void)notifyDismissAlertView: (NSNotification*)note;


@property (nonatomic, retain) UIBarButtonItem *showViewsButton;
@property (nonatomic, retain) UITextField *contactTextField;
@property (nonatomic, retain) pcAlertView *myAlertView;
@property (nonatomic, retain) UIActionSheet *myActionSheet;
@property (nonatomic, retain) NSTimer *timer;
@property (nonatomic, retain) NSIndexPath *selectedIndex;
@property (nonatomic, retain) ContactList *contactList;

@property (nonatomic, retain) ImageLabel *impresaLabel;
@property (nonatomic, retain) ImageButton *imageButton;
@property (nonatomic, retain) ImageLabel *nickNameLabel;
@property (nonatomic, retain) UILabel* toolbarTitle;
@property (nonatomic, retain) UIButton*	statusText;

@end
