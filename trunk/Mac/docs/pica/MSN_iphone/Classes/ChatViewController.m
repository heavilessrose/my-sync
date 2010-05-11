//
//  ChatViewController.m
//  msn_for_mac
//
//  Created by  apple on 08-9-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ChatViewController.h"
#import "ChatViewData.h"
#import "ReceivedMessageView.h"
#import "PersonInfo.h"
#import "OnlineListController.h"
#import "ContactList.h"
#import "SendFileObject.h"
#import "ReceivedFileObject.h"
#import "MSNAppDelegate.h"
#import "AudioRecorder.h"
#import "AudioPlayer.h"
#import "Advertisement.h"
#import "AdvertisementLabel.h"
#import "AdvertisementView.h"
#import "define.h"
#import "SayHiView.h"
#import "ClientNetWorkController.h"
#import "pcAudioClipsViewController.h"
#import "pcAudioActionSheet.h"
#import "API.h"

#import <QuartzCore/CALayer.h>

enum {
	ACTION_REASON_FOR_NONE				= 0,
//	ACTION_REASON_FOR_PHOTOLIBRARY,
//	ACTION_REASON_FOR_CAMERA,
//	ACTION_REASON_FOR_AUDIO_CLIPS,
	ACTION_REASON_FOR_AUDIO_RECORD,
};

#define MAX_AUDIO_CLIP_CONTENTS_SIZE			(100 * 1024.0)

NSString *TransferBeginNotification = @"TransferBegin";
NSString *TransferEndNotification = @"TransferEnd";

SendReceiveStatus isSendReceive = NO;

@implementation ChatViewController

@synthesize myName, picker, pressionPanel, receivedMessageView;
@synthesize recorderView;
@synthesize blockButton;


///VERSION_FOR_SIMPLIFIED

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		sended = YES;
		restore = YES;

		receivedMessageView.dataSource = nil;
		receivedMessageView.delegate = nil;
		
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
		pressionStrings = appdelegate.pressionStrings;
		self.pressionPanel = appdelegate.pressionPanel;
		[self setName];
		
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		
		[nc addObserver:self selector:@selector(notifyKeyboardWillShow:)
				   name:UIKeyboardWillShowNotification 
				 object:sendingMessageView];
		
		[nc addObserver:self selector:@selector(notifyKeyboardWillHide:) 
				   name:UIKeyboardWillHideNotification
				 object:sendingMessageView];
		
		[nc addObserver: self
			   selector: @selector(notifyAuthInfoChanged:) 
				   name: UpdateAuthInfoNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyStateChanged:)
				   name: UpdatePersonNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyTouchInReceivedMessageView:) 
				   name: TouchInReceivedMessageViewNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyTransferBegin:) 
				   name: TransferBeginNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyTransferEnd:) 
				   name: TransferEndNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyDismissAlertView:) 
				   name: DismissAlertViewNotification 
				 object: nil];
	}
	return self;
}
 
- (void)dealloc
{
	LogMethod();
	[self garbage];
	sendingMessageView.delegate = nil;
	receivedMessageView.dataSource = nil;
	receivedMessageView.delegate = nil;
	[pressionPanel dismissWithClickedButtonIndex:0 animated:NO];
	pressionPanel.delegate = nil;
	[pressionPanel release];
    if (picker != nil){
        [self dismissModalViewControllerAnimated: NO];
        [picker.view setHidden: YES];
        picker.delegate = nil;
        [picker release];
    }
    picker = nil;
    
    if (audioClipsController != nil){
        [self.navigationController popViewControllerAnimated: NO];
        [audioClipsController release];
    }
    audioClipsController = nil;
    
	[myName release];
	
	self.recorderView = nil;
	[blockButton release];
	
    [[MSNAppDelegate globalAppDelegate] clearChatViewController];
	[super dealloc];
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning]; 
	if (self.picker != nil)
	{
	    [self dismissModalViewControllerAnimated:YES];
		self.picker = nil;
	}
}

