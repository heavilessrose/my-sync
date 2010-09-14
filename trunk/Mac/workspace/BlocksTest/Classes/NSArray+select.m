//
//  NSArray+select.m
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "NSArray+select.h"


@implementation NSArray (select)

- (NSArray *)select:(BOOL (^)(id))block
{
    NSMutableArray *rslt = [NSMutableArray array];
    for (id aElement in self) {
        if (block(aElement)) {
            [rslt addObject:aElement]; 
        }
    }
    return rslt;
}

@end
