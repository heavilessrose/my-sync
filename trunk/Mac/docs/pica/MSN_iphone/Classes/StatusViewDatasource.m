//
//  StatusViewDatasource.m
//  MSN
//
//  Created by  apple on 08-11-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "StatusViewDatasource.h"


@implementation StatusViewDatasource
@synthesize selectedStatus;

- (void) dealloc
{
	[selectedStatus release];
	[super dealloc];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
	return 4;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{	
	UITableViewCell *cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:@""] autorelease];
	switch (indexPath.row)
	{
		case 0:
			cell.text = NSLocalizedString(@"qtn_status_online",nil);
			break;
		case 1:
			cell.text = NSLocalizedString(@"qtn_status_busy",nil);
			break;
//		case 2:
//			cell.text = NSLocalizedString(@"qtn_status_berightback",nil);
//			break;
		case 2:
			cell.text = NSLocalizedString(@"qtn_status_away",nil);
			break;
//		case 4:
//			cell.text = NSLocalizedString(@"qtn_status_onthephone",nil);
//			break;
//		case 5:
//			cell.text = NSLocalizedString(@"qtn_status_outtolunch",nil);
//			break;
		case 3:
			cell.text = NSLocalizedString(@"qtn_status_appearoffline",nil);
			break;
	}
	cell.accessoryType = [cell.text isEqualToString: selectedStatus] ? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
	
	return cell;
}



@end
