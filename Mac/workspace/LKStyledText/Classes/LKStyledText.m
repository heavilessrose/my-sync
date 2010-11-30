//
//  LKStyledText.m
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import "LKStyledText.h"
#import "LKStyledFrame.h"
#import "LKStyledTextNode.h"
#import "LKStyledLayout.h"
#import "LKStyledLinkNode.h"


@implementation LKStyledText

@synthesize rootFrame, rootNode, lastNode;

- (id)initFotTest {
	
	if (self = [super init]) {
		// test
		NSArray *testStrs = [NSArray arrayWithObjects:
							 @"http://asaaaaaaaaaaaaaaabbbbbbbbbccccccccd.com", 
							 @"@userA", 
							 @"fad", 
							 @"start1http://e.eend1", 
							 @"start2http://e.eend2", 
							 @"start3http://e.eend3", 
							 @"start4http://e.eend4", 
							 @"asdfbhttp://fasdfasfasdfasd.cn end", 
							 @"啥的发的发生的发生多发多发啊啥的发生多发点sdfasdf 11", 
							 @"@userB", 
							 @"start www.123asdf.com end", nil];
		//NSArray *testStrs = [NSArray arrayWithObject:@"aslkdfas;fj啊是多力丰筋啊是癫凤狂龙就阿瑟;的发了看见"];
		for (NSString *astr in testStrs) {
			if ([astr isEqualToString:[testStrs objectAtIndex:1]] || [astr isEqualToString:[testStrs objectAtIndex:9]]) {
				[self addNode:[[LKStyledLinkNode alloc] initWithText:astr]];
				continue;
			}
			[self addNode:[[LKStyledTextNode alloc] initWithText:astr]];
		}
	}
	return self;
}


- (void)drawAtPoint:(CGPoint)aPoint {
	
	DLog(@"------------------------------------start");
	if (!rootFrame) {
		return;
	}
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSaveGState(ctx);
	CGContextTranslateCTM(ctx, aPoint.x, aPoint.y);
	
	LKStyledFrame *frame = self.rootFrame;
	
	// test
	DLog(@"++++++++++++++++++++++++++++++");
	while (frame) {
		DLog(@"%@", frame);
		frame = frame.nextFrame;
	}
	frame = rootFrame;
	DLog(@"++++++++++++++++++++++++++++++\n");
	
	
	
	static int frameCount = 0;
	DLog(@"=================================start draw:\n");
	while (frame) {
		frameCount++;
		DLog(@"draw frame: %@", frame);
		[frame drawInRect:frame.frame];
		frame = frame.nextFrame;
	}
	DLog(@"-------frameCount: %d", frameCount);
	frameCount = 0;
	CGContextRestoreGState(ctx);
}

- (void)dealloc {
	
	[rootNode release];
	[rootFrame release];
	[super dealloc];
}

- (void)addNode:(LKStyledNode *)aNode {
	
	DLog(@"%@", aNode);
	if (!rootNode) {
		self.rootNode = aNode;
	} else {
		self.lastNode.nextNode = aNode;
	}
	if ([aNode isKindOfClass:[LKStyledTextNode class]]) {
		((LKStyledTextNode *)aNode).preNode = lastNode;
	}
	self.lastNode = aNode;
}

- (void)layout {
	
	DLog(@"");
	LKStyledLayout *layout = [[LKStyledLayout alloc] initWithStyledText:self];
	layout._width = 320;
	self.rootFrame = [layout layoutFrames];
	[layout release];
}


- (LKStyledFrame *)touchCheck:(CGPoint)aPoint {
	
	DLog(@"");
	return [self.rootFrame touchCheck:aPoint];
}

#pragma mark -
#pragma mark parse

- (void)debugNode {
	
	DLog(@"*********************");
	DLog(@"current nodes: {");
	LKStyledNode *node = rootNode;
	while (node) {
		DLog(@"%@", node);
		node = node.nextNode;
	}
	DLog(@"}");
	DLog(@"*********************");
}

- (void)parseLink {
	
	if (!rootNode) {
		return;
	}
	LKStyledNode *node = rootNode;
	DLog(@"-------------------------------------------------start");
	while (node) {
		if ([node isKindOfClass:[LKStyledTextNode class]]) {
			[self parseUrl:(LKStyledTextNode *)node];
			[self parseAt:(LKStyledTextNode *)node];
		}
		node = node.nextNode;
	}
	DLog(@"-------------------------------------------------end");
	
	// test
	[self debugNode];
}

- (LKStyledTextNode *)insertNode:(LKStyledTextNode *)newNode afterNode:(LKStyledNode *)aNode {
	
	DLog(@"insert node= [%@] after= [%@]", newNode, aNode);
	aNode.nextNode = newNode;
	newNode.preNode = aNode;
	newNode.nextNode.preNode = newNode;
	
	return newNode;
}

