//
//  ConversationList.m
//  MSN
//
//  Created by  apple on 09-1-12.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ConversationList.h"
#import "MSNAppDelegate.h"
#import "ContactList.h"
#import "Conversation.h"
#import "PersonInfo.h"
#import "define.h"
#import "API.h"


@implementation ConversationList
@synthesize datasource,imid;

- (id)initWithStyle:(UITableViewStyle)style
{
	self = [super initWithStyle:style];
	if (self != nil) 
	{
		self.editing = NO;
	}
	return self;
}

// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	[super loadView];
}

- (void)viewDidAppear:(BOOL)animated
{
	self.navigationController.navigationBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
	[self.navigationController setNavigationBarHidden:NO animated:YES];
	[super viewDidAppear:animated];
}


- (void)setEditing:(BOOL)editing animated:(BOOL)animated 
{
    [super setEditing:editing animated:animated];
    [(UITableView*)self.view setEditing:editing animated:YES];
	self.navigationItem.leftBarButtonItem.enabled = !editing;
    // Disable the add button while editing.
}

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
	UITableViewCellEditingStyle style = UITableViewCellEditingStyleNone;
	if (self.editing)
		style = UITableViewCellEditingStyleDelete;
	return style;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView 
{
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [datasource count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSString *string = [datasource objectAtIndex:indexPath.row];
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:string];
	if (cell == nil) 
	{
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:string] autorelease];
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
		if (imid != nil)
		{
			NSRange rangeOfString = [string rangeOfString:[NSString stringWithFormat:@"%@-",appdelegate.myAccount]];
			if (rangeOfString.location == 0)
				string = [string substringFromIndex:rangeOfString.length];
		}
		else
		{
			PersonInfo *person = [[ContactList getInstance] personWithImid:string];
			if ([person.nickname length] > 0)
				string = person.nickname;
		}

		cell.text = string;
	}
	return cell;
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath 
{
	NSString *string = [datasource objectAtIndex:indexPath.row];
	if (imid != nil){
		Conversation *controller = [[Conversation alloc] initWithStyle:UITableViewStylePlain];
		[controller setDatasourceFile:string];
		[self.navigationController pushViewController:controller animated:YES];
		[controller release];
	}
	else{
		ConversationList *controller = [[ConversationList alloc] initWithStyle:UITableViewStylePlain];
		controller.imid = string;
		[self.navigationController pushViewController:controller animated:YES];
		[controller release];	
	}	
}


- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath 
{
	UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
	if (cell != nil)
	{	
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[MSNAppDelegate globalAppDelegate];
		NSDictionary *dicts = [appdelegate.infoDict objectForKey:@"conversation"];
		if (dicts != nil)
		{
			NSString *file = nil;
			NSString *fileString = nil;
			NSArray *filePaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
			NSString *directory = [filePaths objectAtIndex: 0];
			
			NSMutableDictionary *conversations = [NSMutableDictionary dictionaryWithDictionary:dicts];
			
			if (imid != nil)
			{
				fileString = [directory stringByAppendingPathComponent:cell.reuseIdentifier];
				if ([[NSFileManager defaultManager] fileExistsAtPath:fileString])
					[[NSFileManager defaultManager] removeItemAtPath:fileString error:nil];
				
				[datasource removeObject:cell.reuseIdentifier];
				if ([datasource count] > 0)
					[conversations setObject:datasource forKey:imid];
				else
					[conversations removeObjectForKey:imid];
			}
			else
			{
				NSArray *files = [dicts objectForKey:cell.reuseIdentifier];	
				if (files != nil)
				{
					for (file in files)
					{
						fileString = [directory stringByAppendingPathComponent:file];
						if ([[NSFileManager defaultManager] fileExistsAtPath:fileString])
							[[NSFileManager defaultManager] removeItemAtPath:fileString error:nil];	
					}
					[conversations removeObjectForKey:cell.reuseIdentifier];
					self.datasource = [NSMutableArray arrayWithArray:[conversations allKeys]]; 
				}	
			}	

			[appdelegate.infoDict  setObject:conversations forKey:@"conversation"];
			[self removeConversation: cell.reuseIdentifier imid: imid];
			[appdelegate saveAccountInfo];
		}	
		[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationTop];
	}	
}

- (BOOL)removeConversation: (NSString*)aFileName imid: (NSString*)anImid{
	if ([aFileName length] <= 0) return NO;
	BOOL aRemoved = NO;
	NSString* aKey = nil;
	NSArray* aKeys = nil;
	NSString* theFileName = nil;
	NSArray* anObjects = nil;
	NSMutableArray* aHistories = nil;
	NSArray *aFilePaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *aDir = [aFilePaths objectAtIndex: 0];
	NSString *aPath = [aDir stringByAppendingPathComponent: @"Sessions.plist"];
	NSData *anXmlData = [[NSFileManager defaultManager] contentsAtPath: aPath];
	
	NSMutableDictionary* aConversations = (NSMutableDictionary *)[NSPropertyListSerialization
																  propertyListFromData: anXmlData
																  mutabilityOption: kCFPropertyListMutableContainersAndLeaves
																  format: NULL errorDescription: nil];
	if(aConversations){
		if ([anImid length] > 0){
			aHistories = [[NSMutableArray alloc] initWithArray: [aConversations objectForKey: anImid]];
			[aHistories removeObject: aFileName];
			if ([aHistories count] > 0){
				[aConversations setObject: aHistories forKey: anImid];
			}
			else {
				[aConversations removeObjectForKey: anImid];
			}
			[aHistories release];
			aRemoved = YES;
		}
		else {
			aKeys = [aConversations allKeys];
			for (aKey in aKeys){
				anObjects = [aConversations objectForKey: aKey];
				for (theFileName in anObjects){
					if ([aFileName isEqualToString: theFileName]){
						aHistories = [[NSMutableArray alloc] initWithArray: anObjects];
						[aHistories removeObject: theFileName];
						if ([aHistories count] > 0){
							[aConversations setObject: aHistories forKey: aKey];
						}
						else {
							[aConversations removeObjectForKey: aKey];
						}
						[aHistories release];
						aRemoved = YES;
						break;
					}
				}
			}
		}
		if (aRemoved){
			[aConversations writeToFile: aPath atomically:YES];
		}
	}
	return aRemoved;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)dealloc
{
//	MYLog(@"----------%@ dealloc------------------------",self);
	[imid release];
	[datasource release];
	[super dealloc];
}


- (void)viewDidLoad 
{
	self.navigationItem.rightBarButtonItem = self.editButtonItem;
	[super viewDidLoad];
}


- (void)viewWillAppear:(BOOL)animated 
{
	[super viewWillAppear:animated];
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSDictionary * dicts = [appdelegate.infoDict objectForKey:@"conversation"];
	if (dicts != nil)
	{
		if (imid)
		{
			NSArray *array =  [dicts objectForKey:imid];
			if (array != nil)
				self.datasource = [NSMutableArray arrayWithArray:array];		
		}
		else
		{
			self.datasource = [NSMutableArray arrayWithArray:[dicts allKeys]]; 
		}
	}
	if (datasource == nil)
		self.datasource = [NSMutableArray array];
	
	UITableView* view = (UITableView*)self.view;	
	[view reloadData];
}


- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
}


@end

