//
//  LKLauncherView.h
//  LaunchCube
//
//  Created by luke on 10-12-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LKLauncherScrollView, LKLauncherItem, LKPageControl, LKLauncherButton, LKLauncherHighlightView;
@class LKLauncherView;

@protocol LKLauncherViewDelegate <NSObject>

@optional
- (void)launcherView:(LKLauncherView *)launcher didAddItem:(LKLauncherItem *)item;
- (void)launcherView:(LKLauncherView *)launcher didRemoveItem:(LKLauncherItem *)item;
- (void)launcherView:(LKLauncherView *)launcher didMoveItem:(LKLauncherItem *)item;
- (void)launcherView:(LKLauncherView *)launcher didSelectItem:(LKLauncherItem *)item;
- (void)launcherViewDidEndDragging:(LKLauncherView *)launcher;
- (void)launcherViewDidBeginEditing:(LKLauncherView *)launcher;
- (void)launcherViewDidEndEditing:(LKLauncherView *)launcher;
@end

@interface LKLauncherView : UIView <UIScrollViewDelegate> {

	LKLauncherScrollView *scrollView;
	NSInteger columnCount;
	NSArray *pages;
	NSMutableArray *buttons;
	
	NSInteger rowCount;
	NSString *prompt;
	LKPageControl *pager;
	
	NSTimer *editHoldTimer;
	NSTimer *springLoadTimer;
	
	LKLauncherButton *dragButton;
	UITouch *dragTouch;
	NSInteger positionOrigin;
	CGPoint dragOrigin;
	CGPoint touchOrigin;
	
	LKLauncherHighlightView *highlightView;
	
	BOOL editing;
	BOOL springing;
	
	id<LKLauncherViewDelegate> delegate;
}

@property (nonatomic, retain) LKLauncherScrollView *scrollView;
@property (nonatomic, assign) NSInteger columnCount;
@property (nonatomic, retain) NSArray *pages;
@property (nonatomic, retain) NSMutableArray *buttons;
@property (nonatomic, assign) id<LKLauncherViewDelegate> delegate;
@property (nonatomic, readonly) NSInteger rowCount;
@property (nonatomic) NSInteger currentPageIndex;
@property (nonatomic, copy) NSString *prompt;
@property (nonatomic, readonly) BOOL editing;

- (void)addItem:(LKLauncherItem *)item animated:(BOOL)animated;
- (void)removeItem:(LKLauncherItem *)item animated:(BOOL)animated;
- (LKLauncherItem *)itemWithURL:(NSString *)URL;
- (NSIndexPath*)indexPathOfItem:(LKLauncherItem *)item;
- (void)scrollToItem:(LKLauncherItem *)item animated:(BOOL)animated;
- (void)beginEditing;
- (void)endEditing;

/**
 * Dims the launcher view except for a transparent circle around the given item. The given text
 * will also be shown center-aligned below or above the circle, as appropriate. The item can be
 * tapped while the overlay is up; tapping anywhere else on the overlay simply dismisses the
 * overlay and does not pass the event through.
 */
- (void)beginHighlightItem:(LKLauncherItem *)item withText:(NSString *)text;

/**
 * Removes the highlighting overlay introduced by -beginHighlightItem:withText:. This will be done
 * automatically if the user taps anywhere on the overlay except the transparent circle.
 */
- (void)endHighlightItem:(LKLauncherItem *)item;
@end