- (void)loadView 
{
	[super loadView];
	NSString* string = nil;
	CGRect rect = CGRectMake(0, 0, 320, 366);
	self.view.frame = rect;
	self.view.backgroundColor = [UIColor whiteColor];

	rect = CGRectMake(0, 0, 320, 250);
	receivedMessageView = [[ReceivedMessageView alloc] initWithFrame:rect style:UITableViewStylePlain];
	receivedMessageView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	[self.view addSubview:receivedMessageView];
	[receivedMessageView release];
	
	NSMutableArray* array = [[NSMutableArray alloc] init];
#ifdef VERSION_FOR_SIMPLIFIED
	string = [[NSBundle mainBundle] pathForResource: @"sendface-png"
											 ofType: @"png"];
	[array addObject:[UIImage imageWithContentsOfFile:string]];
	
//	string = [[NSBundle mainBundle] pathForResource:@"block-png" ofType:@"png"];
//	[array addObject:[UIImage imageWithContentsOfFile:string]];
#else
	string = [[NSBundle mainBundle] pathForResource:@"sendface-png" ofType:@"png"];
	[array addObject:[UIImage imageWithContentsOfFile:string]];

	string = [[NSBundle mainBundle] pathForResource:@"sendPictureMsg-png" ofType:@"png"];
	[array addObject:[UIImage imageWithContentsOfFile:string]];
	
	string = [[NSBundle mainBundle] pathForResource:@"sendvoice-png" ofType:@"png"];
	[array addObject:[UIImage imageWithContentsOfFile:string]];

//	string = [[NSBundle mainBundle] pathForResource:@"block-png" ofType:@"png"];
//	[array addObject:[UIImage imageWithContentsOfFile:string]];
#endif
	
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	if ([dict objectForKey:@"second_zwp"] != nil)
	{
		string = nil;
		string = @"Hi!";
		[array addObject: string];
		
		rect = CGRectMake(2, 7, 150, 36);	
	}else {

		rect = CGRectMake(2, 7, 130, 36);
	}

	pressionButton = [[UISegmentedControl alloc] initWithItems: array];
	pressionButton.segmentedControlStyle = UISegmentedControlStyleBar;
	pressionButton.frame = rect;
	pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
	[pressionButton addTarget: self
					   action: @selector(showPressionPanel:)
			 forControlEvents: UIControlEventValueChanged];
	[array release];
	
	array = [[NSMutableArray alloc] init];
	UIBarButtonItem *barButton = [[UIBarButtonItem alloc] initWithCustomView: pressionButton];
	barButton.style = UIBarButtonItemStyleBordered; 
	[pressionButton release];
	[array addObject: barButton];
	[barButton release];
	
	barButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem: UIBarButtonSystemItemFlexibleSpace 
															  target: nil
															  action: nil];
	[array addObject:barButton];
	[barButton release];

	addContactButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"Invite",nil)
												  style:UIBarButtonItemStyleBordered 
												 target:self action:@selector(managerContact:)];
	[array addObject:addContactButton];
	[addContactButton release];
	
	sendButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"qtn_message_text_input_send",nil)
												  style:UIBarButtonItemStyleBordered 
												 target:self action:@selector(send:)];
	[array addObject:sendButton];
	[sendButton release];
	
	rect = CGRectMake(0, 250, 320, 44);
	UIToolbar *toolBar = [[UIToolbar alloc] initWithFrame:rect];
	toolBar.barStyle = UIBarStyleDefault;
	[toolBar setItems:array animated:NO];
	toolBar.autoresizingMask = UIViewAutoresizingFlexibleTopMargin;
	[self.view addSubview: toolBar];
	[toolBar release];
	[array release];
	
	rect = CGRectMake(0, 294, 320, 56);
	sendingMessageView = [[UITextView alloc] initWithFrame:rect];
	sendingMessageView.autoresizingMask = UIViewAutoresizingFlexibleTopMargin;
	[self.view addSubview:sendingMessageView];
	[sendingMessageView release];
}

- (void)viewDidAppear:(BOOL)animated
{
	LogMethod();
	int messages = [self.navigationController.tabBarItem.badgeValue intValue] - chatViewData.hasMessage;
	if (messages > 0)
	{
		self.navigationController.tabBarItem.enabled = YES;
		self.navigationController.tabBarItem.badgeValue = [NSString stringWithFormat:@"%d",messages];
	}
	else
	{
//		self.navigationController.tabBarItem.enabled = NO;
		self.navigationController.tabBarItem.badgeValue = nil;
	}


	chatViewData.hasMessage = 0;
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	Advertisement *advt = appdelegate.chatViewAdvertise;
	CGRect rc = CGRectZero;
	
	if (advt.imageView != nil)
	{
		rc = receivedMessageView.frame;
		if (rc.origin.y == 0)
		{
			rc.origin.y += advt.imageView.frame.size.height;
			rc.size.height -= advt.imageView.frame.size.height;
			receivedMessageView.frame = rc;	
		}
		rc.origin.x = (320 - advt.imageView.frame.size.width)/2;
		rc.origin.y = 0;
		rc.size = advt.imageView.frame.size;
		advt.imageView.frame = rc;
		[self.view addSubview:advt.imageView];
	}
	else if (advt.textLabel != nil)
	{
		rc = receivedMessageView.frame;
		if (rc.origin.y == 0)
		{
			rc.origin.y += advt.textLabel.frame.size.height;
			rc.size.height -= advt.textLabel.frame.size.height;
			receivedMessageView.frame = rc;
		}

		rc.origin.x = (320 - advt.textLabel.frame.size.width)/2;
		rc.origin.y = 0;
		rc.size = advt.textLabel.frame.size;
		advt.textLabel.frame = rc;
		[self.view addSubview:advt.textLabel];
	}
	[self updatePressionButtonState];
	[receivedMessageView reloadData];
	//[sendingMessageView becomeFirstResponder];
	[super viewDidAppear:animated];
}

