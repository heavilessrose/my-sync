//
//  MainViewController.m
//  Fireball
//
//  Created by AppsAmuck on 10/17/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"

@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}

- (void)removeSmoke:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {	
	UIImageView *imageView = context;
	[imageView removeFromSuperview];
}

-(void) onTimer {
	float x = fireBall.center.x;
	float y = fireBall.center.y;
	fireBall.center = CGPointMake(fireBall.center.x + pos.x, fireBall.center.y + pos.y);
	
	if (fireBall.center.x > 320 || fireBall.center.x < 0)
		pos.x = -pos.x;
	if (fireBall.center.y > 460 || fireBall.center.y < 0)
		pos.y = -pos.y;
	
	UIImageView* imageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"smokeball.png"]];
	imageView.frame = CGRectMake(x-16, y-16, 32, 32);
	[self.view addSubview:imageView];
	[UIView beginAnimations:nil context:imageView];
	[UIView setAnimationDuration:3.0];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
	imageView.frame = CGRectMake(x-6, y-6, 12, 12);
	[imageView setAlpha:0.0];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(removeSmoke:finished:context:)];
	[UIView commitAnimations];	
	[imageView release];
	
	[self.view bringSubviewToFront:fireBall];

}

 - (void)viewDidLoad {
	 pos = CGPointMake(15.0, 7.5);
	 self.view.backgroundColor = [UIColor blackColor];
	 fireBall = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"fireball.png"]];
	 fireBall.frame = CGRectMake(0, 0, 32, 32);
	 [self.view addSubview:fireBall];
	 [NSTimer scheduledTimerWithTimeInterval:(0.05) target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
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
	[fireBall release];
	[super dealloc];
}


@end
