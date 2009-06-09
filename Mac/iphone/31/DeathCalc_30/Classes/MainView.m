//
//  MainView.m
//  DeathCalc
//
//  Created by apps on 11/1/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "MainView.h"

@implementation MainView

-(void)awakeFromNib {
    [self deathUpdate];
}


- (IBAction)ageUpdate {
    [self deathUpdate];
}


- (IBAction)genderUpdate {
    [self deathUpdate];
}


- (void)deathUpdate {
	
	NSDateFormatter *dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
	[dateFormatter setDateStyle: NSDateFormatterLongStyle];
	[dateFormatter setTimeStyle: NSDateFormatterNoStyle];
	
	double secondsInAYear = 365.242199 * 24.0 * 60.0 * 60.0;
	
	double lifeExpectancy = 10.0;
	if(genderField.selectedSegmentIndex == 0)
		lifeExpectancy = 77.0;
	else
		lifeExpectancy = 81.0;
	
	lifeExpectancy -= [ageField.text intValue];
	lifeExpectancy *= secondsInAYear;
	
	NSDate *deathDate = [NSDate dateWithTimeIntervalSinceNow: lifeExpectancy];
	
	NSString *HTMLString = @"";
	HTMLString = [HTMLString stringByAppendingString: @"<span style='font-size: 48px;'><center>"];
	HTMLString = [HTMLString stringByAppendingString: @"Your predicted date of death is:<br/><b>"];
	HTMLString = [HTMLString stringByAppendingFormat: [dateFormatter stringFromDate: deathDate]];
	HTMLString = [HTMLString stringByAppendingString: @"</b><br/>You have this many seconds left to live:<br/>"];
	HTMLString = [HTMLString stringByAppendingFormat: @"<b>%1.2f</b><br/>", lifeExpectancy];
	HTMLString = [HTMLString stringByAppendingString: @"Every man dies - Not<br/>"];
	HTMLString = [HTMLString stringByAppendingString: @"every man really lives.<br/>"];
	HTMLString = [HTMLString stringByAppendingString: @"</center></span>"];
	
	[deathInfo loadHTMLString:HTMLString baseURL:nil];
}

-(IBAction) resignResponder {
	[ageField resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	if (textField == ageField) {
		[ageField resignFirstResponder];
	}
	return YES;
}


- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)drawRect:(CGRect)rect {
	// Drawing code
}


- (void)dealloc {
	[super dealloc];
}


@end
