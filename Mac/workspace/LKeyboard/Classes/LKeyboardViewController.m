//
//  LKeyboardViewController.m
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "LKeyboardViewController.h"
#import "LKeyboardView.h"
#import "LKeyboardAppDelegate.h"


@interface LKeyboardViewController ()

@property (nonatomic, retain) UITextField *txtField;
@property (nonatomic, retain) UIButton *showOrHideButton;

@end


@implementation LKeyboardViewController

@synthesize keyBoardView, txtField, showOrHideButton;

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
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
- (void)viewDidLoad
{
    [super viewDidLoad];
	
	[self.showOrHideButton setTitle:@"show" forState:UIControlStateNormal];
}


// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait || interfaceOrientation == UIDeviceOrientationLandscapeRight);
	return YES;
}

- (void)didReceiveMemoryWarning
{
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload
{
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc
{
	[keyBoardView release];
	[txtField release];
	[showOrHideButton release];
	
    [super dealloc];
}

#pragma mark -
#pragma mark LKeyBoardView

- (IBAction)showOrHideEmoKeyboard
{
	if ([showOrHideButton.currentTitle isEqualToString:@"show"]) {
		[self.txtField resignFirstResponder];
		
		self.keyBoardView = [[LKeyboardView alloc] initWithFrame:CGRectMake(0, 244, 320, 216)];
		[self.view addSubview:keyBoardView];
		[keyBoardView release];
		
		[showOrHideButton setTitle:@"hide" forState:UIControlStateNormal];
	} else {
		[self.keyBoardView removeFromSuperview];
		[showOrHideButton setTitle:@"show" forState:UIControlStateNormal];
	}
}

#pragma mark -
#pragma mark rotate handle

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
										 duration:(NSTimeInterval)duration
{
	CGRect r;
	switch ([UIDevice currentDevice].orientation) {
		case UIInterfaceOrientationPortrait:
		case UIInterfaceOrientationPortraitUpsideDown:
			r = kPortraitRect;
			r.origin.y = 244;
			[self.keyBoardView setFrame:r];
			break;
		case UIInterfaceOrientationLandscapeRight:
		case UIInterfaceOrientationLandscapeLeft:
			r = kLandscapRect;
			r.origin.y = 160;
			[self.keyBoardView setFrame:r];
			break;
		default:
			NSLog(@"curOrientation invailide");
			break;
	}
	
	
#if 1
	for (UIView *aView in self.view.subviews) {
		[aView layoutSubviews];
	}
#else
	[self.view layoutSubviews];
#endif
	[super willAnimateRotationToInterfaceOrientation:interfaceOrientation
											duration:duration];
}

@end
