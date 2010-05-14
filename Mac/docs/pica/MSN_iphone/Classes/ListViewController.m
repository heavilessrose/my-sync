//
//  ListViewController.m
//  msn_for_mac
//
//  Created by  apple on 08-9-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ListViewController.h"
#import "ClientNetWorkController.h"
#import "ContactInfoViewController.h"
#import "SettingController.h"
#import "ConversationList.h"
#import "MSNAppDelegate.h"
#import "ContactList.h"
#import "ListViewCell.h"
#import "ListHeaderView.h"
#import "GroupInfo.h"
#import "ImageButton.h"
#import "ImageLabel.h"
#import "PersonInfo.h"
#import "define.h"
#import "API.h"
#import "ZwpController.h"
#import "pcAlertView.h"
#import "pcExtSettingsViewController.h"
#import "pcSearchContactsViewController.h"

NSString *ShowChatViewNotification  = @"ShowChatView";
NSString *SayHiViewNotification     = @"SayHiViewNotification";
//NSString *mClubInviateNotification  = @"mClubInviateNotification";
NSString *ShowChatViewsNotification = @"ShowChatViews";
NSString *SayHiPackageSentNotification = @"SayHiPackageSentNotification";

@implementation ListViewController

@synthesize contactList, showViewsButton, selectedIndex, contactTextField, timer, myAlertView, myActionSheet;

@synthesize impresaLabel, imageButton;
@synthesize nickNameLabel;
@synthesize toolbarTitle;
@synthesize statusText;

- (void)showContactInfoController: (NSIndexPath*)anIndexPath{
	if (anIndexPath == nil) return;
	ContactInfoViewController *aController = [[ContactInfoViewController alloc] init];
	aController.personInfo = [contactList personAtIndex: anIndexPath];
	[self.navigationController pushViewController: aController animated: YES];
	[aController release];
}


- (void)showSessionController: (NSIndexPath*)anIndexPath{
	if (anIndexPath == nil) return;
	PersonInfo *person = [contactList personAtIndex: anIndexPath];
	NSDictionary *info = [NSDictionary dictionaryWithObject:person forKey:@"key"];
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];
}

- (void)showSettingsController{
	SettingController* aController = [[SettingController alloc] initWithStyle: UITableViewStyleGrouped];
	aController.title = NSLocalizedString(@"Status", nil);
	[self.navigationController pushViewController: aController animated: YES];
	[aController release];
}

#pragma mark -
#pragma mark viewControllers delegate methods.

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		selectedIndex = nil;
		contactTextField = nil;
		timer = nil;
		sysDismiss = YES;
		isActionSheet = NO;
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
		infoDictionary = appdelegate.infoDict;
		showBigImage = [[infoDictionary objectForKey:kShowBigImageKey] boolValue];
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		
		[nc addObserver: self
			   selector: @selector(notifyAuthInfoChanged:) 
				   name: UpdateAuthInfoNotification 
				 object: nil];
		[nc addObserver: self
			   selector: @selector(notifyDismissAlertView:) 
				   name: DismissAlertViewNotification 
				 object: nil];
		[nc addObserver: self
			   selector: @selector(addBuddy:) 
				   name: AddBuddyNotification 
				 object: nil];
		[nc addObserver: self
			   selector: @selector(IMFromWap:) 
				   name: IMFromWapNotification 
				 object: nil];
		[nc addObserver:self 
			   selector:@selector(updateLocalName:)
				   name:LocalNameResultNotification 
				 object:nil];
		[nc addObserver:self 
			   selector:@selector(chat)
				   name:SayHiPackageSentNotification 
				 object:nil];
		
	}
	return self;
}

// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	LogMethod();
	[super loadView];
	
	NSMutableArray * array = [NSMutableArray arrayWithCapacity:0];
	actionButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAction 
																 target:self action:@selector(action:)]; 
	actionButton.width = 60;
	actionButton.style = UIBarButtonItemStyleBordered;
	[array addObject:actionButton];
	[actionButton release];
	
	UIBarButtonItem *spaceButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace 
																				 target:nil action:nil];
	
	[array addObject:spaceButton];
	[spaceButton release];
	
	toolbarTitle = [[UILabel alloc] initWithFrame: CGRectMake(0, 0, 180, 44)];
	toolbarTitle.font = [UIFont boldSystemFontOfSize: 20];
	toolbarTitle.textColor = [UIColor whiteColor];
	toolbarTitle.textAlignment = UITextAlignmentCenter;
	toolbarTitle.backgroundColor = [UIColor clearColor];
	toolbarTitle.text = NSLocalizedString(@"qtn_title_contact", nil);
	spaceButton = [[UIBarButtonItem alloc] initWithCustomView: toolbarTitle]; 
	[array addObject: spaceButton];
	[spaceButton release];
	
	spaceButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace 
																				 target:nil action:nil];
	
	[array addObject:spaceButton];
	[spaceButton release];
	
	[array addObject: self.editButtonItem];
	
	CGRect rect = CGRectMake(0, -2, 320, 46);
	UIToolbar *toolBar = [[UIToolbar alloc] initWithFrame:rect];
	toolBar.barStyle = UIBarStyleDefault;
	toolBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
	[toolBar setItems:array animated:NO];
	
	[self.view addSubview:toolBar];
	[toolBar release];
    
	rect = CGRectMake(0, 44, 320, 50);
	UIView *titleView = [[UIView alloc] initWithFrame:rect];
	titleView.backgroundColor = [UIColor clearColor];
	
	// Add the bg image.
	rect = CGRectMake(0, 0, 320, 40);
	NSString* string = [[NSBundle mainBundle] pathForResource:@"bg" ofType:@"png"];
	UIImage *theImage = [[UIImage imageWithContentsOfFile:string] stretchableImageWithLeftCapWidth:1 topCapHeight:0];
	UIImageView * imageView = [[UIImageView alloc] initWithImage:theImage];
	imageView.frame = rect;
	[titleView addSubview:imageView];
	[imageView release];
	
	rect = CGRectMake(-1, -1, 45, 45);
	string = [[NSBundle mainBundle] pathForResource:@"border" ofType:@"png"];
	imageView = [[UIImageView alloc] initWithImage:[UIImage imageWithContentsOfFile:string]];
	imageView.frame = rect;
	imageView.backgroundColor = [UIColor clearColor];
	[titleView addSubview:imageView];
	[imageView release];
	
	rect = CGRectMake(5, 6, 32, 32);
	imageButton = [[ImageButton alloc] initWithFrame: rect];
	[titleView addSubview:imageButton];
	[imageButton release];
	

	rect = CGRectMake(50, 0, 185, 20);
	nickNameLabel= [[ImageLabel alloc] initWithFrame:rect];
	nickNameLabel.textColor = [UIColor blackColor];
	nickNameLabel.font = [UIFont systemFontOfSize:16.0];
	nickNameLabel.backgroundColor = [UIColor clearColor];
	[titleView addSubview:nickNameLabel];
	[nickNameLabel release];

	rect = CGRectMake(226, 7, 22, 22);
	UIImageView* anImageView = [[UIImageView alloc] initWithFrame: rect];
	[titleView addSubview: anImageView];
	rect = CGRectMake(1, 1, 20, 20);
	
	statusView = [[UIImageView alloc] initWithImage:theImage];
	statusView.frame = rect;
	[anImageView addSubview: statusView];
	[anImageView release];
	[statusView release];	
	
	rect = CGRectMake(242, 5, 73, 26);
	self.statusText = [UIButton buttonWithType: UIButtonTypeCustom];
	statusText.lineBreakMode = UILineBreakModeTailTruncation;
	statusText.font = [UIFont systemFontOfSize: 10];
	[statusText setTitleColor: [UIColor blackColor] forState: UIControlStateNormal];
	[statusText setTitleColor: [UIColor blackColor] forState: UIControlStateHighlighted];
	statusText.frame = rect;
	[titleView addSubview: statusText];
	
	[statusText addTarget: self
				   action: @selector(changeStatus:)
		 forControlEvents: UIControlEventTouchDown];
	
	rect = CGRectMake(50, 20, 185, 20);
	impresaLabel= [[ImageLabel alloc] initWithFrame:rect];
	impresaLabel.textColor = [UIColor blackColor];
	impresaLabel.font = [UIFont systemFontOfSize:16.0];
	impresaLabel.backgroundColor = [UIColor clearColor];
	[titleView addSubview:impresaLabel];
	[impresaLabel release];
	
	[self.view addSubview:titleView];
	[titleView release];
	
	rect = CGRectMake(0, 85, 320, 326);
	contactTableView = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
	contactTableView.allowsSelectionDuringEditing = NO;
	[self.view addSubview:contactTableView];
	[contactTableView release];
}


