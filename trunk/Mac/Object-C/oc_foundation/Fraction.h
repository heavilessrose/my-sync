//
//  Fraction.h
//  oc_foundation
//
//  Created by luke on 4/22/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface Fraction : NSObject {
	int numerator; // 分子
	int denominator; // 分母
}

- (void) print;
- (void) setNumerator: (int) d; 
- (void) setDenominator: (int) d; 
- (int) numerator; 
- (int) denominator; 

@end
