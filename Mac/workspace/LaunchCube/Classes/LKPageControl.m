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
		
    }
    return self;
}

- (void)dealloc {
	
	self.dotStyle = nil;
    [super dealloc];
}

#pragma mark -
#pragma mark UIView
- (void)drawRect:(CGRect)rect {

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