- (void)viewDidLoad
{	
	LogMethod();
	[super viewDidLoad];
	
	nickNameLabel.text = [infoDictionary objectForKey:@"nickname"];
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	if (nickNameLabel.text == nil)
	{
		nickNameLabel.text = appdelegate.myAccount;
	}
	
	PersonInfo *personInfo = [[PersonInfo alloc] init];
	personInfo.block = @"0";
	personInfo.state = [appdelegate.infoDict objectForKey:@"show"];
	statusView.image = [personInfo currentStatusImage];
	NSString* aState = getStatusString(personInfo.state, NO);
	[statusText setTitle: aState forState: UIControlStateNormal];
	[statusText setTitle: aState forState: UIControlStateHighlighted];
	[personInfo release];

	impresaLabel.text = [infoDictionary objectForKey:@"impresa"];
	
	BOOL urlChanged = [[infoDictionary objectForKey:@"urlChanged"] boolValue];
	[imageButton updateButtonImage:urlChanged];
	[infoDictionary setObject:[NSNumber numberWithBool:NO] forKey:@"urlChanged"];
	contactTableView.delegate = self;
	contactTableView.dataSource = contactList;
	contactList.listView = contactTableView;
	[contactTableView reloadData];
	showViewsButton.enabled = NO;
}

- (void)viewWillAppear:(BOOL)animated{
	[super viewWillAppear:animated];
	
	self.navigationItem.title = NSLocalizedString(@"qtn_msn_chatlist_buddy",nil);
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	showViewsButton.enabled = ([appdelegate.viewControllers count] > 0);
	
	if (selectedIndex != nil){
		UITableViewCell* aCell = [contactTableView cellForRowAtIndexPath: selectedIndex];
		if ([aCell respondsToSelector: @selector(reloadText)]){
			[aCell reloadText];
		}
	}
}


- (void)viewDidAppear:(BOOL)animated
{
	[self.navigationController setNavigationBarHidden:YES animated:NO];
	[super viewDidAppear:animated];
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning 
{
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	MYLog(@"---------%@ didReceiveMemoryWarning---------",self);
	// Release anything that's not essential, such as cached data
}

- (void)dealloc 
{
	LogMethod();
    
	nickNameLabel = nil;
	impresaLabel = nil;
	statusView  = nil;
	imageButton = nil;
	sortButton = nil;
	showViewsButton = nil;
	actionButton = nil;
	findButton = nil;
	addButton = nil;
	infoDictionary = nil;
    contactTableView = nil;
	
	self.selectedIndex = nil;
	if (contactTextField != nil)
	{
		contactTextField.delegate = nil;
		self.contactTextField = nil;
	}
	
	if (myAlertView != nil)
	{
		myAlertView.delegate = nil;
		[myAlertView dismissWithClickedButtonIndex:0 animated:NO];
		self.myAlertView = nil;
	}
	if (myActionSheet != nil)
	{
		myActionSheet.delegate = nil;
		[myActionSheet dismissWithClickedButtonIndex:0 animated:NO];
	}
	self.myActionSheet = nil;

	if (timer != nil)
		[timer invalidate];	
	self.timer = nil;
	
	[contactList release];
    contactList = nil;
	
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc removeObserver: self
				  name: UpdateAuthInfoNotification
				object: nil];
	
	[nc removeObserver: self
				  name: DismissAlertViewNotification
				object: nil];
	
	[nc removeObserver: self
				  name: AddBuddyNotification
				object: nil];
	
	[nc removeObserver: self
				  name: IMFromWapNotification
				object: nil];
	
	[nc removeObserver: self
				  name: LocalNameResultNotification
				object: nil];
	
	[nc removeObserver: self
				  name: SayHiPackageSentNotification
				object: nil];
	
	[toolbarTitle release];
	[statusText release];
    statusText = nil;
	
	[super dealloc];
}

#pragma mark -
#pragma mark utilities.

- (void)updateLocalName:(NSNotification *)note
{
	LogMethod();
	MYLog(@">>>[note]:%@",note);
	MYLog(@">>>[selectedIndex]:%d",selectedIndex.row);
	NSString *localName   = [[note userInfo] objectForKey:@"localname"];
	NSString *localgbcode = [[note userInfo] objectForKey:@"localgbcode"];
	
	PersonInfo *thePerson = [contactList personAtIndex:selectedIndex];
	thePerson.localname   = localName;
	thePerson.localgbcode = localgbcode;
	[contactTableView reloadData];
}


- (void)IMFromWap:(NSNotification *)note
{
	NSString *theImid = [[note userInfo] objectForKey:kMSGFromWap];
	PersonInfo *person = [[ContactList getInstance] personWithImid:theImid];
	if (person == nil)
	{
		person = [[PersonInfo alloc] init];
		person.imid = theImid;		
	}
	NSDictionary *info = [NSDictionary dictionaryWithObject:person forKey:@"key"];
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];
}

