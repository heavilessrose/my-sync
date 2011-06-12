//
//  UIView+LKAddition.h
//  SK
//
//  Created by luke on 10-11-17.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface UIView (LKAddition)

+ (void)drawGradientInRect:(CGRect)rect withColors:(NSArray*)colors;
+ (void)drawLinearGradientInRect:(CGRect)rect colors:(CGFloat[])colors;

+ (void)drawRectangleInRect:(CGRect)rect color:(UIColor*)color;
+ (void)drawDialogBoxInRect:(CGRect)rect withRadius:(CGFloat)radius strokeColors:(CGFloat[])sColor fillColor:(CGFloat[])fColor;
+ (void)drawRoundRectangleInRect:(CGRect)rect withRadius:(CGFloat)radius color:(UIColor*)color;

+ (void)drawLineInRect:(CGRect)rect red:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha;
+ (void)drawLineInRect:(CGRect)rect colors:(CGFloat[])colors;
+ (void)drawLineInRect:(CGRect)rect colors:(CGFloat[])colors width:(CGFloat)lineWidth cap:(CGLineCap)cap;

@end
