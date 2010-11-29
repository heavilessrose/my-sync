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

	NSString *text;
	UIFont *font;
	UIColor *textColor;
	UIColor *backColor;
	
	NSMutableArray *siblings;
}

@property (nonatomic, retain) NSString *text;
@property (nonatomic, retain) UIFont *font;
@property (nonatomic, retain) UIColor *textColor;
@property (nonatomic, retain) UIColor *backColor;
@property (nonatomic, retain) NSMutableArray *siblings;

- (id)initWithText:(NSString *)atext node:(LKStyledTextNode *)aTNode;

@end
