/*
 *  ScrollShowView.h
 *  Picoffee
 *
 *  Created by luke on 8/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@class ScrollShowView;
@protocol ScrollShowViewPageDelegate
@required
- (UIView *)viewForPageAtIndex:(ScrollShowView *)scrollView pageIndex:(int)index;
- (int)itemCount:(ScrollShowView *)scrollView;
@end


enum _pageStyle {
    PAGESTYLE_NOPADDING = 0,
    PAGESTYLE_PADDING = 1,
    PAGESTYLE_DEFAULT = PAGESTYLE_NOPADDING,
}PageStyle;

@interface ScrollShowView : UIView <UIScrollViewDelegate> 
{
    UIScrollView *scrollView;
    CGSize pageContentSize;
    BOOL _isLayouted;
    BOOL backShadow;
    int pageStyle;
    CGFloat x_padding;
    CGFloat y_padding;
    NSMutableArray *pages;
	id<ScrollShowViewPageDelegate, NSObject> pageDelegate;
}

@property (nonatomic, retain) UIScrollView *scrollView;
@property (nonatomic, assign) CGSize pageContentSize;
@property (nonatomic, assign) BOOL backShadow;
@property (nonatomic, retain) NSMutableArray *pages;
@property (nonatomic, assign) id<ScrollShowViewPageDelegate, NSObject> pageDelegate;
@property (nonatomic, assign) int pageStyle;
@property (nonatomic, assign) CGFloat x_padding;
@property (nonatomic, assign) CGFloat y_padding;

- (id)initWithFrame:(CGRect)frame pageContentSize:(CGSize)pSize;
- (void)didReceiveMemoryWarning;

@end
