/*

File: RootViewController.m
Abstract: View controller that sets up the table view and the time zone data.
 

Version: 1.8

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import "RootViewController.h"
#import "TimeZoneWrapper.h"
#import "Region.h"

#import "TableViewCellSubviewsAppDelegate.h"



#define ROW_HEIGHT 60

@implementation RootViewController

@synthesize displayList;
@synthesize calendar;
@synthesize minuteTimer;


- (id)initWithStyle:(UITableViewStyle)style {
	if (self = [super initWithStyle:style]) {
		self.title = NSLocalizedString(@"Time Zones", @"Time Zones title");
		
		calendar= [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
		// self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
		self.tableView.rowHeight = ROW_HEIGHT;
	}
	return self;
}


- (void)viewDidLoad {
	
	[self setUpDisplayList];
}


- (void)viewWillAppear:(BOOL)animated {

	/*
	 Set up a timer to update the table view every minute on the minute so that it shows the current time.
	 */
    NSDate *date = [NSDate date];
    NSDate *oneMinuteFromNow = [date addTimeInterval:60];
    
	NSCalendarUnit unitFlags = NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit;
	NSDateComponents *timerDateComponents = [calendar components:unitFlags fromDate:oneMinuteFromNow];	
	NSDate *minuteTimerDate = [calendar dateFromComponents:timerDateComponents];
    
	NSTimer *timer = [[NSTimer alloc] initWithFireDate:minuteTimerDate interval:60 target:self selector:@selector(updateTime:) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
	self.minuteTimer = timer;
	[timer release];
}


- (void)viewWillDisappear:(BOOL)animated {
	self.minuteTimer = nil;	
}


- (void)setMinuteTimer:(NSTimer *)newTimer {
	
	if (minuteTimer != newTimer) {
		[minuteTimer invalidate];
		minuteTimer = newTimer;
	}
}


- (NSInteger)numberOfSectionsInTableView:(UITableView *)aTableView {
	// Number of sections is the number of regions
	return [displayList count];
}


- (NSInteger)tableView:(UITableView *)aTableView numberOfRowsInSection:(NSInteger)section {
	// Number of rows is the number of time zones in the region for the specified section
	Region *region = [displayList objectAtIndex:section];
	NSArray *regionTimeZones = region.timeZoneWrappers;
	return [regionTimeZones count];
}


- (NSString *)tableView:(UITableView *)aTableView titleForHeaderInSection:(NSInteger)section {
	// Section title is the region name
	Region *region = [displayList objectAtIndex:section];
	return region.name;
}


#define NAME_TAG 1
#define TIME_TAG 2
#define IMAGE_TAG 3

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath  {
		
	static NSString *CellIdentifier = @"TimeZoneCell";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	
	if (cell == nil) {
		cell = [self tableviewCellWithReuseIdentifier:CellIdentifier];
	}
	
	[self configureCell:cell forIndexPath:indexPath];
	return cell;
}


- (UITableViewCell *)tableviewCellWithReuseIdentifier:(NSString *)identifier {
	
	/*
	 Create an instance of UITableViewCell and add tagged subviews for the name, local time, and quarter image of the time zone.
	 */
	CGRect rect;
		
	rect = CGRectMake(0.0, 0.0, 320.0, ROW_HEIGHT);
	
	UITableViewCell *cell = [[[UITableViewCell alloc] initWithFrame:rect reuseIdentifier:identifier] autorelease];
	
#define LEFT_COLUMN_OFFSET 10.0
#define LEFT_COLUMN_WIDTH 160.0
	
#define MIDDLE_COLUMN_OFFSET 170.0
#define MIDDLE_COLUMN_WIDTH 90.0
	
#define RIGHT_COLUMN_OFFSET 280.0
		
#define MAIN_FONT_SIZE 18.0
#define LABEL_HEIGHT 26.0

#define IMAGE_SIDE 30.0
	
	/*
	 Create labels for the text fields; set the highlight color so that when the cell is selected it changes appropriately.
	*/
	UILabel *label;
	
	rect = CGRectMake(LEFT_COLUMN_OFFSET, (ROW_HEIGHT - LABEL_HEIGHT) / 2.0, LEFT_COLUMN_WIDTH, LABEL_HEIGHT);
	label = [[UILabel alloc] initWithFrame:rect];
	label.tag = NAME_TAG;
	label.font = [UIFont boldSystemFontOfSize:MAIN_FONT_SIZE];
	label.adjustsFontSizeToFitWidth = YES;
	[cell.contentView addSubview:label];
	label.highlightedTextColor = [UIColor whiteColor];
	[label release];
	
	rect = CGRectMake(MIDDLE_COLUMN_OFFSET, (ROW_HEIGHT - LABEL_HEIGHT) / 2.0, MIDDLE_COLUMN_WIDTH, LABEL_HEIGHT);
	label = [[UILabel alloc] initWithFrame:rect];
	label.tag = TIME_TAG;
	label.font = [UIFont systemFontOfSize:MAIN_FONT_SIZE];
	label.textAlignment = UITextAlignmentRight;
	[cell.contentView addSubview:label];
	label.highlightedTextColor = [UIColor whiteColor];
	[label release];

	
	// Create an image view for the quarter image
	 
	rect = CGRectMake(RIGHT_COLUMN_OFFSET, (ROW_HEIGHT - IMAGE_SIDE) / 2.0, IMAGE_SIDE, IMAGE_SIDE);

	UIImageView *imageView = [[UIImageView alloc] initWithFrame:rect];
	imageView.tag = IMAGE_TAG;
	[cell.contentView addSubview:imageView];
	[imageView release];
	
	
	return cell;
}


