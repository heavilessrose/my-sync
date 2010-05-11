//
//  GHTest.h
//  GHKit
//
//  Created by Gabriel Handford on 1/18/09.
//  Copyright 2009. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following
//  conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//

/*!
 Test status.
 */
typedef enum {
	GHTestStatusNone,
	GHTestStatusRunning,
	GHTestStatusFinished,
} GHTestStatus;

/*!
 Generate string from GHTestStatus
 @param status
 */
NSString* NSStringFromGHTestStatus(GHTestStatus status);

/*!
 Test stats.
 */
typedef struct {
	NSInteger runCount;
	NSInteger failureCount;
	NSInteger testCount;
} GHTestStats;

/*!
 Create GHTestStats.
 */
GHTestStats GHTestStatsMake(NSInteger runCount, NSInteger failureCount, NSInteger testCount);

#define NSStringFromGHTestStats(stats) [NSString stringWithFormat:@"%d/%d/%d", stats.runCount, stats.testCount, stats.failureCount]

@protocol GHTestDelegate;

/*!
 Protocol for a test.
 */
@protocol GHTest <NSObject>

- (void)run;

- (NSString *)identifier;
- (NSString *)name;

- (NSTimeInterval)interval;
- (GHTestStatus)status;
- (GHTestStats)stats;

- (void)setDelegate:(id<GHTestDelegate>)delegate;

- (NSException *)exception;

- (void)log:(NSString *)message;
- (NSArray *)log;

@end

/*!
 Protocol for test delegate, that notifies when a test starts and ends.
 */
@protocol GHTestDelegate <NSObject>
- (void)testDidStart:(id<GHTest>)test;
- (void)testDidFinish:(id<GHTest>)test;
@end

/*!
 Default test implementation.
 - Consists of a target/selector
 - Notifies a test delegate
 - Keeps track of status, running time and failures
 - Stores any test specific logging
 */
@interface GHTest : NSObject <GHTest> {
	
	id<GHTestDelegate> delegate_; // weak
	
	id target_;
	SEL selector_;
	
	NSString *name_;
	GHTestStatus status_;
	NSTimeInterval interval_;
	BOOL failed_;
	NSException *exception_; // If failed
	
	GHTestStats stats_;
		
	NSMutableArray *log_;	
	
}

/*!
 Create test with target/selector.
 @param target Target (usually a test case)
 @param selector Selector (usually a test method)
 */
- (id)initWithTarget:(id)target selector:(SEL)selector;

/*!
 Create autoreleased test with target/selector.
 @param target Target (usually a test case)
 @param selector Selector (usually a test method)
 */
+ (id)testWithTarget:(id)target selector:(SEL)selector;

@property (readonly, nonatomic) id target;
@property (readonly, nonatomic) SEL selector;
@property (readonly, nonatomic) NSString *name;
@property (readonly, nonatomic) NSTimeInterval interval;
@property (readonly, nonatomic) NSException *exception;
@property (readonly, nonatomic) GHTestStatus status;
@property (readonly, nonatomic) BOOL failed;
@property (readonly, nonatomic) GHTestStats stats;
@property (readonly, nonatomic) NSArray *log;

@property (assign, nonatomic) id<GHTestDelegate> delegate;

/*!
 Run the test.
 After running, the interval and exception properties may be set.
 @result Yes if passed, NO otherwise
 */
- (void)run;

@end
