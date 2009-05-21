//
//  ViewSwitcherAppDelegate.m
//  ViewSwitcher
//
//  Created by wang luke on 5/21/09.
//  Copyright luke 2009. All rights reserved.
//

#import "ViewSwitcherAppDelegate.h"
#import "SwitchViewController.h"

@implementation ViewSwitcherAppDelegate

@synthesize window;
@synthesize switchViewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    

    // 将根控制器的视图添加到应用程序的主窗口
	[window addSubview:switchViewController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
	[switchViewController release];
    [super dealloc];
}


@end
