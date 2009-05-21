//
//  SwitchViewController.m
//  ViewSwitcher
//
//  Created by wang luke on 5/21/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "SwitchViewController.h"
#import "BlueViewController.h"
#import "YellowViewController.h"

@implementation SwitchViewController
@synthesize blueViewController;
@synthesize yellowViewController;
@synthesize bar;
@synthesize barButton;

- (IBAction) switchViews:(id)sender
{
	NSLog(@"called ");
	// lazy load: 按钮第一次按下时加载Yellow nib
	if(self.yellowViewController == nil){
		YellowViewController *yellowController = [[YellowViewController alloc] initWithNibName:@"YellowView" bundle:nil];
		self.yellowViewController = yellowController;
		[yellowController release];
	}
	
	if(self.blueViewController.view.superview == nil){
		[yellowViewController.view removeFromSuperview];
		[self.view insertSubview:blueViewController.view atIndex:0];
	}else{
		[blueViewController.view removeFromSuperview];
		[self.view insertSubview:yellowViewController.view atIndex:0];
	}
}

- (void)dealloc {
	[yellowViewController release];
	[blueViewController release];
    [super dealloc];
}

#pragma mark -
#pragma mark Application delegate
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

#pragma mark -
#pragma mark 覆盖UIViewController 的方法
// 加载nib完成后调用
- (void) viewDidLoad
{
	BlueViewController *blueController = [[BlueViewController alloc] initWithNibName:@"BlueView" bundle:nil];
	self.blueViewController = blueController;
	[self.view insertSubview:blueController.view atIndex:0];
	[blueController release];
}

@end
