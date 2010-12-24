//
//  LKStyleSheet.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LKStyle;
@interface LKStyleSheet : NSObject {
	
	NSMutableDictionary *styles;
}

+ (LKStyleSheet *)globalStyleSheet;
+ (void)setGlobalStyleSheet:(LKStyleSheet *)styleSheet;

- (LKStyle *)styleWithSelector:(NSString *)selector;
- (LKStyle *)styleWithSelector:(NSString *)selector forState:(UIControlState)state;

- (void)freeMemory;

@end
