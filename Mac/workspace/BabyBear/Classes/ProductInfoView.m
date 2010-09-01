//
//  ProductInfoView.m
//  BabyBear
//
//  Created by luke on 10-9-1.
//  Copyright 2010 Luke. All rights reserved.
//

#import "ProductInfoView.h"

@interface ProductInfoView ()

@end

@implementation ProductInfoView

@synthesize detailInfo, rateView;

- (id)initWithDetailInfo:(NSString *)theDetailInfo andFrame:(CGRect)frame
{
	self.detailInfo = theDetailInfo;
	CGRect aFrame = [ProductInfoView calcFrame:theDetailInfo frame:frame];
	return [self initWithFrame:aFrame];
}

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		self.backgroundColor = [UIColor redColor];
    }
    return self;
}

// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
	self.rateView = [[RateView alloc] initWithFrame:CGRectMake(0, 0, 100, 20)];
	rateView.notSelectedImage = [UIImage imageNamed:@"star_empty.png"];
	rateView.halfSelectedImage = [UIImage imageNamed:@"star_half.png"];
	rateView.fullSelectedImage = [UIImage imageNamed:@"star.png"];
	rateView.rating = 2.5;
	rateView.editable = YES;
	rateView.maxRating = 5;
	rateView.delegate = self;
	
	rateView.backgroundColor = [UIColor clearColor];
	[self addSubview:rateView];
	[rateView release];
}

- (void)dealloc
{
	[detailInfo release];
	[rateView release];
	
    [super dealloc];
}


#pragma mark -
#pragma mark Caculate frame

#define BrifeInfo_H (200.0f)
#define DetailInfo_W (320.0f)

+ (CGRect)calcFrame:(NSString *)theDetailInfo frame:(CGRect)theFrame
{
	CGSize strSize = [theDetailInfo sizeWithFont:[UIFont systemFontOfSize:(CGFloat)10.0f]];
	CGFloat lineCount = strSize.width / DetailInfo_W;
	theFrame.size.height = lineCount * strSize.height + BrifeInfo_H;
	theFrame.size.width = DetailInfo_W;
	return theFrame;
}

#pragma mark -
#pragma mark rateView delegate


- (void)rateView:(RateView *)rateView ratingDidChange:(float)rating
{
	rateView.reviewCountLabel.text = [NSString stringWithFormat:@"%f Reviews", rating];
}


@end
