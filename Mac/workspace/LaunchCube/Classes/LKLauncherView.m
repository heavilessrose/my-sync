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
#import "LKPageControl.h"
#import "LKLauncherHighlightView.h"
#import "UIViewAddition.h"


#define kPagerHeight (20)
#define kDefaultColumnCount (4)

static const CGFloat kMargin = 0;
static const CGFloat kPadding = 0;
static const CGFloat kPromptMargin = 40;
static const CGFloat kWobbleRadians = 1.5;
static const CGFloat kSpringLoadFraction = 0.18;

static const NSTimeInterval kEditHoldTimeInterval = 1;
static const NSTimeInterval kSpringLoadTimeInterval = 0.5;
static const NSTimeInterval kWobbleTime = 0.07;

#define kPromptTag 997


@implementation LKLauncherView

@synthesize scrollView, columnCount, pages, buttons, delegate, rowCount, currentPageIndex, prompt, editing;

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
		
		pager = [[LKPageControl alloc] init];
		pager.dotStyle = @"launcherPageDot:";
		pager.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleBottomMargin;
		[pager addTarget:self action:@selector(pageChanged) forControlEvents:UIControlEventValueChanged];
		[self addSubview:pager];
		
		self.autoresizesSubviews = YES;
		self.columnCount = kDefaultColumnCount;
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
	
	for (NSArray *page in pages) {
		for (LKLauncherItem *item in page) {
			item.launcher = nil;
		}
	}
	
	scrollView.delegate = nil;
	
	[editHoldTimer invalidate];
	editHoldTimer = nil;
	[springLoadTimer invalidate];
	springLoadTimer = nil;
	
	self.scrollView = nil;
	self.pages = nil;
	self.buttons = nil;
	self.prompt = nil;
	[pager release];
	pager = nil;
	
    [super dealloc];
}

#pragma mark -

