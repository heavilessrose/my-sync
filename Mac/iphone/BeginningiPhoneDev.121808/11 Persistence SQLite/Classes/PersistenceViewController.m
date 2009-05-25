//
//  PersistenceAppDelegate.m
//  Persistence
//
//  Created by Jeff LaMarche on 7/29/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "PersistenceViewController.h"
#import "FourLines.h"

@implementation PersistenceViewController
@synthesize field1;
@synthesize field2;
@synthesize field3;
@synthesize field4;

- (NSString *)dataFilePath
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	return [documentsDirectory stringByAppendingPathComponent:kFilename];
}
- (void)applicationWillTerminate:(NSNotification *)notification
{
	//	FourLines *fourLines = [[FourLines alloc] init];
	//	fourLines.field1 = field1.text;
	//	fourLines.field2 = field2.text;
	//	fourLines.field3 = field3.text;
	//	fourLines.field4 = field4.text;
	//	
	//	NSMutableData *data = [[NSMutableData alloc] init];
	//	NSKeyedArchiver *archiver = [[NSKeyedArchiver alloc] initForWritingWithMutableData:data];
	//	[archiver encodeObject:fourLines forKey:kDataKey];
	//	[archiver finishEncoding];
	//	[data writeToFile:[self dataFilePath] atomically:YES];
	//	[fourLines release];
	//	[archiver release];
	//	[data release];
	
	for (int i = 1; i <= 4; i++)
	{
		NSString *fieldName = [[NSString alloc] initWithFormat:@"field%d", i];
		UITextField *field = [self valueForKey:fieldName];
		NSString *update = [[NSString alloc] initWithFormat:@"INSERT OR REPLACE INTO FIELDS (ROW, FIELD_DATA) VALUES (%d, '%@');", i, field.text];
		char * errorMsg;
		
		if (sqlite3_exec (database, [update UTF8String], NULL, NULL, &errorMsg) != SQLITE_OK)
		{
			NSAssert1(0, @"Error updating tables: %s", errorMsg);	
		}
		
		
	}
	
	sqlite3_close(database);		

}
#pragma mark -
- (void)viewDidLoad {
	
	if (sqlite3_open([[self dataFilePath] UTF8String], &database) != SQLITE_OK) {
		sqlite3_close(database);
		NSAssert(0, @"Failed to open database");
	}
	
	char *errorMsg;
	NSString *createSQL = @"CREATE TABLE IF NOT EXISTS FIELDS (ROW INTEGER PRIMARY KEY, FIELD_DATA TEXT);";
	if (sqlite3_exec (database, [createSQL  UTF8String], NULL, NULL, &errorMsg) != SQLITE_OK) {
		sqlite3_close(database);
		NSAssert1(0, @"Error creating table: %s", errorMsg);
	}
	
	
	NSString *query = @"SELECT ROW, FIELD_DATA FROM FIELDS ORDER BY ROW";
	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2( database, [query UTF8String], -1, &statement, nil) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			int row = sqlite3_column_int(statement, 0);
			char *rowData = (char *)sqlite3_column_text(statement, 1);
			
			NSString *fieldName = [[NSString alloc] initWithFormat:@"field%d", row];
			NSString *fieldValue = [[NSString alloc] initWithUTF8String:rowData];
			UITextField *field = [self valueForKey:fieldName];
			field.text = fieldValue;
			[fieldName release];
			[fieldValue release];
		}
		
	}

	
	
	UIApplication *app = [UIApplication sharedApplication];
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(applicationWillTerminate:)
												 name:UIApplicationWillTerminateNotification 
											   object:app];
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
	[field1 release];
	[field2 release];
	[field3 release];
	[field4 release];
	[super dealloc];
}

@end
