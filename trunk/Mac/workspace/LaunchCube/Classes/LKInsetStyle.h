//
//  LKInsetStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"

@interface LKInsetStyle : LKStyle {
	
	UIEdgeInsets inset;
}

@property (nonatomic) UIEdgeInsets inset;

+ (LKInsetStyle *)styleWithInset:(UIEdgeInsets)inset next:(LKStyle *)next;

@end
