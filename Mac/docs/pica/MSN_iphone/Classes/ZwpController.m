//
//  ZwpController.m
//  MSN
//
//  Created by  apple on 09-3-17.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

// Definition of frequency of shake action.
#define SHAKE_VALUE			10 
#define UPDATE_FREQUENCY	50	// bigger is more insensitive

#define LOADING_VIEW_X		60
#define LOADING_VIEW_Y		100
#define LOADING_VIEW_WIDTH	200
#define LOADING_VIEW_HEIGHT	150

#import "ZwpController.h"
#import "ClientNetWorkController.h"
#import "define.h"
#import "public.h"
#import "MSNAppDelegate.h"
#import "ContactList.h"
#import "PersonInfo.h"
#import "public.h"

NSString *AddBuddyNotification = @"addBuddyNotification";
NSString *IMFromWapNotification = @"imMSGFromWapNotification";

@implementation ZwpController
@synthesize targetString,targetType,advisitString,myAlertView,baseUrlString,baseUrlPath,htmlString,session,timer;
@synthesize lastAcceleration;
@synthesize loadingView;


- (id)initWithUrl:(NSString*)theUrl type:(int)theType
{
	self = [super init];
	if (self != nil) 
	{
		requestFrom = theType;
		self.url = theUrl;
		back = NO;
		sysDismiss = NO;
		cacheHtml = NO;
		webUrls = [[NSMutableArray alloc] initWithCapacity:0];
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		
		if (theType == ZWP_REQUEST)
		{
			[nc addObserver:self selector:@selector(notifyZwpTitleChanged:)
					   name:ZwpTitleChangedNotification 
					 object:nil];
			[nc addObserver:self selector:@selector(notifyZwpDataChanged:)
					   name:ZwpDataChangedNotification 
					 object:nil];			
		}else if ( theType == STANDBY_ZWP_REQUEST){
			[nc addObserver:self selector:@selector(notifyZwpTitleChanged:)
					   name:StandbyZwpTitleChangedNotification 
					 object:nil];
			[nc addObserver:self selector:@selector(notifyZwpDataChanged:)
					   name:StandbyZwpDataChangedNotification 
					 object:nil];	
		}
		


		documentUrl = [[NSURL fileURLWithPath:NSTemporaryDirectory() isDirectory:YES] retain];
		loadingView = nil;
	}
	return self;
}


- (NSString*)url
{
	return url;
}

- (void)setUrl:(NSString*)theUrl
{
	LogMethod();
	MYLog(@">>>the url is:%@",theUrl);
	[self setUrl:theUrl headers:nil body:nil method:nil];
}

- (void)setUrl:(NSString*)theUrl headers:(NSDictionary*)headers body:(NSData*)body method:(NSString*)method
{
	if (theUrl != nil)
	{
		NSURL *theUrlString = [NSURL URLWithString:theUrl];	
		if ([theUrlString host] != nil)
		{
			[self setUrlString:theUrl];
		}
		else
		{
			if ([theUrl rangeOfString:@"/"].location != NSNotFound)
				theUrl = [baseUrlString stringByAppendingString:[theUrlString resourceSpecifier]];
			else
				theUrl = [baseUrlPath stringByAppendingString:[theUrlString resourceSpecifier]];
		}
		[[ClientNetWorkController getInstance] sendZwpRequest:theUrl headers:headers body:body method:method session:session type:requestFrom];
	}
	if (url != nil && !back)
	{	
		NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:url,@"url",[NSNumber numberWithBool:cacheHtml],@"cache",nil];
		[webUrls addObject:dict];
		if (cacheHtml)
		{
			NSString *name = [@"name" stringByAppendingFormat:@"%d",[webUrls count]];
			NSString *path = [NSTemporaryDirectory()  stringByAppendingPathComponent:name];
			[[htmlString dataUsingEncoding:NSUTF8StringEncoding] writeToFile:path atomically:YES];
		}
		self.navigationItem.leftBarButtonItem.enabled = YES;	
		[segmentedControl setEnabled:YES forSegmentAtIndex:0];
		
		if (timer != nil)
			[timer invalidate];
		self.timer  = [NSTimer scheduledTimerWithTimeInterval:60.0*15 target:self 
													 selector:@selector(zwpTimeOut:) 
													 userInfo:nil 
													  repeats:NO];
	}
	[theUrl retain];
	[url release];
	url = theUrl;
	back = NO;
	cacheHtml = NO;
}

