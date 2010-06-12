//
//  RootViewController.m
//  SQLTools
//
//  Created by luke on 5/17/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "RootViewController.h"
#import "LKSqlite.h"

@implementation RootViewController
@synthesize sqlite;

- (void)viewDidLoad {
    [super viewDidLoad];

    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    
    sqlite = [[LKSqlite alloc] init];
    [sqlite prepareWithDbfile:@"picadb.sqlite" table:@"phonebook" delegate:self];
    
    NSMutableDictionary *record = [[NSMutableDictionary alloc] init];
    [record setObject:@"0120" forKey:@"ContactID"];
    [record setObject:@"'15965986632'" forKey:@"MainPhone"];
    [record setObject:@"'fuck@test.com'" forKey:@"Email"];
    [sqlite addRecord:(NSDictionary *)record];
    
    NSMutableDictionary *record2 = [[NSMutableDictionary alloc] init];
    [record2 setObject:@"0123" forKey:@"ContactID"];
    [record2 setObject:@"'15965986632'" forKey:@"MainPhone"];
    [record2 setObject:@"'fuck@test.com'" forKey:@"Email"];
    [sqlite addRecord:(NSDictionary *)record2];
    
    NSMutableDictionary *record3 = [[NSMutableDictionary alloc] init];
    [record3 setObject:@"0456" forKey:@"ContactID"];
    [record3 setObject:@"'15965986632'" forKey:@"MainPhone"];
    [record3 setObject:@"'fuck@test.com'" forKey:@"Email"];
    [sqlite addRecord:(NSDictionary *)record3];
    
//    NSMutableDictionary *record2 = [[NSMutableDictionary alloc] init];
//    [record2 setObject:@"0456" forKey:@"ContactID"];
//    [record2 setObject:@"'15965986666'" forKey:@"MainPhone"];
//    [record2 setObject:@"'shit@test.com'" forKey:@"Email"];
//    [record2 setObject:@"2" forKey:@"ROWID"];
//    [sqlite updateRecord:(NSDictionary *)record2 where:@"ROWID"];
    
    //[sqlite deleteRecord:@"ContactID=0456"];
    
    NSMutableArray *selects = [[NSMutableArray alloc] init];
    NSArray *wantColumns = [NSArray arrayWithObjects:@"ROWID", @"ContactID", @"MainPhone", @"Email", nil];
    NSArray *colTypes = [NSArray arrayWithObjects:LKSqlite_INT, LKSqlite_INT, LKSqlite_TEXT, LKSqlite_TEXT, nil];
    [sqlite selectRecords:selects wantColumns:wantColumns types:colTypes wheres:nil];
    for (int i = 0; i < [selects count]; i++) {
        NSDictionary *dict = (NSDictionary *)[selects objectAtIndex:i];
        for(NSString *keystr in dict){
            NSLog(@"label: %@, result: %@", keystr, [dict objectForKey:keystr]);
        }
    }
    [selects release];
}

- (void)dbInitCallback
{
    [sqlite createTable:"CREATE TABLE IF NOT EXISTS main.phonebook (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, ContactID INTEGER UNIQUE NOT NULL DEFAULT 1, ShowName TEXT NOT NULL DEFAULT NoName, MainPhone TEXT, MobilePhone TEXT, HomePhone TEXT, HomeFax TEXT, WorkPhone TEXT, WorkFax TEXT, OtherPhone TEXT, Pager TEXT, Email TEXT, Avatar BLOB)"];
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
	// Return YES for supported orientations.
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
 */

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release anything that can be recreated in viewDidLoad or on demand.
	// e.g. self.myOutlet = nil;
}


#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 0;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	// Configure the cell.

    return cell;
}



/*
// Override to support row selection in the table view.
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {

    // Navigation logic may go here -- for example, create and push another view controller.
	// AnotherViewController *anotherViewController = [[AnotherViewController alloc] initWithNibName:@"AnotherView" bundle:nil];
	// [self.navigationController pushViewController:anotherViewController animated:YES];
	// [anotherViewController release];
}
*/


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
        // Delete the row from the data source.
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view.
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