//- (void)setRootNode:(LKStyledNode *) {
//	
//	
//}

- (void)parseUrl:(LKStyledTextNode *)lNode {
	
	DLog(@"parse node= %@", lNode);
	NSInteger stringIndex = 0;
	NSString *string = lNode.text;
	LKStyledTextNode *tmpNewNode = nil;
	
	while (stringIndex < string.length) {
		NSRange searchRange = NSMakeRange(stringIndex, string.length - stringIndex);
		NSRange startRange = [string rangeOfString:@"http://" options:NSCaseInsensitiveSearch
											 range:searchRange];
		
		if (startRange.location == NSNotFound) {
			NSString *text = [string substringWithRange:searchRange];
			if (!text || [text length] == 0) 
				break;
			LKStyledTextNode *node = [[[LKStyledTextNode alloc] initWithText:text] autorelease];
			if (!tmpNewNode) {
				break;
			} else {
				node.nextNode = tmpNewNode.nextNode;
				tmpNewNode = [self insertNode:node afterNode:tmpNewNode];
			}
			// test
			[self debugNode];
			break;
			
		} else {
			NSRange beforeRange = NSMakeRange(searchRange.location,
											  startRange.location - searchRange.location);
			if (beforeRange.length) {
				NSString *text = [string substringWithRange:beforeRange];
				LKStyledTextNode* node = [[[LKStyledTextNode alloc] initWithText:text] autorelease];
				if (!tmpNewNode) {
					node.nextNode = lNode.nextNode;
					tmpNewNode = [self insertNode:node afterNode:lNode.preNode];
					
					if (lNode == rootNode) {
						DLog(@"替换rootNode= [%@]", rootNode);
						self.rootNode = tmpNewNode;
						DLog(@"-----------------为 [%@]", rootNode);
					}
				} else {
					node.nextNode = tmpNewNode.nextNode;
					tmpNewNode = [self insertNode:node afterNode:tmpNewNode];
				}
				// test
				[self debugNode];
			}
			
			NSRange subSearchRange = NSMakeRange(startRange.location, string.length - startRange.location);
			
			// 遇到whitespace或非ascii字符视为url结束
#if 0
			NSRange endRange = [string rangeOfString:@" " options:NSCaseInsensitiveSearch
											   range:subSearchRange];
#else
			//NSCharacterSet *whitespace = [NSCharacterSet whitespaceAndNewlineCharacterSet];
			NSCharacterSet *alphanumericSet = [NSCharacterSet alphanumericCharacterSet];
			NSMutableCharacterSet *alphanumericSet_m = [alphanumericSet mutableCopy];
			[alphanumericSet_m addCharactersInString:@":/."];
			NSCharacterSet *endUrlSet = [alphanumericSet_m invertedSet];
			NSRange endRange = [string rangeOfCharacterFromSet:endUrlSet options:NSCaseInsensitiveSearch range:subSearchRange];
			[alphanumericSet_m release];
#endif
			
			if (endRange.location == NSNotFound) {
				NSString *URL = [string substringWithRange:subSearchRange];
				LKStyledLinkNode *node = [[[LKStyledLinkNode alloc] initWithText:URL] autorelease];
				node.URL = URL;
				if (!tmpNewNode) {
					node.nextNode = lNode.nextNode;
					tmpNewNode = [self insertNode:node afterNode:lNode.preNode];
					
					if (lNode == rootNode) {
						DLog(@"替换rootNode= [%@]", rootNode);
						self.rootNode = tmpNewNode;
						DLog(@"-----------------为 [%@]", rootNode);
					}
				} else {
					node.nextNode = tmpNewNode.nextNode;
					tmpNewNode = [self insertNode:node afterNode:tmpNewNode];
				}
				//[lNode release];
				//lNode = nil;
				// test
				[self debugNode];
				break;
				
			} else {
				NSRange URLRange = NSMakeRange(startRange.location,
											   endRange.location - startRange.location);
				NSString *URL = [string substringWithRange:URLRange];
				LKStyledLinkNode *node = [[[LKStyledLinkNode alloc] initWithText:URL] autorelease];
				node.URL = URL;
				if (!tmpNewNode) {
					node.nextNode = lNode.nextNode;
					tmpNewNode = [self insertNode:node afterNode:lNode.preNode];
					
					if (lNode == rootNode) {
						DLog(@"替换rootNode= [%@]", rootNode);
						self.rootNode = tmpNewNode;
						DLog(@"-----------------为 [%@]", rootNode);
					}
				} else {
					node.nextNode = tmpNewNode.nextNode;
					tmpNewNode = [self insertNode:node afterNode:tmpNewNode];
				}
				stringIndex = endRange.location;
				// test
				[self debugNode];
			}
		}
	}
}

- (void)parseAt:(LKStyledTextNode *)lNode {
	
	//DLog(@"parse node= [%@]", lNode);
}

@end
