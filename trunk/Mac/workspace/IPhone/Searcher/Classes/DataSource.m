//
//  DataSource.m
//  Searcher
//
//  Created by luke on 10-9-20.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "DataSource.h"
#import "Product.h"
#import "SearcherViewController.h"


@implementation DataSource

@synthesize listContent, tableDelegate, filteredListContent, savedSearchTerm, savedScopeButtonIndex, searchWasActive;

- (id)init
{
	if (self = [super init]) {
		self.listContent = [[NSArray alloc] initWithObjects:
								[Product productWithType:@"Device" name:@"iPhone"],
								[Product productWithType:@"Device" name:@"iPod"],
								[Product productWithType:@"Device" name:@"iPod touch"],
								[Product productWithType:@"Desktop" name:@"iMac"],
								[Product productWithType:@"Desktop" name:@"Mac Pro"],
								[Product productWithType:@"Portable" name:@"iBook"],
								[Product productWithType:@"Portable" name:@"MacBook"],
								[Product productWithType:@"Portable" name:@"MacBook Pro"],
								[Product productWithType:@"Portable" name:@"PowerBook"], nil];
		
		self.filteredListContent = [NSMutableArray arrayWithCapacity:[self.listContent count]];
		return self;
	}
	return nil;
}

#pragma mark -
#pragma mark result table view Delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSLog(@"result table row: %d", indexPath.row);
	UIViewController *detailViewController = [[UIViewController alloc] init];
	Product *aProduct = [self.filteredListContent objectAtIndex:indexPath.row];
	detailViewController.title = aProduct.name;
	[self.tableDelegate.navigationController pushViewController:detailViewController animated:YES];
	[detailViewController release];
}


#pragma mark -
#pragma mark table/result_table DataSource


- (NSInteger)tableView:(UITableView *)table numberOfRowsInSection:(NSInteger)section
{
	if (table == self.tableDelegate.searchDisplayController.searchResultsTableView) {
        return [self.filteredListContent count];
    } else {
        return [self.listContent count];
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	static NSString *kCellID = @"cellID";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kCellID];
	if (cell == nil) {
		NSLog(@"create new cell");
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kCellID] autorelease];
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
	}
	
	/*
	 If the requesting table view is the search display controller's table view, configure the cell using the filtered content, otherwise use the main list.
	 */
	Product *product = nil;
	if (tableView == self.tableDelegate.searchDisplayController.searchResultsTableView) {
        product = [self.filteredListContent objectAtIndex:indexPath.row];
    } else {
        product = [self.listContent objectAtIndex:indexPath.row];
    }
	
	cell.textLabel.text = product.name;
	return cell;
}

#pragma mark -
#pragma mark Content Filtering

- (void)filterContentForSearchText:(NSString*)searchText scope:(NSString*)scope
{
	/*
	 Update the filtered array based on the search text and scope.
	 */
	
	[self.filteredListContent removeAllObjects]; // First clear the filtered array.
	
	/*
	 Search the main list for products whose type matches the scope (if selected) and whose name matches searchText; add items that match to the filtered array.
	 */
	for (Product *product in listContent) {
		if ([scope isEqualToString:@"All"] || [product.type isEqualToString:scope]) {
			NSComparisonResult result = [product.name compare:searchText options:(NSCaseInsensitiveSearch|NSDiacriticInsensitiveSearch) range:NSMakeRange(0, [searchText length])];
            if (result == NSOrderedSame) {
				[self.filteredListContent addObject:product];
            }
		}
	}
}


#pragma mark -
#pragma mark UISearchDisplayController Delegate Methods

- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchString:(NSString *)searchString
{
    [self filterContentForSearchText:searchString scope:
	 [[self.tableDelegate.searchDisplayController.searchBar scopeButtonTitles] objectAtIndex:[self.tableDelegate.searchDisplayController.searchBar selectedScopeButtonIndex]]];
    
	NSLog(@"%x", self.tableDelegate);
    // Return YES to cause the search result table view to be reloaded.
    return YES;
}


- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchScope:(NSInteger)searchOption
{
    [self filterContentForSearchText:[self.tableDelegate.searchDisplayController.searchBar text] scope:
	 [[self.tableDelegate.searchDisplayController.searchBar scopeButtonTitles] objectAtIndex:searchOption]];
    
    // Return YES to cause the search result table view to be reloaded.
    return YES;
}

#pragma mark -
#pragma mark life control

- (void)dealloc
{
	[listContent release];
	[filteredListContent release];
	
	[super dealloc];
}


@end
