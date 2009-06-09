//
//  MainViewController.m
//  PumpkinFace
//
//  Created by AppsAmuck on 10/23/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"

@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}


 - (void)viewDidLoad {
	 
	 // create the view that will execute our animation
	 flameAnimation = [[UIImageView alloc] initWithFrame:self.view.frame];
	 
	 // load all the frames of our animation
	 flameAnimation.animationImages = [NSArray arrayWithObjects:	
									 [UIImage imageNamed:@"iFlame 001.jpg"],
									 [UIImage imageNamed:@"iFlame 002.jpg"],
									 [UIImage imageNamed:@"iFlame 003.jpg"],
									 [UIImage imageNamed:@"iFlame 004.jpg"],
									 [UIImage imageNamed:@"iFlame 005.jpg"],
									 [UIImage imageNamed:@"iFlame 006.jpg"],
									 [UIImage imageNamed:@"iFlame 007.jpg"],
									 [UIImage imageNamed:@"iFlame 008.jpg"],
									 [UIImage imageNamed:@"iFlame 009.jpg"],
									 [UIImage imageNamed:@"iFlame 010.jpg"],
									 [UIImage imageNamed:@"iFlame 009.jpg"],
									 [UIImage imageNamed:@"iFlame 008.jpg"],
									 [UIImage imageNamed:@"iFlame 007.jpg"],
									 [UIImage imageNamed:@"iFlame 006.jpg"],
									 [UIImage imageNamed:@"iFlame 005.jpg"],
									 [UIImage imageNamed:@"iFlame 004.jpg"],
									 [UIImage imageNamed:@"iFlame 003.jpg"],
									 [UIImage imageNamed:@"iFlame 002.jpg"], nil];
	 
	 // all frames will execute in 1.75 seconds
	 flameAnimation.animationDuration = 1.0;
	 // repeat the annimation forever
	 flameAnimation.animationRepeatCount = 0;
	 // start animating
	 [flameAnimation startAnimating];
	 // add the animation view to the main window 
	 [self.view addSubview:flameAnimation];
	 
	 
	 pumpkinFace = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"pumpkinFace.png"]];
	 [self.view addSubview:pumpkinFace];
 }


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc {
	[pumpkinFace release];
	[flameAnimation release];
	[super dealloc];
}


@end
