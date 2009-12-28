//
//  AppTests.m
//  UIFramework
//
//  Created by luke on 09-12-25.
//  Copyright 2009 WeTeam. All rights reserved.
//

#import "AppTests.h"


@implementation AppTests

#if USE_APPLICATION_UNIT_TEST     // all code under test is in the iPhone Application
- (void) testAppDelegate {
    
    id yourApplicationDelegate = [[UIApplication sharedApplication] delegate];
    STAssertNotNil(yourApplicationDelegate, @"UIApplication failed to find the AppDelegate");
}

//- (void) testFail {
//	STFail(@"Must fail to succeed.");
//}
- (void) testPass {
	STAssertTrue(TRUE, @"");
}
#else                           // all code under test must be linked into the Unit Test bundle

- (void) testMath {
    
    STAssertTrue((1+1)==2, @"Compiler isn't feeling well today :-(" );
    
}


#endif


@end
