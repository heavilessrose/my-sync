//
//  LKStyledLayout.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>

#define TEXT_FONT ([UIFont systemFontOfSize:16])

@class LKStyledNode, LKStyledFrame, LKStyledText, LKStyledTextNode, LKStyledTextFrame;

@interface LKStyledLayout : NSObject {
	
	LKStyledNode *rootNode;
	LKStyledFrame *rootFrame;
	LKStyledFrame *lastFrame;
	
	CGFloat _x;
	CGFloat _width;		// 总宽
	CGFloat _height;	// 总高
	CGFloat lineWidth;	// 行当前宽
	CGFloat lineHeight; // 行当前高
	
	BOOL linkHasMoreThanOneFrame;
	LKStyledTextFrame *tmpLinkStartFrame;
}


@property (nonatomic, assign) LKStyledFrame *lastFrame;
@property (nonatomic, assign) LKStyledNode *rootNode;
@property (nonatomic, retain) LKStyledFrame *rootFrame;
@property (nonatomic, assign) CGFloat _width;
@property (nonatomic, assign) CGFloat _height;
@property (nonatomic, assign) LKStyledTextFrame *tmpLinkStartFrame;

- (id)initWithStyledText:(LKStyledText *)stext;
- (LKStyledFrame *)layoutFrames;
- (void)layoutText:(LKStyledTextNode *)aTNode;

- (LKStyledFrame *)addFrameForText:(NSString *)atext 
				   node:(LKStyledTextNode *)aTNode 
				  width:(CGFloat)awidth 
				 height:(CGFloat)aheight;

@end
