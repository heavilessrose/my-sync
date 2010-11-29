//
//  LKStyledTextView.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LKStyledText, LKStyledFrame;

@interface LKStyledTextView : UIView {

	LKStyledText *stext;
	UIEdgeInsets contentInset;
	LKStyledFrame *selectedFrame;
}

@property (nonatomic, retain) LKStyledText *stext;
@property (nonatomic) UIEdgeInsets contentInset;
@property (nonatomic, assign) LKStyledFrame *selectedFrame;

@end
