/*

File: MyView.m
Abstract: MyView contains several subviews, each of which can be moved by touch.
Illustrates handling touch events, including multiple touches.

Version: 1.7

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/


#import "MyView.h"
#import <QuartzCore/QuartzCore.h>  // Needed for animations


@interface MyView()
@property (nonatomic, retain) UIImageView *firstPieceView;
@property (nonatomic, retain) UIImageView *secondPieceView;
@property (nonatomic, retain) UIImageView *thirdPieceView;
@property (nonatomic, retain) UILabel *touchPhaseText;
@property (nonatomic, retain) UILabel *touchInfoText;
@property (nonatomic, retain) UILabel *touchTrackingText;
@property (nonatomic, retain) UILabel *touchInstructionsText;
@property (nonatomic) BOOL piecesOnTop;  
@property (nonatomic) CGPoint startTouchPosition;
@end


@interface MyView (PrivateMethods)
-(UILabel *) newLabelWithOffset:(float)offset numberOfLines:(NSUInteger) lines;
-(UIImageView *)newPieceViewWithImageNamed:(NSString *)imageName atPostion:(CGPoint)centerPoint;
-(void)animateFirstTouchAtPoint:(CGPoint)touchPoint forView:(UIImageView *)theView;
-(void)animateView:(UIImageView *)theView toPosition:(CGPoint) thePosition;
-(void) dispatchFirstTouchAtPoint:(CGPoint)touchPoint forEvent:(UIEvent *)event;
-(void) dispatchTouchEvent:(UIView *)theView toPosition:(CGPoint)position;
-(void) dispatchTouchEndEvent:(UIView *)theView toPosition:(CGPoint)position;
@end


@implementation MyView

#define GROW_ANIMATION_DURATION_SECONDS 0.15    // Determines how fast a piece size grows when it is moved.
#define SHRINK_ANIMATION_DURATION_SECONDS 0.15  // Determines how fast a piece size shrinks when a piece stops moving.

@synthesize firstPieceView;
@synthesize secondPieceView;
@synthesize thirdPieceView;
@synthesize touchPhaseText;
@synthesize touchInfoText;
@synthesize touchTrackingText;
@synthesize touchInstructionsText;
@synthesize piecesOnTop;
@synthesize startTouchPosition;

#pragma mark -
#pragma mark === Setting up and tearing down ===
#pragma mark

/*
 Unarchives the view that is stored in the xib file.
 Initializes the main view and adds three subviews, each of which have the appearance of a piece that the user can move.
 Also creates two text labels, one that displays the touch phase, and the other that displays touch information (such as swipe, number of touches). 
*/
- (id)initWithCoder:(NSCoder*)coder 
{
	if (self = [super initWithCoder:coder]) {
		
		self.backgroundColor =[UIColor blackColor];
		
		// Set up the ability to track multiple touches.
		[self setMultipleTouchEnabled:YES];
		
		// Create the three views -- each calculates its own frame based on the size of its image
		firstPieceView = [self newPieceViewWithImageNamed:@"CyanSquare.png" atPostion:self.center];
		[self addSubview:firstPieceView];
		
		secondPieceView = [self newPieceViewWithImageNamed:@"YellowSquare.png" atPostion:CGPointMake(self.center.x -100.,self.center.y -100.)];
		[self addSubview:secondPieceView];
		
		thirdPieceView = [self newPieceViewWithImageNamed:@"MagentaSquare.png" atPostion:CGPointMake(self.center.x + 100.,self.center.y + 100.0)];
		[self addSubview:thirdPieceView];	
		
		// Set up a text label to show the touch phase.
		// Create and inialize the label.
		touchPhaseText = [self newLabelWithOffset:(self.frame.origin.y + 25) numberOfLines: 1];
		// Set the default text that shows on startup.
		touchPhaseText.text = @"Touches lets you observe touch";
		// Add the label as a subview.
	    [self addSubview:touchPhaseText];
		
		// Create and inialite a text label to show the number of touches tracked.
		touchTrackingText = [self newLabelWithOffset:(self.frame.origin.y + 45) numberOfLines: 1];	
		// Set the default text that shows on startup.
		touchTrackingText.text = @"phases and multiple taps.";
		[self addSubview:touchTrackingText];
		
		// Create and inialite a text label to show multiple tap information.
		touchInfoText = [self newLabelWithOffset:(self.frame.origin.y + 65) numberOfLines: 1];

		// Add the label as a subview.
		[self addSubview:touchInfoText];
		
		
		
		// Create and intiate a label to provide instructions to the user when pieces overlap.
		touchInstructionsText = [self newLabelWithOffset:(self.frame.size.height - 45) numberOfLines: 2];
		[self addSubview:touchInstructionsText];
	}
	return self;
}

