//
//  LogicTests.m
//  UIFramework
//
//  Created by luke on 09-12-25.
//  Copyright 2009 WeTeam. All rights reserved.
//

#import "LogicTests.h"


@implementation LogicTests

#if USE_APPLICATION_UNIT_TEST     // all code under test is in the iPhone Application

- (void) testAppDelegate {
    
    id yourApplicationDelegate = [[UIApplication sharedApplication] delegate];
    STAssertNotNil(yourApplicationDelegate, @"UIApplication failed to find the AppDelegate");
    
}

#else                           // all code under test must be linked into the Unit Test bundle

- (void) testMath {
    
    STAssertTrue((1+1)==2, @"Compiler isn't feeling well today :-(" );
    
}

//- (void) testFail {
//	STFail(@"Must fail to succeed.");
//}
- (void) testPass {
	STAssertTrue(TRUE, @"");
}
#endif


@end
