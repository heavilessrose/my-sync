#import "ShakeAndBreakViewController.h"

@implementation ShakeAndBreakViewController
@synthesize imageView;
@synthesize fixed;
@synthesize broken;
- (void) viewDidLoad {
	UIAccelerometer *accel = [UIAccelerometer sharedAccelerometer];
	accel.delegate = self;
	accel.updateInterval = kUpdateInterval;
	
	NSString *path = [[NSBundle mainBundle] pathForResource:@"glass" ofType:@"wav"];
	AudioServicesCreateSystemSoundID((CFURLRef)[NSURL fileURLWithPath:path], &soundID);
	
	self.fixed = [UIImage imageNamed:@"home.png"];
	self.broken = [UIImage imageNamed:@"homebroken.png"];
	
	imageView.image = fixed;
	
	brokenScreenShowing = NO;
	
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
	[imageView release];
	[fixed release];
	[broken release];
	[super dealloc];
}
- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
	
	if (! brokenScreenShowing) {
		if (fabsf(acceleration.x) > kAccelerationThreshold || fabsf(acceleration.y) > kAccelerationThreshold || fabsf(acceleration.z) > kAccelerationThreshold) {
			imageView.image = broken;
			AudioServicesPlaySystemSound (soundID);	
			brokenScreenShowing = YES;
		}
	}
}
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	imageView.image = fixed;
	brokenScreenShowing = NO;
}
@end