/*
 Allocates and initializes a label object. 
 Sets characteristics to default values.
*/
-(UILabel *) newLabelWithOffset:(float)offset numberOfLines:(NSUInteger) lines
{
	float textHeight = 20.0;	
	UILabel *label =  [[UILabel alloc] initWithFrame:CGRectMake(self.frame.origin.x + 10.0, offset, self.frame.size.width - 20.0, textHeight * lines)];
	// Set the font size, text color, background color, and alignment.
	label.font = [UIFont systemFontOfSize:18.0];
	label.textColor = [UIColor lightTextColor];
	label.backgroundColor = [UIColor clearColor];
	label.textAlignment = UITextAlignmentCenter;
    // Set the number of lines
	label.numberOfLines = lines;
	if (lines > 1)
		// Set the linebreak mode if there is more than one line.
		label.lineBreakMode = UILineBreakModeWordWrap;
	// Initialize as an empty string
	label.text = @"";
	return label;
	
}


/*
 Allocates and initializes a piece view object. 
 Sets characteristics to default values.
*/
-(UIImageView *)newPieceViewWithImageNamed:(NSString *)imageName atPostion:(CGPoint)centerPoint
{
	UIImage *image = [UIImage imageNamed:imageName];
	UIImageView *theView = [[UIImageView alloc] initWithImage:image];
	// Set the center of the view.
	theView.center = centerPoint;
	// Set alpha so it is slightly transparent to allow seeing pieces move over each other.
	theView.alpha = 0.9;
	// Disable user interaction for this view. You must do this if you want to handle touches for more than one object at at time.
	// You'll get events for the superview, and then dispatch them to the appropriate subview in the touch handling methods.
	theView.userInteractionEnabled = NO;
	return theView;
}


// Releases necessary resources. 
- (void)dealloc
{
	// Release each of the subviews
	[firstPieceView release];
	[secondPieceView release];
	[thirdPieceView release];
	// Release the labels
	[touchInfoText release];
	[touchPhaseText release];
	[super dealloc];	
}

#pragma mark -
#pragma mark === Touch handling  ===
#pragma mark

// Handles the start of a touch
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSUInteger numTaps = [[touches anyObject] tapCount];
	touchPhaseText.text = @"Phase: Touches began";
	touchInfoText.text = @"";
	if(numTaps >= 2) {
		touchInfoText.text = [NSString stringWithFormat:@"%d taps",numTaps]; 
		if ((numTaps == 2) && (self.piecesOnTop)) {
			// A double tap positions the three pieces in a diagonal.
			// The user will want to double tap when two or more pieces are on top of each other
			if (firstPieceView.center.x == secondPieceView.center.x)
				secondPieceView.center = CGPointMake(firstPieceView.center.x -45, firstPieceView.center.y -45);		
			if (firstPieceView.center.x == thirdPieceView.center.x)
				thirdPieceView.center  = CGPointMake(firstPieceView.center.x +45, firstPieceView.center.y +45);	
			if (secondPieceView.center.x == thirdPieceView.center.x)
				thirdPieceView.center  = CGPointMake(secondPieceView.center.x +45, secondPieceView.center.y +45);
			touchInstructionsText.text = @"";
		}
	} else {
		touchTrackingText.text = @"";
	}
	// Enumerate through all the touch objects.
	NSUInteger touchCount = 0;
	for (UITouch *touch in touches) {
	    // Send to the dispatch method, which will make sure the appropriate subview is acted upon
		[self dispatchFirstTouchAtPoint:[touch locationInView:self] forEvent:nil];
		touchCount++;  
	}	
}

/*
 Checks to see which view, or views, the point is in and then calls a method to perform the opening animation,
 which  makes the piece slightly larger, as if it is being picked up by the user.
*/
-(void) dispatchFirstTouchAtPoint:(CGPoint)touchPoint forEvent:(UIEvent *)event
{
	if (CGRectContainsPoint([firstPieceView frame], touchPoint)) {
		[self animateFirstTouchAtPoint:touchPoint forView:firstPieceView];
	}
	if (CGRectContainsPoint([secondPieceView frame], touchPoint)) {
		[self animateFirstTouchAtPoint:touchPoint forView:secondPieceView];
	} 
	if (CGRectContainsPoint([thirdPieceView frame], touchPoint)) {
		[self animateFirstTouchAtPoint:touchPoint forView:thirdPieceView];
	}
	
}

