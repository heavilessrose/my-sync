    //
//  LaunchCubeViewController.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LaunchCubeViewController.h"
#import "LKLauncherItem.h"


@implementation LaunchCubeViewController

@synthesize launcherView;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	
	launcherView = [[LKLauncherView alloc] initWithFrame:self.view.bounds];
	launcherView.backgroundColor = [UIColor blackColor];
	//launcherView.delegate = self;
	launcherView.columnCount = 4;
	launcherView.pages = [NSArray arrayWithObjects:
						   [NSArray arrayWithObjects:
							[[[LKLauncherItem alloc] initWithTitle:@"Button 1"
															 image:@"bundle://Icon.png"
															   URL:nil canDelete:YES] autorelease],
							[[[LKLauncherItem alloc] initWithTitle:@"Button 2"
															 image:@"bundle://Icon.png"
															   URL:nil canDelete:YES] autorelease],
							/*
							 [[[LKLauncherItem alloc] initWithTitle:@"Button 3"
							 image:@"bundle://Icon.png"
							 URL:@"fb://item3" canDelete:YES] autorelease],
							 [[[LKLauncherItem alloc] initWithTitle:@"Button 4"
							 image:@"bundle://Icon.png"
							 URL:@"fb://item4" canDelete:YES] autorelease],
							 [[[LKLauncherItem alloc] initWithTitle:@"Button 5"
							 image:@"bundle://Icon.png"
							 URL:nil canDelete:YES] autorelease],
							 [[[LKLauncherItem alloc] initWithTitle:@"Button 6"
							 image:@"bundle://Icon.png"
							 URL:nil canDelete:YES] autorelease],
							 [[[LKLauncherItem alloc] initWithTitle:@"Button 7"
							 image:@"bundle://Icon.png"
							 URL:nil canDelete:YES] autorelease],
							 nil],
							 [NSArray arrayWithObjects:
							 [[[LKLauncherItem alloc] initWithTitle:@"Button 8"
							 image:@"bundle://Icon.png"
							 URL:nil canDelete:YES] autorelease],
							 [[[LKLauncherItem alloc] initWithTitle:@"Button 9"
							 image:@"bundle://Icon.png"
							 URL:nil canDelete:YES] autorelease],
							 */
							nil],
						   nil
						   ];
	[self.view addSubview:launcherView];
	/*
	 LKLauncherItem* item = [_launcherView itemWithURL:@"fb://item3"];
	 item.badgeNumber = 4;
	 */
}


/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
	
	[launcherView release];
    [super dealloc];
}


@end