- (void)addBuddy:(NSNotification *)note
{
	BOOL showAlert = NO;
	BOOL sendToSelf = NO;
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSString *buddyString = [[note userInfo] objectForKey:kAddBuddyKey];
	
	if (!isEmail(buddyString))
	{
		showAlert = YES;
	}
	else if (compareStrings(buddyString, appdelegate.myAccount) == NSOrderedSame)
	{
		showAlert = YES;
		sendToSelf = YES;
	}
	else
		[self sendToImid:buddyString];
	
	if (showAlert)
	{
		NSString *message = NSLocalizedString(@"qtn_buddy_input_mp_error",nil);
		if (sendToSelf)
			message = NSLocalizedString(@"qtn_msn_error_talk_to_myself",nil);
		
		self.myAlertView = [[[UIAlertView alloc] initWithTitle:@""
													   message:message
													  delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil) 
											 otherButtonTitles:nil] autorelease];
		[myAlertView setDelegate:self];
		sysDismiss = YES;
		[myAlertView show];
	}
}

- (void)getPersonPhoto:(NSTimer*)theTimer
{
	self.timer = nil; 
	PersonInfo *info = [theTimer userInfo];
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSString * status = [appdelegate.infoDict objectForKey:@"show"];
	if (![[status lowercaseString] isEqualToString: @"offline"])
	{
		if (info.state != nil && [[info.state lowercaseString] compare:@"offline"] != NSOrderedSame)
		{
			if (info.needPortraitData)
			{
				[[ClientNetWorkController getInstance] sendPortraitRequest:info.jid];
			}
		}
	}
}

- (void)chat{
	[self showSessionController: selectedIndex];
}

- (void)showBlockAlert
{
	if (selectedIndex != nil)
	{
		PersonInfo *person = [contactList personAtIndex:selectedIndex];
		NSString *string = NSLocalizedString(@"qtn_msn_block_buddy_prompt",nil);
		NSString *titleString = NSLocalizedString(@"qtn_msn_block_buddy",nil);
		if([person.block intValue] == 1)
		{
			string = NSLocalizedString(@"qtn_msn_unblock_buddy_prompt",nil);
			titleString = NSLocalizedString(@"qtn_msn_unblock_buddy",nil);
		}
		self.myAlertView = [[[UIAlertView alloc] initWithTitle: titleString
													   message:string
													  delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
											 otherButtonTitles:NSLocalizedString(@"qtn_ok",nil), nil] autorelease];
		sysDismiss = YES;
		[myAlertView show];
	}
}

- (void)showConversation
{
	ConversationList *controller = [[ConversationList alloc] initWithStyle:UITableViewStylePlain];
	if (controller != nil)
	{
		if (isActionSheet)
		{
			MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
			controller.imid = appdelegate.myAccount;	
			[self.navigationController pushViewController:controller animated:YES];
		}
		else
		{
			if (selectedIndex != nil)
			{
				PersonInfo *person = [contactList personAtIndex:selectedIndex];
				if ([person.imid length] > 0)
				{
					controller.imid = person.imid;	
					[self.navigationController pushViewController:controller animated:YES];
				}
			}
		}
	}
	[controller release];
}

- (void)showSendAlert
{
	pcAlertView* anAlertView = [[pcAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_buddy_sendmsn_to",nil)
														  message: NSLocalizedString(@"qtn_msn_full_name",nil) 
														 delegate: self
												cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil) 
												otherButtonTitles: NSLocalizedString(@"qtn_ok",nil), nil];
	[anAlertView setKeyboardType: UIKeyboardTypeEmailAddress];
	self.myAlertView = anAlertView;
	self.contactTextField = anAlertView.textField;
	contactType = SEND_CONTACT;     
	sysDismiss = YES;   
	[myAlertView show];
	[anAlertView release];
}


- (void)sendToImid:(NSString*)theImid
{
	PersonInfo *person = [[ContactList getInstance] personWithImid:theImid];
	if (person == nil)
	{
		person = [[PersonInfo alloc] init];
		person.imid = theImid;		
	}
	NSDictionary *info = [NSDictionary dictionaryWithObject:person forKey:@"key"];
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];
}


- (void)block
{
	if (selectedIndex != nil){
		PersonInfo *person = [contactList personAtIndex:selectedIndex];
		[[ClientNetWorkController getInstance] sendBlockContactRequest:person.jid block: ![person.block intValue]];
	}
}

- (void)logout
{
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	[appdelegate logout];
	//[[ClientNetWorkController getInstance] sendLogOutRequest];
}

- (void)refreshList
{
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	[appdelegate refreshList];
}

