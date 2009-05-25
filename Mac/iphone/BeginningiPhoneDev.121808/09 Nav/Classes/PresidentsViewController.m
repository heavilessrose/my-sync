//
//  PresidentsViewController.m
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "PresidentsViewController.h"
#import "PresidentDetailController.h"
#import "President.h"
#import "NavAppDelegate.h"


@implementation PresidentsViewController
@synthesize list;
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
	NSString *path = [[NSBundle mainBundle] pathForResource:@"Presidents" ofType:@"plist"];
	
	NSData *data;
	NSKeyedUnarchiver *unarchiver;
	
	data = [[NSData alloc] initWithContentsOfFile:path];
	unarchiver = [[NSKeyedUnarchiver alloc] initForReadingWithData:data];
	
	NSMutableArray *array = [unarchiver decodeObjectForKey:@"Presidents"];
	self.list = array;
	[unarchiver finishDecoding];
	[unarchiver release];
	[data release];
	
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
	return [self.list count];
}
- (UITableViewCell *)tableView:(UITableView *)tableView 
		 cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *PresidentListCellIdentifier = @"PresidentListCellIdentifier";
	
	UITableViewCell *cell = [tableView 
							 dequeueReusableCellWithIdentifier:PresidentListCellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero 
									   reuseIdentifier:PresidentListCellIdentifier] autorelease];
	}
	NSUInteger row = [indexPath row];
	President *thePres = [self.list objectAtIndex:row];
	cell.text = thePres.name;
	return cell;
}
#pragma mark -
#pragma mark Table Delegate Methods
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	
	NSUInteger row = [indexPath row];
	President *prez = [self.list objectAtIndex:row];
	
	PresidentDetailController *childController = [[PresidentDetailController alloc] 
												  initWithStyle:UITableViewStyleGrouped];
	
	childController.title = prez.name;
	childController.president = prez;
	
	NavAppDelegate *delegate = [[UIApplication sharedApplication] delegate];
	UINavigationController *navController = [delegate navController];
	[navController pushViewController:childController animated:YES];
}


@end
