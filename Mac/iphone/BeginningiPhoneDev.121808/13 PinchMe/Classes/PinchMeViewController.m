//
//  PinchMeAppDelegate.m
//  PinchMe
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "PinchMeViewController.h"
#import "CGPointUtils.h"

@implementation PinchMeViewController

@synthesize label;
@synthesize initialDistance;
- (void)eraseLabel {
	label.text = @"";
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
	[label release];
	[super dealloc];
}
#pragma mark -
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	if ([touches count] == 2) {
		NSArray *twoTouches = [touches allObjects];
		UITouch *first = [twoTouches objectAtIndex:0];
		UITouch *second = [twoTouches objectAtIndex:1];
		initialDistance = distanceBetweenPoints([first locationInView:self.view], [second locationInView:self.view]);		
	}
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	
	if ([touches count] == 2) {
		NSArray *twoTouches = [touches allObjects];
		UITouch *first = [twoTouches objectAtIndex:0];
		UITouch *second = [twoTouches objectAtIndex:1];
		CGFloat currentDistance = distanceBetweenPoints([first locationInView:self.view], [second locationInView:self.view]);

		if (initialDistance == 0)
			initialDistance = currentDistance; 
		if (currentDistance - initialDistance > kMinimumPinchDelta) {
			label.text = @"Outward Pinch";
			[self performSelector:@selector(eraseLabel) withObject:nil afterDelay:1.6f];
		}
		else if (initialDistance - currentDistance > kMinimumPinchDelta) {
			label.text = @"Inward Pinch";
			[self performSelector:@selector(eraseLabel) withObject:nil afterDelay:1.6f];
		}
	}
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	initialDistance = 0;
}

@end
