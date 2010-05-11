//
//  LoginFlashViewController.m
//  MSN
//
//  Created by  apple on 08-11-3.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "LoginFlashViewController.h"
#import "ClientNetWorkController.h"
#import "NetWorkReachability.h"
#import "MSNAppDelegate.h"
#import "Advertisement.h"
#import "AdvertisementLabel.h"
#import "AdvertisementView.h"
#import "define.h"
#import "HttpClient.h"

#define TIPSFILENAME	@"tips.plist"

@implementation LoginFlashViewController

#pragma mark -
#pragma mark utilities


- (NSString *)getTip
{
	NSString *tipsFile = [[NSBundle mainBundle] pathForResource:@"tips" ofType:@"plist"];
	
	if (tipsFile == nil)
	{
		return @"No tips this time.";
	}
	
	NSArray *tipsArray = [NSArray arrayWithContentsOfFile:tipsFile];
	currentIndex = ++currentIndex % [tipsArray count];
	return [tipsArray objectAtIndex:currentIndex];
}

- (void)updateTips
{
	tipsLabel.text = [self getTip];
}

#pragma mark -

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		tipsLabel = nil;
		statusLabel = nil;
		imageView = nil;
		currentIndex = 0;
		
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self selector:@selector(notifyFindDispatch:)
				   name:FindDispatchNotification 
				 object:nil];
		
		[nc addObserver:self selector:@selector(notifyConnectingServer:) 
				   name:ConnectingServerNotification
				 object:nil];
		
		[nc addObserver: self
			   selector: @selector(notifyLoggingIn:) 
				   name: LoggingInNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifygetList:) 
				   name: LoginNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyH3G:) 
				   name: NetWorkH3GNotification 
				 object: nil];
	}
	return self;
}


// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	[super loadView];
    NSInteger aBottom = 0;
	CGRect rect;
    NSDictionary *dict = nil;
#ifndef FOR_PAYMENT_APP
    NSString* aVersion = NSLocalizedString(APPLICATION_VERSION, nil);
    if ([aVersion rangeOfString: @"5.005"].location == NSNotFound)
#endif
        dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"login-prog-ad"];
	NSDictionary *conf_dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
