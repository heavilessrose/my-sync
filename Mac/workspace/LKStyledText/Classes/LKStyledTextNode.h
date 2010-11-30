//
//  LKStyledTextNode.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyledNode.h"

@interface LKStyledTextNode : LKStyledNode {

	NSString *text;
	LKStyledNode *preNode;
}

@property (nonatomic, retain) NSString *text;
@property (nonatomic, assign) LKStyledNode *preNode;

- (id)initWithText:(NSString *)aStr;

@end
