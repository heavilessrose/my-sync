//
//  SwipesAppDelegate.m
//  Swipes
//
//  Created by Jeff LaMarche on 8/2/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "SwipesViewController.h"

@implementation SwipesViewController
@synthesize label;
@synthesize gestureStartPoint;
- (void)eraseText
{
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
	
	UITouch *touch = [touches anyObject];
	gestureStartPoint = [touch locationInView:self.view];
	
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	
	UITouch *touch = [touches anyObject];
	CGPoint currentPosition = [touch locationInView:self.view];
	
	CGFloat deltaX = fabsf(gestureStartPoint.x - currentPosition.x);
	CGFloat deltaY = fabsf(gestureStartPoint.y - currentPosition.y);
	
	if (deltaX >= kMinimumGestureLength && deltaY <= kMaximumVariance) {
		label.text = @"Horizontal swipe detected";
		[self performSelector:@selector(eraseText) withObject:nil afterDelay:2];
	}	
	else if (deltaY >= kMinimumGestureLength && deltaX <= kMaximumVariance){
		label.text = @"Vertical swipe detected";
		[self performSelector:@selector(eraseText) withObject:nil afterDelay:1.5];
	}	
	
}

@end
