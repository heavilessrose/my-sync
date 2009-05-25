//
//  QuartzFunAppDelegate.m
//  QuartzFun
//
//  Created by Jeff LaMarche on 7/31/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "QuartzFunViewController.h"
#import "QuartzFunView.h"
#import "UIColor-Random.h"
#import "Constants.h"

@implementation QuartzFunViewController
@synthesize colorControl;

- (IBAction)changeColor:(id)sender {
	UISegmentedControl *control = sender;
	NSInteger index = [control selectedSegmentIndex];
	
	QuartzFunView *quartzView = (QuartzFunView *)self.view;
	
	switch (index) {
		case kRedColorTab:
			quartzView.currentColor = [UIColor redColor];
			quartzView.useRandomColor = NO;
			break;
		case kBlueColorTab:
			quartzView.currentColor = [UIColor blueColor];
			quartzView.useRandomColor = NO;
			break;
		case kYellowColorTab:
			quartzView.currentColor = [UIColor yellowColor];
			quartzView.useRandomColor = NO;
			break;
		case kGreenColorTab:
			quartzView.currentColor = [UIColor greenColor];
			quartzView.useRandomColor = NO;
			break;
		case kRandomColorTab:
			quartzView.useRandomColor = YES;
			break;
		default:
			break;
	}
}
- (IBAction)changeShape:(id)sender {
	UISegmentedControl *control = sender;
	[(QuartzFunView *)self.view setShapeType:[control selectedSegmentIndex]];
	
	if ([control selectedSegmentIndex] == kImageShape)
		[colorControl setHidden:YES];
	else
		[colorControl setHidden:NO];
}

- (void)viewDidLoad {
	
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
	[colorControl release];
	[super dealloc];
}

@end
