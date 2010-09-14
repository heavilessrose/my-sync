//
//  main.m
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <stdlib.h>
#import "NSArray+each.h"
#import "NSArray+select.h"
#import "BlocksTests.h"
#import <unistd.h>



char *myCharacters[3] = { "TomJohn", "George", "Charles Condomine" };

#pragma mark Declar a file global block // 文件内全局
int multiplier = 7;
int (^myBlock)(int) = ^(int num) { // 在哪里存储?? 与静态/全局变量一样?
    return num * multiplier;
};

// blocks 声明
typedef float (^MyBlockType)(float, float);


// myCharacters is now { "Charles Condomine", "George", TomJohn" }


// ===============
int main(int argc, char *argv[]) {
    
//    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
//    int retVal = UIApplicationMain(argc, argv, nil, nil);
//    [pool release];
//    return retVal;
	
#pragma mark using a block
	printf("%d\n", myBlock(3));
	
//	int *aBlock = (int *)myBlock;
//	printf("%d\n", *aBlock);
	
#pragma mark Inline using
	int eee = sizeof(char);
	eee = sizeof(char *);
	int count = sizeof(myCharacters) / sizeof(char *);
	qsort_b(myCharacters, 3, sizeof(char *), ^(const void *l, const void *r) {
		char *left = *(char **)l;
		char *right = *(char **)r;
		return strncmp(left, right, 1);
	});
	for (int i = 0; i < count; i++) {
		printf("%s\n", myCharacters[i]);
	}
	
#pragma mark Cocoa use
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSArray *stringsArray = [NSArray arrayWithObjects:
							 @"string 1",
							 @"String 21", // <-
							 @"string 12",
							 @"String 11",
							 @"Strîng 21", // <-
							 @"Striñg 21", // <-
							 @"String 02", nil];
	
	NSLocale *currentLocale = [NSLocale currentLocale];
	__block NSUInteger orderedSameCount = 0; // "orderedSameCount" lives in block storage. block一般在stack中, block storage一般也在stack中.
	
	NSArray *diacriticInsensitiveSortArray = [stringsArray sortedArrayUsingComparator:^(id string1, id string2) {
		
		NSRange string1Range = NSMakeRange(0, [string1 length]);
		NSComparisonResult comparisonResult = [string1 compare:string2 options:NSDiacriticInsensitiveSearch range:string1Range locale:currentLocale];
		
		if (comparisonResult == NSOrderedSame) {
			orderedSameCount++;
		}
		return comparisonResult;
	}];
	
	NSLog(@"diacriticInsensitiveSortArray: %@", diacriticInsensitiveSortArray);
	NSLog(@"orderedSameCount: %d", orderedSameCount);
	
#pragma mark as a category
	NSArray *array = [NSArray arrayWithObjects:@"tom", @"jerry", @"robin", nil];
	[array each:^(id aElement) {NSLog(@"hello %@", aElement);}];
	
	[[array select:^(id aElement) {
        return [aElement isEqualToString:@"robin"];
    }] each:^(id aElement) {
        NSLog(@"hello %@ only", aElement);
    }];
	
#pragma mark as a defined type 
	float (^floadAdd)(float, float) = ^(float f1, float f2) {
		return f1 + f2;
	};
	
	MyBlockType myFirstBlock = floadAdd;
	printf("%f\n", myFirstBlock(1.1f, 8.9f));
	
#pragma mark variables scopes
	extern NSInteger CounterGlobal;
	static NSInteger CounterStatic;
	printf("0 - CounterGlobal = %2d, CounterStatic = %2d, localCounter = no, localCharacter = no\n",
		   CounterGlobal, CounterStatic);
	
	{
		NSInteger localCounter = 42;
		__block char localCharacter;
		
		printf("1 - CounterGlobal = %2d, CounterStatic = %2d, localCounter = %2d, localCharacter = %c\n",
			   CounterGlobal, CounterStatic, localCounter, localCharacter);
		void (^aBlock)(void) = ^(void) {
			printf("3 - CounterGlobal = %2d, CounterStatic = %2d, localCounter = %2d, localCharacter = %c\n",
				   CounterGlobal, CounterStatic, localCounter, localCharacter);
			++CounterGlobal;
			++CounterStatic;
			CounterGlobal = localCounter; // localCounter fixed at block creation
			localCharacter = 'a'; // sets localCharacter in enclosing scope
			printf("4 - CounterGlobal = %2d, CounterStatic = %2d, localCounter = %2d, localCharacter = %c\n",
				   CounterGlobal, CounterStatic, localCounter, localCharacter);
		};
		
		++localCounter; // unseen by the block
		localCharacter = 'b';
		
		printf("2 - CounterGlobal = %2d, CounterStatic = %2d, localCounter = %2d, localCharacter = %c\n",
			   CounterGlobal, CounterStatic, localCounter, localCharacter);
		
		aBlock(); // execute the block
		// localCharacter now 'a'
		printf("5 - CounterGlobal = %2d, CounterStatic = %2d, localCounter = %2d, localCharacter = %c\n",
			   CounterGlobal, CounterStatic, localCounter, localCharacter);
	}
	//~

#pragma mark OC with Blocks
	BlocksTests *tests = [[BlocksTests alloc] init];
	[tests runTest];
	NSLog(@"BlocksTests count = %d", [tests retainCount]);
	[tests release];
	
	sleep(5);
	
	
	[pool release];
	return 0;
}
