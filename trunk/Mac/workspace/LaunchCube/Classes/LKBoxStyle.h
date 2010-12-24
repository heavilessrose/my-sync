//
//  LKBoxStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"
#import "LKPosition.h"

@interface LKBoxStyle : LKStyle {
	UIEdgeInsets  margin;
	UIEdgeInsets  padding;
	CGSize        minSize;
	LKPosition    position;
}

@property (nonatomic) UIEdgeInsets  margin;
@property (nonatomic) UIEdgeInsets  padding;
@property (nonatomic) CGSize        minSize;
@property (nonatomic) LKPosition    position;

+ (LKBoxStyle *)styleWithMargin:(UIEdgeInsets)margin next:(LKStyle *)next;
+ (LKBoxStyle *)styleWithPadding:(UIEdgeInsets)padding next:(LKStyle *)next;
+ (LKBoxStyle *)styleWithFloats:(LKPosition)position next:(LKStyle *)next;
+ (LKBoxStyle *)styleWithMargin:(UIEdgeInsets)margin padding:(UIEdgeInsets)padding
                          next:(LKStyle *)next;
+ (LKBoxStyle *)styleWithMargin:(UIEdgeInsets)margin padding:(UIEdgeInsets)padding
                       minSize:(CGSize)minSize position:(LKPosition)position next:(LKStyle *)next;

@end

