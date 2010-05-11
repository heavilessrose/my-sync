//
//  OnlineListController.m
//  MSN
//
//  Created by  apple on 08-11-10.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OnlineListController.h"
#import "ChatViewData.h"
#import "ClientNetWorkController.h"
#import "MSNAppDelegate.h"
#import "ContactList.h"
#import "PersonInfo.h"
#import "define.h"
#import "ChatViewController.h"

@implementation OnlineListController
@synthesize chatViewData;

- (id)initWithStyle:(UITableViewStyle)style
{
	self = [super initWithStyle:style];
	if (self != nil) 
	{
		persons = [[NSMutableArray alloc] init];
		personsToAdd = [[NSMutableArray alloc] init];
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver: self
			   selector: @selector(notifyPersonStateChanged:) 
				   name: PersonStatusChangedNotification 
				 object: nil];	
		
	}
	return self;
}

// If you need to do additional setup after loading the view, override viewDidLoad.
- (void)viewDidLoad 
{
	UIBarButtonItem *button = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd
																			target:self
																			action:@selector(addContactToConversation:)];
	button.enabled = NO;
	self.navigationItem.rightBarButtonItem = button;
	[button release];
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	ContactList *contactList = [ContactList getInstance];
	NSArray *array = contactList.contactList;
	PersonInfo* info = nil;
	for (info in array)
	{
		if (![info.block boolValue] 
			&& info.state != nil 
			&& [[info.state lowercaseString] compare:@"offline"] != NSOrderedSame)
		{
			if ([info.imid compare:appdelegate.myAccount options:NSCaseInsensitiveSearch] != NSOrderedSame)
			{
				if ([chatViewData.persons indexOfObject:info] == NSNotFound)
					[persons addObject:info];	
			}	
		}
	}
	[super viewDidLoad];
}


- (void)addContactToConversation:(id)sender
{
	PersonInfo *info = chatViewData.contact;
	if (info != nil)
	{
		[chatViewData cancelTransferingFile];
		[chatViewData rejectReveicingFile];
		if ([chatViewData.persons count] == 1
			&& [chatViewData.persons objectAtIndex:0] == info)
		{
			if (info.jid != nil)
			{
				[[ClientNetWorkController getInstance] sendAddParticipantRequest:info.jid 
																			jids:personsToAdd
																			 sid:chatViewData.sid
																		 contact:YES];
			}
			else
			{
				[[ClientNetWorkController getInstance] sendAddParticipantRequest:info.imid 
																			jids:personsToAdd
																			 sid:chatViewData.sid
																		 contact:NO];
			}

		}
		else
		{
			[[ClientNetWorkController getInstance] sendAddParticipantRequest:personsToAdd 
																		 sid:chatViewData.sid];
		}
	}
	[self.navigationController popToRootViewControllerAnimated:YES];
	
}

- (void)dealloc
{
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc removeObserver: self
				  name: PersonStatusChangedNotification
				object: nil];
	
	[persons release];
	[personsToAdd release];
	
	[super dealloc];
}

- (void)notifyPersonStateChanged: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	BOOL hasPerson = NO;
	PersonInfo* aPerson = nil;
	PersonInfo * info = [note object];
	
	for (aPerson in persons){
		if ([aPerson.jid isEqualToString: info.jid] ||
			[aPerson.imid isEqualToString: info.imid]){
			hasPerson = YES;
			break;
		}
	}
	if ([[info.state lowercaseString] isEqualToString: @"offline"] || [info.block boolValue]){
		if (hasPerson){
			[persons removeObject: aPerson];
			[self.tableView reloadData];
		}
	}
	else
	{
		if (!hasPerson){
			for (aPerson in chatViewData.persons){
				if ([aPerson.jid isEqualToString: info.jid] ||
					[aPerson.imid isEqualToString: info.imid]){
					hasPerson = YES;
					break;
				}
			}
			if (!hasPerson){
				[persons addObject:info];
				[self.tableView reloadData];
			}
		}
	}
}

- (void)participantChanged{
	[self.tableView reloadData];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	PersonInfo *info = nil;
	if (indexPath.section == 1)
	{
		UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
		
		if (indexPath.row < [persons count])
		{
			info = [persons objectAtIndex:indexPath.row];	
		}
		
		if (cell.accessoryType == UITableViewCellAccessoryNone)
		{
			cell.accessoryType = UITableViewCellAccessoryCheckmark;
			if (info != nil)
			{
				if ([chatViewData.persons indexOfObject:info] == NSNotFound)
				{
					[personsToAdd addObject:info.jid];
				}
			}
		}
		else
		{
			cell.accessoryType = UITableViewCellAccessoryNone;	
			if (info != nil)
			{
				if ([personsToAdd indexOfObject:info.jid] != NSNotFound)
				{
					[personsToAdd removeObject:info.jid];
				}
			}
		}
		self.navigationItem.rightBarButtonItem.enabled = ([personsToAdd count] > 0);
	}
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
	return 30;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	return 30;
}

/////datasource method
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	// Number of sections is the number of region dictionaries
	return 2;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	int rows = 0;
	switch (section)
	{
		case 0:
			rows = [chatViewData.persons count];
			break;
		case 1:
			rows = [persons count];	
			break;
	}
	return rows;
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	NSString *title = nil;
	switch (section)
	{
		case 0:
			title = NSLocalizedString(@"qtn_chat_member_title",nil);
			break;
		case 1:
			title = NSLocalizedString(@"qtn_invite_buddy",nil);
			break;
	}
	return title;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{	
	UITableViewCell *cell = nil;
	NSArray *array = nil;
	if (indexPath.section == 0)
		array = chatViewData.persons;
	else
		array = persons;
	
	if (indexPath.row < [array count])
	{
		PersonInfo * info = [array objectAtIndex:indexPath.row];
		if (info != nil)
		{
			cell = [tableView dequeueReusableCellWithIdentifier:info.jid];
			if ( cell == nil)
			{
				cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:info.jid] autorelease];				
				cell.selectedTextColor = [UIColor whiteColor];
			}
			if (info.nickname != nil)	
				cell.text = info.nickname;
			else
				cell.text = info.imid;
			
			cell.image = [info currentStatusImage];
			
			if (indexPath.section == 1)
			{
				if ([personsToAdd indexOfObject:info.jid] == NSNotFound)
					cell.accessoryType = UITableViewCellAccessoryNone;	
				else
					cell.accessoryType = UITableViewCellAccessoryCheckmark;		
			}
		}
	}
	else
	{
		MYLog(@"indexPath.row >= [persons count]");
	}
	return cell;
}

@end
