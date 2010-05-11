//
//  AudioPlayer.h
//  MSN
//
//  Created by  apple on 08-12-4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AudioQueueObject.h"

#define kSecondsPerBuffer	0.1

@interface AudioPlayer : AudioQueueObject
{
	UInt32							bufferByteSize;						// the number of bytes to use in each audio queue buffer
	UInt32							numPacketsToRead;					// the number of audio data packets to read into each audio queue buffer
	AudioQueueBufferRef				buffers[kNumberAudioDataBuffers];	// the audio queue buffers for the audio queue
	AudioStreamPacketDescription	*packetDescriptions;
	
	Float32	 gain;
	BOOL	 donePlayingFile;
	BOOL	 audioPlayerShouldStopImmediately;
}

- (BOOL) openUrl:(CFURLRef)theURL;
- (void) copyMagicCookieToQueue: (AudioQueueRef) queue fromFile: (AudioFileID) playbackFileID;
- (void) calculateSizesFor: (Float64) seconds;
- (BOOL) openPlaybackFile: (CFURLRef) fileURL;
- (BOOL) setupPlaybackAudioQueueObject;
- (void) setupAudioQueueBuffers;
- (UInt64)packetsNumber;

- (void) play;
- (void) stop;
- (void) pause;
- (void) resume;

@property (nonatomic,assign)BOOL donePlayingFile;
@property (nonatomic,assign)UInt32 numPacketsToRead;
@property (nonatomic,assign)BOOL audioPlayerShouldStopImmediately;
@property (readwrite) AudioStreamPacketDescription	 *packetDescriptions;
@end
