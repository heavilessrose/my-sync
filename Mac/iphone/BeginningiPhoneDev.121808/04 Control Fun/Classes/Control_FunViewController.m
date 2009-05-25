//
//  Control_FunAppDelegate.m
//  Control Fun
//
//  Created by Jeff LaMarche on 7/2/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "Control_FunViewController.h"

@implementation Control_FunViewController
@synthesize nameField;
@synthesize numberField;
@synthesize sliderLabel;
@synthesize leftSwitch;
@synthesize rightSwitch;
@synthesize switchView;
@synthesize doSomethingButton;
- (void)viewDidLoad
{
	UIImage *buttonImageNormal = [UIImage imageNamed:@"whiteButton.png"];
	UIImage *stretchableButtonImageNormal = [buttonImageNormal stretchableImageWithLeftCapWidth:12 topCapHeight:0];
	[doSomethingButton setBackgroundImage:stretchableButtonImageNormal forState:UIControlStateNormal];
	
	UIImage *buttonImagePressed = [UIImage imageNamed:@"blueButton.png"];
	UIImage *stretchableButtonImagePressed = [buttonImagePressed stretchableImageWithLeftCapWidth:12 topCapHeight:0];
	[doSomethingButton setBackgroundImage:stretchableButtonImagePressed forState:UIControlStateHighlighted];
	
}

- (IBAction)doSomething:(id)sender
{
	UIActionSheet *actionSheet = [[UIActionSheet alloc] 
								  initWithTitle:@"Are you sure?" 
								  delegate:self 
								  cancelButtonTitle:@"No Way!"
								  destructiveButtonTitle:@"Yes, I'm Sure!" 
								  otherButtonTitles:nil];
	[actionSheet showInView:self.view];
	[actionSheet release];
}
- (void)actionSheet:(UIActionSheet *)actionSheet
didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (!buttonIndex == [actionSheet cancelButtonIndex])
	{
		NSString *msg = nil;
		
		if (nameField.text.length > 0)
			msg = [[NSString alloc] initWithFormat:@"You can breathe easy, %@, everything went OK.", nameField.text];
		else
			msg = @"You can breathe easy, everything went OK.";
		
		UIAlertView *alert = [[UIAlertView alloc] 
							  initWithTitle:@"Something was done" 
							  message:msg 
							  delegate:self 
							  cancelButtonTitle:@"Phew!" 
							  otherButtonTitles:nil];
		[alert show];
		[alert release];
		[msg release];
	}
	
}

- (IBAction)switchChanged:(id)sender
{
	UISwitch *whichSwitch = (UISwitch *)sender;
	BOOL setting = whichSwitch.isOn;
	[leftSwitch setOn:setting animated:YES];
	[rightSwitch setOn:setting animated:YES];
}
- (IBAction)toggleShowHide:(id)sender
{
	UISegmentedControl *segmentedControl = (UISegmentedControl *)sender;
	NSInteger segment = segmentedControl.selectedSegmentIndex;
	
	if (segment == kShowSegmentIndex) [switchView setHidden:NO];
	else [switchView setHidden:YES];
	
}

- (IBAction)sliderChanged:(id)sender
{
	UISlider *slider = (UISlider *)sender;
	int progressAsInt = (int)(slider.value + 0.5f);
	NSString *newText = [[NSString alloc] initWithFormat:@"%d", progressAsInt];
	sliderLabel.text = newText;
	[newText release];
}
- (IBAction)backgroundClick:(id)sender
{	
	[nameField resignFirstResponder];
	[numberField resignFirstResponder];
}
- (IBAction)textFieldDoneEditing:(id)sender
{
	[sender resignFirstResponder];
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

	// In Chapter 4, we forgot to mention the need
	// to release the outlets, since we declared the 
	// properties with the retain keyword, it is important
	// to release these here to avoid leaking memory.
	
	// In this example, since it is a single-view application
	// this code will only fire when the application is 
	// terminating, but it is good form to keep your memory
	// clean even when it doesn't have a functional impact
	// on your application.
	
	[nameField release];
	[numberField release];
	[sliderLabel release];
	[leftSwitch release];
	[rightSwitch release];
	[switchView release];
	[doSomethingButton release];

	[super dealloc];
}

@end
