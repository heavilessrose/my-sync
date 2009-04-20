#import <Foundation/Foundation.h>
#import "LotteryEntry.h"

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	NSCalendarDate *now = [[NSCalendarDate alloc] init];
	srandom(time(NULL));
	
	NSMutableArray *array; 
	array = [[NSMutableArray alloc] init];
	//NSLog(@"first item = %@", [array objectAtIndex:0]);
	
	int i;
	for(i = 0; i < 10; i++){
		NSCalendarDate *iWeeksFromNow;
		//iWeeksFromNow = nil;
		iWeeksFromNow = [now dateByAddingYears:0 months:0 days:(i * 7) hours:0 minutes:0 seconds:0];
		
		LotteryEntry *newEntry;
		newEntry = [[LotteryEntry alloc] initWithEntryDate:iWeeksFromNow];
		
		[array addObject:newEntry];
		[newEntry release];
	}
	
	[now release];
	now = nil;
	
	for(LotteryEntry *entryToPrint in array){
		NSLog(@"%@", entryToPrint);
	}
	
	[array release];
	array = nil;

    // insert code here...
//	NSMutableArray *array;
//	array = [[NSMutableArray alloc] init];
//	
//	int i;
//	for(i = 0; i < 10; i++){
//		NSNumber *newNumber = [[NSNumber alloc] initWithInt:(i * 3)];
//		[array addObject:newNumber];
//	}
//	
//	for(i = 0; i < 10; i++){
//		NSNumber *numberPrint = [array objectAtIndex:i];
//		NSLog(@"number at %d is %@", i, numberPrint);
//	}
//	
//    NSLog(@"Hello, World!");
//	
//	// test 
//	const char *foo = "Blah blah";
//	NSString *bar;
//	// Create an NSString from a C string
//	bar = [NSString stringWithUTF8String:foo];
//	NSLog(bar);
//	
//	// Create a C string from an NSString
//	foo = [bar UTF8String];
//	printf("%s", *foo);
//	
//	// 指针可变指针指向的字符串不可变？
//	//*foo = 'c';
//	char c = 'c';
//	foo = &c;
//	printf("%s", *foo);
	
	// auto release pool
    [pool drain];
	//NSLog(@"GC = %@", [NSGarbageCollector defaultCollector]);
    return 0;
}
