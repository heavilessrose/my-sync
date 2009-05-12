//
//  SimpleTableViewController.m
//  SimpleTable
//
//  Created by wang luke on 5/11/09.
//  Copyright luke 2009. All rights reserved.
//

#import "SimpleTableViewController.h"

@implementation SimpleTableViewController

@synthesize listData;

#pragma mark -
#pragma mark controller Methods

- (void) viewDidLoad 
{
	NSArray *array = [[NSArray alloc] initWithObjects:@"a",@"bb",@"c",@"ddd",@"eee",@"ff",@"g",@"h", nil];
	self.listData = array;
	[array release];
	[super viewDidLoad];
}

// Override to allow orientations other than the default portrait orientation.
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
#pragma mark Data Source Methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	
}

@end
