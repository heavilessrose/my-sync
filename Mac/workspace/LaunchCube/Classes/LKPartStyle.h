//
//  LKPartStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyle.h"

@interface LKPartStyle : LKStyle {
	
	NSString *name;
	LKStyle *style;
}

@property (nonatomic, copy) NSString *name;
@property (nonatomic, retain) LKStyle *style;

+ (LKPartStyle *)styleWithName:(NSString *)name style:(LKStyle *)style next:(LKStyle *)next;

- (void)drawPart:(LKStyleContext *)context;

@end
