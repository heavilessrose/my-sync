//
//  DeleteMeController.m
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "DeleteMeController.h"

@implementation DeleteMeController
@synthesize list;
-(IBAction)toggleEdit:(id)sender {
	[self.tableView setEditing:!self.tableView.editing animated:YES];	
}
#pragma mark -
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Initialization code
	}
	return self;
}

- (void)viewDidLoad {
	NSString *path = [[NSBundle mainBundle] pathForResource:@"computers" ofType:@"plist"];
	NSMutableArray *array = [[NSMutableArray alloc] initWithContentsOfFile:path];
	self.list = array;
	
	UIBarButtonItem *editButton = [[[UIBarButtonItem alloc]
									initWithTitle:@"Delete"
									style:UIBarButtonItemStyleBordered
									target:self
									action:@selector(toggleEdit:)] autorelease];
	self.navigationItem.rightBarButtonItem = editButton;
	
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
	
	static NSString *DeleteMeCellIdentifier = @"DeleteMeCellIdentifier";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:DeleteMeCellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero 
									   reuseIdentifier:DeleteMeCellIdentifier] autorelease];
	}
	NSInteger row = [indexPath row];
	cell.text = [self.list objectAtIndex:row];
	return cell;
}

#pragma mark -
#pragma mark Table Delegate Methods
- (void)tableView:(UITableView *)tableView 
commitEditingStyle:(UITableViewCellEditingStyle)editingStyle 
forRowAtIndexPath:(NSIndexPath *)indexPath {
	
	NSUInteger row = [indexPath row];
	[self.list removeObjectAtIndex:row];
	[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] 
					 withRowAnimation:UITableViewRowAnimationFade];
}

@end