- (void)configureCell:(UITableViewCell *)cell forIndexPath:(NSIndexPath *)indexPath {
    
    /*
	 Cache the formatter. Normally you would use one of the date formatter styles (such as NSDateFormatterShortStyle), but here we want a specific format that excludes seconds.
	 */
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateFormat:@"h:mm a"];
	}
	
	// Get the time zones for the region for the section
	Region *region = [displayList objectAtIndex:indexPath.section];
	NSArray *regionTimeZones = region.timeZoneWrappers;
	TimeZoneWrapper *wrapper = [regionTimeZones objectAtIndex:indexPath.row];
	
	UILabel *label;
	
	// Get the time zone wrapper for the row
	label = (UILabel *)[cell viewWithTag:NAME_TAG];
	label.text = wrapper.timeZoneLocaleName;
	
	// Get the time zone wrapper for the row
	[dateFormatter setTimeZone:wrapper.timeZone];
	label = (UILabel *)[cell viewWithTag:TIME_TAG];
	label.text = [dateFormatter stringFromDate:[NSDate date]];
	
	// Get the time zone wrapper for the row
	UIImageView *imageView = (UIImageView *)[cell viewWithTag:IMAGE_TAG];
	imageView.image = wrapper.image;
}    


- (void)updateTime:(NSTimer *)timer {
	/*
     To display the current time, redisplay the time labels.
     Don't reload the table view's data as this is unnecessarily expensive -- it recalculates the number of cells and the height of each item to determine the total height of the view etc.  The external dimensions of the cells haven't changed, just their contents.
     */
    NSArray *visibleCells = self.tableView.visibleCells;
    for (UITableViewCell *cell in visibleCells) {
        NSIndexPath *indexPath = [self.tableView indexPathForCell:cell];
        [self configureCell:cell forIndexPath:indexPath];
        [cell setNeedsDisplay];
    }
}


- (void)setUpDisplayList {
	/*
	 Create an array of Region objects.
	 Each object represents a geographical region.  Each region contains time zones.
	 Much of the information required to display a time zone is expensive to compute, so rather than using NSTimeZone objects directly use wrapper objects that calculate the required derived values on demand and cache the results.
	 */
	NSArray *knownTimeZoneNames = [NSTimeZone knownTimeZoneNames];
	
	NSMutableArray *regions = [[NSMutableArray alloc] init];
	
	for (NSString *timeZoneName in knownTimeZoneNames) {
		
		NSArray *components = [timeZoneName componentsSeparatedByString:@"/"];
		NSString *regionName = [components objectAtIndex:0];
		
		Region *region = [Region regionNamed:regionName];
		if (region == nil) {
			region = [Region newRegionWithName:regionName];
			region.calendar = calendar;
			[regions addObject:region];
			[region release];
		}
		
		NSTimeZone *timeZone = [[NSTimeZone alloc] initWithName:timeZoneName];
		[region addTimeZone:timeZone nameComponents:components];
		[timeZone release];
	}
	
	NSDate *date = [NSDate date];
	// Now sort the time zones by name
	for (Region *region in regions) {
		[region sortZones];
		[region setDate:date];
	}
	// Sort the regions
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"name" ascending:YES];
	NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
	[regions sortUsingDescriptors:sortDescriptors];
	[sortDescriptor release];
	
	self.displayList = regions;
	[regions release];
}


- (void)didReceiveMemoryWarning {
	
	[super didReceiveMemoryWarning];
	[self update:self];
}


- (void)update:sender {
	/*
	 The following sets the date for the regions, hence also for the time zone wrappers. This has the side-effect of "faulting" the time zone wrappers (see TimeZoneWrapper's setDate: method), so can be used to relieve memory pressure.
	 */
	NSDate *date = [NSDate date];
	for (Region *region in displayList) {
		[region setDate:date];
	}
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	/*
	 To conform to the Human Interface Guidelines, selections should not be persistent --
	 deselect the row after it has been selected.
	 */
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}


- (void)dealloc {
	[minuteTimer invalidate];
	[displayList release];
	[calendar release];
	[super dealloc];
}


@end
