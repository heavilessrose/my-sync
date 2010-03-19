//
//  MenuController.h
//  Expo
//
//  Created by luke on 10-3-19.
//  Copyright 2010 Luke. All rights reserved.
//
#import <Three20/Three20.h>

typedef enum {
	MenuPageNone,
	MenuPageCountry, // 国家
	MenuPageMap, // 地图
	MenuPageSearch, // 搜索
	MenuPageSouvenir, // 纪念品
	MenuPageAbout, // 关于
} MenuPage;

@interface MenuController : TTTableViewController {
	MenuPage _page;
}

@property(nonatomic) MenuPage page;

@end
