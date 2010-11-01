//
//  tests.m
//  CoreAudioTest
//
//  Created by luke on 10-10-31.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "tests.h"
#import <AudioToolbox/AudioToolbox.h>
#import <AudioUnit/AudioUnit.h>
#import <CoreAudio/CoreAudioTypes.h>

@implementation tests

- (BOOL)isAudioInputAvailable {
	
	UInt32 audioInputIsAvailable;
	UInt32 propertySize = sizeof(audioInputIsAvailable);
	AudioSessionGetProperty(kAudioSessionProperty_AudioInputAvailable, &propertySize, 
							 &audioInputIsAvailable // A nonzero value on output means that audio input is available
							 );
	if (audioInputIsAvailable) {
		return YES;
	}
	return NO;
}

@end
