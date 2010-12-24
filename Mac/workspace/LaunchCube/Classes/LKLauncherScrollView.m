//
//  LKLauncherScrollView.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKLauncherScrollView.h"


@implementation LKLauncherScrollView

- (BOOL)touchesShouldCancelInContentView:(UIView *)view {
	return !self.delaysContentTouches;
}

@end
