//
//  NSArray+each.h
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@class NSArray;

@interface NSArray (each)


- (void)each:(void (^)(id))block;
@end
