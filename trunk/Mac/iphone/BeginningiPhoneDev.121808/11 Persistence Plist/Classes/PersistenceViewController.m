//
//  PersistenceAppDelegate.m
//  Persistence
//
//  Created by Jeff LaMarche on 7/29/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "PersistenceViewController.h"

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
	NSMutableArray *array = [[NSMutableArray alloc] init];
	[array addObject:field1.text];
	[array addObject:field2.text];
	[array addObject:field3.text];
	[array addObject:field4.text];
	[array writeToFile:[self dataFilePath] atomically:YES];
	[array release];
}
#pragma mark -
- (void)viewDidLoad {
	
	NSString *filePath = [self dataFilePath];
	if ([[NSFileManager defaultManager] fileExistsAtPath:filePath])
	{
		NSMutableArray *array = [[NSMutableArray alloc] initWithContentsOfFile:filePath];
		field1.text = [array objectAtIndex:0];
		field2.text = [array objectAtIndex:1];
		field3.text = [array objectAtIndex:2];
		field4.text = [array objectAtIndex:3];
		[array release];
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
