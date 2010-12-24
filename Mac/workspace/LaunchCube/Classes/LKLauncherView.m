//
//  LKLauncherView.m
//  LaunchCube
//
//  Created by luke on 10-12-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKLauncherView.h"
#import "LKLauncherScrollView.h"
#import "LKLauncherButton.h"
#import "LKLauncherItem.h"

#define kPagerHeight (20)

@implementation LKLauncherView

@synthesize scrollView, columnCount, pages, buttons;

- (id)initWithFrame:(CGRect)frame {
    
    if (self = [super initWithFrame:frame]) {
		self.scrollView = [[LKLauncherScrollView alloc] initWithFrame:CGRectMake(0, 0, self.bounds.size.width, self.bounds.size.height - kPagerHeight)];
		scrollView.delegate = self;
		scrollView.scrollsToTop = NO;
		scrollView.showsVerticalScrollIndicator = NO;
		scrollView.showsHorizontalScrollIndicator = NO;
		scrollView.alwaysBounceHorizontal = YES;
		scrollView.pagingEnabled = YES;
		scrollView.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
		scrollView.delaysContentTouches = NO;
		scrollView.multipleTouchEnabled = NO;
		[self addSubview:scrollView];
	}
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code.
}
*/

- (void)dealloc {
	
	self.scrollView = nil;
	self.pages = nil;
	self.buttons = nil;
    [super dealloc];
}

#pragma mark -
#pragma mark item
- (NSIndexPath *)indexPathOfItem:(LKLauncherItem *)item {
	for (NSUInteger pageIndex = 0; pageIndex < pages.count; ++pageIndex) {
		NSArray *page = [pages objectAtIndex:pageIndex];
		NSUInteger itemIndex = [page indexOfObject:item];
		if (itemIndex != NSNotFound) {
			NSUInteger path[] = {pageIndex, itemIndex};
			return [NSIndexPath indexPathWithIndexes:path length:2];
		}
	}
	return nil;
}

- (LKLauncherButton *)buttonForItem:(LKLauncherItem *)item {
	NSIndexPath *path = [self indexPathOfItem:item];
	if (path) {
		NSInteger pageIndex = [path indexAtPosition:0];
		NSArray *buttonPage = [buttons objectAtIndex:pageIndex];
		
		NSInteger itemIndex = [path indexAtPosition:1];
		return [buttonPage objectAtIndex:itemIndex];
	} else {
		return nil;
	}
}

- (void)updateItemBadge:(LKLauncherItem *)item {
	LKLauncherButton *button = [self buttonForItem:item];
	[button performSelector:@selector(updateBadge)];
}

@end
