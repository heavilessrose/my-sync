//
//  DisclosureButtonController.m
//  FileBrowser
//
//  Created by wang luke on 6/12/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "DisclosureButtonController.h"
#import "FileBrowserAppDelegate.h"
#import "DisclosureDetailController.h"

@implementation DisclosureButtonController
@synthesize list;

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

- (void)viewDidLoad{
	NSArray *array = [[NSArray alloc] initWithObjects:@"Toy story", @"A bug`s life", @"Monsters inc.", nil];
	self.list = array;
	[array release];
	[super viewDidLoad];
}

- (void)dealloc {
	[list release];
	[childController release];
    [super dealloc];
}

#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
	return [list count];
}

// Asks the data source for a cell to insert in a particular location of the table view.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
	static NSString *DisclosureButtonCellIdentifier = @"DisclosureButtonCellIdentifier";
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:DisclosureButtonCellIdentifier];
	if(cell == nil){
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:DisclosureButtonCellIdentifier] autorelease];
	}
	NSUInteger row = [indexPath row];
	NSString *rowString = [list objectAtIndex:row];
	cell.text = rowString;
	[rowString release];
	return cell;
}

#pragma mark -
#pragma mark Table Delegate Methods
// Asks the delegate for the type of standard accessory view to use as a disclosure control for the specified row. 
// 返回该如何显示 列表每行右边的提示图标（是否有展开视图等）
- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath{
	return UITableViewCellAccessoryDetailDisclosureButton;
}

// Tells the delegate that the specified row is now selected.
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:
													@"Hey, do you see the disclosure button?"
													message:@"If you are trying to drill down, touch that instead" 
												   delegate:nil 
										  cancelButtonTitle:@"Won't happen again" 
										  otherButtonTitles:nil];
	[alert show];
	[alert release];
}

// Tells the delegate that the user tapped the accessory (disclosure) view associated with a given row. 
- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath{
	if(childController == nil){
		childController = [[DisclosureDetailController alloc] initWithNibName:@"DisclosureDetail" bundle:nil];
	}
	
	//childController.title = @"Disclosure button pressed";
	NSUInteger row = [indexPath row];
	
	// 子视图与父视图消息传递
	NSString *selectedMovie = [list objectAtIndex:row];
	NSString *detailMessage = [[NSString alloc] initWithFormat:@"You pressed the disclosure button for %@.", selectedMovie];
	childController.message = detailMessage;
	childController.title = selectedMovie;
	[detailMessage release];
	FileBrowserAppDelegate *delegate = [[UIApplication sharedApplication] delegate];
	[delegate.navController pushViewController:childController animated:YES];
}
@end
