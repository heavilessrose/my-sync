//
//  FlipsideViewController.m
//  PhotoFrame
//
//  Created by amuck on 10/28/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "FlipsideViewController.h"
#import "RootViewController.h"
#import "MainViewController.h"
#import "PhotoPickerController.h"

@implementation FlipsideViewController

- (IBAction)doneClicked {
	[[RootViewController getInstance] toggleView];
}

- (IBAction)selectImageClicked {
	PhotoPickerController *photoPicker = [[PhotoPickerController alloc] init];
	[self presentModalViewController:photoPicker animated:TRUE];
	[photoPicker release];
}


- (void)viewDidLoad {
	self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];		
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