- (IBAction)action:(id)sender
{
	UIActionSheet* anActionSheet = [[UIActionSheet alloc]
									initWithTitle:nil 
									delegate:self
									cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil)
									destructiveButtonTitle: nil
									otherButtonTitles: NSLocalizedString(@"Add Contact", nil),
									NSLocalizedString(@"qtn_search_budy",nil), 
									NSLocalizedString(@"qtn_buddy_refresh",nil),
									NSLocalizedString(@"Sort by Status",nil),
									NSLocalizedString(@"conversation",nil),
									NSLocalizedString(@"qtn_main_buddy_view_logout",nil),
//                                    NSLocalizedString(@"qtn_cancel",nil),
                                    nil];
	self.myActionSheet = anActionSheet;
	isActionSheet = YES;
    [myActionSheet showFromToolbar: [MSNAppDelegate globalAppDelegate].tabController.myToolBar];
	[anActionSheet release];
}

- (IBAction)changeStatus: (id)sender{
	[self showSettingsController];
}

- (void)showPersonInformation{
	[self showContactInfoController: selectedIndex];
}

- (IBAction)sort:(id)sender
{
	sortButton.enabled = NO;
	self.selectedIndex = nil;
	if (contactList.sortByStatus)
	{
		[contactList sort:NO];
	}
	else
	{
		[contactList sort:YES];
	}
	sortButton.enabled = YES;
}

- (void)mClubInvite
{
	LogMethod();
	
	if (selectedIndex != nil)
	{
		PersonInfo *person = [contactList personAtIndex:selectedIndex];
		NSDictionary *infoDict = [NSDictionary dictionaryWithObjectsAndKeys: person.imid,@"liveID",nil];
		[[ClientNetWorkController getInstance] sendMClubInvitePackage:infoDict];
		
		// Jump to live-wap
		MSNAppDelegate *theApp = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
		theApp.tabController.segmentedControl.selectedSegmentIndex = [theApp getTheIndex:kStandbyLiveWapVC];
		theApp.tabController.selectedIndex = [theApp getTheIndex:kStandbyLiveWapVC];
	}
}

- (void)moreDetail
{
//	LogMethod();
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	NSString *theUrl = [dict objectForKey:@"msn_cn_detail"];
	if (theUrl == nil)
	{
		MYLog(@">>> Fatal.");
		return;		
	}
	PersonInfo * person = [contactList personAtIndex:selectedIndex];
	theUrl = [NSString stringWithFormat:theUrl,[person.imid UTF8String]];

	MSNAppDelegate *theApp = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
	theApp.tabController.selectedIndex = [theApp getTheIndex:kLiveWapVC];
	theApp.tabController.segmentedControl.selectedSegmentIndex = [theApp getTheIndex:kLiveWapVC];
	[theApp.zwpController setUrl:theUrl];
}

- (void)sayHi
{
	if (selectedIndex != nil)
	{
		PersonInfo *person = [contactList personAtIndex:selectedIndex];
		NSDictionary *info = [NSDictionary dictionaryWithObject:person forKey:@"key"];
		[[NSNotificationCenter defaultCenter] postNotificationName:SayHiViewNotification object:self userInfo:info];
	}
}

- (IBAction)find:(id)sender
{
#if 0
	pcAlertView* anAlertView = [[pcAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_main_buddy_search_buddy",nil)
														  message: NSLocalizedString(@"qtn_msn_full_name",nil) 
														 delegate: self
												cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil) 
												otherButtonTitles: NSLocalizedString(@"qtn_ok",nil), nil];
	[anAlertView setKeyboardType: UIKeyboardTypeEmailAddress];
	self.myAlertView = anAlertView;
	self.contactTextField = anAlertView.textField;
	contactType = FIND_CONTACT;     
	sysDismiss = YES;   
	[myAlertView show];
	[anAlertView release];

	//first expand all group add by ancle 2009-12-24
	GroupInfo* aGroupInfo = nil;
	if (contactList.sortByStatus)
	{
		for (aGroupInfo in contactList.stateGroupList){
			[aGroupInfo.cell forceUnfoldGroup];
		}
	}
	else
	{
		for (aGroupInfo in contactList.groupList){
			[aGroupInfo.cell forceUnfoldGroup];
		}
	}
	///
	[myAlertView show];	
#else
    pcSearchContactsViewController* aController = [[pcSearchContactsViewController alloc] initWithNibName: @"SearchContactsViewController"
                                                                                                   bundle: nil];
    UINavigationController* aNavigation = [[UINavigationController alloc] initWithRootViewController: aController];
	aNavigation.navigationBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
    aController.title = NSLocalizedString(@"qtn_search_budy",nil);
    [self.navigationController presentModalViewController: aNavigation animated: YES];
    [aNavigation release];
    [aController release];
#endif
}

- (IBAction)showChatViews:(id)sender
{ 
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewsNotification object:nil];
}

