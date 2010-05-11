//
//  AudioRecorder.h
//  MSN
//
//  Created by  apple on 08-12-4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AudioQueueObject.h"

@interface AudioRecorder : AudioQueueObject
{
    NSString *pcmDataFilePath;
	NSString *filePath;
    FILE*       fpPcmDataFilePath;
}

- (void) setupRecording;
- (void) record;
- (void) stop;
- (void)stoped;

@property (nonatomic, retain)NSString *filePath;
@property (nonatomic, assign) FILE* fpPcmDataFilePath;
@end
