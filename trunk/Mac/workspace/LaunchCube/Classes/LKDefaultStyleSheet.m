//
//  LKDefaultStyleSheet.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKDefaultStyleSheet.h"
#import "LKStyle.h"
#import "LKPartStyle.h"
#import "LKTextStyle.h"
#import "LKBoxStyle.h"
#import "LKShapeStyle.h"
#import "LKImageStyle.h"
#import "LKBlendStyle.h"
#import "LKSolidFillStyle.h"
#import "LKInsetStyle.h"
#import "LKSolidBorderStyle.h"
#import "LKShadowStyle.h"
#import "LKRoundedRectangleShape.h"


@implementation LKDefaultStyleSheet

#define RGBCOLOR(r,g,b) [UIColor colorWithRed:(r)/255.0 green:(g)/255.0 blue:(b)/255.0 alpha:1]
#define RGBACOLOR(r,g,b,a) [UIColor colorWithRed:(r)/255.0 green:(g)/255.0 blue:(b)/255.0 alpha:(a)]

#define LKSTYLESTATE(_SELECTOR, _STATE) [[LKStyleSheet globalStyleSheet] styleWithSelector:@#_SELECTOR forState:_STATE]

#define LKSTYLE(_SELECTOR) [[LKStyleSheet globalStyleSheet] styleWithSelector:@#_SELECTOR]

- (LKStyle *)launcherButton:(UIControlState)state {
	return
    [LKPartStyle styleWithName:@"image" style:LKSTYLESTATE(launcherButtonImage:, state) next:
	 [LKTextStyle styleWithFont:[UIFont boldSystemFontOfSize:11] color:RGBCOLOR(180, 180, 180)
				minimumFontSize:11 shadowColor:nil
				   shadowOffset:CGSizeZero next:nil]];
}

- (LKStyle *)launcherButtonImage:(UIControlState)state {
	LKStyle *style =
    [LKBoxStyle styleWithMargin:UIEdgeInsetsMake(-7, 0, 11, 0) next:
	 [LKShapeStyle styleWithShape:[LKRoundedRectangleShape shapeWithRadius:8] next:
	  [LKImageStyle styleWithImageURL:nil defaultImage:nil contentMode:UIViewContentModeCenter
								 size:CGSizeZero next:nil]]];
	
	if (state == UIControlStateHighlighted || state == UIControlStateSelected) {
		[style addStyle:
		 [LKBlendStyle styleWithBlend:kCGBlendModeSourceAtop next:
		  [LKSolidFillStyle styleWithColor:RGBACOLOR(0,0,0,0.5) next:nil]]];
	}
	
	return style;
}

- (LKStyle *)launcherCloseButtonImage:(UIControlState)state {
	return
    [LKBoxStyle styleWithMargin:UIEdgeInsetsMake(-2, 0, 0, 0) next:
	 [LKImageStyle styleWithImageURL:nil defaultImage:nil contentMode:UIViewContentModeCenter
								size:CGSizeMake(10,10) next:nil]];
}

- (LKStyle *)launcherCloseButton:(UIControlState)state {
	return
    [LKShapeStyle styleWithShape:[LKRoundedRectangleShape shapeWithRadius:-1] next:
	 [LKInsetStyle styleWithInset:UIEdgeInsetsMake(1, 1, 1, 1) next:
	  [LKShadowStyle styleWithColor:RGBACOLOR(0,0,0,0.5) blur:2 offset:CGSizeMake(0, 3) next:
	   [LKSolidFillStyle styleWithColor:[UIColor blackColor] next:
		[LKInsetStyle styleWithInset:UIEdgeInsetsMake(-1, -1, -1, -1) next:
		 [LKSolidBorderStyle styleWithColor:[UIColor whiteColor] width:2 next:
		  [LKPartStyle styleWithName:@"image" style:LKSTYLE(launcherCloseButtonImage:) next:
		   nil]]]]]]];
}

- (LKStyle *)pageDot:(UIControlState)state {
	if (state == UIControlStateSelected) {
		return [self pageDotWithColor:[UIColor whiteColor]];
	} else {
		return [self pageDotWithColor:RGBCOLOR(77, 77, 77)];
	}
}

- (LKStyle *)launcherPageDot:(UIControlState)state {
	return [self pageDot:state];
}

- (LKStyle *)pageDotWithColor:(UIColor *)color {
	return
    [LKBoxStyle styleWithMargin:UIEdgeInsetsMake(0,0,0,10) padding:UIEdgeInsetsMake(6,6,0,0) next:
	 [LKShapeStyle styleWithShape:[LKRoundedRectangleShape shapeWithRadius:2.5] next:
	  [LKSolidFillStyle styleWithColor:color next:nil]]];
}

@end
