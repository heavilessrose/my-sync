//
//  UIImage+Resizeable.h
//  ColorSet
//
//  Created by luke on 09-11-13.
//  Copyright 2009 winksi.com. All rights reserved.
//

#import <UIKit/UIKit.h>

@class UIImage;

@interface UIImage (Resizeable)

+ (UIImage*)imageWithImage:(UIImage*)image scaledToSize:(CGSize)newSize;
- (UIImage*)imageByScalingToSize:(CGSize)newSize;
@end
