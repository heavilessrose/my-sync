#import "MainView.h"

@implementation MainView

- (IBAction)loadFlickrKML {
	
	NSString* kmlURL = @"maps://?f=q&hl=en&geocode=&q=http%3A%2F%2Fapi.flickr.com%2Fservices%2Ffeeds%2Fgeo%2Fus%2F%26format%3Dkml_nl%26ie%3DUTF8%26z%3D1";

	[[UIApplication sharedApplication] openURL:[NSURL URLWithString: kmlURL]];	
}
@end