- (IBAction)addContact:(id)sender
{ 
	pcAlertView* anAlertView = [[pcAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_main_buddy_view_addbuddy",nil)
														  message: NSLocalizedString(@"qtn_msn_full_name",nil) 
														 delegate: self
												cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil) 
												otherButtonTitles: NSLocalizedString(@"qtn_ok",nil), nil];
	[anAlertView setKeyboardType: UIKeyboardTypeEmailAddress];
	self.myAlertView = anAlertView;
	self.contactTextField = anAlertView.textField;
	contactType = ADD_CONTACT;     
	sysDismiss = YES;   
	[myAlertView show];
	[anAlertView release];
}

- (void)presentSheet
{
	UIActionSheet* anActionSheet = nil;
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	PersonInfo * person = [contactList personAtIndex:selectedIndex];
	
	NSString *blockString = NSLocalizedString(@"qtn_msn_member_shield",nil);
	if ([person.block boolValue])
		blockString = NSLocalizedString(@"qtn_msn_member_unshield",nil);
	
	if ([person.imid compare:appdelegate.myAccount] == NSOrderedSame)
	{
		anActionSheet = [[UIActionSheet alloc]
						 initWithTitle:nil 
						 delegate:self
						 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
						 destructiveButtonTitle:nil
						 otherButtonTitles:NSLocalizedString(@"qtn_msn_member_detial",nil), nil];
		self.myActionSheet = anActionSheet;
	}
	else if ([person.block boolValue]){
		anActionSheet = [[UIActionSheet alloc]
						 initWithTitle:nil 
						 delegate:self
						 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
						 destructiveButtonTitle:nil
						 otherButtonTitles:NSLocalizedString(@"qtn_msn_member_detial",nil), blockString, nil];
		self.myActionSheet = anActionSheet;
	}else if([[person.state lowercaseString] compare:@"offline"] == NSOrderedSame)
	{
		anActionSheet = [[UIActionSheet alloc]
						 initWithTitle:nil 
						 delegate:self
						 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
						 destructiveButtonTitle:nil
						 otherButtonTitles:NSLocalizedString(@"qtn_buddy_write_message",nil),NSLocalizedString(@"qtn_msn_member_detial",nil), 
						 blockString, nil];
		self.myActionSheet = anActionSheet;
	}
	else{
		NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
		if ([dict objectForKey:@"second_zwp"] != nil)
		{
			if ([[person.imid lowercaseString] hasSuffix:@"msn.cn"])
			{
				anActionSheet = [[UIActionSheet alloc]
								 initWithTitle:nil 
								 delegate:self
								 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
								 destructiveButtonTitle:nil
								 otherButtonTitles:NSLocalizedString(@"qtn_buddy_write_message",nil),NSLocalizedString(@"qtn_msn_member_detial",nil), 
								 blockString, NSLocalizedString(@"qtn_buddy_say_hi",nil),\
								 NSLocalizedString(@"qtn_buddy_mclub_invite",nil),NSLocalizedString(@"qtn_buddy_mclub_detail",nil),nil];	
				self.myActionSheet = anActionSheet;
			}
			else 
			{
				anActionSheet = [[UIActionSheet alloc]
								 initWithTitle:nil 
								 delegate:self
								 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
								 destructiveButtonTitle:nil
								 otherButtonTitles:NSLocalizedString(@"qtn_buddy_write_message",nil),NSLocalizedString(@"qtn_msn_member_detial",nil), 
								 blockString, NSLocalizedString(@"qtn_buddy_say_hi",nil),\
								 NSLocalizedString(@"qtn_buddy_mclub_invite",nil),nil];
				self.myActionSheet = anActionSheet;
			}
		}
		else
		{
			anActionSheet = [[UIActionSheet alloc]
							 initWithTitle:nil 
							 delegate:self
							 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
							 destructiveButtonTitle:nil
							 otherButtonTitles:NSLocalizedString(@"qtn_buddy_write_message",nil),NSLocalizedString(@"qtn_msn_member_detial",nil), 
							 blockString,nil];
			self.myActionSheet = anActionSheet;
		}
	}
	isActionSheet = NO;
	[myActionSheet showInView:self.view];
	[anActionSheet release];
}


- (void)notifyAuthInfoChanged: (NSNotification*)note
{	
	RETURN_WHEN_LESS_MEMORY;
	nickNameLabel.text = [infoDictionary objectForKey:@"nickname"];
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	if (nickNameLabel.text == nil)
	{
		nickNameLabel.text = appdelegate.myAccount;
	}
	impresaLabel.text = [infoDictionary objectForKey:@"impresa"];
	NSDictionary *dict = [note userInfo];
	BOOL urlChanged = [[dict objectForKey:@"urlChanged"] boolValue];
	[imageButton updateButtonImage:urlChanged];
	PersonInfo *person = [[PersonInfo alloc] init];
	if (person != nil)
	{
		nickNameLabel.text = [infoDictionary objectForKey:@"nickname"];
		impresaLabel.text = [infoDictionary objectForKey:@"impresa"];
		NSString * string = [infoDictionary objectForKey:@"show"];
		person.state = string;
		statusView.image = [person currentStatusImage];
		[contactTableView reloadData];
		[person release];
		NSString* aState = getStatusString(string, NO);
		[statusText setTitle: aState forState: UIControlStateNormal];
		[statusText setTitle: aState forState: UIControlStateHighlighted];
	}
}