- (CGFloat)rowHeight {
	return round(scrollView.height / 3);
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

- (NSMutableArray *)pageWithItem:(LKLauncherItem *)item {
	for (NSMutableArray *page in pages) {
		NSUInteger itemIndex = [page indexOfObject:item];
		if (itemIndex != NSNotFound) {
			return page;
		}
	}
	return nil;
}

- (NSMutableArray *)pageWithButton:(LKLauncherButton *)button {
	NSIndexPath *path = [self indexPathOfItem:button.item];
	if (path) {
		NSInteger pageIndex = [path indexAtPosition:0];
		return [buttons objectAtIndex:pageIndex];
	} else {
		return nil;
	}
}

- (NSMutableArray *)pageWithFreeSpace:(NSInteger)pageIndex {
	for (NSInteger i = self.currentPageIndex; i < pages.count; ++i) {
		NSMutableArray *page = [pages objectAtIndex:i];
		if (page.count < self.columnCount*self.rowCount) {
			return page;
		}
	}
	
	NSMutableArray *page = [NSMutableArray array];
	[pages addObject:page];
	return page;
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

- (void)updateContentSize:(NSInteger)numberOfPages {
	scrollView.contentSize = CGSizeMake(numberOfPages*scrollView.width, scrollView.height);
	if (numberOfPages != pager.numberOfPages) {
		pager.numberOfPages = numberOfPages;
	}
}

- (void)updatePagerWithContentOffset:(CGPoint)contentOffset {
	CGFloat pageWidth = scrollView.width;
	pager.currentPage = floor((contentOffset.x - pageWidth / 2) / pageWidth) + 1;
}

- (void)showPrompt {
	CGRect boxFrame = CGRectMake(scrollView.width, 0, scrollView.width, scrollView.height);
	CGRect labelFrame = CGRectInset(boxFrame, kPromptMargin, kPromptMargin);
	UILabel* label = [[[UILabel alloc] initWithFrame:labelFrame] autorelease];
	label.tag = kPromptTag;
	label.text = _prompt;
	label.font = [UIFont boldSystemFontOfSize:17];
	label.backgroundColor = [UIColor clearColor];
	label.textColor = RGBCOLOR(81,95,122);
	label.shadowColor = [[UIColor whiteColor] colorWithAlphaComponent:0.6];
	label.shadowOffset = CGSizeMake(0,1);
	label.textAlignment = UITextAlignmentCenter;
	label.numberOfLines = 0;
	[scrollView addSubview:label];
}

- (LKLauncherButton *)addButtonWithItem:(LKLauncherItem *)item {
	LKLauncherButton *button = [[[TTLauncherButton alloc] initWithItem:item] autorelease];
	
	[button addTarget:self action:@selector(buttonTouchedUpInside:)
	 forControlEvents:UIControlEventTouchUpInside];
	[button addTarget:self action:@selector(buttonTouchedUpOutside:)
	 forControlEvents:UIControlEventTouchUpOutside];
	[button addTarget:self action:@selector(buttonTouchedDown:withEvent:)
	 forControlEvents:UIControlEventTouchDown];
	
	[scrollView addSubview:button];
	return button;
}

- (void)layoutButtons {
	[self layoutIfNeeded];
	
	CGFloat buttonWidth = ceil((self.width
								- (kMargin*2 + kPadding*(self.columnCount-1))) / self.columnCount);
	CGFloat buttonHeight = [self rowHeight];
	CGFloat pageWidth = scrollView.width;
	
	CGFloat x = kMargin, minX = 0;
	for (NSMutableArray* buttonPage in buttons) {
		CGFloat y = kMargin;
		for (LKLauncherButton *button in buttonPage) {
			CGRect frame = CGRectMake(x, y, buttonWidth, buttonHeight);
			if (!button.dragging) {
				button.transform = CGAffineTransformIdentity;
				button.frame = button == dragButton ? [scrollView convertRect:frame toView:self] : frame;
			}
			x += buttonWidth + kPadding;
			if (x >= minX+pageWidth) {
				y += buttonHeight + kPadding;
				x = minX+kMargin;
			}
		}
		
		minX += pageWidth;
		x = minX;
	}
	
	NSInteger numberOfPages = pages.count;
	if (numberOfPages == 1 && _prompt) {
		[self showPrompt];
		++numberOfPages;
	}
	
	[self updateContentSize:numberOfPages];
}

- (void)recreateButtons {
	[self layoutIfNeeded];
	[scrollView removeAllSubviews];
	
	[buttons release];
	buttons = [[NSMutableArray alloc] init];
	
	for (NSArray* page in pages) {
		NSMutableArray* buttonPage = [NSMutableArray array];
		[buttons addObject:buttonPage];
		
		for (LKLauncherItem *item in page) {
			LKLauncherButton *button = [self addButtonWithItem:item];
			[buttonPage addObject:button];
		}
	}
	
	[self layoutButtons];
}

- (void)checkButtonOverflow:(NSInteger)pageIndex {
	NSMutableArray* buttonPage = [buttons objectAtIndex:pageIndex];
	NSInteger maxButtonsPerPage = self.columnCount*self.rowCount;
	if (buttonPage.count > maxButtonsPerPage) {
		BOOL isLastPage = pageIndex == buttons.count-1;
		
		NSMutableArray* itemsPage = [pages objectAtIndex:pageIndex];
		
		NSMutableArray* nextButtonPage = nil;
		NSMutableArray* nextItemsPage = nil;
		if (isLastPage) {
			nextButtonPage = [NSMutableArray array];
			[buttons addObject:nextButtonPage];
			nextItemsPage = [NSMutableArray array];
			[pages addObject:nextItemsPage];
		} else {
			nextButtonPage = [buttons objectAtIndex:pageIndex+1];
			nextItemsPage = [pages objectAtIndex:pageIndex+1];
		}
		
		while (buttonPage.count > maxButtonsPerPage) {
			[nextButtonPage insertObject:[buttonPage lastObject] atIndex:0];
			[buttonPage removeLastObject];
			[nextItemsPage insertObject:[itemsPage lastObject] atIndex:0];
			[itemsPage removeLastObject];
		}
		
		if (pageIndex+1 < buttons.count) {
			[self checkButtonOverflow:pageIndex+1];
		}
	}
}

- (void)startDraggingButton:(LKLauncherButton *)button withEvent:(UIEvent*)event {
	TT_INVALIDATE_TIMER(springLoadTimer);
	
	if (button) {
		button.transform = CGAffineTransformIdentity;
		[self addSubview:button];
		button.origin = [scrollView convertPoint:button.origin toView:self];
		[button layoutIfNeeded];
	}
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TT_FAST_TRANSITION_DURATION];
	
	if (dragButton) {
		dragButton.selected = NO;
		dragButton.highlighted = NO;
		dragButton.dragging = NO;
		[self layoutButtons];
	}
	
	if (button) {
		dragButton = button;
		
		NSIndexPath* indexPath = [self indexPathOfItem:button.item];
		positionOrigin = [indexPath indexAtPosition:1];
		
		UITouch* touch = [[event allTouches] anyObject];
		touchOrigin = [touch locationInView:scrollView];
		dragOrigin = button.center;
		dragTouch = touch;
		
		button.dragging = YES;
		
		scrollView.scrollEnabled = NO;
	} else {
		[UIView setAnimationDelegate:self];
		[UIView setAnimationDidStopSelector:@selector(releaseButtonDidStop)];
		scrollView.scrollEnabled = YES;
	}
	
	[UIView commitAnimations];
}

- (void)removeButtonAnimationDidStop:(NSString*)animationID finished:(NSNumber*)finished
							 context:(void*)context {
	LKLauncherButton *button = context;
	[button removeFromSuperview];
}

- (void)springLoadTimer:(NSTimer*)timer {
	springLoadTimer = nil;
	
	if ([(NSNumber*)timer.userInfo boolValue]) {
		CGFloat newX = scrollView.contentOffset.x - scrollView.width;
		if (newX >= 0) {
			CGPoint offset = CGPointMake(newX, 0);
			[scrollView setContentOffset:offset animated:YES];
			[self updatePagerWithContentOffset:offset];
			dragOrigin.x += scrollView.width;
			positionOrigin = -1;
			springing = YES;
			[self performSelector:@selector(springingDidStop) withObject:nil afterDelay:0.3];
		}
	} else {
		CGFloat newX = scrollView.contentOffset.x + scrollView.width;
		if (newX <= scrollView.contentSize.width - scrollView.width) {
			CGPoint offset = CGPointMake(newX, 0);
			[scrollView setContentOffset:offset animated:YES];
			[self updatePagerWithContentOffset:offset];
			dragOrigin.x -= scrollView.width;
			positionOrigin = -1;
			springing = YES;
			[self performSelector:@selector(springingDidStop) withObject:nil afterDelay:0.3];
		}
	}
}

- (void)springingDidStop {
	springing = NO;
}

- (void)releaseButtonDidStop {
	[scrollView addSubview:dragButton];
	dragButton.origin = [self convertPoint:dragButton.origin toView:scrollView];
	dragButton = nil;
}

- (void)buttonTouchedUpInside:(LKLauncherButton *)button {
	if (editing) {
		if (button == dragButton) {
			[self startDraggingButton:nil withEvent:nil];
		}
	} else {
		TT_INVALIDATE_TIMER(editHoldTimer);
		[button setSelected:YES];
		[self performSelector:@selector(deselectButton:) withObject:button
				   afterDelay:TT_TRANSITION_DURATION];
		NSString* URL = button.item.URL;
		if (URL) {
			if ([delegate respondsToSelector:@selector(launcherView:didSelectItem:)]) {
				[delegate launcherView:self didSelectItem:button.item];
			}
		}
	}
}

- (void)buttonTouchedUpOutside:(LKLauncherButton *)button {
	if (editing) {
		if (button == dragButton) {
			[self startDraggingButton:nil withEvent:nil];
		}
	} else {
		TT_INVALIDATE_TIMER(editHoldTimer);
	}
}

- (void)buttonTouchedDown:(LKLauncherButton *)button withEvent:(UIEvent*)event {
	if (editing) {
		if (!dragButton) {
			[self startDraggingButton:button withEvent:event];
		}
	} else {
		TT_INVALIDATE_TIMER(editHoldTimer);
		
		editHoldTimer = [NSTimer scheduledTimerWithTimeInterval:kEditHoldTimeInterval
														  target:self selector:@selector(editHoldTimer:)
														userInfo:[TTUserInfo topic:nil strongRef:event weakRef:button]
														 repeats:NO];
	}
}

- (void)closeButtonTouchedUpInside:(TTButton*)closeButton {
	for (NSArray* buttonPage in buttons) {
		for (LKLauncherButton *button in buttonPage) {
			if (button.closeButton == closeButton) {
				[self removeItem:button.item animated:YES];
				return;
			}
		}
	}
}

- (void)wobble {
	static BOOL wobblesLeft = NO;
	
	if (editing) {
		CGFloat rotation = (kWobbleRadians * M_PI) / 180.0;
		CGAffineTransform wobbleLeft = CGAffineTransformMakeRotation(rotation);
		CGAffineTransform wobbleRight = CGAffineTransformMakeRotation(-rotation);
		
		[UIView beginAnimations:nil context:nil];
		
		NSInteger i = 0;
		NSInteger nWobblyButtons = 0;
		for (NSArray* buttonPage in buttons) {
			for (LKLauncherButton *button in buttonPage) {
				if (button != dragButton) {
					++nWobblyButtons;
					if (i % 2) {
						button.transform = wobblesLeft ? wobbleRight : wobbleLeft;
					} else {
						button.transform = wobblesLeft ? wobbleLeft : wobbleRight;
					}
				}
				++i;
			}
		}
		
		if (nWobblyButtons >= 1) {
			[UIView setAnimationDuration:kWobbleTime];
			[UIView setAnimationDelegate:self];
			[UIView setAnimationDidStopSelector:@selector(wobble)];
			wobblesLeft = !wobblesLeft;
		} else {
			[NSObject cancelPreviousPerformRequestsWithTarget:self];
			[self performSelector:@selector(wobble) withObject:nil afterDelay:kWobbleTime];
		}
		
		[UIView commitAnimations];
	}
}

- (void)editHoldTimer:(NSTimer*)timer {
	editHoldTimer = nil;
	[self beginEditing];
	
	TTUserInfo* info = timer.userInfo;
	LKLauncherButton * button = info.weakRef;
	UIEvent* event = info.strongRef;
	
	button.selected = NO;
	button.highlighted = NO;
	[self startDraggingButton:button withEvent:event];
}

- (void)deselectButton:(LKLauncherButton *)button {
	[button setSelected:NO];
}

- (void)endEditingAnimationDidStop:(NSString*)animationID finished:(NSNumber*)finished
						   context:(void*)context {
	for (NSArray* buttonPage in buttons) {
		for (LKLauncherButton *button in buttonPage) {
			button.editing = NO;
		}
	}
}

- (void)updateTouch {
	CGPoint origin = [dragTouch locationInView:scrollView];
	dragButton.center = CGPointMake(dragOrigin.x + (origin.x - touchOrigin.x),
									 dragOrigin.y + (origin.y - touchOrigin.y));
	
	CGFloat x = origin.x - scrollView.contentOffset.x;
	NSInteger column = round(x/dragButton.width);
	NSInteger row = round(origin.y/dragButton.height);
	NSInteger itemIndex = (row * self.columnCount) + column;
	NSInteger pageIndex = floor(scrollView.contentOffset.x/scrollView.width);
	
	if (itemIndex != positionOrigin) {
		NSMutableArray* currentButtonPage = [buttons objectAtIndex:pageIndex];
		if (itemIndex > currentButtonPage.count) {
			itemIndex = currentButtonPage.count;
		}
		
		if (itemIndex != positionOrigin) {
			[[dragButton retain] autorelease];
			
			NSMutableArray* itemPage = [self pageWithItem:dragButton.item];
			NSMutableArray* buttonPage = [self pageWithButton:dragButton];
			[itemPage removeObject:dragButton.item];
			[buttonPage removeObject:dragButton];
			
			if (itemIndex > currentButtonPage.count) {
				itemIndex = currentButtonPage.count;
			}
			
			BOOL didMove = itemIndex != positionOrigin;
			
			NSMutableArray* currentItemPage = [pages objectAtIndex:pageIndex];
			[currentItemPage insertObject:dragButton.item atIndex:itemIndex];
			[currentButtonPage insertObject:dragButton atIndex:itemIndex];
			positionOrigin = itemIndex;
			
			[self checkButtonOverflow:pageIndex];
			if (didMove) {
				if ([delegate respondsToSelector:@selector(launcherView:didMoveItem:)]) {
					[delegate launcherView:self didMoveItem:dragButton.item];
				}
				
				[UIView beginAnimations:nil context:nil];
				[UIView setAnimationDuration:TT_TRANSITION_DURATION];
				[self layoutButtons];
				[UIView commitAnimations];
			}
		}
	}
	
	CGFloat springLoadDistance = dragButton.width*kSpringLoadFraction;
	TTDCONDITIONLOG(TTDFLAG_LAUNCHERVIEW, @"%f < %f", springLoadDistance, dragButton.center.x);
	BOOL goToPreviousPage = dragButton.center.x - springLoadDistance < 0;
	BOOL goToNextPage = ((scrollView.width - dragButton.center.x) - springLoadDistance) < 0;
	if (goToPreviousPage || goToNextPage) {
		if (!springLoadTimer) {
			springLoadTimer = [NSTimer scheduledTimerWithTimeInterval:kSpringLoadTimeInterval
																target:self selector:@selector(springLoadTimer:)
															  userInfo:[NSNumber numberWithBool:goToPreviousPage] repeats:NO];
		}
	} else {
		TT_INVALIDATE_TIMER(springLoadTimer);
	}
}


#pragma mark -
#pragma mark UIResponder

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent *)event {
	[super touchesMoved:touches withEvent:event];
	if (dragButton && !springing) {
		for (UITouch* touch in touches) {
			if (touch == dragTouch) {
				[self updateTouch];
				break;
			}
		}
	}
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent *)event {
	[super touchesEnded:touches withEvent:event];
	
	if (dragTouch) {
		for (UITouch* touch in touches) {
			if (touch == dragTouch) {
				
				// New delegate method
				if ([delegate respondsToSelector:@selector(launcherViewDidEndDragging:)]) {
					[delegate launcherViewDidEndDragging:self];
				}
				
				dragTouch = nil;
				break;
			}
		}
	}
}


