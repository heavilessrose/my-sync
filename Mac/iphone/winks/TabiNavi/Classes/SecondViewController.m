//
//  SecondViewController.m
//  TabiNavi
//
//  Created by wang luke on 6/29/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "SecondViewController.h"
#import "ThirdViewController.h"

@implementation SecondViewController
@synthesize next;

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
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
	self.title = @"SecondView";
    [super viewDidLoad];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc {
    [super dealloc];
}


#pragma mark -
// 切换到ThirdView
- (IBAction)nextPressed:(id)sender{
	ThirdViewController *thirdViewController = [[ThirdViewController alloc] initWithNibName:@"ThirdView" bundle:nil];
	[self.navigationController pushViewController:thirdViewController animated:YES];
	[thirdViewController release];
}
@end
