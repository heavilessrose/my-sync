//
//  LKRoundedRectangleShape.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKShape.h"


@interface LKRoundedRectangleShape : LKShape {
	
	CGFloat topLeftRadius;
	CGFloat topRightRadius;
	CGFloat bottomRightRadius;
	CGFloat bottomLeftRadius;
}

@property (nonatomic) CGFloat topLeftRadius;
@property (nonatomic) CGFloat topRightRadius;
@property (nonatomic) CGFloat bottomRightRadius;
@property (nonatomic) CGFloat bottomLeftRadius;

+ (LKRoundedRectangleShape *)shapeWithRadius:(CGFloat)radius;

+ (LKRoundedRectangleShape *)shapeWithTopLeft:(CGFloat)topLeft topRight:(CGFloat)topRight
                                 bottomRight:(CGFloat)bottomRight bottomLeft:(CGFloat)bottomLeft;
@end
