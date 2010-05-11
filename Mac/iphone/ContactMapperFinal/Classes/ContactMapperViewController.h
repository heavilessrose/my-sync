//
//  ContactMapperViewController.h
//  ContactMapper
//
//  Created by Bill Dudney on 5/15/09.
//  Copyright Gala Factory Software LLC 2009. All rights reserved.
//
//
//  Licensed with the Apache 2.0 License
//  http://apache.org/licenses/LICENSE-2.0
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import <AddressBookUI/AddressBookUI.h>

@class ContactAnnotation;

@interface ContactMapperViewController : UIViewController <MKMapViewDelegate, 
    CLLocationManagerDelegate, ABPeoplePickerNavigationControllerDelegate, 
    ABPersonViewControllerDelegate> {
  MKMapView *_mapView;
  ContactAnnotation *_newAnnotation;
}

@property(nonatomic, retain) IBOutlet MKMapView *mapView;
@property(nonatomic, retain) ContactAnnotation *newAnnotation;

- (IBAction)choose;

@end

