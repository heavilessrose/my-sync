//
//  LKSolidFillStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"


@interface LKSolidFillStyle : LKStyle {
	
	UIColor *color;
}

@property (nonatomic, retain) UIColor *color;

+ (LKSolidFillStyle *)styleWithColor:(UIColor *)color next:(LKStyle *)next;

@end
