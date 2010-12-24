//
//  LKTextStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"


@interface LKTextStyle : LKStyle {
	
	UIFont *font;
	UIColor *color;
	
	UIColor *shadowColor;
	CGSize shadowOffset;
	
	CGFloat minimumFontSize;
	NSInteger numberOfLines;
	
	UITextAlignment textAlignment;
	UIControlContentVerticalAlignment verticalAlignment;
	
	UILineBreakMode lineBreakMode;
}

@property (nonatomic, retain) UIFont *font;
@property (nonatomic, retain) UIColor *color;

@property (nonatomic, retain) UIColor *shadowColor;
@property (nonatomic)         CGSize shadowOffset;

@property (nonatomic)         CGFloat minimumFontSize;
@property (nonatomic)         NSInteger numberOfLines;

@property (nonatomic)         UITextAlignment                   textAlignment;
@property (nonatomic)         UIControlContentVerticalAlignment verticalAlignment;

@property (nonatomic)         UILineBreakMode lineBreakMode;

+ (LKTextStyle *)styleWithFont:(UIFont *)font next:(LKStyle *)next;
+ (LKTextStyle *)styleWithColor:(UIColor *)color next:(LKStyle *)next;
+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color next:(LKStyle *)next;
+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color
                textAlignment:(UITextAlignment)textAlignment next:(LKStyle *)next;
+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color
                  shadowColor:(UIColor *)shadowColor shadowOffset:(CGSize)shadowOffset
                         next:(LKStyle *)next;
+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color
              minimumFontSize:(CGFloat)minimumFontSize
                  shadowColor:(UIColor *)shadowColor shadowOffset:(CGSize)shadowOffset
                         next:(LKStyle *)next;
+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor*)color
              minimumFontSize:(CGFloat)minimumFontSize
                  shadowColor:(UIColor *)shadowColor shadowOffset:(CGSize)shadowOffset
                textAlignment:(UITextAlignment)textAlignment
            verticalAlignment:(UIControlContentVerticalAlignment)verticalAlignment
                lineBreakMode:(UILineBreakMode)lineBreakMode numberOfLines:(NSInteger)numberOfLines
                         next:(LKStyle *)next;

@end
