/*
 *  TimerController.h
 *  Picoffee
 *
 *  Created by luke on 8/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

@interface TimerController : NSObject {
    NSTimer *repeatingTimer;
    NSTimer *unregisteredTimer;
    NSUInteger timerCount;
}

@property (assign) NSTimer *repeatingTimer;
@property (retain) NSTimer *unregisteredTimer;
@property NSUInteger timerCount;

- (IBAction)startOneOffTimer:sender;

- (IBAction)startRepeatingTimer:sender;
- (IBAction)stopRepeatingTimer:sender;

- (IBAction)createUnregisteredTimer:sender;
- (IBAction)startUnregisteredTimer:sender;
- (IBAction)stopUnregisteredTimer:sender;

- (IBAction)startFireDateTimer:sender;

- (void)timerFireMethod:(NSTimer*)theTimer;
- (void)invocationMethod:(NSDate *)date;
- (void)countedTimerFireMethod:(NSTimer*)theTimer;

- (NSDictionary *)userInfo;

@end