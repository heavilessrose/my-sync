//
//  WidgetsViewController.m
//  Widgets
//
//  Created by luke on 10-1-11.
//  Copyright WeTeam 2010. All rights reserved.
//

#import "WidgetsViewController.h"
#import "FadeTransImageController.h"
#import "AdjustKeyboardTest.h"

@implementation WidgetsViewController

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	// widgets test
	/*
	FadeTransImageController *fadeContrl = [[FadeTransImageController alloc] init];
	fadeContrl.psuperView = self.view;
	[fadeContrl initTrans];
	[fadeContrl startTransAnimation];
	 */
	AdjustKeyboardTest *keyboardContrl = [[AdjustKeyboardTest alloc] init];
	[self.view addSubview:keyboardContrl.view];
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
	
    [super dealloc];
}

@end
