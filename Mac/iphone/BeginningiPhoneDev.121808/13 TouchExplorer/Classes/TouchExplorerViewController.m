//
//  TouchExplorerAppDelegate.m
//  TouchExplorer
//
//  Created by Jeff LaMarche on 8/2/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "TouchExplorerViewController.h"

@implementation TouchExplorerViewController
@synthesize messageLabel;
@synthesize tapsLabel;
@synthesize touchesLabel;

- (void)updateLabelsFromTouches:(NSSet *)touches {
	NSUInteger numTaps = [[touches anyObject] tapCount];
	NSString *tapsMessage = [[NSString alloc] initWithFormat:@"%d taps detected", numTaps];
	tapsLabel.text = tapsMessage;
	[tapsMessage release];
	
	NSUInteger numTouches = [touches count];
	NSString *touchesMessage = [[NSString alloc] initWithFormat:@"%d touches detected", numTouches];
	touchesLabel.text = touchesMessage;
	[touchesMessage release];
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
	[messageLabel release];
	[tapsLabel release];
	[touchesLabel release];
	[super dealloc];
}
#pragma mark -
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	messageLabel.text = @"Touches Began";
	[self updateLabelsFromTouches:touches];
	
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
	
	messageLabel.text = @"Touches Cancelled";
	[self updateLabelsFromTouches:touches];
	
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	messageLabel.text = @"Touches Stopped.";
	[self updateLabelsFromTouches:touches];
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	
	messageLabel.text = @"Drag Detected";
	[self updateLabelsFromTouches:touches];	
	
}
@end
