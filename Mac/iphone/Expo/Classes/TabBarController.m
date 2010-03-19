//
//  TabBarController.m
//  Expo
//
//  Created by luke on 10-3-19.
//  Copyright 2010 Luke. All rights reserved.
//
#import "TabBarController.h"

@implementation TabBarController

///////////////////////////////////////////////////////////////////////////////////////////////////
// UIViewController

- (void)viewDidLoad {
	[self setTabURLs:[NSArray arrayWithObjects:@"tt://menu/1",
					  @"tt://menu/2",
					  @"tt://menu/3",
					  @"tt://menu/4",
					  @"tt://menu/5",
					  nil]];
}

@end
