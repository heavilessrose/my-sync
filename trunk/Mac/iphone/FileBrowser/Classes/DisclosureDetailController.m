//
//  DisclosureDetailController.m
//  FileBrowser
//
//  Created by wang luke on 6/12/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "DisclosureDetailController.h"


@implementation DisclosureDetailController
@synthesize label;
@synthesize message;

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

// viewDidLoad只在第一次加载视图时调用。
- (void)viewWillAppear:(BOOL)animated{
	label.text = message;
	[super viewWillAppear:animated];
}

- (void)dealloc {
	[label release];
	[message release];
    [super dealloc];
}


@end
