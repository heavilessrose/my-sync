//
//  MainViewController.m
//  openURL
//
//  Created by apple on 10/3/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"

@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}

-(IBAction)openMaps {
	// Where is Apple on the map anyway?
	NSString* addressText = @"1 Infinite Loop, Cupertino, CA 95014";
	// URL encode the spaces
	addressText =  [addressText stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding];	
	NSString* urlText = [NSString stringWithFormat:@"http://maps.google.com/maps?q=%@", addressText];
	// lets throw this text on the log so we can view the url in the event we have an issue
	NSLog(urlText);
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlText]];
}

-(IBAction)openEmail {
	// Fire off an email to apple support
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"mailto://devprograms@apple.com"]];
}

-(IBAction)openPhone {
	// Call Google 411
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"tel://8004664411"]];
}

-(IBAction)openSms {
	// Text to Google SMS
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"sms://466453"]];
}

-(IBAction)openBrowser {
	// Lanuch any iPhone developers fav site
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://itunesconnect.apple.com"]];
}

/*
 If you need to do additional setup after loading the view, override viewDidLoad.
 - (void)viewDidLoad {
 }
 */


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
