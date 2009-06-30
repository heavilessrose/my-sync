//
//  SettingsViewController.m
//  TabiNavi
//
//  Created by wang luke on 6/30/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "SettingsViewController.h"

@implementation SettingsViewController
@synthesize textFieldCell;
@synthesize switchCell;
@synthesize TableData;

// 键盘Done事件处理
- (IBAction)fieldsDoneEditing:(id)sender{
	NSLog(@"fieldsDoneEditing");
	[sender resignFirstResponder];
}

// 点击背景关闭键盘
- (IBAction)backgroundClicked:(id)sender{
	NSLog(@"backgroundClicked");
	[textFieldCell resignFirstResponder];
}

- (void)viewDidLoad {
	self.title = @"Settings";
	NSArray *array = [[NSArray alloc] initWithObjects:@"Foo", @"Bar", @"Baz", nil];
	self.TableData = array;
	[super viewDidLoad];
	// Uncomment the following line to display an Edit button in the navigation bar for this view controller.
	self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

/*
 - (void)viewWillAppear:(BOOL)animated {
 [super viewWillAppear:animated];
 }
 */
/*
 - (void)viewDidAppear:(BOOL)animated {
 [super viewDidAppear:animated];
 }
 */
/*
 - (void)viewWillDisappear:(BOOL)animated {
 [super viewWillDisappear:animated];
 }
 */
/*
 - (void)viewDidDisappear:(BOOL)animated {
 [super viewDidDisappear:animated];
 }
 */

/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

#pragma mark -
#pragma mark Table view datasource methods

// tell the table view how many sections we want to display in our table.
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 3;
}


// tell the table view how many rows live in each section.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	switch (section) {
		case 0:
			return 1;
		case 1:
			return 1;
		case 2:
			return 3;
		default:
			return 0;
	}
}


// Customize the appearance of table view cells.
// The actual table view cell for a given index path.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
//    // 默认
//    static NSString *CellIdentifier = @"Cell";
//    
//    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
//    if (cell == nil) {
//        cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:CellIdentifier] autorelease];
//    }
//    
//    // Set up the cell...
//	
//    return cell;
	
	
	UITableViewCell *cell;
	static NSString *CellIdentifier = @"OtherCell";
	switch (indexPath.section) {
		case 0:
			CellIdentifier = @"TextFieldCell";
			cell = (TextFieldCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
			if (cell == nil) {
				NSArray *nib = [[NSBundle mainBundle] loadNibNamed:CellIdentifier owner:self options:nil];
				//cell = textFieldCell;
				cell = [nib objectAtIndex:0];
			}
			break;
		case 1:
			CellIdentifier = @"SwitchCell";
			cell = (SwitchCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
			if (cell == nil) {
				NSArray *nib = [[NSBundle mainBundle] loadNibNamed:CellIdentifier owner:self options:nil];
				//cell = switchCell;
				cell = [nib objectAtIndex:0];
			}
			break;
		default:
			CellIdentifier = @"OtherCell";
			cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
			if (cell == nil) {
				cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero
											   reuseIdentifier:CellIdentifier] autorelease];
				cell.accessoryType = UITableViewCellAccessoryNone;
			}
			// Set up the cell...
			cell.text = [TableData objectAtIndex:indexPath.row];
			break;
	}
//	// debug
//	if(cell == nil)
//		NSLog(@"cell %d is null", indexPath.section);
//	else
//		NSLog(@"cell %d is OK", indexPath.section);
	return cell;
}

// trigger some custom action for selecting a table view row.
// In most cases this would be pushing a new view controller onto the navigation stack for drilling down a table view.
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	// 默认
    // Navigation logic may go here. Create and push another view controller.
	// AnotherViewController *anotherViewController = [[AnotherViewController alloc] initWithNibName:@"AnotherView" bundle:nil];
	// [self.navigationController pushViewController:anotherViewController];
	// [anotherViewController release];
	
	// Retrieve the cell ...
	UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
	
	// For now we just take care of events for the third section of the table view
	if (indexPath.section == 2) {
		if (cell.accessoryType == UITableViewCellAccessoryNone) {
			cell.accessoryType = UITableViewCellAccessoryCheckmark;
		} else {
			cell.accessoryType = UITableViewCellAccessoryNone;
		}
	}
	
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

#pragma mark Table View methods
// 加标题
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section{
	
	switch (section) {
		case 0:
			return @"Your name";
		case 1:
			return @"Turn me on";
		case 2:
			return @"Choose your destiny";
		default:// 不会到达
			return nil;
	}
}

/*
 // Override to support conditional editing of the table view.
 - (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
 // Return NO if you do not want the specified item to be editable.
 return YES;
 }
 */


/*
 // Override to support editing the table view.
 - (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
 
 if (editingStyle == UITableViewCellEditingStyleDelete) {
 // Delete the row from the data source
 [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
 }   
 else if (editingStyle == UITableViewCellEditingStyleInsert) {
 // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
 }   
 }
 */


/*
 // Override to support rearranging the table view.
 - (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
 }
 */


/*
 // Override to support conditional rearranging of the table view.
 - (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
 // Return NO if you do not want the item to be re-orderable.
 return YES;
 }
 */


- (void)dealloc {
    [super dealloc];
}

@end
