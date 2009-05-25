//
//  WhereAmIViewController.h
//  WhereAmI
//
//  Created by Jeff LaMarche on 8/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

@interface WhereAmIViewController : UIViewController <CLLocationManagerDelegate> {
	CLLocationManager	*locationManager;
	
	CLLocation	*startingPoint;
	
	IBOutlet	UILabel *latitudeLabel;
	IBOutlet	UILabel *longitudeLabel;
	IBOutlet	UILabel *horizontalAccuracyLabel;
	IBOutlet	UILabel *altitudeLabel;
	IBOutlet	UILabel *verticalAccuracyLabel;
	IBOutlet	UILabel *distanceTraveled;
}
@property (retain, nonatomic) CLLocationManager	*locationManager;
@property (retain, nonatomic) CLLocation *startingPoint;
@property (retain, nonatomic) UILabel *latitudeLabel;
@property (retain, nonatomic) UILabel *longitudeLabel;
@property (retain, nonatomic) UILabel *horizontalAccuracyLabel;
@property (retain, nonatomic) UILabel *altitudeLabel;
@property (retain, nonatomic) UILabel *verticalAccuracyLabel;
@property (retain, nonatomic) UILabel *distanceTraveled;
@end

