//
//  LKRoundedRectangleShape.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKRoundedRectangleShape.h"


@implementation LKRoundedRectangleShape

@synthesize topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius;

#define RD(_RADIUS) (_RADIUS == -1 ? round(fh/2) : _RADIUS)

#pragma mark -
#pragma mark Class public

+ (LKRoundedRectangleShape *)shapeWithRadius:(CGFloat)radius {
	LKRoundedRectangleShape *shape = [[[LKRoundedRectangleShape alloc] init] autorelease];
	shape.topLeftRadius = shape.topRightRadius = shape.bottomRightRadius = shape.bottomLeftRadius
	= radius;
	return shape;
}

+ (LKRoundedRectangleShape *)shapeWithTopLeft:(CGFloat)topLeft topRight:(CGFloat)topRight
                                 bottomRight:(CGFloat)bottomRight bottomLeft:(CGFloat)bottomLeft {
	LKRoundedRectangleShape *shape = [[[LKRoundedRectangleShape alloc] init] autorelease];
	shape.topLeftRadius = topLeft;
	shape.topRightRadius = topRight;
	shape.bottomRightRadius = bottomRight;
	shape.bottomLeftRadius = bottomLeft;
	return shape;
}


#pragma mark -
#pragma mark Public

- (void)addToPath:(CGRect)rect {
	[self openPath:rect];
	
	CGFloat fw = rect.size.width;
	CGFloat fh = rect.size.height;
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextMoveToPoint(context, fw, floor(fh/2));
	CGContextAddArcToPoint(context, fw, fh, floor(fw/2), fh, RD(bottomRightRadius));
	CGContextAddArcToPoint(context, 0, fh, 0, floor(fh/2), RD(bottomLeftRadius));
	CGContextAddArcToPoint(context, 0, 0, floor(fw/2), 0, RD(topLeftRadius));
	CGContextAddArcToPoint(context, fw, 0, fw, floor(fh/2), RD(topRightRadius));
	
	[self closePath:rect];
}

- (void)addInverseToPath:(CGRect)rect {
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	CGFloat fw = rect.size.width;
	CGFloat fh = rect.size.height;
	
	CGFloat width = 5;
	CGRect shadowRect = CGRectMake(-width, -width, fw+width*2, fh+width*2);
	CGMutablePathRef path = CGPathCreateMutable();
	CGPathAddRect(path, nil, shadowRect);
	CGPathCloseSubpath(path);
	
	CGPathMoveToPoint(path, nil, fw, floor(fh/2));
	CGPathAddArcToPoint(path, nil, fw, fh, floor(fw/2), fh, RD(bottomRightRadius));
	CGPathAddArcToPoint(path, nil, 0, fh, 0, floor(fh/2), RD(bottomLeftRadius));
	CGPathAddArcToPoint(path, nil, 0, 0, floor(fw/2), 0, RD(topLeftRadius));
	CGPathAddArcToPoint(path, nil, fw, 0, fw, floor(fh/2), RD(topRightRadius));
	CGPathCloseSubpath(path);
	
	[self openPath:rect];
	CGContextAddPath(context, path);
	[self closePath:rect];
	
	CGPathRelease(path);
}

- (void)addTopEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource {
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGFloat fw = rect.size.width;
	CGFloat fh = rect.size.height;
	
	if (lightSource >= 0 && lightSource <= 90) {
		CGContextMoveToPoint(context, RD(topLeftRadius), 0);
	} else {
		CGContextMoveToPoint(context, 0, RD(topLeftRadius));
		CGContextAddArcToPoint(context, 0, 0, RD(topLeftRadius), 0, RD(topLeftRadius));
	}
	CGContextAddArcToPoint(context, fw, 0, fw, RD(topRightRadius), RD(topRightRadius));
	CGContextAddLineToPoint(context, fw, RD(topRightRadius));
}

- (void)addRightEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource {
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGFloat fw = rect.size.width;
	CGFloat fh = rect.size.height;
	
	CGContextMoveToPoint(context, fw, RD(topRightRadius));
	CGContextAddArcToPoint(context, fw, fh, fw-RD(bottomRightRadius), fh, RD(bottomRightRadius));
	CGContextAddLineToPoint(context, fw-RD(bottomRightRadius), fh);
}

- (void)addBottomEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource {
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGFloat fw = rect.size.width;
	CGFloat fh = rect.size.height;
	
	CGContextMoveToPoint(context, fw-RD(bottomRightRadius), fh);
	CGContextAddLineToPoint(context, RD(bottomLeftRadius), fh);
	CGContextAddArcToPoint(context, 0, fh, 0, fh-RD(bottomLeftRadius), RD(bottomLeftRadius));
}

- (void)addLeftEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource {
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGFloat fh = rect.size.height;
	
	CGContextMoveToPoint(context, 0, fh-RD(bottomLeftRadius));
	CGContextAddLineToPoint(context, 0, RD(topLeftRadius));
	
	if (lightSource >= 0 && lightSource <= 90) {
		CGContextAddArcToPoint(context, 0, 0, RD(topLeftRadius), 0, RD(topLeftRadius));
	}
}


@end
