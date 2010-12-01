//
//  LKStyledLayout.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledLayout.h"
#import "LKStyledFrame.h"
#import "LKStyledTextNode.h"
#import "LKStyledText.h"
#import "UIFontAdditions.h"
#import "LKStyledTextFrame.h"
#import "LKStyledLinkNode.h"


@implementation LKStyledLayout

@synthesize _width, _height;
@synthesize rootNode, rootFrame, lastFrame, tmpLinkStartFrame;

- (id)initWithStyledText:(LKStyledText *)stext {
	
	if (self = [super init]) {
		self.rootNode = stext.rootNode;
		self.rootFrame = stext.rootFrame;
	}
	return self;
}

- (void)dealloc {
	
	[rootFrame release];
	[super dealloc];
}


- (LKStyledFrame *)layoutFrames {
	
	DLog(@"--------------------------------start");
	LKStyledNode *node = rootNode;
	while (node) {
		if ([node isKindOfClass:[LKStyledTextNode class]]) {
			LKStyledTextNode* textNode = (LKStyledTextNode *)node;
			[self layoutText:textNode];
		}
		node = node.nextNode;
	}
	DLog(@"--------------------------------end");
	return rootFrame;
}

- (LKStyledNode *)lastNode {
	
	LKStyledNode *node = rootNode;
	LKStyledNode *lastNode = nil;
	while (node) {
		lastNode = node;
		node = node.nextNode;
	}
	return lastNode;
}

- (void)expandLineWidth:(CGFloat)aWordWidth {
	
	lineWidth += aWordWidth;
	DLog(@"new lineWidth= %.0f", lineWidth);
}

- (void)extendLineHeight:(CGFloat)aWordHeight {
	
	if (lineHeight < aWordHeight) {
		lineHeight = aWordHeight;
		DLog(@"new lineHeight= %.0f", lineHeight);
	}
}

- (void)breakLine {
	
	_x = 0;
	_height += lineHeight;
	lineWidth = 0;
	lineHeight = 0;
	DLog(@"line breaked: _width= %.0f, _height= %.0f", _width, _height);
}

