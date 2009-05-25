//
//  CheckListController.m
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "CheckListController.h"


@implementation CheckListController
@synthesize list;
@synthesize lastIndexPath;


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Initialization code
	}
	return self;
}

/*
 Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView {
}
 */


- (void)viewDidLoad {
	NSArray *array = [[NSArray alloc] initWithObjects:@"Who Hash",  @"Bubba Gump Shrimp Étouffée", @"Who Pudding", @"Scooby Snacks", @"Everlasting Gobstopper", @"Green Eggs and Ham", @"Soylent Green", @"Hard Tack", @"Lembas Bread",  @"Roast Beast", @"Blancmange", nil];
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
	[lastIndexPath release];
	[super dealloc];
}
#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}
- (NSInteger)tableView:(UITableView *)tableView 
 numberOfRowsInSection:(NSInteger)section {
	return 10;
}
- (UITableViewCell *)tableView:(UITableView *)tableView 
		 cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *CheckMarkCellIdentifier = @"CheckMarkCellIdentifier";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CheckMarkCellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero 
									   reuseIdentifier:CheckMarkCellIdentifier] autorelease];
	}
	NSUInteger row = [indexPath row];
	NSUInteger oldRow = [lastIndexPath row];
	cell.text = [list objectAtIndex:row];
	cell.accessoryType = (row == oldRow && lastIndexPath != nil) ? 
	UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;

	return cell;
	
}

#pragma mark -
#pragma mark Table Delegate Methods
- (void)tableView:(UITableView *)tableView 
didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	
	int newRow = [indexPath row];
	int oldRow = [lastIndexPath row];
	
	if (newRow != oldRow)
	{
		UITableViewCell *newCell = [tableView cellForRowAtIndexPath:indexPath];
		newCell.accessoryType = UITableViewCellAccessoryCheckmark;
		
		UITableViewCell *oldCell = [tableView cellForRowAtIndexPath: lastIndexPath]; 
		oldCell.accessoryType = UITableViewCellAccessoryNone;
		
		lastIndexPath = indexPath;	
	}
	
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

@end
