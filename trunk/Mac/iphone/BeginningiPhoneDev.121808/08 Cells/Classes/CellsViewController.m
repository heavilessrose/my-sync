//
//  CellsAppDelegate.m
//  Cells
//
//  Created by Jeff LaMarche on 7/12/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "CellsViewController.h"
#import "CustomCell.h"

@implementation CellsViewController
@synthesize computers;
- (void)viewDidLoad {
	
	NSDictionary *row1 = [[NSDictionary alloc] initWithObjectsAndKeys:@"MacBook", @"Name", @"White", @"Color", nil];
	NSDictionary *row2 = [[NSDictionary alloc] initWithObjectsAndKeys:@"MacBook Pro", @"Name", @"Silver", @"Color", nil];
	NSDictionary *row3 = [[NSDictionary alloc] initWithObjectsAndKeys:@"iMac", @"Name", @"White", @"Color", nil];
	
	NSArray *array = [[NSArray alloc] initWithObjects:row1, row2, row3, nil];
	self.computers = array;
	
	[row1 release];
	[row2 release];
	[row3 release];
	[array release];
	
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
	[super dealloc];
}
#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	NSLog(@"Returning %d", [self.computers count]);
	return [self.computers count];
}
- (UITableViewCell *)tableView:(UITableView *)tableView 
		 cellForRowAtIndexPath:(NSIndexPath *)indexPath 
{
	
	static NSString *CustomCellIdentifier = @"CustomCellIdentifier ";
	
	CustomCell *cell = (CustomCell *)[tableView dequeueReusableCellWithIdentifier: CustomCellIdentifier];
	if (cell == nil)  
	{
		NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"CustomCellView" 
					owner:self options:nil];
		
		// The behavior here changed between SDK 2.0 and 2.1. In 2.1+, loadNibNamed:owner:options: does not
		// include an entry in the array for File's Owner. In 2.0, it does. This means that if you're on
		// 2.2 or 2.1, you have to grab the object at index 0, but if you're running against SDK 2.0, you
		// have to grab the object at index:0.
		
		// You might be tempted to use UIDevice to get the system version number, however that is the 
		// operating system version, not the version of the SDK that the application is linked against. An
		// iPhone operating running iPhone OS 2.2 can, for example, run an application linked against the
		// 2.0 vesion of the SDK
		
		// The way to handle this is to use what are called "Availability Macros". The pre-compiler macro
		// __IPHONE_2_1 will be defined for SDK 2.1 and later, but not on SDK 2.0
#ifdef __IPHONE_2_1
		cell = (CustomCell *)[nib objectAtIndex:0];
#else
		cell = (CustomCell *)[nib objectAtIndex:1];
#endif
		
	}
	NSUInteger row = [indexPath row];
	NSDictionary *rowData = [self.computers objectAtIndex:row];
	cell.colorLabel.text = [rowData objectForKey:@"Color"];
	cell.nameLabel.text = [rowData objectForKey:@"Name"];
	return cell;
}
#pragma mark -
#pragma mark Table View Delegate Methods
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	return 66;
}
@end