- (void)layoutText:(LKStyledTextNode *)aTNode {
	
	DLog(@"[%@]", aTNode);
	NSString *text = aTNode.text;
	NSUInteger length = text.length;
	linkHasMoreThanOneFrame = NO;
	
	if (!aTNode.nextNode && aTNode == rootNode) {
		DLog(@"唯一的节点");
		CGSize textSize = [text sizeWithFont:TEXT_FONT
						   constrainedToSize:CGSizeMake(_width, CGFLOAT_MAX)
							   lineBreakMode:UILineBreakModeWordWrap];
		if ([aTNode isKindOfClass:[LKStyledLinkNode class]]) {
			linkHasMoreThanOneFrame = YES;
		} else {
			linkHasMoreThanOneFrame = NO;
		}
		[self addFrameForText:text node:aTNode width:textSize.width height:textSize.height];
		_height += textSize.height;
		return;
	}
	
	NSCharacterSet *whitespace = [NSCharacterSet whitespaceAndNewlineCharacterSet];
	NSInteger stringIndex = 0;
	NSInteger lineStartIndex = 0;
	CGFloat frameWidth = 0;
	
	while (stringIndex < length) {
		// 查找下一个whitespace
		NSRange searchRange = NSMakeRange(stringIndex, length - stringIndex);
		NSRange spaceRange = [text rangeOfCharacterFromSet:whitespace options:0 range:searchRange];
		
		// 得到whitespace前的一个word
		NSRange wordRange = spaceRange.location != NSNotFound
		? NSMakeRange(searchRange.location, (spaceRange.location+1) - searchRange.location)
		: NSMakeRange(searchRange.location, length - searchRange.location);
		NSString *word = [text substringWithRange:wordRange];
		DLog(@"分割为word: [%@]", word);
		
		// 避免word wrap, 后面自己会处理wrap
		CGFloat availWidth = _width ? _width : CGFLOAT_MAX;
		DLog(@"availWidth: %.0f", availWidth);
		
		// 检测word是否适合当前行
		CGSize wordSize = [word sizeWithFont:TEXT_FONT];
		if (wordSize.width > _width) {
			DLog(@"word(%.0f)比当前行%.0f 长", wordSize.width, _width);
			DLog(@"按单个letter处理");
			for (NSInteger i = 0; i < word.length; ++i) {
				NSString *c = [word substringWithRange:NSMakeRange(i, 1)];
				CGSize letterSize = [c sizeWithFont:TEXT_FONT];
				
				DLog(@"%@, width= %.0f", c, letterSize.width);
				if (lineWidth + letterSize.width > _width) {
					DLog(@"此行已不够长无法放下这个letter");
					NSRange lineRange = NSMakeRange(lineStartIndex, stringIndex - lineStartIndex);
					if (lineRange.length) {
						NSString* line = [text substringWithRange:lineRange];
						if ([aTNode isKindOfClass:[LKStyledLinkNode class]]) {
							linkHasMoreThanOneFrame = YES;
						} else {
							linkHasMoreThanOneFrame = NO;
						}

						[self addFrameForText:line node:aTNode width:frameWidth
									   height:lineHeight ? lineHeight : [TEXT_FONT lkLineHeight]];
					}
					
					if (lineWidth) {
						[self breakLine];
					}
					
					lineStartIndex = lineRange.location + lineRange.length;
					frameWidth = 0;
				}
				
				frameWidth += letterSize.width;
				[self expandLineWidth:letterSize.width];
				[self extendLineHeight:wordSize.height];
				++stringIndex;
			}
			
			NSRange lineRange = NSMakeRange(lineStartIndex, stringIndex - lineStartIndex);
			if (lineRange.length) {
				NSString *line = [text substringWithRange:lineRange];
				if ([aTNode isKindOfClass:[LKStyledLinkNode class]]) {
					linkHasMoreThanOneFrame = YES;
				} else {
					linkHasMoreThanOneFrame = NO;
				}
				[self addFrameForText:line node:aTNode width:frameWidth
							   height:lineHeight ? lineHeight : [TEXT_FONT lkLineHeight]];
				
				lineStartIndex = lineRange.location + lineRange.length;
				frameWidth = 0;
			}
		} else {
			DLog(@"word(%.0f)没有此行长", wordSize.width);
			if (lineWidth + wordSize.width > _width) {
				DLog(@"此行不够长, 以放下该word (%@)", word);
				DLog(@"%.0f > %.0f, word将被放到下一行, 创建当前行的frame, 并标记换行. lineWidth= %.0f", lineWidth + wordSize.width, _width, lineWidth);
				
				NSRange lineRange = NSMakeRange(lineStartIndex, stringIndex - lineStartIndex);
				if (lineRange.length) {
					NSString *line = [text substringWithRange:lineRange];
					if ([aTNode isKindOfClass:[LKStyledLinkNode class]]) {
						linkHasMoreThanOneFrame = YES;
					} else {
						linkHasMoreThanOneFrame = NO;
					}
					[self addFrameForText:line node:aTNode width:frameWidth 
								   height:lineHeight ? lineHeight : [TEXT_FONT lkLineHeight]];
				}
				
				if (lineWidth) {
					[self breakLine];
				}
				lineStartIndex = lineRange.location + lineRange.length;
				frameWidth = 0;
			}
			
			if (!lineWidth && aTNode == [self lastNode]) {
				DLog(@"新行开始, 并且这是最后一个node, 不进行额外计算, 直接添加一个frame");
				NSString *lines = [text substringWithRange:searchRange];
				CGSize linesSize = [lines sizeWithFont:TEXT_FONT
									 constrainedToSize:CGSizeMake(availWidth, CGFLOAT_MAX)
										 lineBreakMode:UILineBreakModeWordWrap];
				if ([aTNode isKindOfClass:[LKStyledLinkNode class]]) {
					linkHasMoreThanOneFrame = YES;
				} else {
					linkHasMoreThanOneFrame = NO;
				}
				[self addFrameForText:lines node:aTNode width:linesSize.width height:linesSize.height];
				_height += linesSize.height;
				break;
			}
			
			CGFloat frameW_tmp = frameWidth;
			frameWidth += wordSize.width;
			DLog(@"扩大frameWidth(%.0f)+=wordSize.width = %.0f", frameW_tmp, frameWidth);
			DLog(@"--调整行宽,高--start");
			[self expandLineWidth:wordSize.width];
			[self extendLineHeight:wordSize.height];
			DLog(@"--调整行宽,高--end");
			
			DLog(@"stringIndex向后推移这个word (%@) 的长度", word);
			stringIndex = wordRange.location + wordRange.length;
			if (stringIndex >= length) {
				DLog(@"处理到该node[%@]最后一个word (%@)", text, word);
				NSRange lineRange = NSMakeRange(lineStartIndex, (wordRange.location + wordRange.length)
												- lineStartIndex);
				NSString *line = !lineWidth ? word : [text substringWithRange:lineRange];
				[self addFrameForText:line node:aTNode width:frameWidth height:[TEXT_FONT lkLineHeight]];
				DLog(@"重置 frameWidth = 0");
				frameWidth = 0;
			}
		}
	}
}

