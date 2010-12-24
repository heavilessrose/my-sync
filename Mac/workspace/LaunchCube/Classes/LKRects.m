//
//  LKRects.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKRects.h"


CGRect LKRectContract(CGRect rect, CGFloat dx, CGFloat dy) {
	return CGRectMake(rect.origin.x, rect.origin.y, rect.size.width - dx, rect.size.height - dy);
}



CGRect LKRectShift(CGRect rect, CGFloat dx, CGFloat dy) {
	return CGRectOffset(LKRectContract(rect, dx, dy), dx, dy);
}



CGRect LKRectInset(CGRect rect, UIEdgeInsets insets) {
	return CGRectMake(rect.origin.x + insets.left, rect.origin.y + insets.top,
					  rect.size.width - (insets.left + insets.right),
					  rect.size.height - (insets.top + insets.bottom));
}