- (void)zwpTimeOut:(NSTimer*)theTimer
{
	self.timer = nil; 
	self.session = nil;
}

// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	LogMethod();
	[super loadView];
	CGRect rect = CGRectMake(0, 0, 320, 460);
	self.view.frame = rect;
	self.view.backgroundColor = [UIColor blueColor];
	
	zwpView = [[UIWebView alloc] initWithFrame:rect];
	zwpView.backgroundColor = [UIColor whiteColor];
	zwpView.scalesPageToFit = YES;
	zwpView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);
	zwpView.delegate = self;
	[self.view addSubview:zwpView];
}



 //If you need to do additional setup after loading the view, override viewDidLoad.
- (void)viewDidLoad 
{
	LogMethod();
	NSString *string = nil;
	segmentedControl = [[UISegmentedControl alloc] init];
	string = [[NSBundle mainBundle] pathForResource:@"back-png" ofType:@"png"];
	[segmentedControl insertSegmentWithImage:[UIImage imageWithContentsOfFile:string] atIndex:0 animated:NO];
	string = [[NSBundle mainBundle] pathForResource:@"reload-png" ofType:@"png"];
	[segmentedControl insertSegmentWithImage:[UIImage imageWithContentsOfFile:string] atIndex:1 animated:NO];
	
	segmentedControl.frame = CGRectMake(0, 0, 90, 30);
	segmentedControl.segmentedControlStyle = UISegmentedControlStyleBar;
	segmentedControl.momentary = YES;
	segmentedControl.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
	[segmentedControl setEnabled:NO forSegmentAtIndex:0]	;
	[segmentedControl addTarget:self action:@selector(segmentAction:) forControlEvents:UIControlEventValueChanged];
	
	UIBarButtonItem *segmentBarItem = [[UIBarButtonItem alloc] initWithCustomView:segmentedControl];
	self.navigationItem.rightBarButtonItem = segmentBarItem;
	[segmentBarItem release];
	
	string = [[NSBundle mainBundle] pathForResource:@"home-png" ofType:@"png"];
	UIBarButtonItem *button = [[UIBarButtonItem alloc] initWithImage:[UIImage imageWithContentsOfFile:string]
															   style:UIBarButtonItemStyleBordered
															  target:self
															  action:@selector(homeAction:)];
	button.enabled = NO;
	self.navigationItem.leftBarButtonItem = button;
	[button release];
	
#if _DEBUG_
//	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / UPDATE_FREQUENCY)];
//	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
#endif
	
	[super viewDidLoad];
	[self.navigationController setNavigationBarHidden:NO animated:NO];
}
 


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc 
{
	if (myAlertView != nil)
	{
		myAlertView.delegate = nil;
		[myAlertView dismissWithClickedButtonIndex:0 animated:NO];
		self.myAlertView = nil;
	}
	[timer release];
	[segmentedControl release];
	[session release];
	[htmlString release];
	[baseUrlPath release];
	[targetType release];
	[targetString release];
	[advisitString release];
	[documentUrl release];
	[webUrls release];
	[url release];
	[zwpView release];
	[lastAcceleration release];
	[super dealloc];
}

#pragma mark -
#pragma mark utilities methods.

- (void)refreshAction:(id)sender
{
	MYLog(@">>>[theType refreshAction]:%d",requestFrom);
	[[ClientNetWorkController getInstance] sendZwpRequest:url headers:nil body:nil method:nil session:session type:requestFrom];
}

- (void)homeAction:(id)sender
{
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	NSString *theUrl = [dict objectForKey:@"fpage"];
	if (theUrl != nil)
	{
		[webUrls removeAllObjects];
		self.navigationItem.leftBarButtonItem.enabled = NO;
		back = YES;
		self.url = theUrl;
	}
}

-(void)segmentAction:(id)sender
{
	if (segmentedControl == sender)
	{
		if ([segmentedControl selectedSegmentIndex] == 0)
		{
			[self backAction:nil];
		}
		else //if ([segCtl selectedSegmentIndex] == 1)
		{
			[self refreshAction:nil];
		}	
	}
}

