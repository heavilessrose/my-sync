//
//  AccountViewDatasource.m
//  MSN
//
//  Created by  apple on 08-11-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "AccountViewDatasource.h"
#import "define.h"

NSString *DeleteAccountNotification = @"DeleteAccount";
@implementation AccountViewDatasource
@synthesize source, selectedAccount;

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		self.source = [NSMutableDictionary dictionaryWithDictionary: [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey]];
	}
	return self;
}

- (void) dealloc
{
	[selectedAccount release];
	[source release];
	[super dealloc];
}

/////datasource method
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	// Number of sections is the number of region dictionaries
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [source count];
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return nil;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle
											forRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSString *key = [[source allKeys] objectAtIndex:indexPath.row];
	[source removeObjectForKey:key];
	[[NSUserDefaults standardUserDefaults] setObject:source forKey:kAccountsKey];
	[tableView reloadData];
	NSString *lastAccount = [[NSUserDefaults standardUserDefaults] objectForKey:kLastAccountKey];
	if ([key compare:lastAccount] == NSOrderedSame)
	{
		if ([source count] > 0){
			[[NSUserDefaults standardUserDefaults] setObject:[[source allKeys] objectAtIndex:0] forKey:kLastAccountKey];
        }
		else
			[[NSUserDefaults standardUserDefaults] removeObjectForKey:kLastAccountKey];
	}
	NSDictionary *dict = [NSDictionary dictionaryWithObject:indexPath forKey:@"indexPath"];
	[[NSNotificationCenter defaultCenter] postNotificationName:DeleteAccountNotification object:self userInfo:dict];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{	
	UITableViewCell *cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:@""] autorelease];
	cell.text = [[source allKeys] objectAtIndex:indexPath.row];
	
	if (selectedAccount != nil)
	{
		if ([cell.text  compare: selectedAccount] == NSOrderedSame)
			cell.accessoryType = UITableViewCellAccessoryCheckmark;
		else
			cell.accessoryType = UITableViewCellAccessoryNone;		
	}

	return cell;
}



@end
