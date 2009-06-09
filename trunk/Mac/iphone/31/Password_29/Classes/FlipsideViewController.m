//
//  FlipsideViewController.m
//  PasswordGen
//
//  Created by amuck on 8/18/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "FlipsideViewController.h"


@implementation FlipsideViewController


- (void)viewDidLoad {
	//self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];
	self.view.backgroundColor = [UIColor blackColor];
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
	[super dealloc];
}


@end
