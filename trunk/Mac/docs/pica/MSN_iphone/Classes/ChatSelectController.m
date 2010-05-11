//
//  ChatSelectController.m
//  MSN
//
//  Created by  apple on 09-4-2.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ChatSelectController.h"
#import "PersonInfo.h"
#import "ChatViewData.h"
#import "define.h"
#import "MSNAppDelegate.h"
#import "ChatViewController.h"
#import "public.h"

NSString *ShowUpdateMessageNotification = @"messageUpdate";

@implementation ChatSelectController
@synthesize chatViewDatas, myAlertView;
@synthesize currentIndex;

#pragma mark utilities


-(void) refreshVC:(id)sender{
	LogMethod();
	[self.tableView reloadData];
}

#pragma mark --
- (id)initWithStyle:(UITableViewStyle)style{
	if (self = [super initWithStyle:style]){
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		
		[nc addObserver: self
			   selector: @selector(refreshVC:) 
				   name: ReceiveMessageNotification 
				 object: nil];
		[nc addObserver: self
			   selector: @selector(refreshVC:) 
				   name: ReceiveFileNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(refreshVC:) 
				   name: ReceiveVoiceNotification 
				 object: nil];

		[nc addObserver: self
			   selector: @selector(refreshVC:) 
				   name: ParticipantChangedNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyContactStatusChanged:) 
				   name: PersonStatusChangedNotification 
				 object: nil];
	}
	return self;
}


#pragma mark -
#pragma mark tableView delegate methods
- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
	UITableViewCellEditingStyle style = UITableViewCellEditingStyleNone;
	if (self.editing)
		style = UITableViewCellEditingStyleDelete;
	return style;
}


- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [chatViewDatas count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	static NSString *MyIdentifier = @"MyIdentifier";
	
	UITableViewCell *cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:MyIdentifier] autorelease];
	//cell.selectedTextColor = [UIColor whiteColor];
	cell.selectionStyle = UITableViewCellSelectionStyleNone;
	ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex:indexPath.row];
	PersonInfo * info = chatViewData.contact;
	if (info.nickname != nil)	
		cell.text = info.nickname;
	else
		cell.text = info.imid;
	
	if ([chatViewData.persons count] > 1)
	{
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"groupconv-png" ofType:@"png"];
		cell.image = [UIImage imageWithContentsOfFile:imagePath];
		
		
		NSString *string = cell.text;		
		string = [string stringByAppendingString:@"; ..."];
		cell.text = string;
	}
	else
	{
		cell.image = [info currentStatusImage];
	}
	
	if (chatViewData.hasMessage > 0)
	{
		UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(240, 10, 30, 20)];
		label.backgroundColor = [UIColor purpleColor];
		label.alpha = 0.5;
		label.textColor = [UIColor whiteColor];
		label.textAlignment = UITextAlignmentCenter;
		label.text = [NSString stringWithFormat:@"%d",chatViewData.hasMessage];
		[cell addSubview:label];
		[label release];
	}
	
	if (indexPath.row == currentIndex)
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;	
	else
		cell.accessoryType = UITableViewCellAccessoryNone;	
	// Configure the cell
	return cell;
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	UITableViewCell* aCell = nil;
	NSIndexPath* anIndexPath = nil;
	[tableView deselectRowAtIndexPath: indexPath animated: NO];	
	if (currentIndex == indexPath.row){
		ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex:indexPath.row];
		MSNAppDelegate *theApp = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
		theApp.chatViewController.chatViewData = chatViewData;
		//theApp.chatViewController.chatViewData.hasMessage = 0;
		//[[NSNotificationCenter defaultCenter] postNotificationName:ShowUpdateMessageNotification object:self];
		[(UINavigationController *)theApp.tabController.selectedViewController pushViewController:theApp.chatViewController animated:YES];
	}
	else {
		aCell = [tableView cellForRowAtIndexPath: indexPath];
		aCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
		if (currentIndex >= 0){
			anIndexPath = [NSIndexPath indexPathForRow: currentIndex inSection: 0];
			aCell = [tableView cellForRowAtIndexPath: anIndexPath];
			aCell.accessoryType = UITableViewCellAccessoryNone;	
		}
		currentIndex = indexPath.row;	
	}
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
	return 40;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath 
{
	LogMethod();
	if (editingStyle == UITableViewCellEditingStyleDelete) 
	{
		ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex: indexPath.row];
		currentIndex = indexPath.row;
		if ([chatViewData sendingFile]
			|| [chatViewData receivingFile])
		{
			self.myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert", nil)
														   message:NSLocalizedString(@"close_alert",nil)
														  delegate:self 
												 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil) 
												 otherButtonTitles:NSLocalizedString(@"qtn_ok",nil),nil] autorelease];
			[myAlertView setDelegate:self];
			[myAlertView show];
			return;
		}
		else
		{
			chatViewData.hasMessage = 0;
			[[NSNotificationCenter defaultCenter] postNotificationName:ShowUpdateMessageNotification object:self];
			[self closeChatAtIndex:indexPath.row];
		}
	}
}

