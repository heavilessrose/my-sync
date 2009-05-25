//
//  DoubleComponentPickerViewController.m
//  Pickers
//
//  Created by Jeff LaMarche on 7/7/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "DoubleComponentPickerViewController.h"
 
@implementation DoubleComponentPickerViewController
@synthesize doublePicker;
@synthesize breadTypes;
@synthesize fillingTypes;
-(IBAction)buttonPressed:(id)sender
{
	NSInteger breadRow = [doublePicker selectedRowInComponent:kBreadComponent];
	NSInteger fillingRow = [doublePicker selectedRowInComponent:kFillingComponent];
	
	NSString *bread = [self.breadTypes objectAtIndex:breadRow];
	NSString *filling = [self.fillingTypes objectAtIndex:fillingRow];
	
	NSString *message = [[NSString alloc] initWithFormat:@"Your %@ on %@ bread will be right up.", filling, bread];
	
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Thank you for your order" message:message delegate:nil cancelButtonTitle:@"Great!" otherButtonTitles:nil];
	[alert show];
	[alert release];
	[message release];
	
}
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Initialization code
	}
	return self;
}

- (void)viewDidLoad {
	NSArray *breadArray = [[NSArray alloc] initWithObjects:@"White", @"Whole Wheat", @"Rye", @"Sourdough", @"Seven Grain", nil];
	self.breadTypes = breadArray;
	[breadArray release];
	
	NSArray *fillingArray = [[NSArray alloc] initWithObjects:@"Ham", @"Turkey", @"Peanut Butter", @"Tuna Salad", @"Chicken Salad", @"Roast Beef", @"Vegemite", nil];
	self.fillingTypes = fillingArray;
	[fillingArray release];
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
	[doublePicker release];
	[breadTypes release];
	[fillingTypes release];
	[super dealloc];
}
#pragma mark -
#pragma mark Picker Data Source Methods
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
	return 2;
}
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
	if (component == kBreadComponent)
		return [self.breadTypes count];
	
	return [self.fillingTypes count];
}
#pragma mark Picker Delegate Methods
- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
	if (component == kBreadComponent)
		return [self.breadTypes objectAtIndex:row];
	
	return [self.fillingTypes objectAtIndex:row];
}

@end
