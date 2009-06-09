//
//  MainView.m
//  WhereAmI
//
//  Created by acs on 10/10/08.
//  Copyright ACS Technologies 2008. All rights reserved.
//

#import "MainView.h"

@implementation MainView

- (IBAction)update {
	
	locmanager = [[CLLocationManager alloc] init]; 
	[locmanager setDelegate:self]; 
	[locmanager setDesiredAccuracy:kCLLocationAccuracyBest];
	
	[locmanager startUpdatingLocation];
}

	CLLocationManager* locmanager;

-(void)awakeFromNib {
	[self update];
}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation 
{ 
	if (wasFound) return; 
	wasFound = YES;
	 
	CLLocationCoordinate2D loc = [newLocation coordinate];
	
	latitude.text = [NSString stringWithFormat: @"%f", loc.latitude];
	longitude.text	= [NSString stringWithFormat: @"%f", loc.longitude];
	altitude.text = [NSString stringWithFormat: @"%f", newLocation.altitude];
	
//	NSString *mapUrl = [NSString stringWithFormat: @"http://maps.google.com/maps?q=%f,%f", loc.latitude, loc.longitude]; 
//	NSURL *url = [NSURL URLWithString:mapUrl]; 
//	[[UIApplication sharedApplication] openURL:url]; 
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error 
{ 
 
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
