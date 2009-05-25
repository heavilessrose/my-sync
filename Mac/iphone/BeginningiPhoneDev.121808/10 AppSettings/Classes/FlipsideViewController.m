#import "FlipsideViewController.h"
#import "MainViewController.h"

@implementation FlipsideViewController
@synthesize engineSwitch;
@synthesize warpFactorSlider;

- (void)viewDidLoad {
	//self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];	
	
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	engineSwitch.on  = ([[defaults objectForKey:kWarpDriveKey] isEqualToString:@"Engaged"]) ? YES : NO;
	warpFactorSlider.value = [defaults floatForKey:kWarpFactorKey];
}
- (void)viewWillDisappear:(BOOL)animated
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSString *prefValue = (engineSwitch.on) ? @"Engaged" : @"Disabled";
	[defaults setObject:prefValue forKey:kWarpDriveKey];
	[defaults setFloat:warpFactorSlider.value forKey:kWarpFactorKey];
	[super viewWillDisappear:animated];
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
	[engineSwitch release];
	[warpFactorSlider release];
	[super dealloc];
}
@end