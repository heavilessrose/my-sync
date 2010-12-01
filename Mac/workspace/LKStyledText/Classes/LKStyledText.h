//
//  LKStyledText.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LKStyledNode, LKStyledFrame, LKStyledTextNode;

@interface LKStyledText : NSObject {
	
	LKStyledFrame *rootFrame;
	LKStyledNode *rootNode;
	LKStyledNode *lastNode;
}

@property (nonatomic, retain) LKStyledFrame *rootFrame;
@property (nonatomic, retain) LKStyledNode *rootNode;
@property (nonatomic, assign) LKStyledNode *lastNode;

- (id)initFotTest;

- (id)initWithArray:(NSArray *)strs;
- (void)addNode:(LKStyledNode *)aNode;
- (void)layout;

- (LKStyledTextNode *)insertNode:(LKStyledTextNode *)newNode afterNode:(LKStyledNode *)aNode;
- (void)parseLink;
- (void)parseUrl:(LKStyledTextNode *)lNode;
- (void)parseAt:(LKStyledTextNode *)lNode;
@end
