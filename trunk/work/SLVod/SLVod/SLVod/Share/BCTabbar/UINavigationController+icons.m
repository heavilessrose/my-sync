//
//  UINavigationController+icons.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "UINavigationController+icons.h"


@implementation UINavigationController (BCTabBarController)

- (NSString *)iconImageName {
	return [[self.viewControllers objectAtIndex:0] iconImageName];
}

@end
