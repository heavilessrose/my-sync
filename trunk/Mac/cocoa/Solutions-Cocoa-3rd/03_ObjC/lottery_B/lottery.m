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
		LotteryEntry *newEntry;
		newEntry = [[LotteryEntry alloc] init];
		[newEntry prepareRandomNumbers];
		
		// Create a date/time object that is 'i' weeks from now
		[newEntry setEntryDate:[now dateByAddingYears:0
											   months:0
												 days:(i * 7)
												hours:0
											  minutes:0
											  seconds:0]];
		
		[array addObject:newEntry];
	}
	
	// Done with 'now'
	now = nil;
	
	for (LotteryEntry *entryToPrint in array) {
		// Get entry to print
		NSLog(@"%@", entryToPrint);
	}
	
    // Done with array
	array = nil;		
	[pool drain];
	
	return 0;
}
