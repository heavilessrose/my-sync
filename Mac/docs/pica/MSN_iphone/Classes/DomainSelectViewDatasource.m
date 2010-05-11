//
//  DomainSelectViewDatasource.m
//  MSN
//
//  Created by Jun Du on 7/30/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "DomainSelectViewDatasource.h"


@implementation DomainSelectViewDatasource
@synthesize selectedDomain;

- (void) dealloc
{
	[selectedDomain release];
	[super dealloc];
}

#pragma mark -
#pragma mark datasource methods
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	// Number of sections is the number of region dictionaries
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
#ifndef FOR_PAYMENT_APP
	return 5;
#else
    return 4;
#endif
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return nil;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{	
	UITableViewCell *cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:@""] autorelease];
	switch (indexPath.row)
	{
		case 0:
			cell.text = @"@hotmail.com";
			break;
		case 1:
			cell.text = @"@msn.com";
			break;
		case 2:
			cell.text = @"@live.com";
			break;
#ifndef FOR_PAYMENT_APP
		case 3:
			cell.text = @"@live.hk";
			break;
		case 4:
			cell.text = NSLocalizedString(@"qtn_msn_account_other",nil);
			break;
#else
		case 3:
			cell.text = NSLocalizedString(@"qtn_msn_account_other",nil);
			break;
#endif
		default:
			break;
	}
	
	if ([cell.text  compare: selectedDomain] == NSOrderedSame)
		cell.accessoryType = UITableViewCellAccessoryCheckmark;
	else
		cell.accessoryType = UITableViewCellAccessoryNone;
	return cell;
}

@end
