//
//  MainViewController.m
//  PlumbBob
//
//  Created by apple on 10/11/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"
#import <QuartzCore/QuartzCore.h>

// Constant for the number of times per second (Hertz) to sample acceleration.
#define kAccelerometerFrequency 40

@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}

 - (void)viewDidLoad {
	 
	 // set the background color to black
	 self.view.backgroundColor = [UIColor blackColor];
	 
	 // set up the plumb view
	 UIImage* image = [UIImage imageNamed:@"PlumbBob.png"];
	 plumbBobView = [[UIImageView alloc] initWithImage:image];
	 
	 // move the anchor point to the bottom middle of the metronomeArm bounds, 
	 // so rotations occur around that point
	 plumbBobView.layer.anchorPoint = CGPointMake(0.5, 0.0);
	 
	 // set the frame AFTER setting the ancor point so the view will be in the
	 // correct start position.
	 plumbBobView.frame = CGRectMake(self.view.frame.size.width/2 - 20, 0, 40, 450);
	 
	 [self.view addSubview:plumbBobView];
	 [plumbBobView release];
	 
	 // Configure and start the accelerometer
	 [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
	 [[UIAccelerometer sharedAccelerometer] setDelegate:self];
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
	[plumbBobView release];
	[super dealloc];
}

#pragma mark -
#pragma mark === Accelerometer delegate ===
#pragma mark -
// UIAccelerometerDelegate method, called when the device accelerates.
- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
	[self rotatePlumbStringToDegree:-acceleration.x* 30];
}

#pragma mark -
#pragma mark === Swing the plumb and string ===
#pragma mark -

- (void)rotatePlumbStringToDegree:(CGFloat)positionInDegrees {
    [plumbBobView.layer removeAllAnimations];
	
    CATransform3D rotationTransform = CATransform3DIdentity;
    rotationTransform = CATransform3DRotate(rotationTransform, DegreesToRadians(positionInDegrees), 0.0, 0.0, 1.0);
    plumbBobView.layer.transform = rotationTransform;
}

@end

CGFloat DegreesToRadians(CGFloat degrees) {return degrees * M_PI / 180;};
CGFloat RadiansToDegrees(CGFloat radians) {return radians * 180/M_PI;};
