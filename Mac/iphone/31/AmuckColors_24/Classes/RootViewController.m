//
//  RootViewController.m
//  AmuckColors
//
//  Created by AppsAmuck on 10/25/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "RootViewController.h"
#import "MainViewController.h"
#import "FlipsideViewController.h"


@implementation RootViewController

@synthesize flipsideNavigationBar;
@synthesize mainViewController;
@synthesize flipsideViewController;

static RootViewController *instance;
+ (RootViewController *)getInstance {
	return instance;
}

- (void)viewDidLoad {
	instance = self;
	MainViewController *viewController = [[MainViewController alloc] initWithNibName:@"MainView" bundle:nil];
	self.mainViewController = viewController;
	[viewController release];
	[self.view addSubview:mainViewController.view];
}


- (void)loadFlipsideViewController {
	
	FlipsideViewController *viewController = [[FlipsideViewController alloc] initWithNibName:@"FlipsideView" bundle:nil];
	self.flipsideViewController = viewController;
	[viewController release];
	
	// Set up the navigation bar
	UINavigationBar *aNavigationBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0.0, 0.0, 320.0, 44.0)];
	aNavigationBar.barStyle = UIBarStyleBlackOpaque;
	self.flipsideNavigationBar = aNavigationBar;
	[aNavigationBar release];
	
	UIBarButtonItem *buttonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(toggleView)];
	UINavigationItem *navigationItem = [[UINavigationItem alloc] initWithTitle:@"AmuckColors"];
	navigationItem.rightBarButtonItem = buttonItem;
	[flipsideNavigationBar pushNavigationItem:navigationItem animated:NO];
	[navigationItem release];
	[buttonItem release];
}


- (IBAction)toggleView {	
	/*
	 This method is called when the info or Done button is pressed.
	 It flips the displayed view from the main view to the flipside view and vice-versa.
	 */
	if (flipsideViewController == nil) {
		[self loadFlipsideViewController];
	}
	
	UIView *mainView = mainViewController.view;
	UIView *flipsideView = flipsideViewController.view;
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:1];
	[UIView setAnimationTransition:([mainView superview] ? UIViewAnimationTransitionFlipFromRight : UIViewAnimationTransitionFlipFromLeft) forView:self.view cache:YES];
	
	if ([mainView superview] != nil) {
		[flipsideViewController viewWillAppear:YES];
		[mainViewController viewWillDisappear:YES];
		[mainView removeFromSuperview];
		[self.view addSubview:flipsideView];
		[self.view insertSubview:flipsideNavigationBar aboveSubview:flipsideView];
		[mainViewController viewDidDisappear:YES];
		[flipsideViewController viewDidAppear:YES];

	} else {
		[mainViewController viewWillAppear:YES];
		[flipsideViewController viewWillDisappear:YES];
		[flipsideView removeFromSuperview];
		[flipsideNavigationBar removeFromSuperview];
		[self.view addSubview:mainView];
		[flipsideViewController viewDidDisappear:YES];
		[mainViewController viewDidAppear:YES];
	}
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
	[flipsideNavigationBar release];
	[mainViewController release];
	[flipsideViewController release];
	[super dealloc];
}


@end
