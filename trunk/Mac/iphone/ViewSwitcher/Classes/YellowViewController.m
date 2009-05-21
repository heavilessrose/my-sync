//
//  YellowViewController.m
//  ViewSwitcher
//
//  Created by wang luke on 5/21/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "YellowViewController.h"


@implementation YellowViewController

- (IBAction) yellowButtonPressed:(id)sender
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Yellow View Button Pressed" message:@"you press button on the Yellow view" delegate:nil cancelButtonTitle:@"yes,i did" otherButtonTitles:nil];
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
