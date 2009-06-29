//
//  FirstViewController.m
//  Navi1
//
//  Created by wang luke on 6/25/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "FirstViewController.h"
#import "SecondViewController.h"


@implementation FirstViewController
@synthesize pressMe;

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	self.title = @"FirstView";
	
	UIImage *buttonImageNormal = [UIImage imageNamed:@"whiteButton.png"];
	UIImage *stretchableButtonImageNormal = [buttonImageNormal stretchableImageWithLeftCapWidth:12 topCapHeight:0];
	[pressMe setBackgroundImage:stretchableButtonImageNormal forState:UIControlStateNormal];
	
	UIImage *buttonImagePressed = [UIImage imageNamed:@"blueButton.png"];
	UIImage *stretchableButtonImagePressed = [buttonImagePressed stretchableImageWithLeftCapWidth:12 topCapHeight:0];
	[pressMe setBackgroundImage:stretchableButtonImagePressed forState:UIControlStateHighlighted];
	
    [super viewDidLoad];
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

#pragma mark -
// 切换到secondView
- (IBAction)pressMe:(id)sender{
	SecondViewController *secondViewController = [[SecondViewController alloc] initWithNibName:@"SecondView" bundle:nil];
	[self.navigationController pushViewController:secondViewController animated:YES];
	[secondViewController release];
}
@end
