//
//  RootViewController.m
//  FileBrowser
//
//  Created by wang luke on 6/12/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "RootViewController.h"
#import "SecondLevelViewController.h"
#import "FileBrowserAppDelegate.h"
#import "DisclosureButtonController.h"

@implementation RootViewController
@synthesize controllers;

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
	self.title = @"Root Level";
	NSMutableArray *array = [[NSMutableArray alloc] init];
	
	// Disclosure button
	DisclosureButtonController *disclosureButtonController = [[DisclosureButtonController alloc] initWithStyle:UITableViewStylePlain];
	disclosureButtonController.title = @"Disclosure Buttons";
	disclosureButtonController.rowImage = [UIImage imageNamed:@"disclosureButtonControllerIcon.png"];
	[array addObject:disclosureButtonController];
	[disclosureButtonController release];
	
	self.controllers = array;
	[array release];
	[super viewDidLoad];
}

- (void)dealloc {
	[controllers release];
    [super dealloc];
}

#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
	return [self.controllers count];
}

// Asks the data source for a cell to insert in a particular location of the table view.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
	static NSString *RootViewControllerCell = @"RootViewControllerCell";
	
	// Returns a reusable table-view cell object located by its identifier.
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:RootViewControllerCell];
	
	if(cell == nil){
		// Initializes and returns a table-view cell object.
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:RootViewControllerCell] autorelease];
	}
	
	// configure the cell
	NSUInteger row = [indexPath row];
	SecondLevelViewController *controller = [controllers objectAtIndex:row];
	cell.text = controller.title;
	cell.image = controller.rowImage;
	return cell;
}

#pragma mark -
#pragma mark Table View Delegate Methods
// Asks the delegate for the type of standard accessory view to use as a disclosure control for the specified row.
- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath{
	return UITableViewCellAccessoryDisclosureIndicator;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	NSUInteger row = [indexPath row];
	SecondLevelViewController *nextController = [self.controllers objectAtIndex:row];
	
	// 应用程序委托拥有一个指向应用程序导航控制器的输出口，所以可以通过应用程序委托来访问导航控制器
	// 让导航控制器将选中行的相应视图控制器压桟
	FileBrowserAppDelegate *delegate = [[UIApplication sharedApplication] delegate];
	[delegate.navController pushViewController:nextController animated:YES];
}
@end
