//
//  UIView+LKAddition.m
//  SK
//
//  Created by luke on 10-11-17.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "UIView+LKAddition.h"

@implementation UIView (LKAddition)

static CGPoint gStart(CGRect bounds) {
	
	return CGPointMake(bounds.origin.x, bounds.origin.y + bounds.size.height * 0.25);
}

static CGPoint gEnd(CGRect bounds) {
	
	return CGPointMake(bounds.origin.x, bounds.origin.y + bounds.size.height * 0.75);
}

static CGPoint gCenter(CGRect bounds) {
	
	return CGPointMake(CGRectGetMidX(bounds), CGRectGetMidY(bounds));
}

static CGFloat gInnerRadius(CGRect bounds) {
	
	CGFloat r = bounds.size.width < bounds.size.height ? bounds.size.width : bounds.size.height;
	return r * 0.125;
}

#pragma mark -

+ (void)drawGradientInRect:(CGRect)rect withColors:(NSArray *)colors {
	
	NSMutableArray *ar = [NSMutableArray array];
	for (UIColor *c in colors) {
		[ar addObject:(id)c.CGColor];
	}
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSaveGState(context);
	
	CGColorSpaceRef colorSpace = CGColorGetColorSpace([[colors lastObject] CGColor]);
	CGGradientRef gradient = CGGradientCreateWithColors(colorSpace, (CFArrayRef)ar, NULL);
	
	CGContextClipToRect(context, rect);
	
	CGPoint start = CGPointMake(0.0, 0.0);
	CGPoint end = CGPointMake(0.0, rect.size.height);
	
	CGContextDrawLinearGradient(context, gradient, start, end, kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
	
	CGColorSpaceRelease(colorSpace);
	CGGradientRelease(gradient);
	CGContextRestoreGState(context);
}

+ (void)drawLinearGradientInRect:(CGRect)rect colors:(CGFloat[])colours {
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	CGContextSaveGState(context);
	
	CGColorSpaceRef rgb = CGColorSpaceCreateDeviceRGB();
	CGGradientRef gradient = CGGradientCreateWithColorComponents(rgb, colours, NULL, 2);
	CGColorSpaceRelease(rgb);
	CGPoint start, end;
	
	start = gStart(rect);
	end = gEnd(rect);
	
	CGContextClipToRect(context, rect);
	CGContextDrawLinearGradient(context, gradient, start, end, kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
	
	CGGradientRelease(gradient);
	
	CGContextRestoreGState(context);
}

+ (void)drawRectangleInRect:(CGRect)rect color:(UIColor*)color {
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	[color set];
	CGContextAddRect(context, rect);
	CGContextClosePath(context);
	CGContextDrawPath(context, kCGPathFillStroke);
}
	
+ (void)drawRoundRectangleInRect:(CGRect)rect withRadius:(CGFloat)radius color:(UIColor*)color {
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	[color set];
	
	CGRect rrect = CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height );
	
	CGFloat minx = CGRectGetMinX(rrect), midx = CGRectGetMidX(rrect), maxx = CGRectGetMaxX(rrect);
	CGFloat miny = CGRectGetMinY(rrect), midy = CGRectGetMidY(rrect), maxy = CGRectGetMaxY(rrect);
	CGContextMoveToPoint(context, minx, midy);
	CGContextAddArcToPoint(context, minx, miny, midx, miny, radius);
	CGContextAddArcToPoint(context, maxx, miny, maxx, midy, radius);
	CGContextAddArcToPoint(context, maxx, maxy, midx, maxy, radius);
	CGContextAddArcToPoint(context, minx, maxy, minx, midy, radius);
	CGContextClosePath(context);
	CGContextDrawPath(context, kCGPathFill);
}

+ (void)drawDialogBoxInRect:(CGRect)rect withRadius:(CGFloat)radius strokeColors:(CGFloat[])sColor fillColor:(CGFloat[])fColor {
	
#define kOffset_Triangle 30
#define kTriangle_H (12)
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	CGRect rrect = CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height );
	
	CGFloat minx = CGRectGetMinX(rrect), midx = CGRectGetMidX(rrect), maxx = CGRectGetMaxX(rrect);
	CGFloat miny = CGRectGetMinY(rrect), midy = CGRectGetMidY(rrect), maxy = CGRectGetMaxY(rrect);
	CGContextMoveToPoint(context, minx+kOffset_Triangle+kTriangle_H, miny);
	CGContextAddArcToPoint(context, maxx, miny, maxx, midy, radius);
	CGContextAddArcToPoint(context, maxx, maxy, midx, maxy, radius);
	CGContextAddArcToPoint(context, minx, maxy, minx, midy, radius);
	CGContextAddArcToPoint(context, minx, miny, midx, miny, radius);
	
	CGContextAddLineToPoint(context, minx+kOffset_Triangle, miny);
	CGContextAddLineToPoint(context, minx+kOffset_Triangle+kTriangle_H/2, miny-kTriangle_H/2);
	CGContextAddLineToPoint(context, minx+kOffset_Triangle+kTriangle_H, miny);
	CGContextClosePath(context);
	
	CGContextSetLineWidth(context, 1);
	CGContextSetStrokeColor(context, sColor);
	CGContextSetFillColor(context, fColor);
	CGFloat lineDash[2];  
	lineDash[0] = 1.0f;  
	lineDash[1] = 1.0f;   
	CGContextSetLineDash(context, 1.0f,  lineDash, 2);
	//CGContextSetLineCap(context,1.0);
	CGContextDrawPath(context, kCGPathEOFillStroke);
}

+ (void)drawLineInRect:(CGRect)rect colors:(CGFloat[])colors {
	
	[UIView drawLineInRect:rect colors:colors width:1 cap:kCGLineCapButt];
}

+ (void)drawLineInRect:(CGRect)rect red:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha {
	
	CGFloat colors[4];
	colors[0] = red;
	colors[1] = green;
	colors[2] = blue;
	colors[3] = alpha;
	[UIView drawLineInRect:rect colors:colors];
}

+ (void)drawLineInRect:(CGRect)rect colors:(CGFloat[])colors width:(CGFloat)lineWidth cap:(CGLineCap)cap {
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSaveGState(context);
	
	CGContextSetRGBStrokeColor(context, colors[0], colors[1], colors[2], colors[3]);
	CGContextSetLineCap(context,cap);
	CGContextSetLineWidth(context, lineWidth);
	
	CGContextMoveToPoint(context, rect.origin.x, rect.origin.y);
	CGContextAddLineToPoint(context,rect.origin.x+rect.size.width, rect.origin.y+rect.size.height);
	CGContextStrokePath(context);
	
	CGContextRestoreGState(context);
}


#pragma mark -
#pragma mark NSCopying
- (id)copyWithZone:(NSZone *)zone {
	return self;
}

@end