- (void)backAction:(id)sender
{
	if ([webUrls count] > 0)
	{
		NSString *theUrl = [[webUrls lastObject] objectForKey:@"url"];
		BOOL cache = [[[webUrls lastObject] objectForKey:@"url"] boolValue];
		if (cache)
		{
			NSString *name = [@"name" stringByAppendingFormat:@"%d",[webUrls count]];
			NSString *path = [NSTemporaryDirectory()  stringByAppendingPathComponent:name];
			NSData *data = [NSData dataWithContentsOfFile:path];
			[self setUrlString:theUrl];
			cacheHtml = cache;
			htmlString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
			if (htmlString != nil)
				[zwpView loadHTMLString:htmlString baseURL:documentUrl];
		}
		else
		{
			back = YES;
			self.url = theUrl;
		}
		[webUrls removeLastObject];
		
		if ([webUrls count] == 0)
			[segmentedControl setEnabled:YES forSegmentAtIndex:0];
	}
}

- (void)setUrlString:(NSString*)theString
{
	NSURL *theUrlString = [NSURL URLWithString:theString];	
	int len = [theString length];
	
	if ([[[theUrlString resourceSpecifier] lastPathComponent] compare:@"/"] != NSOrderedSame)
		len = len - [[[theUrlString resourceSpecifier] lastPathComponent] length] - 1;

	self.baseUrlPath = [theString substringToIndex:len];
	
	if ([[[theUrlString path] stringByDeletingLastPathComponent] compare:@"/"] != NSOrderedSame)
		len = len - [[[theUrlString path] stringByDeletingLastPathComponent] length];

	self.baseUrlString = [theString substringToIndex:len];
}

- (NSString*)linkString:(NSString*)theUrl
{
	NSString *httpString = nil;
	linkType = 0;
	self.targetType = nil;
	self.advisitString = nil;
	int len = [[documentUrl path] length];
	
	if ([theUrl length] > len + 1)
	{
		NSString *subString = [theUrl substringFromIndex:len + 1];
		NSRange targetRange = NSMakeRange(0, [subString length]);
		targetRange = [subString rangeOfString:@"***" options:0 range:targetRange];
		if (targetRange.location != NSNotFound)
		{
			self.targetType = [subString substringToIndex:targetRange.location];
			if ([subString length] > targetRange.location + 3)
			{
				NSRange typeRange = NSMakeRange(targetRange.location + 3, [subString length] - targetRange.location - 3);
				typeRange = [subString rangeOfString:@"***" options:0 range:typeRange];
				if (typeRange.location != NSNotFound)
				{
					if ([subString length] > typeRange.location + 3)
					{
						NSRange advisitRange = NSMakeRange(typeRange.location + 3, [subString length] - typeRange.location - 3);
						advisitRange = [subString rangeOfString:@"***" options:0 range:advisitRange];
						if (advisitRange.location != NSNotFound)
						{
							if ([subString length] > advisitRange.location + 3)
								httpString = [subString substringFromIndex:advisitRange.location + 3];
							
							subString = [subString substringToIndex:advisitRange.location];
							self.advisitString = [subString substringFromIndex:typeRange.location + 3];
						}
						else
						{
							httpString = [subString substringFromIndex:typeRange.location + 3];
						}
					}
					subString = [subString substringToIndex:typeRange.location];
					subString = [subString substringFromIndex:targetRange.location + 3];
					linkType = [subString intValue];
				}
				else
				{
					httpString = [subString substringFromIndex:targetRange.location + 3];
				}
			}
		}
		else
		{
			httpString = subString;
		}
	}
	return httpString;
}

#pragma mark -
#pragma mark alertView delegate methods.
- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	sysDismiss = NO;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (!sysDismiss)
	{
		alertView.delegate = nil;
		self.myAlertView = nil;
		
		if ([alertView.title compare:NSLocalizedString(@"Alert",nil)] == NSOrderedSame)
		{
			if (buttonIndex == 1)
			{
				if (advisitString != nil)
					[[ClientNetWorkController getInstance] sendAdviceClickedRequest:advisitString];
				NSURL *myURL = nil;
				switch (linkType)
				{
					case 0xF0:
					{
						NSString *theURL = [NSString stringWithString:@"tel:"];
						theURL = [theURL stringByAppendingString:targetString];
						myURL = [NSURL URLWithString:theURL];
					}
						break;
					case 0xF1:
					case 0xF2:
					{
						NSString *theURL = [NSString stringWithString:@"sms:"];
						NSRange range = [targetString rangeOfString:@";" options:0 range:NSMakeRange(0, [targetString length])];
						if (range.location != NSNotFound)
						{
							self.targetString = [targetString substringToIndex:range.location];
							theURL = [theURL stringByAppendingString:targetString];
							myURL = [NSURL URLWithString:theURL];
						}
					}
						break;
					case 0xFF:
					{
						myURL = [NSURL URLWithString:targetString];					
					}
						break;
						
					default:
						break;
				}
				if (myURL != nil)
					[[UIApplication sharedApplication] openURL:myURL];		
			}
		}
	}
}

