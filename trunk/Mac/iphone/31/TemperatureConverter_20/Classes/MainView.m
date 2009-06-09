//
//  MainView.m
//  TemperatureConverter
//
//  Created by apps on 10/21/08.
//  Copyright apps 2008. All rights reserved.
//

#import "MainView.h"

@implementation MainView

- (IBAction)reset {
	fahrenheit.text = @"32";
	celsius.text = @"0";
	kelvin.text = @"-273.15";
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	UITouch *touch = [[event allTouches] anyObject];
	if (touch.tapCount >= 1) {
		[fahrenheit resignFirstResponder];
		[celsius resignFirstResponder];
		[kelvin resignFirstResponder];
	}
}

-(IBAction) resignResponder {
	[fahrenheit resignFirstResponder];
	[celsius resignFirstResponder];
	[kelvin resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	if (textField == fahrenheit) {
		[fahrenheit resignFirstResponder];
		
		double c = (5.0/9.0)*([fahrenheit.text doubleValue]-32.0);
		double k = c - 273.15;
		
		celsius.text = [[NSString alloc]initWithFormat:@"%2.1f", c];
		kelvin.text = [[NSString alloc]initWithFormat:@"%2.1f", k];
	}
	if (textField == celsius) {
		[celsius resignFirstResponder];
		
		double f = (9.0/5.0*[celsius.text doubleValue])+32.0;
		double k = [celsius.text doubleValue] - 273.15;
		
		fahrenheit.text = [[NSString alloc]initWithFormat:@"%2.1f", f];
		kelvin.text = [[NSString alloc]initWithFormat:@"%2.1f", k];
	}
	if (textField == kelvin) {
		[kelvin resignFirstResponder];
		
		double c = [kelvin.text doubleValue] + 273.15;
		double f = (9.0/5.0*c)+32.0;
		
		celsius.text = [[NSString alloc]initWithFormat:@"%2.1f", c];
		fahrenheit.text = [[NSString alloc]initWithFormat:@"%2.1f", f];
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
