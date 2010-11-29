//
//  LKStyledText.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LKStyledNode, LKStyledFrame;

@interface LKStyledText : NSObject {
	
	LKStyledFrame *rootFrame;
	LKStyledNode *rootNode;
	LKStyledNode *lastNode;
}

@property (nonatomic, retain) LKStyledFrame *rootFrame;
@property (nonatomic, retain) LKStyledNode *rootNode;
@property (nonatomic, assign) LKStyledNode *lastNode;

- (id)initFotTest;
- (void)addNode:(LKStyledNode *)aNode;
- (void)layout;
@end
