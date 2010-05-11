//
//  AudioRecorder.m
//  MSN
//
//  Created by  apple on 08-12-4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "AudioRecorder.h"
#import "AudioQueueObject.h"

#import <AudioToolbox/AudioToolbox.h>
#import "define.h"

#include "WaveHead.h"
#include "Converter.h"
#include "GSM.H"

#define PCM_DATA_FILENAME           (@"pcm_data.dat")

NSString*       s_sourcefileName = NULL;

static void recordingCallback (void *inUserData, AudioQueueRef inAudioQueue, AudioQueueBufferRef inBuffer, const AudioTimeStamp *inStartTime,
							   UInt32 inNumPackets, const AudioStreamPacketDescription *inPacketDesc) 
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	AudioRecorder *recorder = (AudioRecorder *) inUserData;
	if (inNumPackets > 0){
        fwrite(inBuffer->mAudioData, inBuffer->mAudioDataByteSize, 1, recorder.fpPcmDataFilePath);
        [recorder incrementStartingPacketNumberBy: inNumPackets];
	}
	[pool release];
	if ([recorder isRunning]){
		AudioQueueEnqueueBuffer (inAudioQueue,inBuffer, 0,NULL);
	}
}

static void propertyListenerCallback (void *inUserData, AudioQueueRef	queueObject, AudioQueuePropertyID propertyID)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	AudioRecorder *recorder = (AudioRecorder *) inUserData;
	[recorder noteStateChanged];
	if (![recorder isRunning]){
		[recorder stoped];
//        if (recorder.fpPcmDataFilePath != NULL)
//            fclose(recorder.fpPcmDataFilePath);
//        recorder.fpPcmDataFilePath = NULL;
	}
	[pool release];
}

static void interruptionListenerCallback (void *inUserData, UInt32 interruptionState) 
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	AudioRecorder *recorder = (AudioRecorder *) inUserData;
	
	if (interruptionState == kAudioSessionBeginInterruption) 
	{
		if ([recorder isRunning])
			[recorder stop];
	} 
	[pool release];
}

@implementation AudioRecorder
@synthesize filePath;
@synthesize fpPcmDataFilePath;

- (id)init 
{
	self = [super init];
	if (self != nil) 
	{
		UInt32 sessionCategory = kAudioSessionCategory_RecordAudio;
		AudioSessionSetProperty (kAudioSessionProperty_AudioCategory,sizeof (sessionCategory),&sessionCategory);
		NSString *directory = NSTemporaryDirectory();
		NSDateComponents *timeComponents = [[NSCalendar currentCalendar] components:(NSYearCalendarUnit | NSMonthCalendarUnit |  NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit ) 
																		   fromDate:[NSDate date]];
		NSString *stringTime = [NSString stringWithFormat: VOICE_CLIP_NAME_FORMAT, 
								[timeComponents year],
								[timeComponents month],
								[timeComponents day],
								[timeComponents hour],
								[timeComponents minute],
								[timeComponents second]];
		
		self.filePath = [directory stringByAppendingPathComponent:stringTime];
        if (s_sourcefileName != NULL){
            [s_sourcefileName release];
        }
        s_sourcefileName = [filePath copy];
        
        NSString* aPcmFilePath = [directory stringByAppendingPathComponent: PCM_DATA_FILENAME];
        fpPcmDataFilePath = fopen([aPcmFilePath UTF8String], "wb+");
        
		audioFormat.mSampleRate			= 8000;
		audioFormat.mFormatID			= kAudioFormatLinearPCM;
		audioFormat.mFormatFlags		= kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
		audioFormat.mFramesPerPacket	= 1;
		audioFormat.mChannelsPerFrame	= 1;
		audioFormat.mBitsPerChannel		= 16;
		audioFormat.mBytesPerPacket		= 2;
		audioFormat.mBytesPerFrame		= 2;
        
		AudioSessionInitialize (NULL,NULL,interruptionListenerCallback,self);
		AudioQueueNewInput (&audioFormat,recordingCallback,self,	NULL,NULL,0,&queueObject);
		AudioQueueAddPropertyListener (queueObject,kAudioQueueProperty_IsRunning,propertyListenerCallback,self);
		UInt32 sizeOfRecordingFormatASBDStruct = sizeof (audioFormat);
		AudioQueueGetProperty (queueObject,kAudioQueueProperty_StreamDescription, &audioFormat, &sizeOfRecordingFormatASBDStruct);
		AudioQueueSetParameter (queueObject, kAudioQueueParam_Volume, 1.0);
	}
	return self;
} 

- (void) record
{
	AudioSessionSetActive (true);
	[self setupRecording];	
	AudioQueueStart (queueObject, NULL);
}


- (void) stop
{
	AudioQueueStop (queueObject,YES);
	AudioSessionSetActive (false);
	//AudioFileClose (audioFileID);
}

- (void)stoped
{
    NSString* aPcmFilePath = [NSTemporaryDirectory() stringByAppendingPathComponent: PCM_DATA_FILENAME];
    if ([[NSFileManager defaultManager] fileExistsAtPath: aPcmFilePath])
        PcmToGsm([aPcmFilePath UTF8String], [s_sourcefileName UTF8String]);
}

- (void) setupRecording 
{
	startingPacketNumber = 0;
	int bufferByteSize = 960;		// this is the maximum buffer size used by the player class
	int bufferIndex;
	
	for (bufferIndex = 0; bufferIndex < kNumberAudioDataBuffers; ++bufferIndex)
	{
		AudioQueueBufferRef buffer;
		
		AudioQueueAllocateBuffer (queueObject,bufferByteSize, &buffer);
		
		AudioQueueEnqueueBuffer (queueObject,buffer,0,NULL);
	}
}


- (void) dealloc 
{
	AudioQueueDispose (queueObject,YES);
	[filePath release];
    if (fpPcmDataFilePath != NULL)
        fclose(fpPcmDataFilePath);
    fpPcmDataFilePath = NULL;
	[super dealloc];
}


@end
