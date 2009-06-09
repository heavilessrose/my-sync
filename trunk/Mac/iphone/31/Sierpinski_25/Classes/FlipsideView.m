//
//  FlipsideView.m
//  Sierpinski
//
//  Created by apps on 10/25/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "FlipsideView.h"

@implementation FlipsideView
- (IBAction)showMeTheWay {
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.AppsAmuck.com/"]];
}

-(void)awakeFromNib {
	
	[self startBackgroundMusic];
}

- (void) startBackgroundMusic{
	
	NSBundle* bundle = [NSBundle mainBundle];
	
	// Note that each of the Sound Engine functions defined in SoundEngine.h return an OSStatus value.
	// Although the code in this application does not check for errors, you'll want to add error checking code 
	// in your own application, particularly during development.
	//Setup sound engine. Run  it at 44Khz to match the sound files
	SoundEngine_Initialize(44100);
	// Assume the listener is in the center at the start. The sound will pan as the position of the rocket changes.
	SoundEngine_SetListenerPosition(0.0, 0.0, kListenerDistance);
	// Load each of the four sounds used in the game.
	SoundEngine_LoadBackgroundMusicTrack([[bundle pathForResource:@"thx" ofType:@"m4v"] UTF8String], true, true);
	
	//Play start sound
	SoundEngine_StartBackgroundMusic();
}


- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)drawRect:(CGRect)rect {
	// Drawing code
}


- (void)dealloc {
	[super dealloc];
}


@end
