//
//  LKStyledTextViewController.m
//  LKStyledText
//
//  Created by luke on 10-11-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKStyledTextViewController.h"
#import "LKStyledText.h"
#import "LKStyledTextView.h"


@implementation LKStyledTextViewController

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	LKStyledText *stxt = [[[LKStyledText alloc] initFotTest] autorelease];
	LKStyledTextView *stxtView = [[LKStyledTextView alloc] initWithFrame:self.view.bounds];
	stxtView.backgroundColor = [UIColor whiteColor];
	stxtView.stext = stxt;
	[stxtView.stext layout];
	[self.view addSubview:stxtView];
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
    [super dealloc];
}

@end
