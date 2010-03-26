//
//  CountryController.m
//  Expo
//
//  Created by luke on 10-3-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import "CountryController.h"


@implementation CountryController

@synthesize content = _content, text = _text;

#pragma mark private


- (void)dismiss {
	[self dismissModalViewControllerAnimated:YES];
}

//- (void)orderAction:(NSString*)action {
//	TTDINFO(@"ACTION: %@", action);
//}
//
- (void)showCountryInfo {
	TTOpenURL([NSString stringWithFormat:@"tt://country/%@/CountryInfo", self.content]);
}

#pragma mark NSObject

- (id)initWithCountry:(NSString*)country {
	if (self = [super init]) {
		
		self.view.backgroundColor = [UIColor blackColor];
		self.statusBarStyle = UIStatusBarStyleBlackTranslucent;
		self.navigationBarStyle = UIBarStyleBlackTranslucent;
		self.navigationBarTintColor = nil;
		self.wantsFullScreenLayout = YES;
		self.hidesBottomBarWhenPushed = YES;
		
		_contentType = ContentTypeCountry;
		self.content = country;
		self.text = [NSString stringWithFormat:@"<b>%@</b> is just country..", country];
		
		self.title = country;
		self.navigationItem.rightBarButtonItem =
		[[[UIBarButtonItem alloc] initWithTitle:@"Info" style:UIBarButtonItemStyleBordered
										 target:self action:@selector(showCountryInfo)] autorelease];
	}
	return self;
}

- (id)initWithCountryInfo:(NSString*)country {
	if (self = [super init]) {
		
		self.view.backgroundColor = [UIColor blackColor];
		self.statusBarStyle = UIStatusBarStyleBlackOpaque;
		self.navigationBarStyle = UIBarStyleBlackOpaque;
		self.navigationBarTintColor = nil;
		self.wantsFullScreenLayout = NO;
		self.hidesBottomBarWhenPushed = NO;
		
		_contentType = ContentTypeCountryInfo;
		self.content = country;
		self.text = [NSString stringWithFormat:@"<b>%@</b> is good..", country];
		
		self.title = @"Country Info";
	}
	return self;
}

- (id)init {
	if (self = [super init]) {
		_contentType = ContentTypeNone;
		_content = nil;
		_text = nil;
		
		self.view.backgroundColor = [UIColor blackColor];
		self.statusBarStyle = UIStatusBarStyleBlackTranslucent;
		self.navigationBarStyle = UIBarStyleBlackTranslucent;
		self.navigationBarTintColor = nil;
		self.wantsFullScreenLayout = YES;
		self.hidesBottomBarWhenPushed = YES;
	}
	return self;
}

- (void)dealloc {
	TT_RELEASE_SAFELY(_content);
	TT_RELEASE_SAFELY(_text);
	[super dealloc];
}

#pragma mark UIViewController

- (void)loadView {
	[super loadView];
	
	
	CGFloat start_x = 10;
	CGFloat start_y = 10 + NAV_BAR_HEIGHT + STATUS_BAR_HEIGHT;
	CGRect frame = CGRectMake(start_x, start_y, self.view.width-(2 * start_x), 480 - (2 * start_y));
	TTStyledTextLabel* label = [[[TTStyledTextLabel alloc] initWithFrame:frame] autorelease];
	label.tag = 42;
	label.font = [UIFont systemFontOfSize:22];
	[self.view addSubview:label];
	
	if (_contentType == ContentTypeCountryInfo) {
		self.view.backgroundColor = [UIColor grayColor];
		label.backgroundColor = self.view.backgroundColor;
		self.hidesBottomBarWhenPushed = YES;
	} else if (_contentType == ContentTypeOrder) {
		UIButton* button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		[button setTitle:@"What do you want to eat?" forState:UIControlStateNormal];
		[button addTarget:@"tt://order/food" action:@selector(openURLFromButton:)
		 forControlEvents:UIControlEventTouchUpInside];
		[button sizeToFit];
		button.top = label.bottom + 20;
		button.left = floor(self.view.width/2 - button.width/2);
		[self.view addSubview:button];
	}
}

- (void)viewWillAppear:(BOOL)animated {
	TTStyledTextLabel* label = (TTStyledTextLabel*)[self.view viewWithTag:42];
	label.html = _text;
}

@end