#pragma mark -
#pragma mark UIView

- (void)setFrame:(CGRect)newFrame {
	[super setFrame:newFrame];
	[self layoutButtons];
}

- (void)layoutSubviews {
	[super layoutSubviews];
	
	pager.frame = CGRectMake(0, scrollView.height, self.width, kPagerHeight);
	
	if (!buttons) {
		[self recreateButtons];
	}
}


#pragma mark -
#pragma mark UIScrollViewDelegate

- (void)scrollViewWillBeginDragging:(UIScrollView*)scrollView {
	TT_INVALIDATE_TIMER(editHoldTimer);
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
	[self updatePagerWithContentOffset:scrollView.contentOffset];
}


#pragma mark -
#pragma mark UIPageControlDelegate

- (void)pageChanged {
	scrollView.contentOffset = CGPointMake(pager.currentPage * scrollView.width, 0);
}


#pragma mark -
#pragma mark Public

- (NSArray*)pages {
	return pages;
}

- (void)setPages:(NSArray*)pages {
	for (NSArray* page in pages) {
		for (LKLauncherItem *item in page) {
			item.launcher = nil;
		}
	}
	
	[pages release];
	pages = [[NSMutableArray alloc] init];
	
	for (NSArray* page in pages) {
		NSMutableArray* pageCopy = [page mutableCopy];
		[pages addObject:pageCopy];
		for (LKLauncherItem *item in pageCopy) {
			item.launcher = self;
		}
		[pageCopy release];
	}
	
	TT_RELEASE_SAFELY(buttons);
	[self setNeedsLayout];
}

