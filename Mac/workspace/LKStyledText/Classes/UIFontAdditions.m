//
//  UIFontAdditions.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "UIFontAdditions.h"


@implementation UIFont (LKStyledTextCategory)

- (CGFloat)lkLineHeight {
	return (self.ascender - self.descender) + 1;
}
@end
