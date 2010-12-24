//
//  LKShapeStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"

@class LKShape;
@interface LKShapeStyle : LKStyle {
	
	LKShape *shape;
}

@property (nonatomic, retain) LKShape *shape;

+ (LKShapeStyle *)styleWithShape:(LKShape *)shape next:(LKStyle *)next;
@end
