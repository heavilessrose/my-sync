//
//  MainView.m
//  Sierpinski
//
//  Created by apps on 10/25/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "MainView.h"

#define RANDOM_SEED() srandom(time(NULL))
#define RANDOM_INT(__MIN__, __MAX__) ((__MIN__) + random() % ((__MAX__+1) - (__MIN__)))

@implementation MainView

int x = 140;
int y = 140;
int c = 0;
int n = 0;

-(void)awakeFromNib {
	
	self.backgroundColor = [UIColor blackColor];
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
	SoundEngine_LoadBackgroundMusicTrack([[bundle pathForResource:@"letsgo" ofType:@"m4v"] UTF8String], true, true);
	
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
	
	while (c++ < 100000) {

		CGContextRef context = UIGraphicsGetCurrentContext();
		
		// Drawing lines with a white stroke color
		CGContextSetRGBStrokeColor(context, y/140.0, (140.0-x)/140.0, x/140, 1.0);
		CGContextSetLineWidth(context, 1.0);
		
		// Draw a single line from left to right
		CGContextMoveToPoint(context, x+10, 10+240-y);
		CGContextAddLineToPoint(context, x+10+1, 10+240-y+1);
		CGContextStrokePath(context);
		
		n = RANDOM_INT(0, 999);
		
		if(n < 333)
		{
			x = .5 * (x + 300);
			y = .5 * y;
		}
		
		if(n > 333 && n < 666)
		{
			x = x * .5;
			y = y * .5;
		}
		
		if(n > 666)
		{
			x = .5 * (x + 140);
			y = .5 * (y + 240);
		}
		
		[self setNeedsDisplay];
		
	}
}


- (void)dealloc {
	[super dealloc];
}


@end
