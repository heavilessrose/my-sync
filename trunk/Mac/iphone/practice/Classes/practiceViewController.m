//
//  practiceViewController.m
//  practice
//
//  Created by wang luke on 4/23/09.
//  Copyright luke 2009. All rights reserved.
//

#import "practiceViewController.h"

@implementation practiceViewController

@synthesize statusText;

- (IBAction)buttonPressed:(id)sender
{
	NSString *title = [sender titleForState:UIControlStateNormal];
	NSString *newTitle = [[NSString alloc] initWithFormat:@"%@ is pressed.", title];
	statusText.text = newTitle;
	[newTitle release];
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
	[statusText release];
    [super dealloc];
}

@end
