//
//  SwitchViewController.m
//  View Switcher
//
//  Created by Jeff LaMarche on 7/6/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "SwitchViewController.h"
#import "BlueViewController.h"
#import "YellowViewController.h"

@implementation SwitchViewController
@synthesize blueViewController;
@synthesize yellowViewController;

- (void)viewDidLoad
{
	BlueViewController *blueController = [[BlueViewController alloc] initWithNibName:@"BlueView" bundle:nil];
	self.blueViewController = blueController;
	[self.view insertSubview:blueController.view atIndex:0];
	[blueController release];
}
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Initialization code
	}
	return self;
}

- (IBAction)switchViews:(id)sender
{
	
	if (self.yellowViewController == nil)
	{
		YellowViewController *yellowController = [[YellowViewController alloc] 
												  initWithNibName:@"YellowView" bundle:nil];
		self.yellowViewController = yellowController;
		[yellowController release];
	}
	
	[UIView beginAnimations:@"View Flip" context:nil];
	[UIView setAnimationDuration:1.25];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	
	UIViewController *coming = nil;
	UIViewController *going = nil;
	UIViewAnimationTransition transition;
	
	if (self.blueViewController.view.superview == nil) 
	{	
		coming = blueViewController;
		going = yellowViewController;
		transition = UIViewAnimationTransitionFlipFromLeft;
	}
	else
	{
		coming = yellowViewController;
		going = blueViewController;
		transition = UIViewAnimationTransitionFlipFromRight;
	}
	
	[UIView setAnimationTransition: transition forView:self.view cache:YES];
	[coming viewWillAppear:YES];
	[going viewWillDisappear:YES];
	[going.view removeFromSuperview];
	[self.view insertSubview: coming.view atIndex:0];
	[going viewDidDisappear:YES];
	[coming viewDidAppear:YES];
	
	[UIView commitAnimations];
	
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
	[yellowViewController release];
	[blueViewController release];
	[super dealloc];
}

@end
