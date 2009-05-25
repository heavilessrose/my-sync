#import <Foundation/Foundation.h>

int main (int argc, const char * argv[]) {
	NSMutableArray *array;
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	array = [[NSMutableArray alloc] init];
	int i;

	for (i = 0; i < 10; i++) {
		NSNumber *newNumber;
		newNumber = [[NSNumber alloc] initWithInt:(i * 3)];
		[array addObject:newNumber];
	}
	
	for (i = 0; i < 10; i++) {
		NSNumber *numberToPrint;
		numberToPrint = [array objectAtIndex:i];
		NSLog(@"The number at index %d is %@", i, numberToPrint);
	}
	
    // Done with array
	array = nil;
		
	// Run the garbage collector (if it being used)
    [pool drain];
	
	return 0;
}
