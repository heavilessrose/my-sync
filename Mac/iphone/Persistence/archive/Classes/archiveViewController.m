//
//  archiveViewController.m
//  archive
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import "archiveViewController.h"
#import "FourLines.h"

@implementation archiveViewController

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
	//NSMutableArray *dataArray = [[NSMutableArray alloc] init];
	
	FourLines *fourLines = [[FourLines alloc] init];
	fourLines.field1 = field1.text;
	fourLines.field2 = field2.text;
	fourLines.field3 = field3.text;
	fourLines.field4 = field4.text;
	
	// 对数据对象进行编码归档
	NSMutableData *data = [[NSMutableData alloc] init];
	NSKeyedArchiver *archiver = [[NSKeyedArchiver alloc] initForWritingWithMutableData:data];
	[archiver encodeObject:fourLines forKey:kDataKey];
	[archiver finishEncoding];
	[data writeToFile:[self dataFilePath] atomically:YES];
	[fourLines release];
	[archiver release];
	[data release];
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
	NSString *filepath = [self dataFilePath];
	NSLog(filepath);
	if([[NSFileManager defaultManager] fileExistsAtPath:filepath]){
		// 对数据对象进行解码
		NSData *data = [[NSMutableData alloc] initWithContentsOfFile:filepath];
		NSKeyedUnarchiver *unarchiver = [[NSKeyedUnarchiver alloc] initForReadingWithData:data];
		FourLines *fourLines = [unarchiver decodeObjectForKey:kDataKey];
		[unarchiver finishDecoding];
		
		field1.text = fourLines.field1;
		field2.text = fourLines.field2;
		field3.text = fourLines.field3;
		field4.text = fourLines.field4;
		
		[unarchiver release];
		[data release];
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
