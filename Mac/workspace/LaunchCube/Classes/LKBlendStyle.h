//
//  LKBlendStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"


@interface LKBlendStyle : LKStyle {
	
	CGBlendMode blendMode;
}

@property (nonatomic) CGBlendMode blendMode;

+ (LKBlendStyle *)styleWithBlend:(CGBlendMode)blendMode next:(LKStyle *)next;

@end