- (void)notifyDismissAlertView: (NSNotification*)note
{
	if (myActionSheet)
	{
		myActionSheet.delegate = nil;
		[myActionSheet dismissWithClickedButtonIndex:0 animated:NO];
		self.myActionSheet = nil;
	}
}

#pragma mark -
#pragma mark alertView delegate methods.

- (void)didPresentAlertView:(UIAlertView *)anAlertView{
	if (anAlertView == myAlertView){
		[contactTextField becomeFirstResponder];
	}
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	sysDismiss = NO;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (!sysDismiss)
	{
		alertView.delegate = nil;
		self.myAlertView = nil;
		BOOL showAlert = NO;
		BOOL sendToSelf = NO;
		if ([alertView.title compare:NSLocalizedString(@"qtn_main_buddy_view_addbuddy",nil)] == NSOrderedSame)
		{
			if (buttonIndex == 1){
				if (!isEmail(contactTextField.text))
					showAlert = YES;
				else
					[[ClientNetWorkController getInstance] sendAddContactRequest:contactTextField.text];	
			}
		}
		else if ([alertView.title compare:NSLocalizedString(@"qtn_main_buddy_search_buddy",nil)] == NSOrderedSame)
		{
			if (buttonIndex == 1){
				if (!isEmail(contactTextField.text))
					showAlert = YES;
				else
					[contactList find:[contactTextField.text lowercaseString]];	
			}
		}
		else if ([alertView.title compare:NSLocalizedString(@"qtn_msn_block_buddy",nil)] == NSOrderedSame \
				 || [alertView.title compare:NSLocalizedString(@"qtn_msn_unblock_buddy",nil)] == NSOrderedSame)
		{
			if (buttonIndex == 1)
				[self block];
		}
		else if ([alertView.title compare:NSLocalizedString(@"qtn_buddy_sendmsn_to",nil)] == NSOrderedSame)
		{
			if (buttonIndex == 1){
				MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
				if (!isEmail(contactTextField.text))
				{
					showAlert = YES;
				}
				else if (compareStrings([contactTextField.text lowercaseString], appdelegate.myAccount) == NSOrderedSame)
				{
					showAlert = YES;
					sendToSelf = YES;
				}
				else
					[self sendToImid:[contactTextField.text lowercaseString]];
			}
		}
		
		if (contactTextField != nil)
		{
			[contactTextField resignFirstResponder];
			contactTextField.delegate = nil;
			self.contactTextField = nil;	
		}
		if (showAlert)
		{
			NSString *message = NSLocalizedString(@"qtn_buddy_input_mp_error",nil);
			if (sendToSelf)
				message = NSLocalizedString(@"qtn_msn_error_talk_to_myself",nil);
			
 			self.myAlertView = [[[UIAlertView alloc] initWithTitle: @""
														   message: message
														  delegate: self
												 cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
												 otherButtonTitles: nil] autorelease];
			[myAlertView setDelegate:self];
			sysDismiss = YES;
			[myAlertView show];
		}
	}
	else
	{
		if (contactTextField != nil)
		{
			[contactTextField resignFirstResponder];
		}
	}
}

#pragma mark -
#pragma mark actionSheet delegate methods.

