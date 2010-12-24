//
//  LKImageStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"


@interface LKImageStyle : LKStyle {
	
	NSString*         imageURL;
	UIImage*          image;
	UIImage*          defaultImage;
	
	CGSize            size;
	
	UIViewContentMode contentMode;
}

@property (nonatomic, copy)   NSString *imageURL;
@property (nonatomic, retain) UIImage *image;
@property (nonatomic, retain) UIImage *defaultImage;
@property (nonatomic)         CGSize size;

@property (nonatomic)         UIViewContentMode contentMode;

+ (LKImageStyle *)styleWithImageURL:(NSString *)imageURL next:(LKStyle *)next;
+ (LKImageStyle *)styleWithImageURL:(NSString *)imageURL defaultImage:(UIImage *)defaultImage
                              next:(LKStyle *)next;
+ (LKImageStyle *)styleWithImageURL:(NSString *)imageURL defaultImage:(UIImage *)defaultImage
                       contentMode:(UIViewContentMode)contentMode
                              size:(CGSize)size next:(LKStyle *)next;
+ (LKImageStyle *)styleWithImage:(UIImage *)image next:(LKStyle *)next;
+ (LKImageStyle *)styleWithImage:(UIImage *)image defaultImage:(UIImage *)defaultImage
                           next:(LKStyle *)next;
+ (LKImageStyle *)styleWithImage:(UIImage *)image defaultImage:(UIImage *)defaultImage
                    contentMode:(UIViewContentMode)contentMode
                           size:(CGSize)size next:(LKStyle *)next;
@end
