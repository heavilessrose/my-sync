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
}
- (IBAction)stopIt:(id)sender
{
	NSLog(@"stopping");
	[speechSynth stopSpeaking];
}

@end
