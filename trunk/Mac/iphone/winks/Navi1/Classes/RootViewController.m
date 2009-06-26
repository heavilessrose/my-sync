//
//  RootViewController.m
//  Navi1
//
//  Created by wang luke on 6/25/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "RootViewController.h"
#import "FirstViewController.h"


@implementation RootViewController
@synthesize pressMe;

- (void)viewDidLoad{
	self.title = @"RootLevel";
	UIImage *buttonImageNormal = [UIImage imageNamed:@"whiteButton.png"];
	UIImage *stretchableButtonImageNormal = [buttonImageNormal stretchableImageWithLeftCapWidth:12 topCapHeight:0];
	[pressMe setBackgroundImage:stretchableButtonImageNormal forState:UIControlStateNormal];
	
	UIImage *buttonImagePressed = [UIImage imageNamed:@"blueButton.png"];
	UIImage *stretchableButtonImagePressed = [buttonImagePressed stretchableImageWithLeftCapWidth:12 topCapHeight:0];
	[pressMe setBackgroundImage:stretchableButtonImagePressed forState:UIControlStateHighlighted];
	
	[super viewDidLoad];
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

#pragma mark -
// 切换到firstView
- (IBAction)pressMe:(id)sender{
	FirstViewController *firstViewController = [[FirstViewController alloc] initWithNibName:@"FirstView" bundle:nil];
	[self.navigationController pushViewController:firstViewController animated:YES];
	[firstViewController release];
}
@end
