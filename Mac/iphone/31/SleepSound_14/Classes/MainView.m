//
//  MainView.m
//  Waterfall
//
//

#import "MainView.h"
#import <QuartzCore/QuartzCore.h>
#import "SoundEngine.h"

#define kListenerDistance			1.0  // Used for creating a realistic sound field

@implementation MainView

- (IBAction)playOceanWaves {

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
	SoundEngine_LoadBackgroundMusicTrack([[bundle pathForResource:@"oceanwaves" ofType:@"m4v"] UTF8String], true, true);
	
	//Play start sound
	SoundEngine_StartBackgroundMusic();
	
}

- (void)drawRect:(CGRect)rect {
	
}


- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)dealloc {
	[super dealloc];
}


@end
