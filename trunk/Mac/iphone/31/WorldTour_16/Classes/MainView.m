//
//  MainView.m
//  WorldTour
//
//

#import "MainView.h"
#import <QuartzCore/QuartzCore.h>
#import "SoundEngine.h"

#define kListenerDistance			1.0  // Used for creating a realistic sound field

@implementation MainView

UIImage *apple1;
UIImage *apple2;
UIImage *apple3;
UIImage *apple4;

UIImage *currentImage;

int tileIndex = 0;
CGFloat direction = 5.0;

-(void)awakeFromNib {
	
	[self startBackgroundMusic];
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:1];
	[schedule setAlpha:0.9];
	[UIView commitAnimations];
	
	apple1 = [UIImage imageNamed:@"apple1.png"];
	apple2 = [UIImage imageNamed:@"apple2.png"];
	apple3 = [UIImage imageNamed:@"apple3.png"];
	apple4 = [UIImage imageNamed:@"apple4.png"];
	
	currentImage = apple1;

	[NSTimer scheduledTimerWithTimeInterval:(0.1) target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
	[NSTimer scheduledTimerWithTimeInterval:(2.0) target:self selector:@selector(onTimerAnimate) userInfo:nil repeats:YES];
}

- (void)onTimer
{
	tileIndex += 1;
	
	if(currentImage == apple1)
		currentImage = apple2;
	else if (currentImage == apple2)
		currentImage = apple3;
	else if (currentImage == apple3)
		currentImage = apple4;
	else if (currentImage == apple4)
		currentImage = apple1;
	
	[self setNeedsDisplay];
}

- (void)onTimerAnimate
{
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:1];
	
	CGFloat radians = direction * 0.0174532925;
	[schedule setTransform:CGAffineTransformMakeRotation(radians)];
	
	[UIView commitAnimations];
	
	direction = direction * -1;
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

- (void)drawRect:(CGRect)rect {
	
	CGImageRef image = CGImageRetain(currentImage.CGImage);
	
	CGRect imageRect;
	imageRect.origin = CGPointMake(tileIndex, tileIndex);
	imageRect.size = CGSizeMake(64.0, 64.0);
	
	CGContextRef uiContext = UIGraphicsGetCurrentContext();
	
	CGContextClipToRect(uiContext, CGRectMake(0.0, 0.0, rect.size.width, rect.size.height));
	
	CGContextDrawTiledImage(uiContext, imageRect, image);
	
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
