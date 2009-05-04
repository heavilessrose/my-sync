//
//  SQLiteViewController.m
//  SQLite
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import "SQLiteViewController.h"
#import "FourLines.h"

@implementation SQLiteViewController

@synthesize field1;
@synthesize field2;
@synthesize field3;
@synthesize field4;

#pragma mark -
#pragma mark propertyListViewController_方法实现

// 返回保存文件的完整路径
- (NSString *)dataFilePath
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	return [documentsDirectory stringByAppendingPathComponent:kFilename];
}


#pragma mark -
#pragma mark 通知方法。（此处非UIApplicationDelegate方法）
// 是一种通知方法。所有通知方法都使用一个NSNotification实例作为参数。通知是发生了某些事件的指示。
// 被传递的发布通知的对象在其文档中包含一个通知列表（如UIApplication类文档中有一个notification列表）。
// 退出时调用
- (void)applicationWillTerminate:(NSNotification *)notification
{
	for(int i = 1; i <= 4; i++){
		NSString *fieldName = [[NSString alloc] initWithFormat:@"field%d", i];
		// 根据名称得到property
		UITextField *field = [self valueForKey:fieldName];
		[fieldName release];
		
		// 更新FIELDS表（INSERT OR REPLACE语句，不存在则插入，存在则更新行号匹配的现有行）
		NSString *update = [[NSString alloc] initWithFormat:@"INSERT OR REPLACE INTO FIELDS (ROW, FIELD_DATA) VALUES (%d, '%@');", i, field.text];
		char *errorMsg;
		
		// 执行更新并处理更新不成功的情况
		if(sqlite3_exec(database, [update UTF8String], NULL, NULL, &errorMsg) != SQLITE_OK){
			NSAssert1(0, @"Error updating tables: %s", errorMsg);
			sqlite3_free(errorMsg);
		}
	}
	sqlite3_close(database);
}

#pragma mark -
#pragma mark UIViewControllerDelegate方法
// Sent to the view controller when the application receives a memory warning.
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
	// Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

// Invoked when the view is finished loading.
- (void)viewDidLoad
{
	// 打开数据库，并处理打开失败的情况
	if(sqlite3_open([[self dataFilePath] UTF8String], &database) != SQLITE_OK){
		sqlite3_close(database);
		NSAssert(0, @"Failed to open database");
	}
	
	char *errorMsg;
	// 建Fields表CREATE TABLE IF NOT EXISTS
	NSString *creatSQL = @"CREATE TABLE IF NOT EXISTS FIELDS (ROW INTEGER PRIMARY KEY, FIELD_DATA TEXT);";
	if(sqlite3_exec(database, [creatSQL UTF8String], NULL, NULL, &errorMsg) != SQLITE_OK){
		sqlite3_close(database);
		NSAssert1(0, @"Error creating table: %s", errorMsg);
	}
	
	// 加载数据select，请求数据库中的所有行
	NSString *query = @"SELECT ROW, FIELD_DATA FROM FIELDS ORDER BY ROW";
	sqlite3_stmt *statement;
	if(sqlite3_prepare_v2(database, [query UTF8String], -1, &statement, nil) == SQLITE_OK){
		// 单步调试每个返回行
		while (sqlite3_step(statement) == SQLITE_ROW) {
			// 得到字段值
			int row = sqlite3_column_int(statement, 0);
			char *rowData = (char *)sqlite3_column_text(statement, 1);
			
			NSString *fieldName = [[NSString alloc] initWithFormat:@"field%d", row];
			NSString *fieldValue = [[NSString alloc] initWithUTF8String:rowData];
			UITextField *field = [self valueForKey:fieldName];
			field.text = fieldValue;
			[fieldName release];
			[fieldValue release];
		}
		// 删除statement
		sqlite3_finalize(statement);
	}
	
	// Returns the singleton application instance
	UIApplication *app = [UIApplication sharedApplication];
	// !!!: 订阅UIApplicationWillTerminateNotification通知。self是需要被通知的对象。selector为收到通知后要调用的方法。name为我们感兴趣的通知名称。app为The object whose notifications the observer wants to receive（发出通知的对象）
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillTerminate:) name:UIApplicationWillTerminateNotification object:app];
	[super viewDidLoad];
}

// Returns a Boolean value indicating whether the view controller autorotates its view.
- (BOOL)shouldAutorotateToInterfaceOrientation: (UIInterfaceOrientation)interfaceOrientation
{
	// return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark -
#pragma mark 释放资源
- (void)dealloc {
	// !!!: 应释放所有资源
	[field1 release];
	[field2 release];
	[field3 release];
	[field4 release];
    [super dealloc];
}

@end
