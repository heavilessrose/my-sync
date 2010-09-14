//
//  NSArray+select.h
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface NSArray (select)

- (NSArray *)select:(BOOL (^)(id))block;
@end