- (void)viewDidLoad 
{
	[super viewDidLoad];
	
	actionReason			= ACTION_REASON_FOR_NONE;
	audioClipsController	= nil;
	forceScrollChatView		= NO;
	
	sendingMessageView.font = [UIFont systemFontOfSize:18.0];
	sendingMessageView.autocorrectionType = UITextAutocorrectionTypeNo;
	receivedMessageView.dataSource = chatViewData;
	receivedMessageView.delegate = chatViewData;
	receivedMessageView.separatorColor = [UIColor whiteColor];
	
	UIImage* anImage = [UIImage imageNamed: @"block-png.png"];
	self.blockButton = [UIButton buttonWithType: UIButtonTypeCustom];
	blockButton.frame = CGRectMake(200, 10, 40, 40);
	[blockButton setImage: anImage forState: UIControlStateNormal];
	[blockButton setImage: anImage forState: UIControlStateHighlighted];
	[blockButton addTarget: self
					action: @selector(block:)
		  forControlEvents: UIControlEventTouchDown];
	UIBarButtonItem* anItem = [[UIBarButtonItem alloc] initWithCustomView: blockButton];
	self.navigationItem.rightBarButtonItem = anItem;
	[anItem release];
}

- (void)viewWillAppear:(BOOL)animated
{
	if (chatViewData == nil)
	{
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
		self.chatViewData = [[[appdelegate viewControllers] allValues] objectAtIndex:0];
	}
	[self reloadMessageData];
	[super viewWillAppear:animated];
	
	self.chatViewData.hasMessage = 0;
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowUpdateMessageNotification object:self];
	if (audioClipsController != nil){
		if (audioClipsController.selectedIndexPath){
			NSString* aFileName = [audioClipsController.audioClips objectAtIndex: audioClipsController.selectedIndexPath.row];
			NSString* aSrcFilePath = audioClipPath(aFileName);
			NSString* aDstFilePath = [NSTemporaryDirectory() stringByAppendingPathComponent: aFileName];
			[[NSFileManager defaultManager] linkItemAtPath: aSrcFilePath
													toPath: aDstFilePath
													 error: nil];
			[chatViewData sendAuioFile: aDstFilePath];
		}
		[audioClipsController release];
		audioClipsController = nil;
	}
	
}

- (void)viewWillDisappear:(BOOL)animated
{
	[sendingMessageView resignFirstResponder];
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	Advertisement *advt = appdelegate.chatViewAdvertise;
	if (advt.imageView != nil)
	{
		[advt.imageView removeFromSuperview];
	}
	else if (advt.textLabel != nil)
	{
		[advt.textLabel removeFromSuperview];		
	}
	receivedMessageView.frame = CGRectMake(0, 0, 320, 252);
	[super viewWillDisappear:animated];
}

#pragma mark -
#pragma mark utilities.


- (void)showBlockAlertView
{
	LogMethod();
	PersonInfo *person = chatViewData.contact;
	
	NSString *string = NSLocalizedString(@"qtn_msn_block_buddy_prompt",nil);
	NSString *titleString = NSLocalizedString(@"qtn_msn_block_buddy",nil);
	if([person.block intValue] == 1)
	{
		string = NSLocalizedString(@"qtn_msn_unblock_buddy_prompt",nil);
		titleString =  NSLocalizedString(@"qtn_msn_unblock_buddy",nil);
	}
	UIAlertView *myAlertView = [[[UIAlertView alloc] initWithTitle: titleString
												   message:string
												  delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
										 otherButtonTitles:NSLocalizedString(@"qtn_ok",nil), nil] autorelease];
	[myAlertView show];
}


- (void)setName
{
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSString *name = [appdelegate.infoDict objectForKey:@"nickname"];
	if (name == nil)
	{
		name = appdelegate.myAccount;
	}
	if ([name length] > 15)
	{
		name = [name substringToIndex:15];
		name = [name stringByAppendingString:@"..."];
	}
	self.myName = name;
}


- (UIImage *)getImage
{
	UIImage *image = nil;
	CGRect frameRect = [UIScreen mainScreen].applicationFrame;
	UIGraphicsBeginImageContext(frameRect.size);
    CGContextRef ctx = UIGraphicsGetCurrentContext();
	CALayer *layers = self.view.layer;
	[layers renderInContext:ctx];
	image =  UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
	return image;
}

- (ChatViewData*)chatViewData
{
	return chatViewData;
}

