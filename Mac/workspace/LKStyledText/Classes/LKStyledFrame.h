//
//  LKStyledFrame.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface LKStyledFrame : NSObject {

	CGRect frame;
	LKStyledFrame *nextFrame;
	LKStyledFrame *childFrame; // 一个node被分为多个frame时使用
}

@property (nonatomic, assign) CGRect frame;
@property (nonatomic, retain) LKStyledFrame *nextFrame;
@property (nonatomic, retain) LKStyledFrame *childFrame;

- (void)drawInRect:(CGRect)aRect;

@end
