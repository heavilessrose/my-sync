/*
 * Copyright (c) 2010-2010 Sebastian Celis
 * All rights reserved.
 */

#import "SCAppUtils.h"

@implementation SCAppUtils

+ (void)customizeNavigationController:(UINavigationController *)navController withImg:(NSString *)imgName
{
    UINavigationBar *navBar = [navController navigationBar];
    [navBar setTintColor:kSCNavigationBarTintColor];
    
    UIImageView *imageView = (UIImageView *)[navBar viewWithTag:kSCNavigationBarBackgroundImageTag];
    if (imageView == nil)
    {
        imageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:imgName]];
        [imageView setTag:kSCNavigationBarBackgroundImageTag];
        [navBar insertSubview:imageView atIndex:0];
        [imageView release];
    }
}

@end
