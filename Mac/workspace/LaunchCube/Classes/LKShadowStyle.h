//
//  LKShadowStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"

@interface LKShadowStyle : LKStyle {
	
	UIColor *color;
	CGFloat blur;
	CGSize  offset;
}

@property (nonatomic, retain) UIColor *color;
@property (nonatomic)         CGFloat blur;
@property (nonatomic)         CGSize  offset;

+ (LKShadowStyle *)styleWithColor:(UIColor *)color blur:(CGFloat)blur offset:(CGSize)offset
                            next:(LKStyle *)next;
@end
