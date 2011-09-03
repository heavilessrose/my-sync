/*
 * Copyright (c) 2010-2010 Sebastian Celis
 * All rights reserved.
 */

#import <UIKit/UIKit.h>

#define kSCNavigationBarBackgroundImageTag 6183746
#define kSCNavigationBarTintColor [UIColor colorWithRed:0.39 green:0.72 blue:0.62 alpha:1.0]

#define kSCNavBarImgName            (@"bg_navbar.png")
#define kSCNavBarImgWithLogoName    (@"bg_navbar_logo.png")

@interface SCAppUtils : NSObject
{
}

+ (void)customizeNavigationController:(UINavigationController *)navController withImg:(NSString *)imgName;

@end
