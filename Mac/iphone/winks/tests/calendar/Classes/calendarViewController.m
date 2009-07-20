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
	
	NSDateComponents *comps = [[NSDateComponents alloc] init];
	[comps setDay:6];
	[comps setMonth:5];
	[comps setYear:2004];
	NSCalendar *gregorian = [[NSCalendar alloc]
							 initWithCalendarIdentifier:NSGregorianCalendar];
	NSDate *date = [gregorian dateFromComponents:comps];
	[comps release];
	NSDateComponents *weekdayComponents =
    [gregorian components:NSWeekdayCalendarUnit fromDate:date];
	int weekday = [weekdayComponents weekday];
	
	NSDateComponents *yearComponents = [gregorian components:NSYearCalendarUnit fromDate:date];
	int year = [yearComponents year];
	printf("%d", year);
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



@end
