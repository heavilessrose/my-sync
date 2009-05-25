#import <Foundation/Foundation.h>
#import "LotteryEntry.h"

int main (int argc, const char * argv[]) {
	NSMutableArray *array;
	
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSCalendarDate *now;
	now = [[NSCalendarDate alloc] init];
	
	// Seed the random number generator
	srandom(time(NULL));
	
	array = [[NSMutableArray alloc] init];
	int i;

	for (i = 0; i < 10; i++) {
			
		// Create a date/time object that is 'i' weeks from now
		NSCalendarDate *iWeeksFromNow;
		iWeeksFromNow = [now dateByAddingYears:0
										months:0
										  days:(i * 7)
										 hours:0
									   minutes:0
									   seconds:0];
		
		// Create a new instance of Lottery Entry
		LotteryEntry *newEntry;
		newEntry = [[LotteryEntry alloc] initWithEntryDate:iWeeksFromNow];
		[array addObject:newEntry];
		[newEntry release];
	}
	
	
	// Done with 'now'
	[now release];
	now = nil;
	
	for (LotteryEntry *entryToPrint in array) {
		NSLog(@"%@", entryToPrint);
	}
	
    // Done with array
	[array release];
	array = nil;		
	[pool drain];
	NSLog(@"gc = %@", [NSGarbageCollector defaultCollector]);
	//[pool release];
	return 0;
}
