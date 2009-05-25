//
//  DisclosureButtonController.m
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "DisclosureButtonController.h"
#import "NavAppDelegate.h"
#import "DisclosureDetailController.h"

@implementation DisclosureButtonController
@synthesize list;
-(id)initWithStyle:(UITableViewStyle)style {
	if (self = [super initWithStyle:style]) {
		
	}
	return self;
}
- (void)viewDidLoad {

	NSArray *array = [[NSArray alloc] initWithObjects:@"Toy Story", @"A Bug's Life", @"Toy Story 2", @"Monsters, Inc.", @"Finding Nemo", @"The Incredibles", @"Cars", @"Ratatouille", @"WALL-E", @"Up", nil];
	self.list = array;
	[array release];
	[super viewDidLoad];
}
 


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc {
	[childController release];
	[super dealloc];
}

#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [list count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView 
		 cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *MyIdentifier = @"DisclosureButtonCellIdentifier";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:MyIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero 
									   reuseIdentifier:MyIdentifier] autorelease];
	}
	// Configure the cell
	NSUInteger row = [indexPath row];
	NSString *rowString = [list objectAtIndex:row];
	cell.text = rowString;
	[rowString release];
	return cell;
}

#pragma mark -
#pragma mark Table Delegate Methods
- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView 
		 accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath
{
	return UITableViewCellAccessoryDetailDisclosureButton;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Hey, do you see the disclosure button?" 
													message:@"If you're trying to drill down, touch that instead" 
												   delegate:nil 
										  cancelButtonTitle:@"Won't happen again" 
										  otherButtonTitles:nil];
	[alert show];
	[alert release];
	
}
- (void)tableView:(UITableView *)tableView 
accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath
{
	if (childController == nil)
		childController = [[DisclosureDetailController alloc] 
						   initWithNibName:@"DisclosureDetail" bundle:nil];
	
	childController.title = @"Disclosure Button Pressed";
	NSUInteger row = [indexPath row];
	
	NSString *selectedMovie = [list objectAtIndex:row];
	NSString *detailMessage  = [[NSString alloc] 
								initWithFormat:@"You pressed the disclosure button for %@.", selectedMovie];
	childController.message = detailMessage;
	childController.title = selectedMovie;
	[detailMessage release];
	NavAppDelegate *delegate = [[UIApplication sharedApplication] delegate];
	[delegate.navController pushViewController:childController animated:YES];
	
}
@end