- (void)setChatViewData:(ChatViewData*)theData
{
	[theData retain];
	
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	
	if (chatViewData != nil)
	{
		//[self getImage];
		receivedMessageView.dataSource = nil;
		receivedMessageView.delegate = nil;
		chatViewData.sendingText = sendingMessageView.text;
		chatViewData.controller = nil;
		[chatViewData release];
		chatViewData = nil;
		[nc removeObserver: self
					  name: PersonStatusChangedNotification 
					object: nil];
	}
	
	if (theData != nil)
	{
		chatViewData = theData;
		chatViewData.controller = self;
		receivedMessageView.dataSource = chatViewData;
		receivedMessageView.delegate = chatViewData;
		sendingMessageView.text = chatViewData.sendingText;
		[self setTitleText];
		[nc addObserver: self
			   selector: @selector(notifyPersonStateChanged:) 
				   name: PersonStatusChangedNotification 
				 object: chatViewData.contact];
	}
	[self updatePressionButtonState];
}

- (void)setTitleText
{
	NSString *string = nil;
	if ([chatViewData.contact.nickname length] > 0)
		string = chatViewData.contact.nickname;	
	else
		string = chatViewData.contact.imid;
	if ([chatViewData.persons count] > 1){
		string = [string stringByAppendingString:@"; ..."];
	}
	self.navigationItem.title = string;
}

- (void)reloadMessageData
{
	[receivedMessageView reloadData];
	int count = [chatViewData messageCount] - 1;
	if (count >= 0)
	{
		if (forceScrollChatView){
			forceScrollChatView = NO;
			[receivedMessageView scrollToRowAtIndexPath: [NSIndexPath indexPathForRow:count inSection:0] 
									   atScrollPosition:UITableViewScrollPositionBottom
											   animated:NO];		
		}
		else {
			BOOL aNeedScroll = NO;
			NSIndexPath* anIndexPath = nil;
			UITableViewCell* aCell = nil;
			NSArray *aCells = [receivedMessageView visibleCells];
			for (aCell in aCells){
				anIndexPath = [receivedMessageView indexPathForCell: aCell];
				if (count == 0 || anIndexPath.row == (count - 1)){
					aNeedScroll = YES;
					break;
				}
			}
			if (aNeedScroll){
				[receivedMessageView scrollToRowAtIndexPath: [NSIndexPath indexPathForRow:count inSection:0] 
										   atScrollPosition: UITableViewScrollPositionBottom
												   animated: NO];		
			}
		}
	}
}

- (void)setClearButtonState:(BOOL)flag
{
//modify by liubing20091229		
	//clearButton.enabled = flag;
//end	
}

- (void)setSendBUttonState:(BOOL)flag
{
	sendButton.enabled = flag;
}


- (void)updatePressionButtonState{
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	BOOL isBlock = ([chatViewData.contact.block intValue] == 1);
	BOOL isStranger = ([chatViewData.contact.jid length] == 0 || [chatViewData.contact.sync isEqualToString: @"deleted"]);
	BOOL isMultiPersonConversation = ([chatViewData.persons count] > 1);
	BOOL isOffLine = (([chatViewData.contact.state length] == 0) || ([[chatViewData.contact.state lowercaseString] isEqualToString: @"offline"]));
#ifdef VERSION_FOR_SIMPLIFIED
	[pressionButton setEnabled: !(isStranger || isMultiPersonConversation) forSegmentAtIndex: 1];
	if ([pressionButton numberOfSegments] > 1)
		[pressionButton setEnabled: (!isBlock && ([dict objectForKey:@"second_zwp"] != nil)) forSegmentAtIndex: 1];
	addContactButton.enabled = !(isStranger || isOffLine || isBlock);
#else
	[pressionButton setEnabled: (!isSendReceive && !isStranger && !isOffLine && !isMultiPersonConversation && !isBlock) forSegmentAtIndex: 1];
	[pressionButton setEnabled: (!isSendReceive && !isStranger && !isOffLine && !isMultiPersonConversation && !isBlock) forSegmentAtIndex: 2];
//	[pressionButton setEnabled: !(isStranger || isMultiPersonConversation) forSegmentAtIndex: 3];
	if ([pressionButton numberOfSegments] > 3)
		[pressionButton setEnabled: (!isBlock && ([dict objectForKey:@"second_zwp"] != nil)) forSegmentAtIndex: 3];
	addContactButton.enabled = !(isStranger || isOffLine || isBlock);
#endif
	blockButton.enabled = !(isStranger || isMultiPersonConversation);
}

