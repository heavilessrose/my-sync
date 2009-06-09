//
//  MainViewController.m
//  PhotoFrame
//
//  Created by amuck on 10/28/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"

@implementation MainViewController

static MainViewController *sharedInstance;
+ (MainViewController *)getInstance {
	return sharedInstance;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}

- (void)loadImage {
	if(imageView) {
		[imageView removeFromSuperview];
	}
	
	NSString *imagePath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"] stringByAppendingPathComponent:@"selectedImage.png"];
	UIImage *image = [UIImage imageWithContentsOfFile:imagePath];
	imageView = [[UIImageView alloc] initWithImage:image];
	[image release];
	imageView.frame = CGRectMake(0, 0, 320, 480);
	[self.view addSubview:imageView];
	
	[self.view bringSubviewToFront:frameView];
}

- (void)viewDidLoad {
	sharedInstance = self;

	UIImage *image = [UIImage imageNamed:@"gframe.png"];
	frameView = [[UIImageView alloc] initWithImage:image];
	[image release];
	frameView.frame = CGRectMake(0, 0, 320, 480);
	[self.view addSubview:frameView];
 }


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc {
	[frameView release];
	if(imageView)
		[imageView release];
	[super dealloc];
}


@end