#pragma mark -
#pragma mark webview delegate methods.
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
	UIApplication *application = [UIApplication sharedApplication];
	application.networkActivityIndicatorVisible = NO;
	
	if (loadingView != nil)
	{
		[loadingView removeView];
		loadingView = nil;
	}
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
	UIApplication *application = [UIApplication sharedApplication];
	application.networkActivityIndicatorVisible = YES;
	
	if (loadingView == nil)
	{
		loadingView = [LoadingView loadingViewInView:zwpView 
												rect:CGRectMake(LOADING_VIEW_X, LOADING_VIEW_Y,LOADING_VIEW_WIDTH,LOADING_VIEW_HEIGHT)];	
	}
	
	NSMutableDictionary *headers = nil;
	NSDictionary *dict = [request allHTTPHeaderFields];
	NSString * key = nil;
	for (key in [dict allKeys])
	{
		if ([key compare:@"User-Agent"] != NSOrderedSame
			&& [key compare:@"Accept"] != NSOrderedSame)
		{
			if (headers == nil)
			{
				[NSMutableDictionary dictionaryWithCapacity:0];			
			}
			[headers setObject:[dict valueForKey:key] forKey:key];
		}
	}
	
	
	BOOL ret = YES;
	
	if ([[[request URL] path] rangeOfString:[documentUrl path]].location == NSNotFound)
	{
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
		
		NSString *theURL = [[request URL] absoluteString];
		if ([theURL hasPrefix:@"addbuddy:"] || [theURL hasPrefix:@"im:"])
		{
			NSArray  *component = [theURL componentsSeparatedByString:@":"];
			
			if ([component count] == 2)
			{
				// Sth like this: "addbuddy:9025@pica.com"
				MYLog(@">>> function for addbuddy.");
				ret = NO;
				// Jump to chat view tab.
				appdelegate.tabController.selectedIndex = [appdelegate getTheIndex:kListChatVC];
				appdelegate.tabController.segmentedControl.selectedSegmentIndex = [appdelegate getTheIndex:kListChatVC];
				
				[self.loadingView removeView];
				// Call addbuddy function here.
				NSDictionary *infoDict = [NSDictionary dictionaryWithObject:[[request URL] resourceSpecifier] forKey:kAddBuddyKey];
				[[NSNotificationCenter defaultCenter] postNotificationName:AddBuddyNotification object:nil userInfo:infoDict];
			}else if ([component count] == 3){
				// Sth like this: "im:yincp@msn.cn:hello"
				ret = NO;
				// Jump to chat view tab
				appdelegate.tabController.selectedIndex = [appdelegate getTheIndex:kChatVC];
				appdelegate.tabController.segmentedControl.selectedSegmentIndex = [appdelegate getTheIndex:kChatVC];
				[self.loadingView removeView];
				
				// im:yincp@msn.cn:hello
				NSString *theIMID = [component objectAtIndex:1];
				// Send Notification .
				NSDictionary *infoDict = [NSDictionary dictionaryWithObject:theIMID forKey:kMSGFromWap];
				
				[[NSNotificationCenter defaultCenter] postNotificationName:IMFromWapNotification
																	object:nil
																  userInfo:infoDict];
				/*
				PersonInfo *thePerson = nil;
				ContactList * list = [ContactList getInstance];
				for (PersonInfo *person in list.contactList)
				{
					if ([person.imid compare:theIMID] == NSOrderedSame)
					{
						thePerson = person;
						break;
					}
				}
				if (thePerson == nil)
				{
					// Todo: stranger message.
					//	thePerson = [[list.contactList objectAtIndex:0] copy];
					//	thePerson.imid = theIMID;
					//	thePerson.nickname = theIMID;
				}
				
				if (thePerson != nil)
				{
					NSDictionary *info = [NSDictionary dictionaryWithObject:thePerson forKey:@"key"];
					[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];				
				}else{
					//NSLog(@"! Fatal, thePerson shoud not be nil.");
					[Public alertMessage:@"" addMsg:NSLocalizedString(@"%@ is_a_stranger",theIMID)];
				}			
				 */
			}
			return ret;
		}
		ret = NO;
		[self setUrl:[[request URL] absoluteString] headers:headers
				body:[request HTTPBody] method:[request HTTPMethod]];

	}
	else
	{
		NSString* urlString = [[request URL] resourceSpecifier];
		if (urlString != nil)
		{
			self.targetString = [self linkString:urlString];
			if (targetString != nil && targetType != nil)
			{
				if ([targetType compare:@"link"] == NSOrderedSame)
				{
					ret = NO;
					
					NSString *message = nil;
					switch (linkType)
					{
						case 0xF0:
							message = [NSString stringWithFormat:NSLocalizedString(@"AdvertisementWarning",nil),NSLocalizedString(@"Tel",nil)];	
							break;
						case 0xF1:
						case 0xF2:
							message = [NSString stringWithFormat:NSLocalizedString(@"AdvertisementWarning",nil),NSLocalizedString(@"SMS",nil)];
							break;
						case 0xFF:
							message = [NSString stringWithFormat:NSLocalizedString(@"AdvertisementWarning",nil),@"Safari"];
							break;
						default:
							if (advisitString != nil)
								[[ClientNetWorkController getInstance] sendAdviceClickedRequest:advisitString];
							[self setUrl:[[request URL] absoluteString] headers:headers body:[request HTTPBody] method:[request HTTPMethod]];
							break;
					}
					if (message != nil)
					{
						self.myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert",nil)
																	   message:message
																	  delegate:self cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil)
															 otherButtonTitles:NSLocalizedString(@"qtn_ok",nil),nil] autorelease];
						[myAlertView setDelegate:self];
						sysDismiss = YES;
						[myAlertView show];
					}
				}
			}
		}
	}

	return ret;
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
	UIApplication *application = [UIApplication sharedApplication];
	application.networkActivityIndicatorVisible = NO;
	
	if (loadingView != nil)
	{
		[loadingView removeView];
		loadingView = nil;
	}
}


