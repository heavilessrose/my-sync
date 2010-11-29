//
//  LKStyledFrame.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyledNode.h"


@interface LKStyledFrame : NSObject {

	LKStyledNode *node;
	CGRect frame;
	LKStyledFrame *nextFrame;
	LKStyledFrame *childFrame; // 一个node被分为多个frame时使用
	BOOL selected;
}

@property (nonatomic, readonly) LKStyledNode *node;
@property (nonatomic, assign) CGRect frame;
@property (nonatomic, retain) LKStyledFrame *nextFrame;
@property (nonatomic, retain) LKStyledFrame *childFrame;
@property (nonatomic, assign) BOOL selected;

- (void)drawInRect:(CGRect)aRect;
- (LKStyledFrame *)touchCheck:(CGPoint)aPoint;

@end
