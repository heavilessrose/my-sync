/*

File: RootViewController.m
Abstract: View controller that serves as the table view's data source and
delegate. It also set up the data.

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
#import "SimpleIndexedTableViewAppDelegate.h"
#import <CoreFoundation/CoreFoundation.h>


NSString *localeNameForTimeZoneNameComponents(NSArray *nameComponents);


@implementation RootViewController


@synthesize displayList;
@synthesize indexLetters;


- (id)initWithStyle:(UITableViewStyle)style {
	if (self = [super initWithStyle:style]) {
		self.title = NSLocalizedString(@"Time Zones", @"Time Zones title");
	}
	return self;
}


- (void)viewDidLoad {
	[self setUpDisplayList];
}


- (void)dealloc {
	
	[displayList release];
    [super dealloc];
}


- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	// Number of sections is the number of region dictionaries
	return [displayList count];
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	// Number of rows is the number of names in the region dictionary for the specified section
	NSDictionary *letterDictionary = [displayList objectAtIndex:section];
	NSArray *zonesForLetter = [letterDictionary objectForKey:@"timeZones"];
	return [zonesForLetter count];
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	// The header for the section is the region name -- get this from the dictionary at the section index
	NSDictionary *sectionDictionary = [displayList objectAtIndex:section];
	return [sectionDictionary valueForKey:@"letter"];
}


- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView {
	/*
	Return the index titles for each of the sections (e.g. "A", "B", "C"...).
	Use key-value coding to get the value for the key @"letter" in each of the dictionaries in list.
	 */
	return [displayList valueForKey:@"letter"];
}


- (NSInteger)tableView:(UITableView *)tableView sectionForSectionIndexTitle:(NSString *)title atIndex:(NSInteger)index {
	// Return the index for the given section title
	return [indexLetters indexOfObject:title];
}



- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *MyIdentifier = @"MyIdentifier";
	
	// Try to retrieve from the table view a now-unused cell with the given identifier
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:MyIdentifier];
	
	// If no cell is available, create a new one using the given identifier
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:MyIdentifier] autorelease];
	}
	
	NSDictionary *letterDictionary = [displayList objectAtIndex:indexPath.section];
	NSArray *zonesForLetter = [letterDictionary objectForKey:@"timeZones"];
	NSDictionary *timeZoneDictionary = [zonesForLetter objectAtIndex:indexPath.row];
	
	// Set the cell's text to the name of the time zone at the row
	cell.text = [timeZoneDictionary objectForKey:@"timeZoneLocaleName"];
	return cell;
}


- (void)setUpDisplayList {
	/*
	 Create an array (timeZones) of dictionaries
	 Each dictionary groups together the time zones with locale names beginning with a particular letter:
	 key = "letter" value = e.g. "A"
	 key = "timeZones" value = [array of dictionaries]
	 
	 Each dictionary in "timeZones" contains keys "timeZone" and "timeZoneLocaleName"
	 */
	
	SimpleIndexedTableViewAppDelegate *appDelegate = (SimpleIndexedTableViewAppDelegate *)[[UIApplication sharedApplication] delegate];
	NSArray *knownTimeZoneNames = [appDelegate list];
	
	NSMutableDictionary *indexedTimeZones = [[NSMutableDictionary alloc] init];
	
	for (NSString *name in knownTimeZoneNames) {
		
		// The region name is unused in this example
		NSArray *components = [name componentsSeparatedByString:@"/"];
		
		NSString *timeZoneLocaleName = localeNameForTimeZoneNameComponents(components);
		
		NSString *firstLetter = [timeZoneLocaleName substringToIndex:1];
		NSMutableArray *indexArray = [indexedTimeZones objectForKey:firstLetter];
		if (indexArray == nil) {
			indexArray = [[NSMutableArray alloc] init];
			[indexedTimeZones setObject:indexArray forKey:firstLetter];
			[indexArray release];
		}
		
		NSTimeZone *timeZone = [NSTimeZone timeZoneWithName:name];
		NSDictionary *timeZoneDictionary = [[NSDictionary alloc] initWithObjectsAndKeys:timeZone, @"timeZone", timeZoneLocaleName, @"timeZoneLocaleName", nil];
		[indexArray addObject:timeZoneDictionary];
	}
	
	/*
	 Finish setting up the data structure:
	 Create the timezones array;
	 Sort the used index letters and keep as an instance variable;
	 Sort the contents of the timeZones arrays;
	*/
	NSMutableArray *timeZones = [[NSMutableArray alloc] init];

	// Normally we'd use a localized comparison to present information to the user, but here we know the data only contains unaccented uppercase letters
	self.indexLetters = [[indexedTimeZones allKeys] sortedArrayUsingSelector:@selector(compare:)];
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"timeZoneLocaleName" ascending:YES];
	NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
		
	for (NSString *indexLetter in indexLetters) {
		
		NSMutableArray *timeZoneDictionaries = [indexedTimeZones objectForKey:indexLetter];
		[timeZoneDictionaries sortUsingDescriptors:sortDescriptors];
		
		NSDictionary *letterDictionary = [[NSDictionary alloc] initWithObjectsAndKeys:indexLetter, @"letter", timeZoneDictionaries, @"timeZones", nil];
		[timeZones addObject:letterDictionary];
		[letterDictionary release];
	}
	[sortDescriptor release];
	
	self.displayList = timeZones;
	[timeZones release];
}


/*
 To conform to Human Interface Guildelines, since selecting a row would have no effect (such as navigation), make sure that rows cannot be selected.
 */
- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	return nil;
}


@end


NSString *localeNameForTimeZoneNameComponents(NSArray *nameComponents) {
	if ([nameComponents count] == 2) {
		return [nameComponents objectAtIndex:1];
	}
	NSString *localeName = [NSString stringWithFormat:@"%@ (%@)", [nameComponents objectAtIndex:2], [nameComponents objectAtIndex:1]];
	return localeName;
}

