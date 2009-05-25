/*

File: ViewTransitionsAppDelegate.m
Abstract: The UIApplication delegate class, the central controller of the
application.

Version: 1.6

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

#import "ViewTransitionsAppDelegate.h"
#import <QuartzCore/QuartzCore.h>

@implementation ViewTransitionsAppDelegate

@synthesize window;
@synthesize view1;
@synthesize view2;

- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	// Override point for customization after app launch
    
    [window setBackgroundColor:[UIColor blackColor]];
    
    // Create the two views to transition between and add the first one to the image view, 
    // which is the content view of the window.
	view1 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"image1.jpg"]];
	view2 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"image2.jpg"]];
    CGRect view1Frame = view1.frame;
    view1Frame.size.height = transitionView.frame.size.height;
    [transitionView addSubview:view1];
    [transitionView setDelegate:self];
}


- (void)dealloc {
	[window release];
    [transitionView release];
    [view1 release];
    [view2 release];
	[super dealloc];
}

// TransitionViewDelegate methods. 
- (void)transitionViewDidStart:(TransitionView *)view {
}
- (void)transitionViewDidFinish:(TransitionView *)view {
}
- (void)transitionViewDidCancel:(TransitionView *)view {
}

- (void)performTransition {
	
	if([transitionView isTransitioning]) {
		// Don't interrupt an ongoing transition
		return;
	}
        
	// Chose transition type and direction at random from the arrays of supported transitions and directions
	NSArray *transitions = [NSArray arrayWithObjects:kCATransitionMoveIn, kCATransitionPush, kCATransitionReveal, kCATransitionFade, nil];
	NSUInteger transitionsIndex = random() % [transitions count];
	NSString *transition = [transitions objectAtIndex:transitionsIndex];
	
	NSArray *directions = [NSArray arrayWithObjects:kCATransitionFromLeft, kCATransitionFromRight, kCATransitionFromTop, kCATransitionFromBottom, nil];
	NSUInteger directionsIndex = random() % [directions count];
	NSString *direction = [directions objectAtIndex:directionsIndex];
    
	// If view 1 is already a subview of the transition view replace it with view 2, and vice-versa.
	if([view1 superview]) {
		[transitionView replaceSubview:view1 withSubview:view2 transition:transition direction:direction duration:0.75];
	} else {
		[transitionView replaceSubview:view2 withSubview:view1 transition:transition direction:direction duration:0.75];
    }
}

- (IBAction)nextTransition:(id)sender {
    [self performTransition];
}


@end
