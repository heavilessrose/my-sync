/*
 *  ScrollShowView.m
 *  Picoffee
 *
 *  Created by luke on 8/4/10.
 *  Copyright 2010 Luke. All rights reserved.
 *
 */

#include "ScrollShowView.h"

@interface ScrollShowView ()

@property (nonatomic, assign) BOOL _isLayouted;
@property (nonatomic, retain) UIScrollView *scrollView;
@property (nonatomic, assign) BOOL backShadow;
@property (nonatomic, retain) NSMutableArray *pages;
@property (nonatomic, assign) id<ScrollShowViewPageDelegate, NSObject> pageDelegate;

/*!
 @function
 @abstract   调整pageContent(the UIImageView)宽度, 使所有page的宽度和与scrollViewd的宽度成正比 
 @discussion 
 @param      
 @result     
 */

- (void)adjustPageContentSize;

@end



@implementation ScrollShowView
@synthesize scrollView, pageContentSize, backShadow, pages, pageDelegate;
@synthesize pageStyle, x_padding, y_padding;

#define kShadow_h 20.0f
#define kShadow_Inverse_h 10.0f
#define kShadow_ratio (kShadow_Inverse_h / kShadow_h)

#define kDefault_x_padding 10.0f
#define kDefault_y_padding 10.0f

