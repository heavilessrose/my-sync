//
//  MainViewController.m
//  DigiClock
//
//  Created by amuck on 10/29/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"
#import <QuartzCore/CAAnimation.h>

@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}

- (UILabel *)createLabelWithBackgroundInRect:(CGRect)rect withStartText:(NSString *)startText{	
	UILabel *label = [[UILabel alloc] initWithFrame:rect];
	[label setFont:[UIFont fontWithName:@"DBLCDTempBlack" size:55.0]];
	label.backgroundColor = [UIColor clearColor];
	label.textColor = [UIColor greenColor];
	[self.view addSubview:label];
	label.text = startText;
	label.textAlignment = UITextAlignmentRight;
	label.alpha = 0.15;
	
	label = [[UILabel alloc] initWithFrame:rect];
	[label setFont:[UIFont fontWithName:@"DBLCDTempBlack" size:55.0]];
	label.backgroundColor = [UIColor clearColor];
	label.textColor = [UIColor greenColor];
	[self.view addSubview:label];
	label.text = startText;
	label.textAlignment = UITextAlignmentRight;
	return label;
}

- (void)viewDidLoad {
	 
	int x = 65;
	int y = 170;
	labels[0] = [self createLabelWithBackgroundInRect:CGRectMake(x, y, 40, 80) withStartText:@"8"];
	labels[1] = [self createLabelWithBackgroundInRect:CGRectMake(x+=35, y, 40, 80) withStartText:@"8"];
	labels[2] = [self createLabelWithBackgroundInRect:CGRectMake(x+=35, y-7, 22, 80) withStartText:@":"];
	labels[3] = [self createLabelWithBackgroundInRect:CGRectMake(x+=20, y, 40, 80) withStartText:@"8"];
	labels[4] = [self createLabelWithBackgroundInRect:CGRectMake(x+=35, y, 40, 80) withStartText:@"8"];

	labels[5] = [self createLabelWithBackgroundInRect:CGRectMake(40, y-26, 32, 80) withStartText:@"."];
	labels[6] = [self createLabelWithBackgroundInRect:CGRectMake(40, y-12, 32, 80) withStartText:@"."];
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.50];
	[UIView setAnimationDelay:0.50];
	[UIView setAnimationRepeatCount:99999999.0];
	[UIView setAnimationRepeatAutoreverses:YES];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	labels[2].alpha = 0.0;
	[UIView commitAnimations];
	
	[self onTimer];
	timer = [NSTimer scheduledTimerWithTimeInterval:(1.0) target:self selector:@selector(onTimer) userInfo:nil repeats:YES];	
 }

- (void)onTimer {
	NSDate *now = [NSDate date];
	int dig[5];
	int hourOfDay = [[now dateWithCalendarFormat:nil timeZone:nil] hourOfDay];
	int minuteOfHour = [[now dateWithCalendarFormat:nil timeZone:nil] minuteOfHour]; 
	
	labels[5].alpha = hourOfDay > 11 ? 0.0 : 1.0;
	labels[6].alpha = hourOfDay > 11 ? 1.0 : 0.0;
	
	if (hourOfDay > 12)
		hourOfDay -= 12;
	dig[0] = hourOfDay / 10;
	dig[1] = hourOfDay % 10;
	dig[2] = 0;
	dig[3] = minuteOfHour / 10;
	dig[4] = minuteOfHour % 10;;
	
	labels[0].text = [NSString stringWithFormat:@"%d", dig[0]];
	labels[1].text = [NSString stringWithFormat:@"%d", dig[1]];
	labels[3].text = [NSString stringWithFormat:@"%d", dig[3]];
	labels[4].text = [NSString stringWithFormat:@"%d", dig[4]];
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
	[timer invalidate];
	[timer release];
	[super dealloc];
}


@end
