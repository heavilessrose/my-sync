//
//  LKStyledTextFrame.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyledFrame.h"

@class LKStyledTextNode;

@interface LKStyledTextFrame : LKStyledFrame {

	LKStyledTextNode *tnode;
	NSString *text; // 该frame会画这个
	UIFont *font;
}

@property (nonatomic, readonly) LKStyledTextNode *tnode;
@property (nonatomic, retain) NSString *text;
@property (nonatomic, retain) UIFont *font;

- (id)initWithText:(NSString *)atext node:(LKStyledTextNode *)aTNode;

@end
