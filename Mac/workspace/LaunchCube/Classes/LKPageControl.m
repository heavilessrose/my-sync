//
//  LKPageControl.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKPageControl.h"


@implementation LKPageControl

@synthesize numberOfPages, currentPage, hidesForSinglePage, dotStyle;

- (id)initWithFrame:(CGRect)frame {
    
    if (self = [super initWithFrame:frame]) {
		self.backgroundColor = [UIColor clearColor];
		self.dotStyle = @"pageDot:";
		self.hidesForSinglePage = NO;
		self.contentMode = UIViewContentModeRedraw;
    }
    return self;
}

- (void)dealloc {
	
	self.dotStyle = nil;
//	self.normalDotStyle = nil;
//	self.currentDotStyle = nil;
    [super dealloc];
}

#pragma mark -
#pragma mark UIView
- (void)drawRect:(CGRect)rect {
	
	if(numberOfPages <= 1 && hidesForSinglePage) {
		return;
	}
	
	TTStyleContext *context = [[[TTStyleContext alloc] init] autorelease];
	TTBoxStyle* boxStyle = [self.normalDotStyle firstStyleOfClass:[TTBoxStyle class]];
	
	CGSize dotSize = [self.normalDotStyle addToSize:CGSizeZero context:context];
	
	CGFloat dotWidth = dotSize.width + boxStyle.margin.left + boxStyle.margin.right;
	CGFloat totalWidth = (dotWidth * _numberOfPages) - (boxStyle.margin.left + boxStyle.margin.right);
	CGRect contentRect = CGRectMake(round(self.width/2 - totalWidth/2),
									round(self.height/2 - dotSize.height/2),
									dotSize.width, dotSize.height);
	
	for (NSInteger i = 0; i < _numberOfPages; ++i) {
		contentRect.origin.x += boxStyle.margin.left;
		
		context.frame = contentRect;
		context.contentFrame = contentRect;
		
		if (i == _currentPage) {
			[self.currentDotStyle draw:context];
		} else {
			[self.normalDotStyle draw:context];
		}
		contentRect.origin.x += dotSize.width + boxStyle.margin.right;
	}
}


#pragma mark -
#pragma mark UIControl
- (void)endTrackingWithTouch:(UITouch *)touch withEvent:(UIEvent *)event {

}

#pragma mark -
#pragma mark public
- (void)setNumberOfPages:(NSInteger)aNumberOfPages {
	
	if (aNumberOfPages != numberOfPages) {
		assert(aNumberOfPages >= 0);
		
		numberOfPages = MAX(0, aNumberOfPages);
		[self setNeedsDisplay];
	}
}

- (void)setCurrentPage:(NSInteger)aCurrentPage {
	
	if (aCurrentPage != currentPage) {
		currentPage = MAX(0, MIN(numberOfPages - 1, aCurrentPage));
		[self setNeedsDisplay];
	}
}

- (void)setDotStyle:(NSString*)aDotStyle {
	
	if (![aDotStyle isEqualToString:dotStyle]) {
		[dotStyle release];
		dotStyle = [aDotStyle copy];
//		self.normalDotStyle = nil;
//		self.currentDotStyle = nil;
	}
}

@end
