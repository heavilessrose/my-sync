//
//  UIImage+roundCorner.m
//  Picoffee
//
//  Created by luke on 10-7-17.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "UIImage+roundCorner.h"


@implementation UIImage (roundCorner)
/*
static void addRoundedRectToPath(CGContextRef context, CGRect rect, float ovalWidth,float ovalHeight) 
{ 
	float fw, fh; 
	if (ovalWidth == 0 || ovalHeight == 0) { 
		CGContextAddRect(context, rect); 
		return; 
	} 
	
	CGContextSaveGState(context); 
	CGContextTranslateCTM(context, CGRectGetMinX(rect), CGRectGetMinY(rect)); 
	CGContextScaleCTM(context, ovalWidth, ovalHeight); 
	fw = CGRectGetWidth(rect) / ovalWidth;
	fh = CGRectGetHeight(rect) / ovalHeight; 
	CGContextMoveToPoint(context, fw, fh/2); 
	CGContextAddArcToPoint(context, fw, fh, fw/2, fh, 1); 
	CGContextAddArcToPoint(context, 0, fh, 0, fh/2, 1); 
	CGContextAddArcToPoint(context, 0, 0, fw/2, 0, 1); 
	CGContextAddArcToPoint(context, fw, 0, fw, fh/2, 1); 
	CGContextClosePath(context); 
	CGContextRestoreGState(context); 
}

+ (UIImage *)makeRoundCornerImageWithWidth:(int)width withHeight:(int)height 
							   cornerWidth:(int)cornerWidth cornerHeight:(int)cornerHeight
{
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGContextRef context = CGBitmapContextCreate(NULL, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedFirst);
	CGRect rect = CGRectMake(0, 0, width, height);
	
	CGContextSetAllowsAntialiasing(context,NO);
	
	addRoundedRectToPath(context, rect, cornerWidth, cornerHeight);
	
	CGContextSetRGBStrokeColor(context, 0, 0, 0, 1);
	CGContextSetLineWidth (context,1);
	CGContextDrawPath(context, kCGPathStroke);
	
	CGImageRef imageMasked = CGBitmapContextCreateImage(context);
	
	CGContextRelease(context);
	CGColorSpaceRelease(colorSpace);
	
	return [UIImage imageWithCGImage:imageMasked];	
}
*/
#pragma mark 2
+ (UIImage *)roundedImage:(UIImage *)img targetRect:(CGRect)targetRect
{
	CGContextRef mainViewContentContext;
	CGColorSpaceRef colorSpace;
	CGSize targetSize = targetRect.size;
	
	colorSpace = CGColorSpaceCreateDeviceRGB();
	
	// create a bitmap graphics context the size of the image
	mainViewContentContext = CGBitmapContextCreate (NULL, targetSize.width, targetSize.height, 8, 0, colorSpace, kCGImageAlphaPremultipliedLast);
	
	// free the rgb colorspace
	CGColorSpaceRelease(colorSpace);    
	
	if (mainViewContentContext==NULL)
		return NULL;
	
	CGImageRef maskImage = [img CGImage];
	CGContextClipToMask(mainViewContentContext, CGRectMake(0, 0, targetSize.width, targetSize.height), maskImage);
	CGContextDrawImage(mainViewContentContext, CGRectMake(targetRect.origin.x, targetRect.origin.y, targetSize.width, targetSize.height), maskImage);
	
	
	// Create CGImageRef of the main view bitmap content, and then
	// release that bitmap context
	CGImageRef mainViewContentBitmapContext = CGBitmapContextCreateImage(mainViewContentContext);
	CGContextRelease(mainViewContentContext);
	
	// convert the finished resized image to a UIImage 
	UIImage *theImage = [UIImage imageWithCGImage:mainViewContentBitmapContext];
	// image is retained by the property setting above, so we can 
	// release the original
	CGImageRelease(mainViewContentBitmapContext);
	
	// return the image
	return theImage;
}

#pragma mark 3
static void addRoundedRectToPath(CGContextRef context, CGRect rect, float ovalWidth, float ovalHeight)
{
    float fw, fh;
    if (ovalWidth == 0 || ovalHeight == 0) {
        CGContextAddRect(context, rect);
        return;
    }
    CGContextSaveGState(context);
    CGContextTranslateCTM (context, CGRectGetMinX(rect), CGRectGetMinY(rect));
    CGContextScaleCTM (context, ovalWidth, ovalHeight);
    fw = CGRectGetWidth (rect) / ovalWidth;
    fh = CGRectGetHeight (rect) / ovalHeight;
    CGContextMoveToPoint(context, fw, fh/2);
    CGContextAddArcToPoint(context, fw, fh, fw/2, fh, 1);
    CGContextAddArcToPoint(context, 0, fh, 0, fh/2, 1);
    CGContextAddArcToPoint(context, 0, 0, fw/2, 0, 1);
    CGContextAddArcToPoint(context, fw, 0, fw, fh/2, 1);
    CGContextClosePath(context);
    CGContextRestoreGState(context);
}

+ (UIImage *)makeRoundCornerImage:(UIImage*)img cornerWidth:(int)cornerWidth cornerHeight:(int)cornerHeight
{
	UIImage * newImage = nil;
	
	if( nil != img)
	{
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		int w = img.size.width;
		int h = img.size.height;
		
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		CGContextRef context = CGBitmapContextCreate(NULL, w, h, 8, 4 * w, colorSpace, kCGImageAlphaPremultipliedFirst);
		
		CGContextBeginPath(context);
		CGRect rect = CGRectMake(0, 0, img.size.width, img.size.height);
		addRoundedRectToPath(context, rect, cornerWidth, cornerHeight);
		CGContextClosePath(context);
		CGContextClip(context);
		
		CGContextDrawImage(context, CGRectMake(0, 0, w, h), img.CGImage);
		
		CGImageRef imageMasked = CGBitmapContextCreateImage(context);
		CGContextRelease(context);
		CGColorSpaceRelease(colorSpace);
		[img release];
		
		newImage = [[UIImage imageWithCGImage:imageMasked] retain];
		CGImageRelease(imageMasked);
		
		[pool release];
	}
	
    return newImage;
}

@end