- (void)setColumnCount:(NSInteger)aColumnCount {
	if (columnCount != aColumnCount) {
		columnCount = aColumnCount;
		rowCount = 0;
		TT_RELEASE_SAFELY(buttons);
		[self setNeedsLayout];
	}
}

- (NSInteger)rowCount {
	if (!rowCount) {
		rowCount = floor(self.height / [self rowHeight]);
	}
	return rowCount;
}

- (NSInteger)currentPageIndex {
	return floor(scrollView.contentOffset.x/scrollView.width);
}

- (void)setCurrentPageIndex:(NSInteger)pageIndex {
	scrollView.contentOffset = CGPointMake(scrollView.width*pageIndex, 0);
}

- (void)addItem:(LKLauncherItem *)item animated:(BOOL)animated {
	if (![self itemWithURL:item.URL]) {
		item.launcher = self;
		
		if (!pages) {
			pages = [[NSMutableArray arrayWithObject:[NSMutableArray arrayWithObject:item]] retain];
		} else {
			NSMutableArray* page = [self pageWithFreeSpace:self.currentPageIndex];
			[page addObject:item];
		}
		
		if ([delegate respondsToSelector:@selector(launcherView:didAddItem:)]) {
			[delegate launcherView:self didAddItem:item];
		}
		
		if (buttons) {
			[self recreateButtons];
		}
		
		[self scrollToItem:item animated:animated];
	}
}