// Handles the continuation of a touch.
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{  

	NSUInteger touchCount = 0;
	touchPhaseText.text = @"Phase: Touches moved";
	// Enumerates through all touch objects
   for (UITouch *touch in touches){
		// Send to the dispatch method, which will make sure the appropriate subview is acted upon
	 	[self dispatchTouchEvent:[touch view] toPosition:[touch locationInView:self]];
	    touchCount++;
	}
	
	// When multiple touches, report the number of touches. 
	if (touchCount > 1) {
		touchTrackingText.text = [NSString stringWithFormat:@"Tracking %d touches", touchCount];
	} else {
		touchTrackingText.text = [NSString stringWithFormat:@"Tracking 1 touch", touchCount];
	}
}

/*
 Checks to see which view, or views, the point is in and then sets the center of each moved view to the new postion.
 If views are directly on top of each other, they move together.
*/
-(void) dispatchTouchEvent:(UIView *)theView toPosition:(CGPoint)position
{
	// Check to see which view, or views,  the point is in and then move to that position.
	if (CGRectContainsPoint([firstPieceView frame], position)) {
        firstPieceView.center = position;
	} 
	if (CGRectContainsPoint([secondPieceView frame], position)) {
        secondPieceView.center = position;
	} 
	if (CGRectContainsPoint([thirdPieceView frame], position)) {
		thirdPieceView.center = position;
	}
}

// Handles the end of a touch event.
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    touchPhaseText.text = @"Phase: Touches ended";
    // Enumerates through all touch object
    for (UITouch *touch in touches){
		// Sends to the dispatch method, which will make sure the appropriate subview is acted upon
		[self dispatchTouchEndEvent:[touch view] toPosition:[touch locationInView:self]];
	}
}

/*
 Checks to see which view, or views,  the point is in and then calls a method to perform the closing animation,
 which is to return the piece to its original size, as if it is being put down by the user.
*/
-(void) dispatchTouchEndEvent:(UIView *)theView toPosition:(CGPoint)position
{   
	// Check to see which view, or views,  the point is in and then animate to that position.
	if (CGRectContainsPoint([firstPieceView frame], position)) {
        [self animateView:firstPieceView toPosition: position];
	} 
	if (CGRectContainsPoint([secondPieceView frame], position)) {
        [self animateView:secondPieceView toPosition: position];
	} 
	if (CGRectContainsPoint([thirdPieceView frame], position)) {
		[self animateView:thirdPieceView toPosition: position];
	}
	// If one piece obscures another, display a message so the user can move the pieces apart
	if (((firstPieceView.center.x == secondPieceView.center.x) && (firstPieceView.center.y == secondPieceView.center.y)) ||
		((secondPieceView.center.x  == thirdPieceView.center.x ) && (secondPieceView.center.y  == thirdPieceView.center.y)) ||
		((firstPieceView.center.x  == thirdPieceView.center.x ) && (firstPieceView.center.y  == thirdPieceView.center.y))){
		touchInstructionsText.text = @"Double tap the background to move the pieces apart.";
		self.piecesOnTop = YES;
	} else {
		self.piecesOnTop = NO;
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	touchPhaseText.text = @"Phase: Touches cancelled";
    // Enumerates through all touch object
    for (UITouch *touch in touches){
		// Sends to the dispatch method, which will make sure the appropriate subview is acted upon
		[self dispatchTouchEndEvent:[touch view] toPosition:[touch locationInView:self]];
	}
}

#pragma mark -
#pragma mark === Animating subviews ===
#pragma mark

// Scales up a view slightly which makes the piece slightly larger, as if it is being picked up by the user.
- (void)animateFirstTouchAtPoint:(CGPoint)touchPoint forView:(UIImageView *)theView 
{
	// Pulse the view by scaling up, then move the view to under the finger.
	NSValue *touchPointValue = [[NSValue valueWithCGPoint:touchPoint] retain];
	[UIView beginAnimations:nil context:touchPointValue];
	[UIView setAnimationDuration:GROW_ANIMATION_DURATION_SECONDS];
	CGAffineTransform transform = CGAffineTransformMakeScale(1.2, 1.2);
	theView.transform = transform;
	[UIView commitAnimations];
}

// Scales down the view and moves it to the new position. 
- (void)animateView:(UIImageView *)theView toPosition:(CGPoint) thePosition
{
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:SHRINK_ANIMATION_DURATION_SECONDS];
	// Set the center to the final postion
	theView.center = thePosition;
	// Set the transform back to the identity, thus undoing the previous scaling effect.
	theView.transform = CGAffineTransformIdentity;
	[UIView commitAnimations];	
}


@end