+ (void)debugFrame:(CGRect)frame tip:(NSString *)tip
{
    NSLog(@"%@: %f, %f, %f, %f", tip, frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
}

- (void)adjustPageContentSize
{
    int pageCount = [pages count];
    CGFloat adjustAll = (CGFloat)((int)scrollView.frame.size.width % pageCount);
    CGFloat adjustSingle = adjustAll / pageCount;
    CGFloat pageContent_w = pageContentSize.width + adjustSingle;
    CGSize aSize = pageContentSize;
    aSize.width = pageContent_w;
    pageContentSize = aSize;
}

/*!
    @function
    @abstract   Calculate which page is visible
    @discussion 
    @param      
    @result     
*/

- (int)currentPage
{
#if 0
	CGFloat pageWidth = 0.0f;
    if(pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
        pageWidth = pageContentSize.width;
    } else {
        pageWidth = pageContentSize.width + x_padding * 2;
    }

	int page = floor((scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
	
#else
	CGFloat pageHeight = 0.0f;
    if(pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
        pageHeight = pageContentSize.height;
    } else {
        pageHeight = pageContentSize.height + y_padding * 2;
    }
	
	CGFloat off = (scrollView.contentOffset.y - pageHeight / 2) / pageHeight;
	int page = floor(off) + 1;
#endif
	
	return page;
}

- (void)loadPage:(int)aPageIndex
{
    if (aPageIndex < 0 || aPageIndex >= [pages count])
        return;
	
	UIView *aPageView = nil;
    UIView *aPageContentView = [pages objectAtIndex:aPageIndex];
	
	if ((NSNull *)aPageContentView == [NSNull null]) {
		aPageContentView = [pageDelegate viewForPageAtIndex:self pageIndex:aPageIndex];
		[pages replaceObjectAtIndex:aPageIndex withObject:aPageContentView];
        aPageContentView.frame = CGRectMake(0.0f, 0.0f, pageContentSize.width, pageContentSize.height);
	}
	
	if (aPageContentView.superview == nil) {
        if (pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
            CGRect viewFrame = aPageContentView.frame;
            viewFrame.origin.x = viewFrame.size.width * aPageIndex;
            viewFrame.origin.y = (self.frame.size.height - viewFrame.size.height) / 2;
            aPageContentView.frame = viewFrame;
            [scrollView addSubview:aPageContentView];
        } else if (pageStyle == PAGESTYLE_PADDING) {
            CGFloat aPageView_w = pageContentSize.width + x_padding * 2;
            CGFloat aPageView_h = pageContentSize.height + y_padding * 2;
            
#if 0
            CGRect aPageViewFrame = CGRectMake(aPageView_w * aPageIndex, 0.0f, 
                                               aPageView_w, aPageView_h);
#else
			CGRect aPageViewFrame = CGRectMake(0.0f, aPageView_h * aPageIndex, aPageView_w, aPageView_h);
#endif
            aPageView = [[UIView alloc] initWithFrame:aPageViewFrame];
            aPageView.backgroundColor = [UIColor clearColor];
            aPageContentView.frame = CGRectMake(x_padding, y_padding, 
                                                aPageContentView.frame.size.width, aPageContentView.frame.size.height);
            [ScrollShowView debugFrame:aPageContentView.frame tip:@"aPageContentView"];
            [aPageView addSubview:aPageContentView];
            [scrollView addSubview:aPageView];
            [aPageView release];
        }
	}
}

#pragma mark -
#pragma mark UIScrollViewDelegate methods

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
//- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
//- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView
{
	int currentPage = [self currentPage];
	
	[self loadPage:currentPage - 1];
	[self loadPage:currentPage];
	[self loadPage:currentPage + 1];
    // 可见的一般为最左边那个,依情况载入多个
	[self loadPage:currentPage + 2];
}

- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate {
	
	int index = [self currentPage];
	NSLog(@"%d", index);
}


#pragma mark -
#pragma mark Memory management

// didReceiveMemoryWarning is not called automatically for views, 
// make sure you call it from your view controller
- (void)didReceiveMemoryWarning 
{
	// Calculate the current page in scroll view
    int currentPage = [self currentPage];
	
	// unload the pages which are no longer visible
	for (int i = 0; i < [pages count]; i++) {
		UIView *viewController = [pages objectAtIndex:i];
        if((NSNull *)viewController != [NSNull null]) {
			if(i < currentPage-1 || i > currentPage+1) {
				[viewController removeFromSuperview];
				[pages replaceObjectAtIndex:i withObject:[NSNull null]];
			}
		}
	}
}

#pragma mark -
#pragma mark shadow
// Shadow code from http://cocoawithlove.com/2009/08/adding-shadow-effects-to-uitableview.html
- (CAGradientLayer *)shadowAsInverse:(BOOL)inverse
{
    CAGradientLayer *newShadow = [[[CAGradientLayer alloc] init] autorelease];
    CGRect newShadowFrame =	CGRectMake(0, 0, self.frame.size.width, inverse ? kShadow_Inverse_h : kShadow_h);
    newShadow.frame = newShadowFrame;
    CGColorRef darkColor =[UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:inverse ? kShadow_ratio * 0.5 : 0.5].CGColor;
    CGColorRef lightColor =	[self.backgroundColor colorWithAlphaComponent:0.0].CGColor;
    newShadow.colors = [NSArray arrayWithObjects: (id)(inverse ? lightColor : darkColor), (id)(inverse ? darkColor : lightColor), nil];
    return newShadow;
}

- (void)showShadow
{
    CAGradientLayer *topShadowLayer = [self shadowAsInverse:NO];
    CAGradientLayer *bottomShadowLayer = [self shadowAsInverse:YES];
    [self.layer insertSublayer:topShadowLayer atIndex:0];
    [self.layer insertSublayer:bottomShadowLayer atIndex:0];
    
    [CATransaction begin];
    [CATransaction setValue:(id)kCFBooleanTrue forKey:kCATransactionDisableActions];
    
    // Position and stretch the shadow layers to fit
    CGRect topShadowLayerFrame = topShadowLayer.frame;
    topShadowLayerFrame.size.width = self.frame.size.width;
    topShadowLayerFrame.origin.y = 0;
    topShadowLayer.frame = topShadowLayerFrame;
    
    CGRect bottomShadowLayerFrame = bottomShadowLayer.frame;
    bottomShadowLayerFrame.size.width = self.frame.size.width;
    bottomShadowLayerFrame.origin.y = self.frame.size.height - bottomShadowLayer.frame.size.height;
    bottomShadowLayer.frame = bottomShadowLayerFrame;
    
    [CATransaction commit];
}

#pragma mark -

@synthesize _isLayouted;

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        self._isLayouted = NO;
        self.x_padding = 0.0f;
        self.y_padding = 0.0f;
        self.pageStyle = PAGESTYLE_DEFAULT;
        self.backShadow = NO;
        self.pageContentSize = CGSizeZero;
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame pageContentSize:(CGSize)pSize pageDelegate:(id<ScrollShowViewPageDelegate, NSObject>)thePageDelegate
{
    if (self = [self initWithFrame:frame]) {
		self._isLayouted = NO;
        self.backShadow = YES;
		self.pageDelegate = thePageDelegate;
        self.pageContentSize = pSize;
        self.scrollView = [[UIScrollView alloc] initWithFrame:CGRectZero];
		scrollView.clipsToBounds = NO; // Important, this creates the "preview"
		scrollView.pagingEnabled = YES;
		scrollView.showsHorizontalScrollIndicator = NO;
		scrollView.showsVerticalScrollIndicator = YES;
        scrollView.userInteractionEnabled = YES;
        scrollView.scrollEnabled = YES;
		scrollView.delegate = self;
        scrollView.indicatorStyle = UIScrollViewIndicatorStyleBlack;
		
		int i = [scrollView retainCount];
		[self addSubview:scrollView];
		i = [scrollView retainCount];
        [scrollView release];
		i = [scrollView retainCount];
        self.userInteractionEnabled = YES;
    }
    
    return self;
}

- (void)dealloc
{
	NSLog(@"ScrollShow --------- dealloc start");
    [pages release];
	int i = [scrollView retainCount];
    [scrollView release];
	
    [super dealloc];
	NSLog(@"ScrollShow --------- dealloc end");
}

- (void)layoutSubviews
{
    if (!self._isLayouted) {
        if (backShadow) {
            [self showShadow];
        }
        
        int pageCount = [pageDelegate itemCount:self];
        scrollView.frame = CGRectMake(0.0f, 0.0f, self.frame.size.width, self.frame.size.height);
        
        self.pages = [[NSMutableArray alloc] initWithCapacity:pageCount];
        
        for (int i = 0; i < pageCount; i++) {
            [pages addObject:[NSNull null]];
        }
        [pages release];
#if 0
        if (pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
            [scrollView setContentSize:CGSizeMake(pageCount * pageContentSize.width, pageContentSize.height)];
        } else {
            [scrollView setContentSize:CGSizeMake(pageCount * (pageContentSize.width + x_padding * 2), (pageContentSize.height + y_padding * 2))];
        }
#else
		[scrollView setContentSize:CGSizeMake(100, 1000)];	
#endif
        
		// Load the should visalble pages
		int vPageCount = 0.0f;
		if (pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
			vPageCount = self.frame.size.width / pageContentSize.width + 1;
		} else {
			// 不应调整, 应根据宽度调整ContentSize
			[self adjustPageContentSize];
			vPageCount = self.frame.size.width / (pageContentSize.width + x_padding * 2) + 1;
		}
		
#if 0
		for (int i = 0; i < vPageCount; i++) {
			[self loadPage:i];
		}
#else
		[self loadPage:0];
		[self loadPage:1];
#endif
        self._isLayouted = YES;
    }
}

@end

