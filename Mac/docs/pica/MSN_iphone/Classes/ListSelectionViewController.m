//
//  Created by wangwei on 8/16/08.
//  Copyright Beijing Mobile Security Technology Co., Ltd 2008. All rights reserved.
//

#import "ListSelectionViewController.h"
#import "MyUITextAndValueTableCell.h"

#import "Constants.h"

@implementation ListSelectionData

@synthesize data, title, selected;

- (void)dealloc {
    [data release];
    [title release];
    [super dealloc];
}

@end

@implementation ListSelectionViewController

@synthesize myTableView, data;

- (void)dealloc
{
	[data release];
	[myTableView release];
	[super dealloc];
}

- (void)viewDidLoad
{
    self.title = data.title;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [data.data count];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	if(indexPath.row == data.selected) return;
	
	MyUITextAndValueTableCell* cell = (MyUITextAndValueTableCell*)[tableView cellForRowAtIndexPath: indexPath];	
	[cell setChecked: YES];
	
	NSIndexPath* index = [NSIndexPath indexPathForRow:data.selected inSection:0];
	cell = (MyUITextAndValueTableCell*)[tableView cellForRowAtIndexPath: index];	
	[cell setChecked: NO];
	
	data.selected = indexPath.row;
	
	[self.navigationController popViewControllerAnimated: YES];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	MyUITextAndValueTableCell* cell = (MyUITextAndValueTableCell*)[tableView dequeueReusableCellWithIdentifier: @"sel"];
	if (cell == nil)
	{	
		cell = [[[MyUITextAndValueTableCell alloc] initWithFrame:CGRectZero reuseIdentifier: @"sel"] autorelease];
	}
	[cell setTitle: [data.data objectAtIndex: indexPath.row]];
	[cell setChecked: indexPath.row == data.selected];
	return cell;
}

@end
