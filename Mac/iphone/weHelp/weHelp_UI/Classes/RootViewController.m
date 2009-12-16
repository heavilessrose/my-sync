//
//  RootViewController.m
//  weHelp_UI
//
//  Created by luke on 09-12-10.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "RootViewController.h"
#import "weHelp_UIViewController.h"
#import "InfoViewController.h"

@implementation RootViewController
@synthesize mainViewControl;
@synthesize infoViewControl;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

- (void)loadMainViewController
{
	if(self.mainViewControl == nil)
	{
		weHelp_UIViewController *tmp = [[weHelp_UIViewController alloc] initWithNibName:@"weHelp_UIViewController" bundle:nil];
		self.mainViewControl = tmp;
		self.mainViewControl.flipDelegate = self;
		[tmp release];
	}
}

- (void)loadInfoViewController
{
	if(self.infoViewControl == nil)
	{
		InfoViewController *tmp = [[InfoViewController alloc] initWithNibName:@"InfoViewController" bundle:nil];
		self.infoViewControl = tmp;
		self.infoViewControl.flipDelegate = self;
		[tmp release];
	}
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	[self loadMainViewController];
	[self.view addSubview:mainViewControl.view];
//	[self loadInfoViewController];
//	[self.view addSubview:infoViewControl.view];
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
	[mainViewControl release];
	if (infoViewControl != nil) {
		[infoViewControl release];
	}
    [super dealloc];
}

#pragma mark -
#pragma mark 翻转delegate
// This method is called when either of the subviews send a delegate message to us.
// It flips the displayed view from the whoever sent the message to the other.
- (void)toggleView:(id)sender {
	if (infoViewControl == nil) {
		[self loadInfoViewController];
	}
	
	UIView *mainView = mainViewControl.view;
	UIView *infoView = infoViewControl.view;
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:1];
	[UIView setAnimationTransition:((mainViewControl == sender) ? UIViewAnimationTransitionFlipFromRight : UIViewAnimationTransitionFlipFromLeft) forView:self.view cache:YES];
	
	if (mainViewControl == sender) {
		[infoViewControl viewWillAppear:YES];
		[mainViewControl viewWillDisappear:YES];
		[mainView removeFromSuperview];
		[self.view addSubview:infoView];
		[mainViewControl viewDidDisappear:YES];
		[infoViewControl viewDidAppear:YES];
	} else {
		[mainViewControl viewWillAppear:YES];
		[infoViewControl viewWillDisappear:YES];
		[infoView removeFromSuperview];
		[self.view addSubview:mainView];
		[infoViewControl viewDidDisappear:YES];
		[mainViewControl viewDidAppear:YES];
	}
	[UIView commitAnimations];
}

@end
