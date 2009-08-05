//
//  numPadViewController.m
//  numPad
//
//  Created by wang luke on 8/5/09.
//  Copyright luke 2009. All rights reserved.
//

#import "numPadViewController.h"
#import <AudioToolbox/AudioServices.h>

@implementation numPadViewController

@synthesize selectedTextField, doneButton, numberPadShowing;

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
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
											 selector:@selector(keyboardWillShow:) 
												 name:UIKeyboardWillShowNotification object:nil];
	
	if (!selectedTextField) selectedTextField=[[UITextField alloc] init];
	
	numberPadShowing = NO;
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField{
	printf("textFieldShouldBeginEditing\n");
	[textField resignFirstResponder];
	numberPadShowing = (textField.keyboardType == UIKeyboardTypeNumberPad);
	return YES;
}

- (void)viewWillDisappear:(BOOL)animated {
	printf("viewWillDisappear\n");
	if (numberPadShowing) {
		[doneButton removeFromSuperview];
	}	
	numberPadShowing = NO;
}

- (void)keyboardWillShow:(NSNotification *)note {  
	
	printf("keyboardWillShow\n");
    UIWindow* tempWindow = [[[UIApplication sharedApplication] windows] objectAtIndex:1];
    UIView* keyboard;
	for (keyboard in tempWindow.subviews) {
 		if([[keyboard description] hasPrefix:@"<UIKeyboard"] == YES)
			if (numberPadShowing) {
				doneButton = [UIButton buttonWithType:UIButtonTypeCustom];
				doneButton.tag = 123;
				doneButton.frame = CGRectMake(0, 163, 106, 53);
				if ([[[UIDevice currentDevice] systemVersion] hasPrefix:@"3"]) {
					[doneButton setImage:[UIImage imageNamed:@"DoneUp3.png"] forState:UIControlStateNormal];
					[doneButton setImage:[UIImage imageNamed:@"DoneDown3.png"] forState:UIControlStateHighlighted];
				} else {        
					[doneButton setImage:[UIImage imageNamed:@"DoneUp.png"] forState:UIControlStateNormal];
					[doneButton setImage:[UIImage imageNamed:@"DoneDown.png"] forState:UIControlStateHighlighted];
				}
				[keyboard addSubview:doneButton];
				[doneButton addTarget:self action:@selector(doneButton:) forControlEvents:UIControlEventTouchUpInside];
				return;					
				break;
			} else {
				for (UIView *v in [keyboard subviews]){
					if ([v tag]==123)
						[v removeFromSuperview];
				}
			}
	}
}

- (void)doneButton:(id)sender {
	[selectedTextField resignFirstResponder];
	
	SystemSoundID klick;
	AudioServicesCreateSystemSoundID((CFURLRef)[NSURL fileURLWithPath:[[NSBundle mainBundle]pathForResource:@"click_off" ofType:@"wav"]isDirectory:NO],&klick);
	AudioServicesPlaySystemSound(klick);
}

- (void)dealloc {
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[selectedTextField release];
	[super dealloc];
}

@end
