//
//  LKRects.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
 * @return a rectangle with dx and dy subtracted from the width and height, respectively.
 *
 * Example result: CGRectMake(x, y, w - dx, h - dy)
 */
CGRect LKRectContract(CGRect rect, CGFloat dx, CGFloat dy);

/**
 * @return a rectangle whose origin has been offset by dx, dy, and whose size has been
 * contracted by dx, dy.
 *
 * Example result: CGRectMake(x + dx, y + dy, w - dx, h - dy)
 */
CGRect LKRectShift(CGRect rect, CGFloat dx, CGFloat dy);

/**
 * @return a rectangle with the given insets.
 *
 * Example result: CGRectMake(x + left, y + top, w - (left + right), h - (top + bottom))
 */
CGRect LKRectInset(CGRect rect, UIEdgeInsets insets);
