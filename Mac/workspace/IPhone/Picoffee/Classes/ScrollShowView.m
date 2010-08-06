/*
 *  ScrollShowView.m
 *  Picoffee
 *
 *  Created by luke on 8/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ScrollShowView.h"


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

- (int)currentPage
{
	// Calculate which page is visible 
	CGFloat pageWidth = 0.0f;
    if(pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
        pageWidth = pageContentSize.width;
    } else {
        pageWidth = pageContentSize.width + x_padding * 2;
    }

	int page = floor((scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
	
	return page;
}

- (void)loadPage:(int)aPage
{
    if (aPage < 0 || aPage >= [pages count])
        return;
	
	UIView *aPageView = nil;
    UIView *aPageContentView = [pages objectAtIndex:aPage];
	
	if ((NSNull *)aPageContentView == [NSNull null]) {
		aPageContentView = [pageDelegate viewForPageAtIndex:self pageIndex:aPage];
		[pages replaceObjectAtIndex:aPage withObject:aPageContentView];
	}
	
	if (aPageContentView.superview == nil) {
        if (pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
            CGRect viewFrame = aPageContentView.frame;
            viewFrame.origin.x = viewFrame.size.width * aPage;
            viewFrame.origin.y = (self.frame.size.height - viewFrame.size.height) / 2;
            aPageContentView.frame = viewFrame;
            [scrollView addSubview:aPageContentView];
        } else if (pageStyle == PAGESTYLE_PADDING) {
            CGFloat aPageView_w = pageContentSize.width + x_padding * 2;
            CGFloat aPageView_h = pageContentSize.height + y_padding * 2;
            
            CGRect aPageViewFrame = CGRectMake(aPageView_w * aPage, 0.0f, 
                                               aPageView_w, aPageView_h);
            aPageView = [[UIView alloc] initWithFrame:aPageViewFrame];
            aPageView.backgroundColor = [UIColor clearColor];
            aPageContentView.frame = CGRectMake(x_padding, y_padding, 
                                                aPageContentView.frame.size.width, aPageContentView.frame.size.height);
            [aPageView addSubview:aPageContentView];
            [scrollView addSubview:aPageView];
            [aPageView release];
        }
	}
}

#pragma mark -
#pragma mark UIScrollViewDelegate methods

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
//- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView
{
	int currentPage = [self currentPage];
	
	[self loadPage:currentPage - 1];
	[self loadPage:currentPage];
	[self loadPage:currentPage + 1];
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
- (id)initWithFrame:(CGRect)frame
{
    if(self = [super initWithFrame:frame]) {
        _isLayouted = NO;
        self.x_padding = 0.0f;
        self.y_padding = 0.0f;
        self.pageStyle = PAGESTYLE_DEFAULT;
        self.backShadow = NO;
        self.pageContentSize = CGSizeZero;
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame pageContentSize:(CGSize)pSize
{
    if(self = [self initWithFrame:frame]) {
        self.pageContentSize = pSize;
        self.scrollView = [[UIScrollView alloc] initWithFrame:CGRectZero];
		//scrollView.clipsToBounds = NO; // Important, this creates the "preview"
		scrollView.pagingEnabled = YES;
		scrollView.showsHorizontalScrollIndicator = YES;
		scrollView.showsVerticalScrollIndicator = NO;
        scrollView.userInteractionEnabled = YES;
        scrollView.scrollEnabled = YES;
		scrollView.delegate = self;
        scrollView.indicatorStyle = UIScrollViewIndicatorStyleBlack;
		[self addSubview:scrollView];
        [scrollView release];
        self.userInteractionEnabled = YES;
    }
    
    return self;
}

- (void)dealloc
{
    [pages release];
    [scrollView release];
    [super dealloc];
}

- (void)layoutSubviews
{
    if(!_isLayouted) {
        if(backShadow){
            [self showShadow];
        }
        
        int pageCount = [pageDelegate itemCount:self];
        scrollView.frame = CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
        
        if(pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
            [scrollView setContentSize:CGSizeMake(pageCount * pageContentSize.width, pageContentSize.height)];
        } else {
            [scrollView setContentSize:CGSizeMake(pageCount * (pageContentSize.width + x_padding * 2), (pageContentSize.height + y_padding * 2))];
        }
        
        self.pages = [[NSMutableArray alloc] initWithCapacity:pageCount];
        
        for(int i = 0; i < pageCount; i++) {
            [pages addObject:[NSNull null]];
        }
        [pages release];
        
        // Load the should visalble pages
        int vPageCount = 0.0f;
        if(pageStyle == PAGESTYLE_NOPADDING || pageStyle == PAGESTYLE_DEFAULT) {
            vPageCount = self.frame.size.width / pageContentSize.width + 1;
        } else {
            vPageCount = self.frame.size.width / (pageContentSize.width + x_padding * 2) + 1;
        }
        for(int i = 0; i < vPageCount; i++){
            [self loadPage:i];
        }
        
        _isLayouted = YES;
    }
}

@end

