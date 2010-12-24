//
//  LKSolidBorderStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"


@interface LKSolidBorderStyle : LKStyle {
	
	UIColor *color;
	CGFloat width;
}

@property (nonatomic, retain) UIColor *color;
@property (nonatomic)         CGFloat width;

+ (LKSolidBorderStyle *)styleWithColor:(UIColor *)color width:(CGFloat)width next:(LKStyle *)next;

@end
