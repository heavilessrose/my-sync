//
//  AutosizeAppDelegate.m
//  Autosize
//
//  Created by Jeff LaMarche on 6/15/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "AutosizeViewController.h"

@implementation AutosizeViewController
@synthesize button1;
@synthesize button2;
@synthesize button3;
@synthesize button4;
@synthesize button5;
@synthesize button6;

//- (void)willAnimateFirstHalfOfRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
- (void)willAnimateSecondHalfOfRotationFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation duration:(NSTimeInterval)duration {
	
	UIInterfaceOrientation toInterfaceOrientation = self.interfaceOrientation;
	[UIView beginAnimations:@"move buttons" context:nil];

	if (toInterfaceOrientation == UIInterfaceOrientationPortrait 
		|| toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
	{
		button1.frame = CGRectMake(20, 20, 125, 125);
		button2.frame = CGRectMake(175, 20, 125, 125);
		button3.frame = CGRectMake(20, 168, 125, 125);
		button4.frame = CGRectMake(175, 168, 125, 125);
		button5.frame = CGRectMake(20, 315, 125, 125);
		button6.frame = CGRectMake(175, 315, 125, 125);
	}
	else 
	{
		button1.frame = CGRectMake(20, 20, 125, 125);
		button2.frame = CGRectMake(20, 155, 125, 125);
		button3.frame = CGRectMake(177, 20, 125, 125);
		button4.frame = CGRectMake(177, 155, 125, 125);
		button5.frame = CGRectMake(328, 20, 125, 125);
		button6.frame = CGRectMake(328, 155, 125, 125);
	}
	
	[UIView commitAnimations];
}
/*
 Implement loadView if you want to create a view hierarchy programmatically
 - (void)loadView {
 }
 */

/*
 Implement viewDidLoad if you need to do additional setup after loading the view.
 - (void)viewDidLoad {
 [super viewDidLoad];
 }
 */


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations
	/*
	 UIInterfaceOrientationPortrait          
	 UIInterfaceOrientationPortraitUpsideDown
	 UIInterfaceOrientationLandscapeLeft     
	 UIInterfaceOrientationLandscapeRight    
	 */
	return (UIInterfaceOrientationPortrait || UIInterfaceOrientationPortraitUpsideDown);
}

- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc {
	[button1 release];
	[button2 release];
	[button3 release];
	[button4 release];
	[button5 release];
	[button6 release];
	
	[super dealloc];
}

@end
