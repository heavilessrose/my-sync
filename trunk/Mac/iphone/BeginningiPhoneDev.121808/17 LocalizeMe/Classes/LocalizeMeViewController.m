#import "LocalizeMeViewController.h"

@implementation LocalizeMeViewController
@synthesize localeLabel;
@synthesize label1;
@synthesize label2;
@synthesize label3;
@synthesize label4;
@synthesize label5;

- (void)viewDidLoad {
	
	NSLocale *locale = [NSLocale currentLocale];
	NSLog (@"%@", [locale localeIdentifier]);
	NSString *displayNameString = [locale displayNameForKey:NSLocaleIdentifier 
													  value:[locale localeIdentifier]];
	localeLabel.text = displayNameString;
	
	@try {
		label1.text = NSLocalizedString(@"One", @"The number 1");
		label2.text = NSLocalizedString(@"Two", @"The number 2");
		label3.text = NSLocalizedString(@"Three", @"The number 3");
		label4.text = NSLocalizedString(@"Four", @"The number 4");
		label5.text = NSLocalizedString(@"Five", @"The number 5");
	}
	@catch (NSException * e) {
	}
	
	[super viewDidLoad];
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
	[localeLabel release];
	[label1 release];
	[label2 release];
	[label3 release];
	[label4 release];
	[label5 release];
	[super dealloc];
}
@end
