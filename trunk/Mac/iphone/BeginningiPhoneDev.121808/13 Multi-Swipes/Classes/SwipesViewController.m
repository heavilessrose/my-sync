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
	
	
	SwipeType swipeType = kNoSwipe;	
	for (UITouch *touch in touches) {

			
			CGPoint currentPosition = [touch locationInView:self.view];
			
			CGFloat deltaX = fabsf(currentPosition.x - gestureStartPoint.x);
			CGFloat deltaY = fabsf(currentPosition.y - gestureStartPoint.y);
			
			if (deltaX >= kMinimumGestureLength && deltaY <= kMaximumVariance)
				swipeType = kHorizontalSwipe;
			else if (deltaY >= kMinimumGestureLength && deltaX <= kMaximumVariance)
				swipeType = kVerticalSwipe;
	}
	
	BOOL allFingersFarEnoughAway = YES;
	if (swipeType != kNoSwipe) {
		for (UITouch *touch in touches) {
			CGPoint currentPosition = [touch locationInView:self.view];
			
			CGFloat distance;
			if (swipeType == kHorizontalSwipe)
				distance = fabsf(currentPosition.x - gestureStartPoint.x);
			else
				distance = fabsf(currentPosition.y - gestureStartPoint.y);
			
			if (distance < kMinimumGestureLength)
				allFingersFarEnoughAway = NO;
		}
	}
	if (allFingersFarEnoughAway && swipeType != kNoSwipe)
	{
		NSString *swipeCountString= nil;
		if ([touches count] == 2)
			swipeCountString = @"Double ";
		else if ([touches count] == 3)
			swipeCountString = @"Triple ";
		else if ([touches count] == 4)
			swipeCountString = @"Quadruple ";
		else
			swipeCountString = @"";
		
		NSString *swipeTypeString = (swipeType == kHorizontalSwipe) ? @"Horizontal" : @"Vertical";
		
		NSString *message = [[NSString alloc] initWithFormat:@"%@%@ Swipe Detected.", swipeCountString, swipeTypeString];
		label.text = message;
		[message release];
		[self performSelector:@selector(eraseText) withObject:nil afterDelay:2];
		
	}
}	



@end
