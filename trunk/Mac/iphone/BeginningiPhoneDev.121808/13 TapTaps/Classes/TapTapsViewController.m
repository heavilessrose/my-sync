//
//  TapTapsAppDelegate.m
//  TapTaps
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "TapTapsViewController.h"

@implementation TapTapsViewController
@synthesize singleLabel;
@synthesize doubleLabel;
@synthesize tripleLabel;
@synthesize quadrupleLabel;
- (void)singleTap {
	singleLabel.text = @"Single Tap Detected";
	[self performSelector:@selector(eraseMe:) withObject:singleLabel afterDelay:1.6f];
}
- (void)doubleTap {
	doubleLabel.text = @"Double Tap Detected";
	[self performSelector:@selector(eraseMe:) withObject:doubleLabel afterDelay:1.6f];
}
- (void)tripleTap {
	tripleLabel.text = @"Triple Tap Detected";
	[self performSelector:@selector(eraseMe:) withObject:tripleLabel afterDelay:1.6f];
}
- (void)quadrupleTap {
	quadrupleLabel.text = @"Quadruple Tap Detected";
	[self performSelector:@selector(eraseMe:) withObject:quadrupleLabel afterDelay:1.6f];
}
- (void)eraseMe:(UITextField *)textField {
	textField.text = @"";
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
	[singleLabel release];
	[doubleLabel release];
	[tripleLabel release];
	[quadrupleLabel release];
	[super dealloc];
}
#pragma mark -
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	UITouch *touch = [touches anyObject];
	NSUInteger tapCount = [touch tapCount];
	
	switch (tapCount) {
		case 1:
			[self singleTap];
			break;
		case 2:
			[self doubleTap];
			break;
		case 3:
			[self tripleTap];
			break;
		case 4:
			[self quadrupleTap];
			break;
		default:
			break;
	}

}
@end
