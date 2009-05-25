//
//  RootViewController.m
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "RootViewController.h"
#import "SecondLevelViewController.h"
#import "NavAppDelegate.h"
#import "DisclosureButtonController.h"
#import "CheckListController.h"
#import "RowControlsController.h"
#import "MoveMeController.h"
#import "DeleteMeController.h"
#import "PresidentsViewController.h"

@implementation RootViewController
@synthesize controllers;

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
	self.title = @"Root Level";
	NSMutableArray *array = [[NSMutableArray alloc] init];
	
	// Disclosure Button
	DisclosureButtonController *disclosureButtonController = [[DisclosureButtonController alloc] 
															  initWithStyle:UITableViewStylePlain];
	disclosureButtonController.title = @"Disclosure Buttons";
	disclosureButtonController.rowImage = [UIImage imageNamed:@"disclosureButtonControllerIcon.png"];
	[array addObject:disclosureButtonController];
	[disclosureButtonController release];
	
	// Check List 
	CheckListController *checkListController = [[CheckListController alloc] 
												initWithStyle:UITableViewStylePlain];
	checkListController.title = @"Check One";
	checkListController.rowImage = [UIImage imageNamed:@"checkmarkControllerIcon.png"];
	[array addObject:checkListController];
	[checkListController release];
	
	// Table Row Controls
	RowControlsController *rowControlsController = [[RowControlsController alloc] 
													initWithStyle:UITableViewStylePlain];
	rowControlsController.title = @"Row Controls";
	rowControlsController.rowImage = [UIImage imageNamed:@"rowControlsIcon.png"];
	[array addObject:rowControlsController];
	[rowControlsController release];
	
	// Move Me
	MoveMeController *moveMeController = [[MoveMeController alloc]
										  initWithStyle:UITableViewStylePlain];
	moveMeController.title = @"Move Me";
	moveMeController.rowImage = [UIImage imageNamed:@"moveMeIcon.png"];
	[array addObject:moveMeController];
	[moveMeController release];
	
	
	// Delete Me
	DeleteMeController *deleteMeController = [[DeleteMeController alloc] 
											  initWithStyle:UITableViewStylePlain];
	deleteMeController.title = @"Delete Me";
	deleteMeController.rowImage = [UIImage imageNamed:@"deleteMeIcon.png"];
	[array addObject:deleteMeController];
	[deleteMeController release];
	
	// President View/Edit
	PresidentsViewController *presidentsViewController = [[PresidentsViewController alloc] 
														  initWithStyle:UITableViewStylePlain];
	presidentsViewController.title = @"Detail Edit";
	presidentsViewController.rowImage = [UIImage imageNamed:@"detailEditIcon.png"];
	[array addObject:presidentsViewController];
	[presidentsViewController release];
	
	
	self.controllers = array;
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
	[controllers release];
	[super dealloc];
}

#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [self.controllers count];
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *TopLevelCellIdentifier = @"TopLevelCellIdentifier";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:TopLevelCellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:TopLevelCellIdentifier] autorelease];
	}
	// Configure the cell
	NSUInteger row = [indexPath row];
	SecondLevelViewController *controller = [controllers objectAtIndex:row];
	cell.text = controller.title;
	cell.image = controller.rowImage;
	return cell;
}

#pragma mark -
#pragma mark Table View Delegate Methods
- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath
{
	return UITableViewCellAccessoryDisclosureIndicator;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NSUInteger row = [indexPath row];
	SecondLevelViewController *nextController = [self.controllers objectAtIndex:row];
	
	NavAppDelegate *delegate = [[UIApplication sharedApplication] delegate];
	[delegate.navController pushViewController:nextController animated:YES];
}
@end