- (void)actionSheet:(UIActionSheet *)actionSheet willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	LogMethod();
}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex > -1)
	{
		NSString *title = [actionSheet buttonTitleAtIndex:buttonIndex];
		if ([title compare:NSLocalizedString(@"qtn_buddy_write_message",nil)] == NSOrderedSame)
		{
			[self chat];
		}
		else if ([title compare:NSLocalizedString(@"Add Contact",nil)] == NSOrderedSame)
		{
			[self addContact: nil];
		}
		else if ([title compare:NSLocalizedString(@"Sort by Status",nil)] == NSOrderedSame)
		{
			[self sort: nil];
		}
		else if ([title compare:NSLocalizedString(@"qtn_search_budy",nil)] == NSOrderedSame)
		{
			[self find:nil];
		}
		else if ([title compare:NSLocalizedString(@"qtn_buddy_say_hi",nil)] == NSOrderedSame)
		{
			[self sayHi];
		}
		else if ([title compare:NSLocalizedString(@"qtn_buddy_mclub_detail",nil)] == NSOrderedSame)
		{
			[self moreDetail];
		}
		else if ([title compare:NSLocalizedString(@"qtn_buddy_mclub_invite",nil)] == NSOrderedSame)
		{
			[self mClubInvite];
		}
		else if ([title compare:NSLocalizedString(@"qtn_msn_member_shield",nil)] == NSOrderedSame
				 || [title compare:NSLocalizedString(@"qtn_msn_member_unshield",nil)] == NSOrderedSame)
		{
			[self showBlockAlert];
		}
		else if ([title compare:NSLocalizedString(@"conversation",nil)] == NSOrderedSame)
		{
			[self showConversation];
		}
		else if ([title compare:NSLocalizedString(@"qtn_main_buddy_view_logout",nil)] == NSOrderedSame)
		{
			[self logout];
		}
		else if ([title compare:NSLocalizedString(@"qtn_buddy_refresh",nil)] == NSOrderedSame)
		{
            [[NSNotificationCenter defaultCenter] postNotificationName: @"ExitThreadNotification"
                                                                object: nil];
			[self refreshList];
		}
		else if ([title compare:NSLocalizedString(@"qtn_msn_member_detial",nil)] == NSOrderedSame)
		{
			[self showPersonInformation];
		}
		else if ([title compare:NSLocalizedString(@"qtn_buddy_sendmsn_to",nil)] == NSOrderedSame)
		{
			[self showSendAlert];
		}
	}
}


#pragma mark -
#pragma mark tableView delegate methods.
- (void)scrollToRowAtIndexPath:(NSIndexPath *)indexPath atScrollPosition:(UITableViewScrollPosition)scrollPosition animated:(BOOL)animated
{
	LogMethod();
	MYLog(@">>> indexPath:%@",indexPath);
	MYLog(@">>> scrollPosition:%d",scrollPosition);
	MYLog(@">>> animated:%d",animated);
}

- (void)setEditing:(BOOL)editing animated:(BOOL)animated 
{
    [super setEditing:editing animated:animated];
    [contactTableView setEditing:editing animated:YES];
	//imageButton.enabled = !editing;
	sortButton.enabled = !editing;
	actionButton.enabled = !editing;
	findButton.enabled = !editing;
	addButton.enabled = !editing;
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	if([appdelegate.viewControllers count] == 0)
		showViewsButton.enabled = NO;
	else
		showViewsButton.enabled = !editing;
}

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
	PersonInfo* aPerson = nil;
	UITableViewCellEditingStyle style = UITableViewCellEditingStyleNone;
	if (self.editing){
		aPerson = [contactList personAtIndex: indexPath];
#if 1
		if (!isBuddyImid(aPerson.imid))
			style = UITableViewCellEditingStyleDelete;
#else
		style = UITableViewCellEditingStyleDelete;
#endif
	}
	return style;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
	return showBigImage ? 48.0 : 33.0;
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
	return [contactList tableView:tableView heightForHeaderInSection:section];
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
	return [contactList tableView:tableView viewForHeaderInSection:section];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	
	[tableView deselectRowAtIndexPath: indexPath animated: NO];
	self.selectedIndex = indexPath;
	MSNAppDelegate *anAppDelegate = (MSNAppDelegate*)[MSNAppDelegate globalAppDelegate];
	PersonInfo * aPerson = [contactList personAtIndex: indexPath];

	if (![aPerson.imid isEqualToString: anAppDelegate.myAccount]){
		[self showSessionController: indexPath];
	}
	if (showBigImage){
		if (timer != nil)
			[timer invalidate];
		self.timer  = [NSTimer scheduledTimerWithTimeInterval: 3.0
													   target: self 
													 selector: @selector(getPersonPhoto:) 
													 userInfo: aPerson 
													  repeats: NO];	
	}
}

- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath{
	return UITableViewCellAccessoryDetailDisclosureButton;
}

- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)anIndexPath{
	[self showContactInfoController: anIndexPath];
}

#pragma mark -
#pragma mark textField delegate methods.

- (BOOL)textFieldShouldClear:(UITextField *)aTextField
{
	return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)aTextField
{
	if (aTextField == contactTextField){
		[aTextField resignFirstResponder];
	}
	return YES;	
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	BOOL ret = YES;
	int length = [textField.text length] - range.length + [string length];
	
	if (textField == contactTextField)
	{
		switch (contactType) 
		{
			case FIND_CONTACT:
				if(length >= 64)
					ret = NO;
				break;
			case ADD_CONTACT:
			case SEND_CONTACT:
				if(length > 193)
					ret = NO;
				break;
			default:
				break;
		}
	}
	return ret;
}

@end
