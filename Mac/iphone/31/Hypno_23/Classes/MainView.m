//
//  MainView.m
//  Hypno
//
//  Created by apps on 10/23/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "MainView.h"

#define RANDOM_SEED() srandom(time(NULL))
#define RANDOM_INT(__MIN__, __MAX__) ((__MIN__) + random() % ((__MAX__+1) - (__MIN__)))

@implementation MainView

-(void)awakeFromNib {
	
	[self startBackgroundMusic];
	
	self.backgroundColor = [UIColor blackColor];
	
	[NSTimer scheduledTimerWithTimeInterval:(0.1) target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
}


- (void)onTimer
{
	UIImageView *dollarView;
	
	UIImage *dollarImage = [UIImage imageNamed:@"hypno.png"];
	
	dollarView = [[UIImageView alloc] initWithImage:dollarImage];
	dollarView.alpha = 0.8;
	
	[self addSubview:dollarView];
	dollarView.frame = CGRectMake(RANDOM_INT(-150, 370), RANDOM_INT(-150, 665), 300, 300);
	
	[UIView beginAnimations:nil context:dollarView];	
	[UIView setAnimationDuration:5];
	[dollarView setAlpha:0.0];
	
	int radians = RANDOM_INT(0, 360) * 0.0174532925;		
	[dollarView setTransform:CGAffineTransformMakeRotation(radians)];
	
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(goPutThemBack:finished:context:)];
	[UIView commitAnimations];
}


- (void)goPutThemBack:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	
	UIImageView *dollarView2 = context;
	
	[dollarView2 removeFromSuperview];
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
	// Drawing code
}


- (void)dealloc {
	[super dealloc];
}


@end
