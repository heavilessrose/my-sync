//
//  UIImage+roundCorner.h
//  Picoffee
//
//  Created by luke on 10-7-17.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class UIImage;
@interface UIImage (roundCorner)

/*
+ (UIImage *)makeRoundCornerImageWithWidth:(int)width withHeight:(int)height 
							   cornerWidth:(int)cornerWidth cornerHeight:(int)cornerHeight;

+ (UIImage *)roundedImage:(UIImage *)img targetRect:(CGRect)targetRect;
*/

#pragma mark 3
+ (UIImage *)makeRoundCornerImage:(UIImage*)img cornerWidth:(int)cornerWidth cornerHeight:(int)cornerHeight;
@end
