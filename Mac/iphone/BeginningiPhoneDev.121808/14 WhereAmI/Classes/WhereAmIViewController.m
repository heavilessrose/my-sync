//
//  WhereAmIAppDelegate.m
//  WhereAmI
//
//  Created by Jeff LaMarche on 8/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "WhereAmIViewController.h"

@implementation WhereAmIViewController
@synthesize locationManager;
@synthesize startingPoint;
@synthesize latitudeLabel;
@synthesize longitudeLabel;
@synthesize horizontalAccuracyLabel;
@synthesize altitudeLabel;
@synthesize verticalAccuracyLabel;
@synthesize distanceTraveled;

#pragma mark -
- (void)viewDidLoad {
	self.locationManager = [[CLLocationManager alloc] init];
	[locationManager startUpdatingLocation];
	locationManager.delegate = self;
	locationManager.distanceFilter = kCLDistanceFilterNone; 
	locationManager.desiredAccuracy = kCLLocationAccuracyBest;
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
	
	[locationManager release];
	[startingPoint release];
	[latitudeLabel release];
	[longitudeLabel release];
	[horizontalAccuracyLabel release];
	[altitudeLabel release];
	[verticalAccuracyLabel release];
	[distanceTraveled release];
	
	[super dealloc];
}
#pragma mark -
#pragma mark CLLocationManagerDelegate Methods
- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation {
	
	if (startingPoint == nil)
		self.startingPoint = newLocation;
	
	NSString *latitudeString = [[NSString alloc] initWithFormat:@"%g°", newLocation.coordinate.latitude];
	latitudeLabel.text = latitudeString;
	[latitudeString release];
	
	NSString *longitudeString = [[NSString alloc] initWithFormat:@"%g°", newLocation.coordinate.longitude];
	longitudeLabel.text = longitudeString;
	[longitudeString release];
	
	NSString *horizontalAccuracyString = [[NSString alloc] initWithFormat:@"%gm", newLocation.horizontalAccuracy];
	horizontalAccuracyLabel.text = horizontalAccuracyString;
	[horizontalAccuracyString release];
	
	NSString *altitudeString = [[NSString alloc] initWithFormat:@"%gm", newLocation.altitude];
	altitudeLabel.text = altitudeString;
	[altitudeString release];
	
	NSString *verticalAccuracyString = [[NSString alloc] initWithFormat:@"%gm", newLocation.verticalAccuracy];
	verticalAccuracyLabel.text = verticalAccuracyString;
	[verticalAccuracyString release];
	
	CLLocationDistance distance = [newLocation getDistanceFrom:startingPoint];
	NSString *distanceString = [[NSString alloc] initWithFormat:@"%gm", distance];
	distanceTraveled.text = distanceString;
	[distanceString release];
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error {

	NSString *errorType = (error.code == kCLErrorDenied) ? @"Access Denied" : @"Unknown Error";
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error gettingg location from Core Location" message:errorType delegate:nil cancelButtonTitle:@"Okay" otherButtonTitles:nil];
	[alert show];
	[alert release];

}

@end
