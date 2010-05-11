//
//  AudioQueueObject.h
//  MSN
//
//  Created by  apple on 08-12-4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <AudioToolbox/AudioToolbox.h>


#define kNumberAudioDataBuffers	3

@class ChatViewController;
@interface AudioQueueObject : NSObject
{
	AudioQueueRef queueObject;					// the audio queue object being used for playback
	AudioFileID audioFileID;					// the identifier for the audio file to play
	CFURLRef audioFileURL;
	int	startingPacketNumber;
	ChatViewController *controller;
	AudioStreamBasicDescription	audioFormat;
}


- (void)incrementStartingPacketNumberBy:  (UInt32) inNumPackets;
- (void)noteStateChanged;
- (BOOL)isRunning;


@property (nonatomic, readwrite)AudioQueueRef queueObject;
@property (nonatomic, readwrite)CFURLRef audioFileURL;
@property (nonatomic, readwrite)AudioFileID	 audioFileID;
@property (nonatomic, readwrite)int	startingPacketNumber;
@property (nonatomic, assign)ChatViewController *controller;
@end
