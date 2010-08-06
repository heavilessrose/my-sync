/*
 *  TimerController.m
 *  Picoffee
 *
 *  Created by luke on 8/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TimerController.h"

@implementation TimerController
@synthesize repeatingTimer, unregisteredTimer, timerCount;

- (NSDictionary *)userInfo
{
    return [NSDictionary dictionaryWithObject:[NSDate date] forKey:@"StartDate"];
}

- (void)targetMethod:(NSTimer*)theTimer
{
    NSDate *startDate = [[theTimer userInfo] objectForKey:@"StartDate"];
    NSLog(@"Timer started on %@", startDate);
}

- (void)invocationMethod:(NSDate *)date
{
    NSLog(@"Invocation for timer started on %@", date);
}

/*!
    @function
    @abstract   start a timer that not repeats.
    @discussion 
    @param      
    @result     
*/

- (IBAction)startOneOffTimer:sender
{
    [NSTimer scheduledTimerWithTimeInterval:2.0
                                     target:self
                                   selector:@selector(targetMethod:)
                                   userInfo:[self userInfo]
                                    repeats:NO];
}

/*!
    @function
    @abstract   start a repeats timer
    @discussion 
    @param      
    @result     
*/

- (IBAction)startRepeatingTimer:sender
{
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.5
                                                      target:self selector:@selector(timerFireMethod:)
                                                    userInfo:[self userInfo] repeats:YES];
    self.repeatingTimer = timer;
}


- (IBAction)createUnregisteredTimer:sender
{
    NSMethodSignature *methodSignature = [self methodSignatureForSelector:@selector(invocationMethod:)];
    NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSignature];
    [invocation setTarget:self];
    [invocation setSelector:@selector(invocationMethod:)];
    NSDate *startDate = [NSDate date];
    [invocation setArgument:&startDate atIndex:2];
    
    NSTimer *timer = [NSTimer timerWithTimeInterval:0.5 invocation:invocation repeats:YES];
    self.unregisteredTimer = timer;
}

- (IBAction)startUnregisteredTimer:sender
{
    if (unregisteredTimer != nil) {
        NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
        [runLoop addTimer:unregisteredTimer forMode:NSDefaultRunLoopMode];
    }
}


- (IBAction)startFireDateTimer:sender
{
    NSDate *fireDate = [NSDate dateWithTimeIntervalSinceNow:1.0];
    NSTimer *timer = [[NSTimer alloc] initWithFireDate:fireDate
                                              interval:0.5
                                                target:self
                                              selector:@selector(countedtargetMethod:)
                                              userInfo:[self userInfo]
                                               repeats:YES];
    
    timerCount = 1;
    NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
    [runLoop addTimer:timer forMode:NSDefaultRunLoopMode];
    [timer release];
}

#pragma mark stop timer
- (IBAction)stopRepeatingTimer:sender
{
    [repeatingTimer invalidate];
    self.repeatingTimer = nil;
}

- (IBAction)stopUnregisteredTimer:sender
{
    [unregisteredTimer invalidate];
    self.unregisteredTimer = nil;
}

/*!
    @function
    @abstract   invalidate the timer which start with a Fire Date after it has fired three times. 
    @discussion 
    @param      
    @result     
*/

- (void)countedtargetMethod:(NSTimer*)theTimer
{
    NSDate *startDate = [[theTimer userInfo] objectForKey:@"StartDate"];
    NSLog(@"Timer started on %@; fire count %d", startDate, timerCount);
    
    timerCount++;
    if (timerCount > 3) {
        [theTimer invalidate];
    }
}

@end
