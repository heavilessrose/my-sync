//
//  CheckPleaseAppDelegate.m
//  CheckPlease
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "CheckPleaseViewController.h"
#import "CGPointUtils.h"

@implementation CheckPleaseViewController
@synthesize label;
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
	UITouch *touch = [touches anyObject];
	CGPoint point = [touch locationInView:self.view];
	lastPreviousPoint = point;
	lastCurrentPoint = point;
	lineLengthSoFar = 0.0f;
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	
	UITouch *touch = [touches anyObject];
	CGPoint previousPoint = [touch previousLocationInView:self.view];
	CGPoint currentPoint = [touch locationInView:self.view];
	CGFloat angle = angleBetweenLines(lastPreviousPoint, lastCurrentPoint, previousPoint, currentPoint);
	
	if (angle >= kMinimumCheckMarkAngle&& angle <= kMaximumCheckMarkAngle && lineLengthSoFar > kMinimumCheckMarkLength){
		label.text = @"Checkmark";
		[self performSelector:@selector(eraseLabel) withObject:nil afterDelay:1.6];
	}
	
	lineLengthSoFar += distanceBetweenPoints(previousPoint, currentPoint);
	lastPreviousPoint = previousPoint;
	lastCurrentPoint = currentPoint;
}

@end