- (IBAction)pressionSelected:(id)sender
{
	if ([sender isKindOfClass:[UIButton class]])
	{
		UIButton *theButton = (UIButton*)sender;
		restore = YES;
		[pressionPanel dismissWithClickedButtonIndex:0 animated:YES]; 
		[sendingMessageView becomeFirstResponder];
		pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
		int i = theButton.frame.origin.y / 5 + theButton.frame.origin.x / 40;
		
		NSArray *strings = [[pressionStrings objectAtIndex:i] componentsSeparatedByString:@","];
		if (sendingMessageView.selectedRange.location >= [sendingMessageView.text length])
		{
			sendingMessageView.text = [sendingMessageView.text stringByAppendingString:[strings objectAtIndex:0]];
		}
		else
		{
			NSString *string = [sendingMessageView.text substringToIndex:sendingMessageView.selectedRange.location];
			string = [string stringByAppendingString:[strings objectAtIndex:0]];
			if ([sendingMessageView.text length] > sendingMessageView.selectedRange.location)
				string = [sendingMessageView.text substringFromIndex:sendingMessageView.selectedRange.location];
			sendingMessageView.text = string;
		}
	}
	pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
}

- (IBAction)showListView:(id)sender
{
	[sendingMessageView resignFirstResponder];
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowContactListNotification object:self];
}

- (IBAction)close:(id)sender
{
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewsNotification object:nil];
}

- (IBAction)clear:(id)sender
{
	[chatViewData clear];
}

- (IBAction)send:(id)sender
{
	if ([sendingMessageView.text length] > 0)
	{
		forceScrollChatView = YES;
		
		[chatViewData send:sendingMessageView.text];

		[sendingMessageView resignFirstResponder];
		sendingMessageView.text = @"";	
	}
}

- (IBAction)managerContact:(id)sender
{
	[sendingMessageView resignFirstResponder];
	OnlineListController *onlineListController = [[OnlineListController alloc] initWithStyle:UITableViewStyleGrouped];
	onlineListController.chatViewData = chatViewData;
	[self.navigationController pushViewController:onlineListController animated:YES];
	[onlineListController release];
}

- (IBAction)block: (id)sender{
	[self showBlockAlertView];
	pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
}

- (void)displayRecorderView{
	pcAudioActionSheet* anActionSheet = [[pcAudioActionSheet alloc] initWithTitle: @""
																		 delegate: self 
																cancelButtonTitle: nil
														   destructiveButtonTitle: nil
																otherButtonTitles: nil];
	self.recorderView = anActionSheet;
	[anActionSheet.leftButton setTitle: NSLocalizedString(@"qtn_attache_record",nil) forState:UIControlStateNormal];
	[anActionSheet.rightButton setTitle: NSLocalizedString(@"qtn_cancel",nil) forState:UIControlStateNormal];
	[anActionSheet setLabelText: @""];
	[anActionSheet showInView: self.view];	
	[anActionSheet release];
}

- (void)showAudioClipsViewController{
	if (audioClipsController != nil)
		[audioClipsController release];
	audioClipsController = [[pcAudioClipsViewController alloc] initWithNibName: @"AudioClips"
																		bundle: nil];
	audioClipsController.title = NSLocalizedString(@"Clips", nil);
	UINavigationController* aNavigation = [[UINavigationController alloc] initWithRootViewController: audioClipsController];
	[self.navigationController presentModalViewController: aNavigation animated: YES];
	[aNavigation release];
}

- (IBAction)showPressionPanel:(id)sender
{
#ifdef VERSION_FOR_SIMPLIFIED
	switch (pressionButton.selectedSegmentIndex)
	{
		case 0:
			if (sended)
			{
				[UIView beginAnimations:nil context:NULL];
				[UIView setAnimationDuration:0.3];
				CGRect rect = self.view.frame;
				rect.size.height -= 170;
				self.view.frame = rect;
				[UIView commitAnimations];
				sended = NO;	
			}
			restore = NO;
			[sendingMessageView resignFirstResponder];
			pressionPanel.delegate = self;
			[pressionPanel show];
			break;
//		case 1:
//			[self showBlockAlertView];
//			pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
//			break;
		case 1:
			[SayHiView SayHiInView:self.view rect:CGRectMake(25, 25, 270, 330) liveID:chatViewData.contact.imid];
			pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
			break;
	}
#else
	UIActionSheet *menu = nil;
	switch (pressionButton.selectedSegmentIndex)
	{
		case 0:
			if (sended)
			{
				[UIView beginAnimations:nil context:NULL];
				[UIView setAnimationDuration:0.3];
				CGRect rect = self.view.frame;
				rect.size.height -= 170;
				self.view.frame = rect;
				[UIView commitAnimations];
				sended = NO;	
			}
			restore = NO;
			[sendingMessageView resignFirstResponder];
			pressionPanel.delegate = self;
			[pressionPanel show];
			break;
		case 1:
			[sendingMessageView resignFirstResponder];
			[self showPhotoLibrary:nil];
			break;
		case 2:
			{
				[sendingMessageView resignFirstResponder];
				menu = [[UIActionSheet alloc] initWithTitle: nil 
												   delegate: self
										  cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil)
									 destructiveButtonTitle: nil
										  otherButtonTitles: NSLocalizedString(@"Audio Clips",nil), NSLocalizedString(@"Audio Recorder",nil), nil];
				actionReason = ACTION_REASON_FOR_AUDIO_RECORD;
				[menu showInView: self.view];
				pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
			}
			break;
//		case 3:
			// Show block dialog.
//			[self showBlockAlertView];
//			pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
//			break;
		case 3:
			[SayHiView SayHiInView:self.view rect:CGRectMake(25, 25, 270, 330) liveID:chatViewData.contact.imid];
			pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
			break;
		default:
			break;
	}
