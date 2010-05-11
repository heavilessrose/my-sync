//
//  ContactMapperViewController.m
//  ContactMapper
//
//  Created by Bill Dudney on 5/15/09.
//  Copyright Gala Factory Software LLC 2009. All rights reserved.
//
//
//  Licensed with the Apache 2.0 License
//  http://apache.org/licenses/LICENSE-2.0
//

#import "ContactMapperViewController.h"
#import "ContactAnnotation.h"
#import "AddressGeocoder.h"

@implementation ContactMapperViewController

@synthesize mapView = _mapView;
@synthesize newAnnotation = _newAnnotation;

- (IBAction)choose {
  ABPeoplePickerNavigationController *picker =
  [[ABPeoplePickerNavigationController alloc] init];
  picker.peoplePickerDelegate = self;
  [self presentModalViewController:picker animated:YES];
  [picker release];
}

- (IBAction)stopEditingPerson {
  [self dismissModalViewControllerAnimated:YES];
}

- (void)setCurrentLocation:(CLLocation *)location {
  MKCoordinateRegion region = {{0.0f, 0.0f}, {0.0f, 0.0f}};
  region.center = location.coordinate;
  region.span.longitudeDelta = 0.15f;
  region.span.latitudeDelta = 0.15f;
  [self.mapView setRegion:region animated:YES];
}

- (void)recenterMap {
  NSArray *coordinates = [_mapView valueForKeyPath:@"annotations.coordinate"];
  CLLocationCoordinate2D maxCoord = {-90.0f, -180.0f};
  CLLocationCoordinate2D minCoord = {90.0f, 180.0f};
  for(NSValue *value in coordinates) {
    CLLocationCoordinate2D coord = {0.0f, 0.0f};
    [value getValue:&coord];
    if(coord.longitude > maxCoord.longitude) {
      maxCoord.longitude = coord.longitude;
    }
    if(coord.latitude > maxCoord.latitude) {
      maxCoord.latitude = coord.latitude;
    }
    if(coord.longitude < minCoord.longitude) {
      minCoord.longitude = coord.longitude;
    }
    if(coord.latitude < minCoord.latitude) {
      minCoord.latitude = coord.latitude;
    }
  }
  MKCoordinateRegion region = {{0.0f, 0.0f}, {0.0f, 0.0f}};
  region.center.longitude = (minCoord.longitude + maxCoord.longitude) / 2.0;
  region.center.latitude = (minCoord.latitude + maxCoord.latitude) / 2.0;
  region.span.longitudeDelta = maxCoord.longitude - minCoord.longitude;
  region.span.latitudeDelta = maxCoord.latitude - minCoord.latitude;
  [self.mapView setRegion:region animated:YES];
}

- (void)viewDidAppear:(BOOL)animated {
  [super viewDidAppear:animated];
  // we implement here so that we get nice animations
  // placing this in the people picker picked a person
  // method works but the animation is not as nice
  if(nil != self.newAnnotation) {
    [self.mapView addAnnotation:self.newAnnotation];
    self.newAnnotation = nil;
  }
  if(self.mapView.annotations.count > 1) {
    [self recenterMap];
  }
}

#pragma mark Map View Delegate Methods

- (MKAnnotationView *)mapView:(MKMapView *)mapView 
            viewForAnnotation:(id <MKAnnotation>)annotation {
  MKPinAnnotationView *view = nil; // return nil for the current user location
  if(annotation != mapView.userLocation) {
    view = (MKPinAnnotationView *)[mapView
                dequeueReusableAnnotationViewWithIdentifier:@"identifier"];
    if(nil == view) {
      view = [[[MKPinAnnotationView alloc]
               initWithAnnotation:annotation reuseIdentifier:@"identifier"]
              autorelease];
      view.rightCalloutAccessoryView = 
          [UIButton buttonWithType:UIButtonTypeDetailDisclosure];
    }
    [view setPinColor:MKPinAnnotationColorPurple];
    [view setCanShowCallout:YES];
    [view setAnimatesDrop:YES];
  } else {
    CLLocation *location = [[CLLocation alloc] 
                            initWithLatitude:annotation.coordinate.latitude
                            longitude:annotation.coordinate.longitude];
    [self setCurrentLocation:location];
  }
  return view;
}

- (void)mapView:(MKMapView *)mapView 
 annotationView:(MKAnnotationView *)view
calloutAccessoryControlTapped:(UIControl *)control {
  ContactAnnotation *ann = (ContactAnnotation *)view.annotation;
  ABPersonViewController *personVC =
      [[[ABPersonViewController alloc] init] autorelease];
  UINavigationController *nav = 
      [[[UINavigationController alloc] initWithRootViewController:personVC]
          autorelease];

  personVC.navigationItem.leftBarButtonItem = [[[UIBarButtonItem alloc] 
                     initWithBarButtonSystemItem:UIBarButtonSystemItemStop 
                                          target:self
                                          action:@selector(stopEditingPerson)]
                                               autorelease];

  personVC.displayedPerson = ann.person;
  personVC.personViewDelegate = self;
  [self presentModalViewController:nav animated:YES];
}

- (void)mapView:(MKMapView *)mapView // there is a bug in the map view in beta 5
// that makes this method required, the map view is nto checking if we 
// respond before invoking so it blows up if we don't
didSelectSearchResult:(id)result
  userInitiated:(BOOL)userInitiated {
}

#pragma mark People View Delegate Methods

- (BOOL)personViewController:(ABPersonViewController *)personViewController 
shouldPerformDefaultActionForPerson:(ABRecordRef)person 
                    property:(ABPropertyID)property
                  identifier:(ABMultiValueIdentifier)identifierForValue {
  [self dismissModalViewControllerAnimated:YES];
  return NO;
}

#pragma mark People Picker Delegate Methods

- (void)peoplePickerNavigationControllerDidCancel:
(ABPeoplePickerNavigationController *)peoplePicker {
  [self dismissModalViewControllerAnimated:YES];
}

- (BOOL)peoplePickerNavigationController:
(ABPeoplePickerNavigationController *)peoplePicker
      shouldContinueAfterSelectingPerson:(ABRecordRef)person {
  return YES;
}

- (BOOL)peoplePickerNavigationController:
(ABPeoplePickerNavigationController *)peoplePicker
      shouldContinueAfterSelectingPerson:(ABRecordRef)person
                                property:(ABPropertyID)property
                              identifier:(ABMultiValueIdentifier)identifier{
  if(kABPersonAddressProperty == property) {
    NSString *fullName = (NSString *)ABRecordCopyCompositeName(person);
    CLLocationCoordinate2D coordinate = {0.0f, 0.0f}; // start in the ocean
    self.newAnnotation = [ContactAnnotation annotationWithCoordinate:coordinate];
    self.newAnnotation.title = fullName;
    self.newAnnotation.person = person;
    [fullName release];

    ABMultiValueRef addresses = ABRecordCopyValue(person, kABPersonAddressProperty);
    CFIndex selectedAddressIndex = ABMultiValueGetIndexForIdentifier(addresses, identifier);
    CFDictionaryRef address = ABMultiValueCopyValueAtIndex(addresses, selectedAddressIndex);
    
    self.newAnnotation.coordinate = [AddressGeocoder locationOfAddress:address];
    [self dismissModalViewControllerAnimated:YES];
  }
  return NO;
}

#pragma mark -

- (void)dealloc {
  self.mapView = nil;
  self.newAnnotation = nil;
  [super dealloc];
}

@end
