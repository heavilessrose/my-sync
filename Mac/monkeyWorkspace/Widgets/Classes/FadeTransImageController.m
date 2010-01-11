//
//  FadeTransImageController.m
//  Widgets
//
//  Created by luke on 10-1-11.
//  Copyright 2010 WeTeam. All rights reserved.
//

#import "FadeTransImageController.h"


@implementation FadeTransImageController

@synthesize layer1, layer2, psuperView;
@synthesize transTimer;

#pragma mark -
#pragma mark fade

#define USE_TIMER_TRANS

#define FADE_IN_RATE		1.0/500.0
#define FADE_OUT_RATE		1.0/500.0
#define FADE_TIMER          0.005f
// 换图
static BOOL shouldChange = NO;

- (void)initTrans
{
	self.layer1 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"image1.png"]];
	layer1.alpha = 0;
	[psuperView addSubview:layer1];
	
	self.layer2 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"image2.png"]];
	[psuperView addSubview:layer2];
}

- (void)startTransAnimation
{
#ifdef USE_TIMER_TRANS
	// Call the timer every one hundreth of a second
	transTimer = [NSTimer scheduledTimerWithTimeInterval:FADE_TIMER target:self 
												selector:@selector(transTimerEvent) userInfo:nil repeats:YES];
#else
	// core animation
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:5];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
	[UIView setAnimationsEnabled:YES];
	//[UIView setAnimationDidStopSelector:@selector(animationEnded)];
	[UIView setAnimationDelegate:self];
	layer2.alpha = 0;
	[UIView commitAnimations];
#endif
}

- (void)stopTransAnimation
{
	[transTimer invalidate];
    transTimer = nil;      
}

- (void)transTimerEvent
{
	if (shouldChange) {
		layer2.alpha += FADE_IN_RATE;
		layer1.alpha -= FADE_OUT_RATE;
	} else {
		layer1.alpha += FADE_IN_RATE;
		layer2.alpha -= FADE_OUT_RATE;
	}
	
	if (layer1.alpha >= 1 || layer1.alpha <= 0) {
		shouldChange = !shouldChange;
	}
}

#pragma mark -
#pragma mark app
/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	[self startTransAnimation];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
	NSLog(@"FadeTransImageController dealloc...");
	[self stopTransAnimation];
	[layer1 release];
	[layer2 release];
	[psuperView release];
	
    [super dealloc];
	NSLog(@"FadeTransImageController dealloc...end");
}


@end