#endif
}


- (void)record:(id)sender
{
	if (sender != nil && [sender isKindOfClass:[UIButton class]])
	{
		UIButton *recordButton = (UIButton*)sender;
		if ([recordButton.currentTitle compare:NSLocalizedString(@"qtn_attache_record",nil)] == NSOrderedSame)
		{
			recorder = [[AudioRecorder alloc] init];
			recorder.controller = self;
			[recorder record];
		}
		else
		{
			[recorder stop];
			[recorderView dismissWithClickedButtonIndex:5 animated:NO];
			self.recorderView = nil;
			pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
		}
//		recordButton.enabled = NO;
	}
	else
	{
		if ([recorder isRunning])
		{
			[recorder stop];
			if (recorderView != nil)
			{
				[recorderView dismissWithClickedButtonIndex:5 animated:NO];
				self.recorderView = nil;
				pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
			}
		}
	}
}


- (void)cancelRecord:(id)sender
{
	if (recorder != nil)
	{
		recorder.controller = nil;
		if ([recorder isRunning])
		{
			[recorder stop];
		}
		[recorder release];
		recorder = nil;
	}
	if (recorderView != nil)
	{
		[recorderView dismissWithClickedButtonIndex:5 animated:NO];
		self.recorderView = nil;
	}
	pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;	
}

- (void)showPlayView:(id)sender
{
	if (sender != nil)
	{ 
		NSString *file = nil;
		if ([sender isKindOfClass:[ReceivedFileObject class]])
		{
			ReceivedFileObject *obj = (ReceivedFileObject*)sender;
			file = obj.fileName;
		}
		else if ([sender isKindOfClass:[SendFileObject class]])
		{
			SendFileObject *obj = (SendFileObject*)sender;
			file = obj.fileName;
		}
		if (file != nil)
		{
			player = [[AudioPlayer alloc] init];
			NSString *directory = NSTemporaryDirectory();
			
			CFURLRef audioFileURL =	CFURLCreateWithFileSystemPath (NULL,
																   (CFStringRef)[directory stringByAppendingPathComponent:file],
																   kCFURLPOSIXPathStyle,
																   false);
			if ([player openUrl:audioFileURL])
			{
				player.controller = self;
				
				[sendingMessageView resignFirstResponder];
				pcAudioActionSheet* anActionSheet = [[pcAudioActionSheet alloc] initWithTitle: @""
																					 delegate: self 
																			cancelButtonTitle: nil
																	   destructiveButtonTitle: nil
																			otherButtonTitles: nil];
				self.recorderView = anActionSheet;
				[anActionSheet.leftButton setTitle:NSLocalizedString(@"qtn_vc_play",nil) forState:UIControlStateNormal];
				[anActionSheet.rightButton setTitle:NSLocalizedString(@"qtn_cancel",nil) forState:UIControlStateNormal];
				[anActionSheet showInView: self.view];	
				[anActionSheet release];
			}
			else
			{
				[player release];
				player = nil;
				UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"qtn_vc_play",nil) 
																 message:NSLocalizedString(@"Error",nil)  
																delegate:self 
													   cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil)
													   otherButtonTitles:nil] autorelease];
				[alert show];
				
			}
			CFRelease(audioFileURL);	
		}
	}

}

- (void)play:(id)sender
{
	if (player != nil)
	{
		UIButton *playButton = (UIButton*)sender;
		if ([playButton.currentTitle isEqualToString: NSLocalizedString(@"qtn_vc_play",nil)])
		{
			[player play];
		}
		else
		{
			player.audioPlayerShouldStopImmediately = YES;
			[player stop];
		}
//		playButton.enabled = NO;
	}

}

- (void)closePlay:(id)sender
{
	if (player != nil)
	{
		player.controller = nil;
		if ([player isRunning])
		{
			player.audioPlayerShouldStopImmediately = YES;
			[player stop];
		}
		[player release];
		player = nil;
	}
	if (recorderView != nil)
	{
		[recorderView dismissWithClickedButtonIndex:5 animated:NO];
		self.recorderView = nil;
	}
	pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
}

