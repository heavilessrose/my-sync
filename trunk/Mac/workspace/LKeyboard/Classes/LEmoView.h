//
//  LEmoView.h
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol LEmoDelegate <NSObject>

- (void)emoSelected:(NSArray *)emoStr;

@end

@interface LEmoView : UIView {
	
	NSArray *emoStrings;
	UIButton *emoButton;
	NSInteger emoIndex;
	id <LEmoDelegate> delegate;
}

@property (nonatomic, retain) NSArray *emoStrings;
@property (nonatomic, assign) NSInteger emoIndex;
@property (nonatomic, assign) id <LEmoDelegate> delegate;

- (id)initWithFrame:(CGRect)frame emoImg:(UIImage *)emoImg;
@end
