//
//  LEmoView.h
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface LEmoView : UIView {
	UIButton *emoButton;
	NSInteger emoIndex;
}

@property (nonatomic, assign) NSInteger emoIndex;

- (id)initWithFrame:(CGRect)frame emoImg:(UIImage *)emoImg;
@end
