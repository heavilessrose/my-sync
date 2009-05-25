//
//  GLFunAppDelegate.m
//  GLFun
//
//  Created by Jeff LaMarche on 7/31/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "GLFunViewController.h"
#import "GLFunView.h"
#import "UIColor-Random.h"

@implementation GLFunViewController
@synthesize colorControl;
- (IBAction)changeColor:(id)sender {
	UISegmentedControl *control = sender;
	NSInteger index = [control selectedSegmentIndex];
	
	GLFunView *glView = (GLFunView *)self.view;
	
	switch (index) {
		case kRedColorTab:
			glView.currentColor = [UIColor redColor];
			glView.useRandomColor = NO;
			break;
		case kBlueColorTab:
			glView.currentColor = [UIColor blueColor];
			glView.useRandomColor = NO;
			break;
		case kYellowColorTab:
			glView.currentColor = [UIColor yellowColor];
			glView.useRandomColor = NO;
			break;
		case kGreenColorTab:
			glView.currentColor = [UIColor greenColor];
			glView.useRandomColor = NO;
			break;
		case kRandomColorTab:
			glView.useRandomColor = YES;
			break;
		default:
			break;
	}
}
- (IBAction)changeShape:(id)sender {
	UISegmentedControl *control = sender;
	[(GLFunView *)self.view setShapeType:[control selectedSegmentIndex]];
	if ([control selectedSegmentIndex] == kImageShape)
		[colorControl setHidden:YES];
	else
		[colorControl setHidden:NO];
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
