//
//  UIImage+Resizeable.m
//  ColorSet
//
//  Created by luke on 09-11-13.
//  Copyright 2009 winksi.com. All rights reserved.
//

#import "UIImage+Resizeable.h"

@implementation UIImage (Resizeable)

+ (UIImage*)imageWithImage:(UIImage*)image scaledToSize:(CGSize)newSize;
{
	UIGraphicsBeginImageContext(newSize);
	[image drawInRect:CGRectMake(0,0,newSize.width,newSize.height)];
	UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();
	
	return newImage;
}


- (UIImage*)imageByScalingToSize:(CGSize)newSize
{
	return [UIImage imageWithImage:self scaledToSize:newSize];
}
@end
