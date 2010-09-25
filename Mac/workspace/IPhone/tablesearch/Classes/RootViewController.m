//
//  RootViewController.m
//  TableView
//
//  Created by iPhone SDK Articles on 1/17/09.
//  Copyright www.iPhoneSDKArticles.com 2009. 
//

#import "RootViewController.h"
#import "TableViewAppDelegate.h"
#import "DetailViewController.h"
#import "OverlayViewController.h"

@implementation RootViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	
	//Initialize the array.
	listOfItems = [[NSMutableArray alloc] init];
	
	NSArray *countriesToLiveInArray = [NSArray arrayWithObjects:@"Iceland", @"Greenland", @"Switzerland", @"Norway", @"New Zealand", @"Greece", @"Italy", @"Ireland", nil];
	NSDictionary *countriesToLiveInDict = [NSDictionary dictionaryWithObject:countriesToLiveInArray forKey:@"Countries"];
	
	NSArray *countriesLivedInArray = [NSArray arrayWithObjects:@"India", @"U.S.A", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", @"Test", nil];
	NSDictionary *countriesLivedInDict = [NSDictionary dictionaryWithObject:countriesLivedInArray forKey:@"Countries"];
	
	[listOfItems addObject:countriesToLiveInDict];
	[listOfItems addObject:countriesLivedInDict];
	
	//Initialize the copy array.
	copyListOfItems = [[NSMutableArray alloc] init];
	
	//Set the title
	self.navigationItem.title = @"Countries";
	
	//Add the search bar
	self.tableView.tableHeaderView = searchBar;
	searchBar.autocorrectionType = UITextAutocorrectionTypeNo;
	
	searching = NO;
	letUserSelectRow = YES;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    
	if (searching)
		return 1;
	else
		return [listOfItems count];
}

// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	
	if (searching)
		return [copyListOfItems count];
	else {
		
		//Number of rows it should expect should be based on the section
		NSDictionary *dictionary = [listOfItems objectAtIndex:section];
		NSArray *array = [dictionary objectForKey:@"Countries"];
		return [array count];
	}
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	
	if(searching)
		return @"Search Results";
	
	if(section == 0)
		return @"Countries to visit";
	else
		return @"Countries visited";
}

- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView {
	
	if(searching)
		return nil;
	
	NSMutableArray *tempArray = [[NSMutableArray alloc] init];
	[tempArray addObject:@"1"];
	[tempArray addObject:@"2"];
	[tempArray addObject:@"3"];
	[tempArray addObject:@"4"];
	[tempArray addObject:@"5"];
	[tempArray addObject:@"6"];
	[tempArray addObject:@"7"];
	[tempArray addObject:@"8"];
	[tempArray addObject:@"9"];
	[tempArray addObject:@"10"];
	[tempArray addObject:@"11"];
	[tempArray addObject:@"12"];
	[tempArray addObject:@"13"];
	[tempArray addObject:@"14"];
	[tempArray addObject:@"15"];
	[tempArray addObject:@"16"];
	[tempArray addObject:@"17"];
	[tempArray addObject:@"18"];
	[tempArray addObject:@"19"];
	[tempArray addObject:@"20"];
	[tempArray addObject:@"21"];
	[tempArray addObject:@"22"];
	[tempArray addObject:@"23"];
	[tempArray addObject:@"24"];
	[tempArray addObject:@"25"];
	[tempArray addObject:@"26"];

	return tempArray;
}

- (NSInteger)tableView:(UITableView *)tableView sectionForSectionIndexTitle:(NSString *)title atIndex:(NSInteger)index {

	if(searching)
		return -1;
	
	return index % 2;
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	
	if(cell == nil)
		cell = [self getCellContentView:CellIdentifier];
	
	UILabel *lblTemp1 = (UILabel *)[cell viewWithTag:1];
	UILabel *lblTemp2 = (UILabel *)[cell viewWithTag:2];
	
	if(searching) {
		
		lblTemp1.text = [copyListOfItems objectAtIndex:indexPath.row];
		lblTemp2.text = @"";
	}
	else {
		
		//First get the dictionary object
		NSDictionary *dictionary = [listOfItems objectAtIndex:indexPath.section];
		NSArray *array = [dictionary objectForKey:@"Countries"];
		NSString *cellValue = [array objectAtIndex:indexPath.row];
		
		lblTemp1.text = cellValue;
		lblTemp2.text = @"Sub Value";
		
		[cellValue release];
	}
	
    return cell;
}