- (void)notifyZwpTitleChanged: (NSNotification*)note
{
	NSString *title = [[note userInfo] objectForKey:@"title"];
	self.navigationItem.title = title;
}

- (void)notifyZwpDataChanged: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	self.htmlString = [[note userInfo] objectForKey:@"html"];
	if (htmlString != nil)
		[zwpView loadHTMLString:htmlString baseURL:documentUrl];
	NSDictionary *headers = [[note userInfo] objectForKey:@"headers"];
	if (headers != nil)
	{
		self.session = [headers objectForKey:@"Pica-Session"];
		NSString *cache = [headers objectForKey:@"Cache-Control"];
		if (cache == nil || [cache compare:@"no-cache"] == NSOrderedSame)
			cacheHtml = NO;
		else
			cacheHtml = YES;
	}
}

#pragma mark -
#pragma mark shake functions
- (void)shakeAction 
{
	LogMethod();
	NSDate *today = [NSDate date];
	NSString *versionString = [NSString stringWithFormat:@"Version:1.0 (%@)",[today description]];
	[Public alertMessage:@"Version" addMsg:versionString];
}

- (BOOL) AccelerationIsShakingLast:(UIAcceleration *)last current:(UIAcceleration *)current threshold:(double)threshold {
    double
    deltaX = fabs(last.x - current.x),
    deltaY = fabs(last.y - current.y),
    deltaZ = fabs(last.z - current.z);
	
    return
    (deltaX > threshold && deltaY > threshold) ||
    (deltaX > threshold && deltaZ > threshold) ||
    (deltaY > threshold && deltaZ > threshold);
}

// UIAccelerometer delegate method, which delivers the latest acceleration data.
- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
	if (self.lastAcceleration) {
        if ([self AccelerationIsShakingLast:self.lastAcceleration current:acceleration threshold:0.7] && shakeCount >= SHAKE_VALUE) {
			//Shaking here, DO stuff.
			[self shakeAction];
			shakeCount = 0;
        } else if ([self AccelerationIsShakingLast:self.lastAcceleration current:acceleration threshold:0.7]) {
			shakeCount = shakeCount + 5;
        }else if (![self AccelerationIsShakingLast:self.lastAcceleration current:acceleration threshold:0.2]) {
			if (shakeCount > 0) {
				shakeCount--;
			}
        }
    }
    self.lastAcceleration = acceleration;
}

@end
