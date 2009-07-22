//
//  calendarViewController.m
//  calendar
//
//  Created by wang luke on 7/20/09.
//  Copyright luke 2009. All rights reserved.
//

#import "calendarViewController.h"

@implementation calendarViewController



/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	//[[NSCalendar autoupdatingCurrentCalendar] ] 
//	NSArray* languages = [NSLocale preferredLanguages];
//	
//	for(int i = 0; i < 10; i++)
//		NSLog(@"language: %@", [languages objectAtIndex:i]);
	
//	NSDateComponents *comps = [[NSDateComponents alloc] init];
//	[comps setDay:6];
//	[comps setMonth:5];
//	[comps setYear:2004];
//	NSCalendar *gregorian = [[NSCalendar alloc]
//							 initWithCalendarIdentifier:NSGregorianCalendar];
//	NSDate *date = [gregorian dateFromComponents:comps];
//	[comps release]; 
	

//	NSDateComponents *weekdayComponents =
//    [gregorian components:NSWeekdayCalendarUnit fromDate:date];
//	int weekday = [weekdayComponents weekday];
	
//	NSDateComponents *yearComponents = [gregorian components:NSYearCalendarUnit fromDate:date];
//	int year = [yearComponents year];
//	printf("%d", year);
//	
//	NSDateComponents *monthComponents = [gregorian components:NSMonthCalendarUnit fromDate:date];
//	int month = [monthComponents month];
//	printf("%d", month);
	
	getdateinfo();
	
	// Local Time Zone [Asia/Shanghai (CST) offset 28800]
	getTimeZone();
	
}


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


- (void)dealloc {
    [super dealloc];
}

#pragma mark -
// 得到年月日时间表示
void getdateinfo(void){
	NSCalendar *gregorian = [[NSCalendar alloc]
							 initWithCalendarIdentifier:NSGregorianCalendar];
	NSDate *date = [NSDate date];
	
	unsigned unitFlags = NSYearCalendarUnit
	| NSMonthCalendarUnit 
	| NSDayCalendarUnit
	| NSHourCalendarUnit
	| NSMinuteCalendarUnit
	| NSSecondCalendarUnit
	| NSWeekdayCalendarUnit;
	NSDateComponents *dateComponents = [gregorian components:unitFlags fromDate:date];
	
	unsigned long year = [dateComponents year];
	unsigned char month = [dateComponents month];
	unsigned char day = [dateComponents day];
	unsigned char hour = [dateComponents hour];
	unsigned char minute = [dateComponents minute];
	unsigned char second = [dateComponents second];
	unsigned char weekday = [dateComponents weekday];
	
	//[gregorian release];
	//[dateComponents release];
	//[date release];
	
	printf("year = %d\n", year);
	printf("month = %d\n", month);
	printf("day = %d\n", day);
	printf("hour = %d\n", hour);
	printf("minute = %d\n", minute);
	printf("second = %d\n", second);
	printf("weekday = %d\n", weekday);
}

// Local Time Zone [Asia/Shanghai (CST) offset 28800]
void getTimeZone(){
	NSTimeZone * zone = [NSTimeZone systemTimeZone];
	NSLog([zone description]);
	NSInteger offsetSec = [zone secondsFromGMT];
	printf("\n%d, %d\n", offsetSec, offsetSec / 3600);
	
	// 得到一个时区
	NSTimeZone *atz = [NSTimeZone timeZoneForSecondsFromGMT:-28800];
	NSInteger offsetatz = [atz secondsFromGMT];
	printf("\n%d, %d\n", offsetatz, offsetatz / 3600);
}

@end
