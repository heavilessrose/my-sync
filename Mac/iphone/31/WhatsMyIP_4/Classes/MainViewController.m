//
//  MainViewController.m
//  WhatsMyIP
//
//  Created by apple on 10/4/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"

@implementation MainViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		textView.text = @"";
	}
	return self;
}

-(NSString*)getAddress {
	char iphone_ip[255];
	strcpy(iphone_ip,"127.0.0.1"); // if everything fails
	NSHost* myhost =[NSHost currentHost];
	if (myhost)
	{
		NSString *ad = [myhost address];
		if (ad)
			strcpy(iphone_ip,[ad cStringUsingEncoding: NSISOLatin1StringEncoding]);
	}
	return [NSString stringWithFormat:@"%s",iphone_ip]; 
}

 - (void)viewDidLoad {
	 NSString* address = [self getAddress];
	 NSString* myIPAdress = [NSString stringWithFormat:@"IP Address: %@", address];
	 textView.text = myIPAdress;
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
