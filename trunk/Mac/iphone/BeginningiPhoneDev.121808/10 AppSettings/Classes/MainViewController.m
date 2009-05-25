//
//  MainViewController.m
//  AppSettings
//
//  Created by Jeff LaMarche on 7/24/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"

@implementation MainViewController
@synthesize usernameLabel;
@synthesize passwordLabel;
@synthesize protocolLabel;
@synthesize warpDriveLabel;
@synthesize warpFactorLabel;
@synthesize favoriteTeaLabel;
@synthesize favoriteCandyLabel;
@synthesize favoriteGameLabel;
@synthesize favoriteExcuseLabel;
@synthesize favoriteSinLabel;

-(void)refreshFields
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	usernameLabel.text = [defaults objectForKey:kUsernameKey];
	passwordLabel.text = [defaults objectForKey:kPasswordKey];
	protocolLabel.text = [defaults objectForKey:kProtocolKey];
	warpDriveLabel.text = [defaults objectForKey:kWarpDriveKey];
	warpFactorLabel.text = [[defaults objectForKey:kWarpFactorKey] stringValue];
	favoriteTeaLabel.text = [defaults objectForKey:kFavoriteTeaKey];
	favoriteCandyLabel.text = [defaults objectForKey:kFavoriteCandyKey];
	favoriteGameLabel.text = [defaults objectForKey:kFavoriteGameKey];
	favoriteExcuseLabel.text = [defaults objectForKey:kFavoriteExcuseKey];
	favoriteSinLabel.text = [defaults objectForKey:kFavoriteSinKey];

}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}

- (void)viewDidLoad {
	[self refreshFields];
 }
 
- (void)viewDidAppear:(BOOL)animated {
	[self refreshFields];
	[super viewDidAppear:animated];
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
	[usernameLabel release];
	[passwordLabel release];
	[protocolLabel release];
	[warpDriveLabel release];
	[warpFactorLabel release];
	[favoriteTeaLabel release];
	[favoriteCandyLabel release];
	[favoriteGameLabel release];
	[favoriteExcuseLabel release];
	[favoriteSinLabel release];
	[super dealloc];
}


@end
