//
//  NSArray+each.m
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "NSArray+each.h"


@implementation NSArray (each)

- (void)each:(void (^)(id))block
{
    for (id aElement in self) {
        block(aElement);
    }
}

@end
