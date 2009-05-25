//
//  CustomPickerViewController.m
//  Pickers
//
//  Created by Jeff LaMarche on 7/7/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "CustomPickerViewController.h"
#import <AudioToolbox/AudioToolbox.h>

@implementation CustomPickerViewController
@synthesize picker;
@synthesize column1;
@synthesize column2;
@synthesize column3;
@synthesize column4;
@synthesize column5;
@synthesize winLabel;
@synthesize button;
-(void)showButton
{
	button.hidden = NO;
}
-(void)playWinSound
{
	NSString *path = [[NSBundle mainBundle] pathForResource:@"win" ofType:@"wav"];
	SystemSoundID soundID;
	AudioServicesCreateSystemSoundID((CFURLRef)[NSURL fileURLWithPath:path], &soundID);
	AudioServicesPlaySystemSound (soundID);		
	winLabel.text = @"WIN!";
	[self performSelector:@selector(showButton) withObject:nil afterDelay:1.5];
}
-(IBAction)spin:(id)sender
{
	BOOL win = NO;
	int numInRow = 1;
	int lastVal = -1;
	for (int i = 0; i < 5; i++)
	{
		int newValue = random() % [self.column1 count];

		if (newValue == lastVal)
			numInRow++;
		else
			numInRow = 1;
		
		lastVal = newValue;
		[picker selectRow:newValue inComponent:i animated:YES];
		[picker reloadComponent:i];
		if (numInRow >= 3)
			win = YES;
		
	}
	
	button.hidden = YES;
	NSString *path = [[NSBundle mainBundle] pathForResource:@"crunch" ofType:@"wav"];
	SystemSoundID soundID;
	AudioServicesCreateSystemSoundID((CFURLRef)[NSURL fileURLWithPath:path], &soundID);
	AudioServicesPlaySystemSound (soundID);

	if (win)
		[self performSelector:@selector(playWinSound) withObject:nil afterDelay:.5];
	else
		[self performSelector:@selector(showButton) withObject:nil afterDelay:.5];
	
	winLabel.text = @"";
	
	
}

- (void)viewDidLoad {
	
	UIImage *seven = [UIImage imageNamed:@"seven.png"];
	UIImage *bar = [UIImage imageNamed:@"bar.png"];
	UIImage *crown = [UIImage imageNamed:@"crown.png"];
	UIImage *cherry = [UIImage imageNamed:@"cherry.png"];
	UIImage *lemon = [UIImage imageNamed:@"lemon.png"];
	UIImage *apple = [UIImage imageNamed:@"apple.png"];
	
	for (int i = 1; i <= 5; i++)
	{
		UIImageView *sevenView = [[UIImageView alloc] initWithImage:seven];
		UIImageView *barView = [[UIImageView alloc] initWithImage:bar];
		UIImageView *crownView = [[UIImageView alloc] initWithImage:crown];
		UIImageView *cherryView = [[UIImageView alloc] initWithImage:cherry];
		UIImageView *lemonView = [[UIImageView alloc] initWithImage:lemon];
		UIImageView *appleView = [[UIImageView alloc] initWithImage:apple];
		
		NSArray *imageViewArray = [[NSArray alloc] initWithObjects: sevenView, barView, crownView, cherryView, lemonView, appleView, nil];
		
		NSString *fieldName = [[NSString alloc] initWithFormat:@"column%d", i];
		[self setValue:imageViewArray forKey:fieldName];
		[fieldName release];
		[imageViewArray release];
		
		[sevenView release];
		[barView release];
		[crownView release];
		[cherryView release];
		[lemonView release];
		[appleView release];
	

	}
	
	// Seed the random number generator
	srandom(time(NULL));
	
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Initialization code
	}
	return self;
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
	[picker release];
	[column1 release];
	[super dealloc];
}
#pragma mark -
#pragma mark Picker Data Source Methods
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
	return 5;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{

		return [self.column1 count];
}
#pragma mark Picker Delegate Methods

- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row
		  forComponent:(NSInteger)component reusingView:(UIView *)view
{
	if (component == 0)
		return [self.column1 objectAtIndex:row];
	else if (component == 1)
		return [self.column2 objectAtIndex:row];
	else if (component == 2)
		return [self.column3 objectAtIndex:row];
	else if (component == 3)
		return [self.column4 objectAtIndex:row];
	return [self.column5 objectAtIndex:row];
	
}
@end