- (void)audioStateChanged
{
	if (recorder != nil)
	{
		if ([recorder isRunning])
		{
			[recorderView.leftButton setTitle: NSLocalizedString(@"qtn_message_text_input_send",nil) forState:UIControlStateNormal];
//			recorderView.leftButton.enabled = YES;
		}
		else
		{
			NSString *filePath = [NSString stringWithString:recorder.filePath];
            sleep(4);
			[recorder release];
			recorder = nil;
			[chatViewData sendVoiceClip:filePath];
		}
	}
	else if (player != nil)
	{
		if ([recorder isRunning])
		{
			[recorderView.leftButton setTitle:NSLocalizedString(@"qtn_vc_stop",nil) forState:UIControlStateNormal];
//			recorderView.leftButton.enabled = YES;
		}
		else
		{
			if ([[recorderView.leftButton titleForState:UIControlStateNormal] isEqualToString: NSLocalizedString(@"qtn_vc_play",nil)])
			{
				[recorderView.leftButton setTitle:NSLocalizedString(@"qtn_vc_stop",nil) forState:UIControlStateNormal];
//				recorderView.leftButton.enabled = NO;	
			}else {
				[recorderView.leftButton setTitle:NSLocalizedString(@"qtn_vc_play",nil) forState:UIControlStateNormal];
//				recorderView.leftButton.enabled = YES;	
			}
			[recorderView.progress setProgress:0.0f];
		}
	}
}

- (void)audioCallBack:(NSNumber*)theBytes
{
	int bytes = [theBytes intValue];
	if (player != nil)
	{
		SInt64 size = [player packetsNumber];
		[recorderView.progress setProgress:bytes*1.0/size];
	}
	else
	{
		if (recorderView != nil && ![recorderView.progress isHidden]){
			[recorderView.progress setProgress:((float)bytes)/MAX_AUDIO_CLIP_CONTENTS_SIZE];
			if (recorder != nil){
				float rate = (float)bytes/MAX_AUDIO_CLIP_CONTENTS_SIZE;
				if (rate > 1){
					[self record:nil];
				}
			}
		}
		else{
			[recorderView setLabelText: [NSString stringWithFormat:@"%@ %.0f Kb",NSLocalizedString(@"qtn_record_text",nil),bytes*1.0/1024]];
		}
	}
}

- (void)showPhotoLibrary:(id)sender
{
	if ( [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary])
	{
		self.picker = [[[UIImagePickerController alloc] init] autorelease];
		picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
		picker.delegate = self;
		picker.allowsImageEditing = YES;
		[self presentModalViewController:picker animated:YES];
	}	
}

- (void)capturePhoto:(id)sender
{
	if ( [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
	{
		self.picker = [[[UIImagePickerController alloc] init] autorelease];
		picker.sourceType = UIImagePickerControllerSourceTypeCamera;
		picker.delegate = self;
		picker.allowsImageEditing = YES;
		
		[self presentModalViewController:picker animated:YES];	
	}
}

- (NSIndexPath*)findIndexPathByCell: (UITableViewCell*)aCell{
	if (aCell == nil) return nil;
	return [receivedMessageView indexPathForCell: aCell];
}


- (void)notifyDismissAlertView: (NSNotification*)note
{
	if ([note object] != nil)
	{
		[self cancelRecord:nil];
		[self closePlay:nil];	
	}
}

- (void)notifyTransferBegin: (NSNotification*)note
{
//modify by liubing20091229		
	//clearButton.enabled = NO;
//end	
	id obj = [note object];
	if ([obj isKindOfClass:[SendFileObject class]])
		isSendReceive = SEND;
	else if ([obj isKindOfClass:[ReceivedFileObject class]])
		isSendReceive = RECEIVE;
	[self updatePressionButtonState];
}

- (void)notifyTransferEnd: (NSNotification*)note
{
//modify by liubing20091229		
	//if (![chatViewData receivingFile] && ![chatViewData sendingFile])
	//	clearButton.enabled = YES;
//end	
	RETURN_WHEN_LESS_MEMORY;
	isSendReceive = NONE;
	[self updatePressionButtonState];
}

- (void)notifyTouchInReceivedMessageView: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	[sendingMessageView resignFirstResponder];
}

- (void)notifyStateChanged: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	[self updatePressionButtonState];
}


- (void)notifyAuthInfoChanged: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	[self setName];
}


- (void)notifyPersonStateChanged: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	PersonInfo *info = [note object];
	if (chatViewData.contact == info)
	{
		[self updatePressionButtonState];
	}
}


- (void)notifyKeyboardWillShow: (NSNotification*)note
{
	if (sended && restore)
	{
		[UIView beginAnimations:nil context:NULL];
		[UIView setAnimationDuration:0.3];
		CGRect rect = self.view.frame;
		rect.size.height -= 170;
		self.view.frame = rect;
		[UIView commitAnimations];
		if ([chatViewData messageCount] > 0)
			[receivedMessageView scrollToRowAtIndexPath: [NSIndexPath indexPathForRow:[chatViewData messageCount]-1 inSection:0] 
									   atScrollPosition:UITableViewScrollPositionBottom
											   animated:NO];
		sended = NO;	
	}
}

