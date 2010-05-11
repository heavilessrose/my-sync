//
//  AudioPlayer.m
//  MSN
//
//  Created by  apple on 08-12-4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "AudioPlayer.h"

#import <AudioToolbox/AudioToolbox.h>


static void playbackCallback (void *inUserData, AudioQueueRef inAudioQueue, AudioQueueBufferRef bufferReference) 
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	AudioPlayer *player = (AudioPlayer *) inUserData;
	if (player.donePlayingFile)
		return;
	
	UInt32 numBytes;
	UInt32 numPackets = player.numPacketsToRead;
	AudioFileReadPackets ([player audioFileID],
						  NO,
						  &numBytes,
						  player.packetDescriptions,
						  player.startingPacketNumber,
						  &numPackets, 
						  bufferReference->mAudioData);
	
	if (numPackets > 0) 
	{
		bufferReference->mAudioDataByteSize = numBytes;		
		
		AudioQueueEnqueueBuffer (inAudioQueue,
								 bufferReference,
								 (player.packetDescriptions ? numPackets : 0),
								 player.packetDescriptions);
		[player incrementStartingPacketNumberBy: (UInt32) numPackets];
	}
	else
	{
		if (player.audioPlayerShouldStopImmediately == NO)
			[player stop];
	}
	[pool release];
}
static void propertyListenerCallback (void *inUserData, AudioQueueRef queueObject,AudioQueuePropertyID propertyID)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	AudioPlayer *player = (AudioPlayer *) inUserData;
	if (![player isRunning])
	{
		player.donePlayingFile = YES;		// 'donePlayingFile' used by playbackCallback and setupAudioQueueBuffers
	}
	[player noteStateChanged];
	[pool release];
}

static void interruptionListenerCallback (void *inUserData, UInt32 interruptionState) 
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	AudioPlayer *player = (AudioPlayer *) inUserData;
	
	if (interruptionState == kAudioSessionBeginInterruption) 
	{
		if ([player isRunning])
			[player pause];
	} 
	else if (interruptionState == kAudioSessionEndInterruption)
	{
		if ([player isRunning])
			[player resume];
	}
	[pool release];
}

@implementation AudioPlayer
@synthesize donePlayingFile,audioPlayerShouldStopImmediately, numPacketsToRead, packetDescriptions;

- (id)init;
{
	self = [super init];
	
	if (self != nil) 
	{
		UInt32 sessionCategory = kAudioSessionCategory_MediaPlayback;
		AudioSessionSetProperty (kAudioSessionProperty_AudioCategory,sizeof (sessionCategory),&sessionCategory);
		self.donePlayingFile = NO;
		self.audioPlayerShouldStopImmediately = NO;
		AudioSessionInitialize (NULL,NULL,interruptionListenerCallback,self);
	}
	return self;
} 

- (BOOL)openUrl:(CFURLRef)theURL
{
	self.audioFileURL = theURL;
	BOOL ret = [self openPlaybackFile: audioFileURL];
	if (ret)
		ret = [self setupPlaybackAudioQueueObject];
	return ret;
}

- (void) copyMagicCookieToQueue: (AudioQueueRef) queue fromFile: (AudioFileID) file 
{	
	UInt32 propertySize = sizeof (UInt32);	
	OSStatus result = AudioFileGetPropertyInfo (file, kAudioFilePropertyMagicCookieData, &propertySize, NULL);
	
	if (!result && propertySize) 
	{
		char *cookie = (char *) malloc (propertySize);		
		AudioFileGetProperty (file, kAudioFilePropertyMagicCookieData, &propertySize, cookie);
		
		AudioQueueSetProperty (queue,kAudioQueueProperty_MagicCookie,cookie,propertySize);
		free (cookie);
	}
}

- (BOOL) openPlaybackFile: (CFURLRef) soundFile 
{
	BOOL ret = YES;
	OSStatus result = AudioFileOpenURL ((CFURLRef) self.audioFileURL, 0x01, 0, &audioFileID);
	if (result == noErr)
	{
		UInt32 sizeOfPlaybackFormatASBDStruct = sizeof (audioFormat);
		result = AudioFileGetProperty (self.audioFileID, kAudioFilePropertyDataFormat, &sizeOfPlaybackFormatASBDStruct, &audioFormat);
		if (result != noErr)
			ret = NO;
	}
	else
		ret = NO;
	return ret;
}

