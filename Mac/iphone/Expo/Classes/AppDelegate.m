//
//  ExpoAppDelegate.m
//  Expo
//
//  Created by luke on 10-3-19.
//  Copyright Luke 2010. All rights reserved.
//

#import "AppDelegate.h"
#import "TabBarController.h"
#import "MenuController.h"
#import "CountryController.h"

@implementation AppDelegate


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	TTNavigator* navigator = [TTNavigator navigator];
	navigator.persistenceMode = TTNavigatorPersistenceModeAll;
	navigator.window = [[[UIWindow alloc] initWithFrame:TTScreenBounds()] autorelease];
	
	TTURLMap* map = navigator.URLMap;
	
	// 单件tab bar controller. 加载此URL显示tab bar.
	[map from:@"tt://tabBar" toSharedViewController:[TabBarController class]];
	
	// 每个tab创建一个单件Menu controllers, 加载这些URL会切换到包含这些menu的tab
	[map from:@"tt://menu/(initWithMenu:)" toSharedViewController:[MenuController class]];
	
	// 每次打开country url都会创建一个新的 country controllers
	[map from:@"tt://country/(initWithCountry:)" toViewController:[CountryController class]];
	
	// This is an example of how to use a transition.  Opening the nutrition page will do a flip
	[map from:@"tt://country/(initWithCountryInfo:)/CountryInfo" toViewController:[CountryController class]
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
