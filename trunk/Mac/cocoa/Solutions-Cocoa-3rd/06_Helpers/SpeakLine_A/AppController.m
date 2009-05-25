//
//  AppController.m
//  SpeakLine
//
//  Created by Aaron Hillegass on 9/13/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "AppController.h"


@implementation AppController

- (id)init
{
	[super init];
	NSLog(@"init");
	speechSynth = [[NSSpeechSynthesizer alloc] initWithVoice:nil];
	[speechSynth setDelegate:self];
	return self;
}

- (IBAction)sayIt:(id)sender
{
	NSString *string = [textField stringValue];
	
	// Is 'string' zero-length?
	if ([string length] == 0) {
		return;
	}
	
	[speechSynth startSpeakingString:string];
	NSLog(@"Have started to say: %@", string);
	[stopButton setEnabled:YES];
	[startButton setEnabled:NO];
}
- (IBAction)stopIt:(id)sender
{
	NSLog(@"stopping");
	[speechSynth stopSpeaking];
}

- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender
        didFinishSpeaking:(BOOL)finishedNormally
{
	NSLog(@"didFinish:%d", finishedNormally);
	[stopButton setEnabled:NO];
	[startButton setEnabled:YES];
}

@end