#pragma mark -
#pragma mark  alertView delegate methods

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
		
		if (buttonIndex == 1)
			[self closeChatAtIndex:currentIndex];
		else{
			[self setEditing: NO animated: YES];
		}
	}
}

#pragma mark -
#pragma mark ViewController methods
- (void)closeChatAtIndex:(int)index
{
	ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex: index];
	NSString *key = [[chatViewDatas allKeys] objectAtIndex: index];
	if ((index < currentIndex) ||
		(currentIndex == index) && (index == ([chatViewDatas count] - 1))){
		currentIndex--;
	}
	
	[chatViewData saveTranscripts];
	if (key != nil){
		[chatViewDatas removeObjectForKey: key];
	}
	
	[self.tableView reloadData];
}


- (void)dealloc 
{
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	
	[nc removeObserver: self
				  name: ReceiveMessageNotification
				object: nil];
	
	[nc removeObserver: self
				  name: ReceiveFileNotification
				object: nil];
	
	[nc removeObserver: self
				  name: ReceiveVoiceNotification
				object: nil];
	
	[nc removeObserver: self
				  name: ParticipantChangedNotification
				object: nil];
	
	[nc removeObserver: self
				  name: PersonStatusChangedNotification
				object: nil];
	
	if (myAlertView != nil){
		myAlertView.delegate = nil;
		[myAlertView dismissWithClickedButtonIndex:0 animated:NO];
		self.myAlertView = nil;
	}
	chatViewDatas = nil;
	[super dealloc];
}


- (void)viewDidLoad {
	currentIndex		= -1;
	self.navigationItem.rightBarButtonItem = self.editButtonItem;
	[super viewDidLoad];
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	[self refreshVC:nil];
}

- (void)notifyContactStatusChanged: (NSNotification*)aNotify{
	RETURN_WHEN_LESS_MEMORY;
	if ([aNotify userInfo] == nil) return;
	ChatViewData *aChatViewData = nil;
	PersonInfo * aPerson = nil;
	UITabBarController_ColorManage* aTabbarController = nil;
	PersonInfo* anUpdatePerson = (PersonInfo*)[[aNotify userInfo] objectForKey: @"UpdatedPerson"];
	NSArray* aChatViewDatas = [chatViewDatas allValues];
	for (aChatViewData in aChatViewDatas){
		if ([aChatViewData.persons count] > 1) continue;
		aPerson = aChatViewData.contact;
		if ((([anUpdatePerson.jid length] > 0) && ([aPerson.jid isEqualToString: anUpdatePerson.jid])) ||
		    (([anUpdatePerson.imid length] > 0) && ([aPerson.imid isEqualToString: anUpdatePerson.imid]))){
			aPerson.nickname = anUpdatePerson.nickname;
			aPerson.sync = anUpdatePerson.sync;
			aPerson.state = anUpdatePerson.state;
			aPerson.block = anUpdatePerson.block;
			
			aTabbarController = [MSNAppDelegate globalAppDelegate].tabController;
			if ([aTabbarController selectedIndex] == 2 && [[aTabbarController selectedViewController] isKindOfClass: [UINavigationController class]] &&
				[[[aTabbarController selectedViewController] topViewController] isKindOfClass: [ChatSelectController class]]){
				[self refreshVC: nil];
			}
			break;
		}
	}
}

@end