//    NSLog(@"login-prog-ad dict : \n %@ \n\n\n advertise image view : %@", dict, advt.imageView);
    BOOL existConfig = ([conf_dict objectForKey:@"second_zwp"] != nil);
	if (dict != nil)
	{
		rect = CGRectMake(139, 50, 42, 36);
		imageView = [[UIImageView alloc] initWithFrame:rect];
		[self.view addSubview:imageView];
		[imageView release];

		CGRect rc = CGRectZero;
		advt = [[Advertisement alloc] initWithDictionary:dict];
		if (advt.imageView != nil)
		{
			rc = CGRectZero;
			rc.origin.x = (320 - advt.imageView.frame.size.width)/2;
			rc.origin.y = (378 - 20 - advt.imageView.frame.size.height);
			rc.size = advt.imageView.frame.size;
			advt.imageView.frame = rc;
			[self.view addSubview:advt.imageView];
//            NSLog(@"\n\n\n\n advertise image view rect(%.0f, %.0f, %.0f, %.0f)",
//                  rc.origin.x, rc.origin.y, rc.size.width, rc.size.height);
		}
		else if (advt.textLabel != nil)
		{
			rc = CGRectZero;
			rc.origin.x = (320 - advt.imageView.frame.size.width)/2;
			rc.origin.y = (378 - 20 - advt.imageView.frame.size.height);
			rc.size = advt.imageView.frame.size;
			advt.imageView.frame = rc;
			[self.view addSubview:advt.textLabel];
		}
        aBottom = rc.origin.y;
        
        if (existConfig){
            aBottom -= 30;
        }
        
        aBottom = (aBottom - 86 - 80) / 2;
		rect = CGRectMake(20, 110, 280, 80);//20
        rect.origin.y = 86 + aBottom;
		statusLabel = [[UILabel alloc] initWithFrame:rect];
		statusLabel.numberOfLines = 4;
        statusLabel.textAlignment = UITextAlignmentCenter;
		statusLabel.text = NSLocalizedString(@"qtn_msn_login_prompt",nil);
		[self.view addSubview:statusLabel];
		[statusLabel release];	
//        NSLog(@"\n\n\n\n statuslabel rect(%.0f, %.0f, %.0f, %.0f)",
//              rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	}
	else
	{
		rect = CGRectMake(139, 150, 42, 36);
		imageView = [[UIImageView alloc] initWithFrame:rect];
		[self.view addSubview:imageView];
		[imageView release];
		rect = CGRectMake(20, 210, 280, 80);//20
		statusLabel = [[UILabel alloc] initWithFrame:rect];
		statusLabel.numberOfLines = 4;
		statusLabel.textAlignment = UITextAlignmentCenter;
		statusLabel.text = NSLocalizedString(@"qtn_msn_login_prompt",nil);
		[self.view addSubview:statusLabel];
		[statusLabel release];	
	}
	
	if (existConfig){
        rect = CGRectMake(20, 335, 280, 20);
        rect.origin.y = advt.imageView.frame.origin.y - 10 - 20;
        tipsLabel = [[UILabel alloc] initWithFrame:rect];
        tipsLabel.textAlignment = UITextAlignmentCenter;
        tipsLabel.text = [self getTip];
		[self.view addSubview:tipsLabel];
        [tipsLabel release];
        
//        NSLog(@"\n\n\n\n tipsLabel rect(%.0f, %.0f, %.0f, %.0f)",
//              rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	}
	
	UIButton *cancelBUtton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	cancelBUtton.frame = CGRectMake(76, 378, 166, 35);
	[cancelBUtton setTitle:NSLocalizedString(@"pica_str_cmd_cancel",nil) forState:UIControlStateNormal];
	[cancelBUtton addTarget:self action:@selector(logout:) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:cancelBUtton];
}

- (void)viewWillAppear:(BOOL)animated
{
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	if ([dict objectForKey:@"second_zwp"] != nil)
	{
		if (timer == nil)
		{
			timer = [NSTimer
					 scheduledTimerWithTimeInterval:3.0
					 target:self
					 selector:@selector(updateTips)
					 userInfo:nil
					 repeats:YES];		
		}
	}
	[super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
	if (timer != nil)
	{
		[timer invalidate];
		timer = nil;		
	}
}

// If you need to do additional setup after loading the view, override viewDidLoad.
- (void)viewDidLoad 
{
	int i = 1;
	NSString *imagePath = nil;
	UIImage *img = nil;
	ClientNetWorkController *netWorkController = [ClientNetWorkController getInstance];
	NetworkStatus status = [netWorkController.wifiStatus updateReachabilityChanges];
	NSString *string = nil;
	switch (status) 
	{
		case 0:
		case 1:
			break;
		case 2:
			string = @"WiFi";
			break;
		default:
			break;
	}
	if (string == nil)
		statusLabel.text = NSLocalizedString(@"qtn_msn_login_prompt",nil);
	else
		statusLabel.text = NSLocalizedString(@"pica_str_tip_connect_gprs",nil);
	
	NSMutableArray *bflies = [[NSMutableArray alloc] init];
	for (; i < 9; i++) 
	{
		imagePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"signin_%d", i]
													ofType:@"bmp"];	
		
		img = [UIImage imageWithContentsOfFile:imagePath];
		if (img) [bflies addObject:img];
	}
	
	// begin the animation
	[imageView setAnimationImages:bflies];
	imageView.animationDuration = 1.0;
	[imageView startAnimating];
	[bflies release];
	[super viewDidLoad];
}
 
- (IBAction)logout:(id)sender
{
	ClientNetWorkController* aController = (ClientNetWorkController*)[ClientNetWorkController getInstance];
	[aController.httpClient.h3gTimer invalidate];
	aController.httpClient.h3gTimer = nil;
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	[appdelegate performSelectorOnMainThread:@selector(logout) withObject:nil waitUntilDone:NO];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc 
{
	[advt release];
	[super dealloc];
}

- (void)notifyFindDispatch: (NSNotification*)note
{
	statusLabel.text = NSLocalizedString(@"pica_str_tip_dispatching",nil);
}
- (void)notifyConnectingServer: (NSNotification*)note
{
	statusLabel.text = NSLocalizedString(@"pica_str_tip_dispatching",nil);
}
- (void)notifyLoggingIn: (NSNotification*)note
{
	statusLabel.text = NSLocalizedString(@"pica_str_tip_logging_in",nil);
}
- (void)notifygetList: (NSNotification*)note
{
	statusLabel.text = NSLocalizedString(@"pica_str_tip_auth_and_get_info",nil);
}
- (void)notifyH3G: (NSNotification*)note
{
	NSString *message = nil;
	if ([note userInfo] != nil)
		message = [[note userInfo] objectForKey:@"message"];
	statusLabel.text = message;
}

@end