- (void)removeItem:(LKLauncherItem *)item animated:(BOOL)animated {
	NSMutableArray* itemPage = [self pageWithItem:item];
	if (itemPage) {
		LKLauncherButton *button = [self buttonForItem:item];
		NSMutableArray* buttonPage = [self pageWithButton:button];
		
		item.launcher = nil;
		[itemPage removeObject:button.item];
		
		if (buttonPage) {
			[buttonPage removeObject:button];
			
			if (animated) {
				[UIView beginAnimations:nil context:button];
				[UIView setAnimationDuration:0.2];
				[UIView setAnimationDelegate:self];
				[UIView setAnimationDidStopSelector:@selector(removeButtonAnimationDidStop:finished:context:)];
				[self layoutButtons];
				button.transform = CGAffineTransformMakeScale(0.01, 0.01);
				button.alpha = 0;
				[UIView commitAnimations];
			} else {
				[button removeFromSuperview];
				[self layoutButtons];
			}
		}
		
		if ([delegate respondsToSelector:@selector(launcherView:didRemoveItem:)]) {
			[delegate launcherView:self didRemoveItem:item];
		}
	}
}

- (LKLauncherItem *)itemWithURL:(NSString*)URL {
	for (NSArray* page in pages) {
		for (LKLauncherItem *item in page) {
			if ([item.URL isEqualToString:URL]) {
				return item;
			}
		}
	}
	return nil;
}

