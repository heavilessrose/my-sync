//
//  ExpoAppDelegate.m
//  Expo
//
//  Created by luke on 10-3-19.
//  Copyright Luke 2010. All rights reserved.
//

#import "AppDelegate.h"
#import "TabBarController.h"
#import "TableMenuController.h"
#import "CountryController.h"
#import "SearchPhotosViewController.h"
#import "PhotoInfoController.h"

@implementation AppDelegate


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	TTNavigator* navigator = [TTNavigator navigator];
	navigator.persistenceMode = TTNavigatorPersistenceModeAll;
	navigator.window = [[[UIWindow alloc] initWithFrame:TTScreenBounds()] autorelease];
	
	TTURLMap* map = navigator.URLMap;
	
	// 单件tab bar controller. 加载此URL显示tab bar.
	[map from:@"tt://tabBar" toSharedViewController:[TabBarController class]];
	
	// 每个tab创建一个单件TableMenuController, 加载这些URL会切换到包含这些menu的tab
	[map from:@"tt://menu/(initWithMenu:)" toSharedViewController:[TableMenuController class]];
	
	// 每次打开country url都会创建一个新的 country controllers
	[map from:@"tt://country/(initWithCountry:)" toViewController:[CountryController class]];
	
	// 打开 CountryInfo page 有 flip效果
	[map from:@"tt://country/(initWithCountryInfo:)/CountryInfo" toViewController:[CountryController class]
			transition:UIViewAnimationTransitionFlipFromLeft];
	// 图片搜索
	[map from:@"tt://search/Image" toSharedViewController:[SearchPhotosViewController class]];
	
	// 图片信息
	[map from:@"tt://photo/(initWithPhotoItem:)/PhotoInfo" toViewController:[PhotoInfoController class]
			transition:UIViewAnimationTransitionFlipFromLeft];
	
	// 尝试恢复上次退出时的状态
	if (![navigator restoreViewControllers]) {
		// This is the first launch, so we just start with the tab bar
		[navigator openURLAction:[TTURLAction actionWithURLPath:@"tt://tabBar"]];
	}
}


- (void)dealloc {
    [super dealloc];
}


@end
