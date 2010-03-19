//
//  MenuController.m
//  Expo
//
//  Created by luke on 10-3-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import "MenuController.h"


@implementation MenuController

@synthesize page = _page;

#pragma mark private
- (NSString*)nameForMenuPage:(MenuPage)page {
	switch (page) {
		case MenuPageCountry:
			return @"Country";
		case MenuPageMap:
			return @"Map";
		case MenuPageSearch:
			return @"Search";
		case MenuPageSouvenir:
			return @"Souvenir";
		case MenuPageAbout:
			return @"About";
		default:
			return @"";
	}
}

#pragma mark NSObject

- (id)initWithMenu:(MenuPage)page {
	if (self = [super init]) {
		self.page = page;
	}
	return self;
}

- (id)init {
	if (self = [super init]) {
		_page = MenuPageNone;
	}
	return self;
}

- (void)dealloc {
	[super dealloc];
}

#pragma mark TTViewController

- (void)setPage:(MenuPage)page {
	_page = page;
	
	self.title = [self nameForMenuPage:page];
	
	UIImage* image = [UIImage imageNamed:@"tab.png"];
	self.tabBarItem = [[[UITabBarItem alloc] initWithTitle:self.title image:image tag:0] autorelease];
	
	self.navigationItem.rightBarButtonItem =
    [[[UIBarButtonItem alloc] initWithTitle:@"Order" style:UIBarButtonItemStyleBordered
									 target:@"tt://order?waitress=Betty&ref=toolbar"
									 action:@selector(openURLFromButton:)] autorelease];
	
	if (_page == MenuPageCountry) {
		self.dataSource = [TTSectionedDataSource dataSourceWithObjects:
						   @"ASIA",
						   [TTTableSubtitleItem itemWithText:@"China" subtitle:@"ChinaInfo" imageURL:@"bundle://China.png"
												defaultImage:TTIMAGE(@"bundle://tab.png") URL:@"tt://country/China" accessoryURL:nil],
						   //[TTTableTextItem itemWithText:@"China" URL:@"tt://country/China"],
						   [TTTableTextItem itemWithText:@"Japan" URL:@"tt://country/Japan"],
						   @"EURO",
						   [TTTableTextItem itemWithText:@"France" URL:@"tt://country/France"],
						   [TTTableTextItem itemWithText:@"German" URL:@"tt://country/German"],
						   @"Other",
						   nil];
	} else if (_page == MenuPageMap) {
		self.dataSource = [TTSectionedDataSource dataSourceWithObjects:
						   @"Map",
						   nil];
	} else if (_page == MenuPageSearch) {
		self.dataSource = [TTSectionedDataSource dataSourceWithObjects:
						   @"Search",
						   nil];
	} else if (_page == MenuPageSouvenir) {
		self.dataSource = [TTSectionedDataSource dataSourceWithObjects:
						   @"Souvenir",
						   [TTTableTextItem itemWithText:@"Souvenir 1" URL:@"tt://Souvenir/1"],
						   [TTTableTextItem itemWithText:@"Souvenir 2" URL:@"tt://Souvenir/2"],
						   @"Other",
						   nil];
	} else if (_page == MenuPageAbout) {
		self.dataSource = [TTListDataSource dataSourceWithObjects:
						   [TTTableTextItem itemWithText:@"Our Story" URL:@"tt://about/story"],
						   [TTTableTextItem itemWithText:@"Call Us" URL:@"tel:5555555"],
						   [TTTableTextItem itemWithText:@"Text Us" URL:@"sms:5555555"],
						   [TTTableTextItem itemWithText:@"Website" URL:@"http://www.diffchina.com/expo/index.html"],
						   [TTTableTextItem itemWithText:@"Complaints Dept." URL:@"tt://about/complaints"],
						   nil];
	}
}

@end
