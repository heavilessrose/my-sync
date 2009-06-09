//
//  MainViewController.m
//  AmuckColors
//
//  Created by AppsAmuck on 10/25/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"
#import "RootViewController.h"

@implementation MainViewController

@synthesize toolBar;

- (IBAction)toggleView {
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.appsamuck.com/amuckcolors.aspx"]];
	//[[RootViewController getInstance] toggleView];
}

- (IBAction)visiBone {
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.visibone.com/"]];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}

 - (void)viewDidLoad {
	 UIScrollView *scrollView = [[UIScrollView alloc] initWithFrame:self.view.frame];
	 scrollView.contentSize = CGSizeMake(847, 1129);
	 scrollView.showsHorizontalScrollIndicator = FALSE;
	 scrollView.showsVerticalScrollIndicator = FALSE;
	 UIImage *image = [UIImage imageNamed:@"chart_847.gif"];
	 UIImageView *imageView = [[UIImageView alloc] initWithImage: image];
	 [image release];
	 [scrollView addSubview:imageView];
	 [imageView release];
	 [self.view addSubview:scrollView];
	 [scrollView release];
	 
	 [self.view bringSubviewToFront:toolBar];
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
	[toolBar release];
	[super dealloc];
}


@end
