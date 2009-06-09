//
//  MainView.m
//  Altimeter
//
//

#import "MainView.h"

@implementation MainView

-(void)awakeFromNib {
	
	locmanager = [[CLLocationManager alloc] init]; 
	[locmanager setDelegate:self]; 
	[locmanager setDesiredAccuracy:kCLLocationAccuracyBest];
	
	[locmanager startUpdatingLocation];
	
}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation 
{ 
	
	altitude.text = [NSString stringWithFormat: @"%.2f m", newLocation.altitude];
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error 
{ 
	altitude.text = @"0.00 m";
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
