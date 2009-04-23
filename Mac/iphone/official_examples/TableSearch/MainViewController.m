/*

File: MainViewController.m
Abstract: The application's main view controller.

Version: 1.1

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import "AppDelegate.h"
#import "MainViewController.h"

@implementation MainViewController

@synthesize listContent, filteredListContent, savedContent, myTableView, mySearchBar;

- (void)dealloc
{
    [myTableView release];
	[mySearchBar release];
	
	[listContent release];
	[filteredListContent release];
	[savedContent release];
	
	[super dealloc];
}

- (void)awakeFromNib
{	
	// create the master list
	listContent = [[NSArray alloc] initWithObjects:	@"iPhone", @"iPod", @"iPod Touch", @"iMac", @"iBook",
													@"MacBook", @"MacBook Pro", @"Mac Pro", @"PowerBook", nil];
	
	// create our filtered list that will be the data source of our table, start its content from the master "listContent"
	filteredListContent = [[NSMutableArray alloc] initWithCapacity: [listContent count]];
	[filteredListContent addObjectsFromArray: listContent];
	
	// this stored the current list in case the user cancels the filtering
	savedContent = [[NSMutableArray alloc] initWithCapacity: [listContent count]]; 
					
	// don't get in the way of user typing
	mySearchBar.autocorrectionType = UITextAutocorrectionTypeNo;
	mySearchBar.autocapitalizationType = UITextAutocapitalizationTypeNone;
	mySearchBar.showsCancelButton = NO;
}


#pragma mark UIViewController

- (void)viewWillAppear:(BOOL)animated
{
	NSIndexPath *tableSelection = [myTableView indexPathForSelectedRow];
	[myTableView deselectRowAtIndexPath:tableSelection animated:NO];
}


#pragma mark UITableViewDataSource

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [filteredListContent count];
}

- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath
{
	return UITableViewCellAccessoryNone;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cellID"];
	if (cell == nil)
	{
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:@"cellID"] autorelease];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
	}
	
	cell.text = [filteredListContent objectAtIndex:indexPath.row];
	
	return cell;
}


#pragma mark UISearchBarDelegate

- (void)searchBarTextDidBeginEditing:(UISearchBar *)searchBar
{
	// only show the status bar's cancel button while in edit mode
	mySearchBar.showsCancelButton = YES;
	
	// flush and save the current list content in case the user cancels the search later
	[savedContent removeAllObjects];
	[savedContent addObjectsFromArray: filteredListContent];
}

- (void)searchBarTextDidEndEditing:(UISearchBar *)searchBar
{
	mySearchBar.showsCancelButton = NO;
}

- (void)searchBar:(UISearchBar *)searchBar textDidChange:(NSString *)searchText
{
	[filteredListContent removeAllObjects];	// clear the filtered array first
	
	// search the table content for cell titles that match "searchText"
	// if found add to the mutable array and force the table to reload
	//
	NSString *cellTitle;
	for (cellTitle in listContent)
	{
		NSComparisonResult result = [cellTitle compare:searchText options:NSCaseInsensitiveSearch
										range:NSMakeRange(0, [searchText length])];
		if (result == NSOrderedSame)
		{
			[filteredListContent addObject:cellTitle];
		}
	}
	
	[myTableView reloadData];
}

// called when cancel button pressed
- (void)searchBarCancelButtonClicked:(UISearchBar *)searchBar
{
	// if a valid search was entered but the user wanted to cancel, bring back the saved list content
	if (searchBar.text.length > 0)
	{
		[filteredListContent removeAllObjects];
		[filteredListContent addObjectsFromArray: savedContent];
	}
	
	[myTableView reloadData];
	
	[searchBar resignFirstResponder];
	searchBar.text = @"";
}

// called when Search (in our case "Done") button pressed
- (void)searchBarSearchButtonClicked:(UISearchBar *)searchBar
{
	[searchBar resignFirstResponder];
}

@end

