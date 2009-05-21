//
//  BlueViewController.m
//  ViewSwitcher
//
//  Created by wang luke on 5/21/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "BlueViewController.h"


@implementation BlueViewController

- (IBAction) blueButtonPressed:(id)sender
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Blue View Button Pressed" message:@"you press button on the blue view" delegate:nil cancelButtonTitle:@"yes,i did" otherButtonTitles:nil];
	[alert show];
	[alert release];
}

// Override to allow orientations other than the default portrait orientation.
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
