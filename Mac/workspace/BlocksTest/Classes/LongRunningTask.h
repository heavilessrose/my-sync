//
//  LongRunningTask.h
//  BlocksTest
//
//  Created by luke on 10-9-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface LongRunningTask : NSObject {
	UIBackgroundTaskIdentifier bgTaskID;
}

@property (assign) UIBackgroundTaskIdentifier bgTaskID;

- (BOOL)runTask;
@end
