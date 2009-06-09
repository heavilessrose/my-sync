//
//  MainViewController.m
//  ZipWeather
//
//  Created by AppsAmuck on 10/15/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"
#import "AmuckWeather.h"

@implementation MainViewController

- (IBAction)goClick {
	NSString* weatherXml = [AmuckWeather getWeatherXmlForZipCode: textField.text];
	NSData* htmlData = [weatherXml dataUsingEncoding:NSUTF8StringEncoding];
	[webView loadData:htmlData MIMEType:@"text/html" textEncodingName:@"UTF-8" baseURL:[NSURL URLWithString:@"http://www.mywebsite.com/about.html"]];  	 
	
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}


 - (void)viewDidLoad {
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
	[super dealloc];
}

@end