- (NSIndexPath*)indexPathOfItem:(LKLauncherItem *)item {
	for (NSUInteger pageIndex = 0; pageIndex < pages.count; ++pageIndex) {
		NSArray* page = [pages objectAtIndex:pageIndex];
		NSUInteger itemIndex = [page indexOfObject:item];
		if (itemIndex != NSNotFound) {
			NSUInteger path[] = {pageIndex, itemIndex};
			return [NSIndexPath indexPathWithIndexes:path length:2];
		}
	}
	return nil;
}

- (void)scrollToItem:(LKLauncherItem *)item animated:(BOOL)animated {
	NSIndexPath* path = [self indexPathOfItem:item];
	if (path) {
		NSUInteger page = [path indexAtPosition:0];
		CGFloat x = page * scrollView.width;
		[scrollView setContentOffset:CGPointMake(x, 0) animated:animated];
	}
}

- (void)beginEditing {
	editing = YES;
	scrollView.delaysContentTouches = YES;
	
	UIView* prompt = [self viewWithTag:kPromptTag];
	[prompt removeFromSuperview];
	
	for (NSArray* buttonPage in buttons) {
		for (LKLauncherButton *button in buttonPage) {
			button.editing = YES;
			[button.closeButton addTarget:self action:@selector(closeButtonTouchedUpInside:)
						 forControlEvents:UIControlEventTouchUpInside];
		}
	}
	
	// Add a page at the end
	[pages addObject:[NSMutableArray array]];
	[buttons addObject:[NSMutableArray array]];
	[self updateContentSize:pages.count];
	
	[self wobble];
	
	if ([delegate respondsToSelector:@selector(launcherViewDidBeginEditing:)]) {
		[delegate launcherViewDidBeginEditing:self];
	}
}

- (void)endEditing {
	editing = NO;
	scrollView.delaysContentTouches = NO;
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:0.3];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(endEditingAnimationDidStop:finished:context:)];
	
	for (NSArray* buttonPage in buttons) {
		for (LKLauncherButton *button in buttonPage) {
			button.transform = CGAffineTransformIdentity;
			button.closeButton.alpha = 0;
		}
	}
	
	[UIView commitAnimations];
	
	for (NSInteger i = 0; i < pages.count; ++i) {
		NSArray* page = [pages objectAtIndex:i];
		if (!page.count) {
			[pages removeObjectAtIndex:i];
			[buttons removeObjectAtIndex:i];
			--i;
		}
	}
	
	[self layoutButtons];
	
	if ([delegate respondsToSelector:@selector(launcherViewDidEndEditing:)]) {
		[delegate launcherViewDidEndEditing:self];
	}
}

- (void)beginHighlightItem:(LKLauncherItem *)item withText:(NSString*)text {
	if (nil == highlightView) {
		highlightView = [[LKLauncherHighlightView alloc] initWithFrame:CGRectZero];
		highlightView.parentView = self;
		[self.window addSubview:highlightView];
	}
	
	LKLauncherButton *button = [self buttonForItem:item];
	highlightView.text = text;
	highlightView.highlightRect = LKRectInset([self.window convertRect:button.frame
															   fromView:scrollView],
											   UIEdgeInsetsMake(8.0, 2.0, -4.0, 2.0));
	[highlightView setNeedsDisplay];
	[highlightView appear:YES];
}

- (void)endHighlightItem:(LKLauncherItem *)item {
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:0.2];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(highlightEndDidFinish)];
	highlightView.alpha = 0.0;
	[UIView commitAnimations];
}

- (void)highlightEndDidFinish {
	[highlightView removeFromSuperview];
	TT_RELEASE_SAFELY(highlightView);
}

@end