- (void)notifyKeyboardWillHide: (NSNotification*)note
{
	if (!sended && restore)
	{
		[UIView beginAnimations:nil context:NULL];
		[UIView setAnimationDuration:0.3];
		CGRect rect = self.view.frame;
		rect.size.height += 170;
		self.view.frame = rect;	
		[UIView commitAnimations];
		sended = YES;
	 }
}

#pragma mark -
#pragma mark alertView delegate methods.
- (void)willPresentAlertView:(UIAlertView *)alertView
{
	if (pressionPanel == alertView)
		alertView.frame = CGRectMake(0, 230, 320, 200);
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == 1){
		PersonInfo *person = chatViewData.contact;
		[[ClientNetWorkController getInstance] sendBlockContactRequest: person.jid block: ![person.block intValue]];
	}
}


#pragma mark -
#pragma mark imagePickerController delegate methods.
- (void)imagePickerController:(UIImagePickerController *)picker
		didFinishPickingImage:(UIImage *)theImage
				  editingInfo:(NSDictionary *)editingInfo
{
	[self dismissModalViewControllerAnimated:YES];
    self.picker = nil;
    UIImage* anImage = [editingInfo objectForKey: UIImagePickerControllerOriginalImage];
    if (anImage == nil)
        anImage = theImage;
	[chatViewData sendImage: anImage];
	pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
	//[sendingMessageView becomeFirstResponder];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [self dismissModalViewControllerAnimated:YES];
    self.picker = nil;
	pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
	//[sendingMessageView becomeFirstResponder];
}


#pragma mark -
#pragma mark actionSheet delegate methods.
- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (actionReason == ACTION_REASON_FOR_AUDIO_RECORD){
		if (buttonIndex == 0){
			[self showAudioClipsViewController];
		}
		else if (buttonIndex == 1){
			[self displayRecorderView];
		}
		else {
		}
	}
	else {
		NSString *title = [actionSheet buttonTitleAtIndex:buttonIndex];
		if ([title compare:NSLocalizedString(@"qtn_cancel",nil)] == NSOrderedSame)
		{
			pressionButton.selectedSegmentIndex = UISegmentedControlNoSegment;
			[sendingMessageView becomeFirstResponder];
		}
		else if ([title compare:NSLocalizedString(@"qtn_camera_select",nil)] == NSOrderedSame)
		{
			[self showPhotoLibrary:nil];
		}
		else if ([title compare:NSLocalizedString(@"qtn_camera",nil)] == NSOrderedSame)
		{
			[self capturePhoto:nil];
		}
	}
	actionReason = ACTION_REASON_FOR_NONE;
	[actionSheet release];
	actionSheet = nil;
}

#pragma mark -
#pragma mark textView delegate methods.

- (BOOL)textViewShouldBeginEditing:(UITextView *)textView
{
	return YES;
}

- (BOOL)textViewShouldEndEditing:(UITextView *)textView
{
	return YES;
}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
	BOOL ret = NO;
	int length = [textView.text length] - range.length + [text length];
	if (length <= 500)
	{
		ret = YES;
	}
	return ret;
}

- (IBAction)leftButtonClick: (id)sender{
	NSString* aLeftButtonTitle = nil;
	if (sender == recorderView){
		aLeftButtonTitle = [recorderView.leftButton titleForState: UIControlStateNormal];
		if ([aLeftButtonTitle isEqualToString: NSLocalizedString(@"qtn_attache_record",nil)] ||
			[aLeftButtonTitle isEqualToString: NSLocalizedString(@"qtn_message_text_input_send",nil)]){
			[self record: recorderView.leftButton];
		}
		else {
			[self play: recorderView.leftButton];
		}
	}
}

- (IBAction)rightButtonClick: (id)sender{
	NSString* aLeftButtonTitle = nil;
	if (sender == recorderView){
		aLeftButtonTitle = [recorderView.leftButton titleForState: UIControlStateNormal];
		if ([aLeftButtonTitle isEqualToString: NSLocalizedString(@"qtn_attache_record",nil)] ||
			[aLeftButtonTitle isEqualToString: NSLocalizedString(@"qtn_message_text_input_send",nil)]){
			[self cancelRecord: recorderView.rightButton];
		}
		else {
			[self closePlay: recorderView.rightButton];
		}
	}
}

- (void)garbage{
	if (recorder != nil){
		[self cancelRecord: nil];	
		[recorder release];
	}
    recorder = nil;
	if (player != nil){
		[self closePlay: nil];
		[player release];
	}
    player = nil;
	
	self.myName = nil;
	self.chatViewData = nil;
//	[chatViewData garbage];
}

@end