- (UITableViewCell *) getCellContentView:(NSString *)cellIdentifier {
	
	CGRect CellFrame = CGRectMake(0, 0, 300, 60);
	CGRect Label1Frame = CGRectMake(10, 10, 290, 25);
	CGRect Label2Frame = CGRectMake(10, 33, 290, 25);
	UILabel *lblTemp;
	
	UITableViewCell *cell = [[[UITableViewCell alloc] initWithFrame:CellFrame reuseIdentifier:cellIdentifier] autorelease];
	
	//Initialize Label with tag 1.
	lblTemp = [[UILabel alloc] initWithFrame:Label1Frame];
	lblTemp.tag = 1;
	[cell.contentView addSubview:lblTemp];
	[lblTemp release];
	
	//Initialize Label with tag 2.
	lblTemp = [[UILabel alloc] initWithFrame:Label2Frame];
	lblTemp.tag = 2;
	lblTemp.font = [UIFont boldSystemFontOfSize:12];
	lblTemp.textColor = [UIColor lightGrayColor];
	[cell.contentView addSubview:lblTemp];
	[lblTemp release];
	
	return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	return 60;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	
	//Get the selected country
	
	NSString *selectedCountry = nil;
	
	if(searching)
		selectedCountry = [copyListOfItems objectAtIndex:indexPath.row];
	else {
	
		NSDictionary *dictionary = [listOfItems objectAtIndex:indexPath.section];
		NSArray *array = [dictionary objectForKey:@"Countries"];
		selectedCountry = [array objectAtIndex:indexPath.row];
	}
	
	//Initialize the detail view controller and display it.
	DetailViewController *dvController = [[DetailViewController alloc] initWithNibName:@"DetailView" bundle:[NSBundle mainBundle]];
	dvController.selectedCountry = selectedCountry;
	[self.navigationController pushViewController:dvController animated:YES];
	[dvController release];
	dvController = nil;
}

- (NSIndexPath *)tableView :(UITableView *)theTableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	
	if(letUserSelectRow)
		return indexPath;
	else
		return nil;
}

- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath {
	
	//return UITableViewCellAccessoryDetailDisclosureButton;
	//return UITableViewCellAccessoryDisclosureIndicator;
	return UITableViewCellAccessoryNone;
}

- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath {
	
	[self tableView:tableView didSelectRowAtIndexPath:indexPath];
}

#pragma mark -
#pragma mark Search Bar 

- (void) searchBarTextDidBeginEditing:(UISearchBar *)theSearchBar {
	
	//This method is called again when the user clicks back from teh detail view.
	//So the overlay is displayed on the results, which is something we do not want to happen.
	if(searching)
		return;
	
	//Add the overlay view.
	if(ovController == nil)
		ovController = [[OverlayViewController alloc] initWithNibName:@"OverlayView" bundle:[NSBundle mainBundle]];
	
	CGFloat yaxis = self.navigationController.navigationBar.frame.size.height;
	CGFloat width = self.view.frame.size.width;
	CGFloat height = self.view.frame.size.height;
	
	//Parameters x = origion on x-axis, y = origon on y-axis.
	CGRect frame = CGRectMake(0, yaxis, width, height);
	ovController.view.frame = frame;	
	ovController.view.backgroundColor = [UIColor grayColor];
	ovController.view.alpha = 0.5;
	
	ovController.rvController = self;
	
	[self.tableView insertSubview:ovController.view aboveSubview:self.parentViewController.view];
	
	searching = YES;
	letUserSelectRow = NO;
	self.tableView.scrollEnabled = NO;
	
	//Add the done button.
	self.navigationItem.rightBarButtonItem = [[[UIBarButtonItem alloc] 
											   initWithBarButtonSystemItem:UIBarButtonSystemItemDone 
											   target:self action:@selector(doneSearching_Clicked:)] autorelease];
	
}

- (void)searchBar:(UISearchBar *)theSearchBar textDidChange:(NSString *)searchText {

	//Remove all objects first.
	[copyListOfItems removeAllObjects];
	
	if([searchText length] > 0) {
		
		[ovController.view removeFromSuperview];
		searching = YES;
		letUserSelectRow = YES;
		self.tableView.scrollEnabled = YES;
		[self searchTableView];
	}
	else {
		
		[self.tableView insertSubview:ovController.view aboveSubview:self.parentViewController.view];
		
		searching = NO;
		letUserSelectRow = NO;
		self.tableView.scrollEnabled = NO;
	}
	
	[self.tableView reloadData];
}

- (void) searchBarSearchButtonClicked:(UISearchBar *)theSearchBar {
	
	[self searchTableView];
}

- (void) searchTableView {
	
	NSString *searchText = searchBar.text;
	NSMutableArray *searchArray = [[NSMutableArray alloc] init];
	
	for (NSDictionary *dictionary in listOfItems)
	{
		NSArray *array = [dictionary objectForKey:@"Countries"];
		[searchArray addObjectsFromArray:array];
	}
	
	for (NSString *sTemp in searchArray)
	{
		NSRange titleResultsRange = [sTemp rangeOfString:searchText options:NSCaseInsensitiveSearch];
		
		if (titleResultsRange.length > 0)
			[copyListOfItems addObject:sTemp];
	}
	
	[searchArray release];
	searchArray = nil;
}

- (void) doneSearching_Clicked:(id)sender {
	
	searchBar.text = @"";
	[searchBar resignFirstResponder];
	
	letUserSelectRow = YES;
	searching = NO;
	self.navigationItem.rightBarButtonItem = nil;
	self.tableView.scrollEnabled = YES;
	
	[ovController.view removeFromSuperview];
	[ovController release];
	ovController = nil;
	
	[self.tableView reloadData];
}

- (void)dealloc {
	
	[ovController release];
	[copyListOfItems release];
	[searchBar release];
	[listOfItems release];
    [super dealloc];
}


@end

