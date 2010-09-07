//
//  ProductInfoView.h
//  BabyBear
//
//  Created by luke on 10-9-1.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RateView.h"

@interface ProductInfoView : UIView <RateViewDelegate> {
	NSString *detailInfo;
	RateView *rateView;
}

@property (nonatomic, retain) NSString *detailInfo;
@property (nonatomic, retain) RateView *rateView;

+ (CGRect)calcFrame:(NSString *)theDetailInfo frame:(CGRect)theFrame;
- (id)initWithDetailInfo:(NSString *)theDetailInfo andFrame:(CGRect)frame;

@end
