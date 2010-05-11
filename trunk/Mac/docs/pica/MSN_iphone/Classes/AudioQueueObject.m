//
//  AudioQueueObject.m
//  MSN
//
//  Created by  apple on 08-12-4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "AudioQueueObject.h"
#import "ChatViewController.h"
#import "define.h"
#import <AudioToolbox/AudioToolbox.h>

@implementation AudioQueueObject

@synthesize audioFileURL, queueObject, startingPacketNumber, audioFileID,controller;

- (void)incrementStartingPacketNumberBy: (UInt32) inNumPackets 
{
	startingPacketNumber += inNumPackets;
	[controller performSelectorOnMainThread:@selector(audioCallBack:) withObject: [NSNumber numberWithInt:startingPacketNumber] waitUntilDone: NO];
}

- (void)noteStateChanged
{
	[controller performSelectorOnMainThread:@selector(audioStateChanged) withObject:nil  waitUntilDone:NO];
}

- (BOOL)isRunning 
{
	UInt64 isRunning = 1;

	UInt32 propertySize = sizeof (isRunning);
	OSStatus result;
	
	result = AudioQueueGetProperty (queueObject,kAudioQueueProperty_IsRunning,&isRunning,&propertySize);
	
	if (result != noErr)
	{
		isRunning  = 0;
	}
	return isRunning;
}

- (void) dealloc 
{
	[super dealloc];
}
@end