#pragma mark -
#pragma mark frame

- (void)addFrame:(LKStyledFrame *)aFrame {
	
	if (!rootFrame) {
		DLog(@"--> real addFrame as rootFrame: %@", aFrame);
		rootFrame = [aFrame retain];
		DLog(@"rootFrame count= %u", [rootFrame retainCount]);
	} else {
		lastFrame.nextFrame = aFrame;
		DLog(@"--> real addFrame as lastFrame.nextFrame: %@", aFrame);
		DLog(@"lastFrame.nextFrame count= %u", [lastFrame.nextFrame retainCount]);
	}
	lastFrame = aFrame;
}

- (LKStyledFrame *)addFrameForText:(NSString *)atext 
				   node:(LKStyledTextNode *)aTNode 
				  width:(CGFloat)aTWidth 
				 height:(CGFloat)aTHeight
{
	DLog(@"text: [%@], node: [%@], width: %.0f, height: %0.f", atext, aTNode, aTWidth, aTHeight);
	LKStyledTextFrame* frame = [[[LKStyledTextFrame alloc] initWithText:atext node:aTNode] autorelease];
//	if ([aTNode isKindOfClass:[LKStyledLinkNode class]]) {
//		frame.textColor = [UIColor blueColor];
//	} else if ([aTNode isKindOfClass:[LKStyledTextNode class]]) {
//		frame.textColor = [UIColor blackColor];
//	}
	
	frame.font = TEXT_FONT;
	frame.frame = CGRectMake(_x, _height, aTWidth, aTHeight);
	_x += aTWidth;
	//[self addContentFrame:frame width:aTWidth height:aTHeight];
	[self addFrame:frame];
	
	// 记录多个frame组成的link
	if (linkHasMoreThanOneFrame) {
		if (!tmpLinkStartFrame) {
			tmpLinkStartFrame = frame;
		}
		DLog(@"记录多个frame组成的link: %@", tmpLinkStartFrame);
		DLog(@"+++++++%@", frame);
		if (!tmpLinkStartFrame.siblings) {
			tmpLinkStartFrame.siblings = [NSMutableArray array];
		}
		[tmpLinkStartFrame.siblings addObject:frame];
		// 其他相关frame也要记录
		frame.siblings = [NSMutableArray arrayWithArray:(NSArray *)tmpLinkStartFrame.siblings];
	} else {
		self.tmpLinkStartFrame = nil;
	}

	return frame;
}


@end
