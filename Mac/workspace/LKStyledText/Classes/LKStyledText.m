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


@implementation LKStyledText

@synthesize rootFrame, rootNode, lastNode;

- (id)initFotTest {
	
	if (self = [super init]) {
		// test
		NSArray *testStrs = [NSArray arrayWithObjects:@"asd", @"fad", @"fasdfasfasdfasd 啊啥的发放 啊!", @"啥的发的发生的发生多发多发啊啥的发生多发点sdfasdf 11", nil];
		for (NSString *astr in testStrs) {
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
	static int frameCount = 0;
	NSLog(@"=================================start draw:\n");
	while (frame) {
		frameCount++;
		NSLog(@"draw frame: %@", frame);
		[frame drawInRect:frame.frame];
		frame = frame.nextFrame;
	}
	NSLog(@"-------frameCount: %d", frameCount);
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
	
	self.lastNode = aNode;
}

- (void)layout {
	
	DLog(@"");
	LKStyledLayout *layout = [[LKStyledLayout alloc] initWithStyledText:self];
	layout._width = 300;
	self.rootFrame = [layout layoutFrames];
	[layout release];
}


@end
