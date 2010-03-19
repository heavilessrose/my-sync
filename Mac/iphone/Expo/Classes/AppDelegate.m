//
//  ExpoAppDelegate.m
//  Expo
//
//  Created by luke on 10-3-19.
//  Copyright Luke 2010. All rights reserved.
//

#import "AppDelegate.h"
//#import "TabBarController.h"
//#import "MenuController.h"
//#import "ContentController.h"

@implementation AppDelegate


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	TTNavigator* navigator = [TTNavigator navigator];
	navigator.persistenceMode = TTNavigatorPersistenceModeAll;
	navigator.window = [[[UIWindow alloc] initWithFrame:TTScreenBounds()] autorelease];
	
	TTURLMap* map = navigator.URLMap;

}


- (void)dealloc {
    [super dealloc];
}


@end
