//
//  MoveMeController.m
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "MoveMeController.h"


@implementation MoveMeController
@synthesize list;
-(IBAction)toggleMove:(id)sender
{
	[self.tableView setEditing:!self.tableView.editing animated:YES];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Initialization code
	}
	return self;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}

- (void)viewDidLoad {
	NSMutableArray *array = [[NSMutableArray alloc] initWithObjects:@"Eeny", @"Meeny", 
							 @"Miney", @"Moe", @"Catch", @"A", @"Tiger", @"By", @"The", @"Toe", nil];
	self.list = array;
	[array release];
	
	UIBarButtonItem *moveButton = [[UIBarButtonItem alloc]
								   initWithTitle:@"Move"
								   style:UIBarButtonItemStyleBordered
								   target:self
								   action:@selector(toggleMove:)];
	self.navigationItem.rightBarButtonItem = moveButton;
	[moveButton release];
	[super viewDidLoad];
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
	
	static NSString *MoveMeCellIdentifier = @"MoveMeCellIdentifier";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:MoveMeCellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero
									   reuseIdentifier:MoveMeCellIdentifier] autorelease];
		cell.showsReorderControl = YES;
		
	}
	NSUInteger row = [indexPath row];
	cell.text = [list objectAtIndex:row];
	
	return cell;
}
- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView 
		   editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath {
	return UITableViewCellEditingStyleNone;
}
- (BOOL)tableView:(UITableView *)tableView 
	canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
	return YES;
}
- (void)tableView:(UITableView *)tableView 
moveRowAtIndexPath:(NSIndexPath *)fromIndexPath 
	  toIndexPath:(NSIndexPath *)toIndexPath {
	NSUInteger fromRow = [fromIndexPath row];
	NSUInteger toRow = [toIndexPath row];
	
	id object = [[list objectAtIndex:fromRow] retain];
	[list removeObjectAtIndex:fromRow];
	[list insertObject:object atIndex:toRow];
	[object release];
}
@end
