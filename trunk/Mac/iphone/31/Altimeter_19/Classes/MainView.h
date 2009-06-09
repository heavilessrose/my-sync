//
//  MainView.h
//  Altimeter
//
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h> 
#import <CoreLocation/CLLocationManagerDelegate.h> 

@interface MainView : UIView {
    IBOutlet UILabel *altitude;

	CLLocationManager   *locmanager;

}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *) oldLocation ;
- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *) error;

@end
