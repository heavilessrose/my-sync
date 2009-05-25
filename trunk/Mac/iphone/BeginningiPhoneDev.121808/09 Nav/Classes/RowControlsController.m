//
//  RowControlsController.m
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "RowControlsController.h"


@implementation RowControlsController
@synthesize list;
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Initialization code
	}
	return self;
}

- (void)viewDidLoad {
	NSArray *array = [[NSArray alloc] initWithObjects:@"R2-D2", @"C3PO", @"Tik-Tok", @"Robby", @"Rosie", @"Uniblab", @"Bender", @"Marvin", @"Lt. Commander Data", @"Evil Brother Lore", @"Optimus Prime", @"Tobor", @"HAL", @"Orgasmatron", nil];
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
	[list release];
	[super dealloc];
}
#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)tableView:(UITableView *)tableView 
 numberOfRowsInSection:(NSInteger)section {
	return [list count];
}
- (UITableViewCell *)tableView:(UITableView *)tableView 
		 cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *ControlRowIdentifier = @"ControlRowIdentifier";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:ControlRowIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero 
									   reuseIdentifier:ControlRowIdentifier] autorelease];
		UISwitch *switchView = [[UISwitch alloc] init];
		switchView.tag = kSwitchTag;
		cell.accessoryView = switchView;
		[switchView release];
	}
	//////////////////////////////////////////////////////////////////
	// NOTE: In this example, we are not keeping track of the state
	//		 of the switch. As a result, if our list were longer, we
	//       would lose because the table view would dequeue the cell
	//		 with the switch in it. This is okay when you know your
	//		 table view's list will be small, but generally, you'll
	//		 want to keep track of any state in your application's
	//		 data model and shouldn't rely on table view cells to
	//		 hold onto values for you. For simplicity's sake, this
	//		 example doesn't have a data model, but in the last 
	//		 subcontroller, you'll see how to store the data rather
	//		 than relying on the table view cell, which could get
	//		 dequeued.
	//////////////////////////////////////////////////////////////////

	NSUInteger row = [indexPath row];
	NSString *rowTitle = [list objectAtIndex:row];
	cell.text = rowTitle;
	[rowTitle release];
	
	return cell;
} 
#pragma mark -
#pragma mark Table Delegate Methods
- (void)tableView:(UITableView *)tableView 
didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSUInteger row = [indexPath row];
	UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
	UISwitch *switchView = (UISwitch *)[cell viewWithTag:kSwitchTag];
	
	NSString *baseString = @"%@ %@.";
	NSString *onString = (switchView.on) ? @"IS on" : @"IS NOT on";
	NSString *robot = [list objectAtIndex:row];
	NSString *messageString = [[NSString alloc] initWithFormat:baseString, robot, onString];
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Row Selected." 
													message:messageString 
												   delegate:nil 
										  cancelButtonTitle:@"Thanks!" 
										  otherButtonTitles:nil];
	[alert show];
	[alert release];
	[messageString release];
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
} 
@end
