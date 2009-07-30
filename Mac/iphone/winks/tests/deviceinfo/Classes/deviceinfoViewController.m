//
//  deviceinfoViewController.m
//  deviceinfo
//
//  Created by wang luke on 7/24/09.
//  Copyright luke 2009. All rights reserved.
//

#import "deviceinfoViewController.h"

@implementation deviceinfoViewController
@synthesize show;

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	///IMEI
	//	NetworkController *ntc=[[NetworkController sharedInstance] autorelease];
	//	NSString *imeistring = [ntc IMEI];
	//	NSLog(imeistring);
	NSString *_name = [[UIDevice currentDevice] name];
	NSLog(@"name = %@", _name);
	NSString *model = [[UIDevice currentDevice] model];
	NSLog(@"model = %@", model);
	NSString *localizedModel = [[UIDevice currentDevice] localizedModel];
	NSLog(@"localizedModel = %@", localizedModel);
	NSString *systemName = [[UIDevice currentDevice] systemName];
	NSLog(@"systemName = %@", systemName);
	NSString *systemVersion = [[UIDevice currentDevice] systemVersion];
	NSLog(@"systemVersion = %@", systemVersion);
	
	NSString *UDID = [[UIDevice currentDevice] uniqueIdentifier];
	NSLog(@"UDID = %@", UDID);
	char imei_str[128];
	int imei_len = sizeof(imei_str);
	memcpy(imei_str, [UDID cStringUsingEncoding:NSASCIIStringEncoding], imei_len);
	int imei_str_len = strlen(imei_str);
//	strncpy(imei_str, [UDID cStringUsingEncoding:NSASCIIStringEncoding], sizeof(imei_str));
	printf("imei_str(%d): %s\nimei_str_len: %d \n", imei_len, imei_str, imei_str_len);
	
	NSString* str;
	str = [NSString stringWithFormat:@"name = %@\nmodel = %@\nlocalizedModel = %@\nsystemName = %@\nsystemVersion = %@\nUDID = %@", _name,model,localizedModel,systemName,systemVersion,UDID];
	NSLog(str);
	[show setText:str];
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc {
    [super dealloc];
}

@end