- (BOOL) setupPlaybackAudioQueueObject
{
	BOOL ret = YES;
	OSStatus result = AudioQueueNewOutput (&audioFormat, playbackCallback, self, CFRunLoopGetCurrent (), kCFRunLoopCommonModes, 0,	&queueObject);
	if (result == noErr)
	{
		gain = 1.0;
		result = AudioQueueSetParameter (queueObject, kAudioQueueParam_Volume, gain);
		if (result == noErr)
		{
			result = AudioQueueAddPropertyListener (self.queueObject,kAudioQueueProperty_IsRunning,propertyListenerCallback,self);
			
			if (result == noErr)
				[self copyMagicCookieToQueue: queueObject fromFile: audioFileID];
			else
				ret = NO;	
		}
		else
			ret = NO;
	}
	else
		ret = NO;
	return ret;
}

- (void) setupAudioQueueBuffers 
{
	[self calculateSizesFor: (Float64) kSecondsPerBuffer];		
	int bufferIndex;
	for (bufferIndex = 0; bufferIndex < kNumberAudioDataBuffers; ++bufferIndex) 
	{
		if (buffers[bufferIndex])
		{
			AudioQueueFreeBuffer(queueObject,buffers[bufferIndex]);
		}
		AudioQueueAllocateBuffer (queueObject, bufferByteSize, &buffers[bufferIndex]);
		
		playbackCallback (self,queueObject,buffers[bufferIndex]);
		
		if (donePlayingFile) 
			break;
	}
}


- (void) play
{
	donePlayingFile = NO;
	startingPacketNumber = 0;
	audioPlayerShouldStopImmediately = NO;
	AudioSessionSetActive (true);
	[self setupAudioQueueBuffers];
	AudioQueueStart (queueObject,NULL);
}

- (void) stop
{
	AudioQueueStop (queueObject,audioPlayerShouldStopImmediately);
	AudioSessionSetActive (false);
}


- (void) pause 
{
	AudioQueuePause (queueObject);
}


- (void) resume 
{	
	AudioQueueStart (queueObject,NULL);
}


- (void) calculateSizesFor: (Float64) seconds
{
	UInt32 maxPacketSize;
	UInt32 propertySize = sizeof (maxPacketSize);
	
	AudioFileGetProperty (audioFileID, kAudioFilePropertyPacketSizeUpperBound, &propertySize, &maxPacketSize);
	
	static const int maxBufferSize = 0x10000;	// limit maximum size to 64K
	
	if (audioFormat.mFramesPerPacket) 
	{
		Float64 numPacketsForTime = audioFormat.mSampleRate / audioFormat.mFramesPerPacket * seconds;
		bufferByteSize =  numPacketsForTime * maxPacketSize;
	} 
	else
		bufferByteSize = maxBufferSize > maxPacketSize ? maxBufferSize : maxPacketSize;
	
	if (bufferByteSize > maxBufferSize && bufferByteSize > maxPacketSize) 
	{
		bufferByteSize = maxBufferSize;
	} 
	numPacketsToRead = bufferByteSize / maxPacketSize;
}

- (UInt64)packetsNumber
{
	UInt64 packetsNu = 0;
	UInt32 propertySize = sizeof (packetsNu);
	AudioFileGetProperty (audioFileID,kAudioFilePropertyAudioDataPacketCount,&propertySize,&packetsNu);
	return packetsNu;
}

- (void)dealloc 
{
	int bufferIndex;
	for (bufferIndex = 0; bufferIndex < kNumberAudioDataBuffers; ++bufferIndex) 
	{
		if (buffers[bufferIndex])
		{
			AudioQueueFreeBuffer(queueObject,buffers[bufferIndex]);
		}
	}
	AudioFileClose (audioFileID);
	AudioQueueDispose (queueObject, YES);
	
	[super dealloc];
}


@end
