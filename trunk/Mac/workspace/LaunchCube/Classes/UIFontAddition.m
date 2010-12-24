//
//  UIFontAddition.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "UIFontAddition.h"


@implementation UIFont (LKCategory)

- (CGFloat)ttLineHeight {
	return (self.ascender - self.descender) + 1;
}

@end
