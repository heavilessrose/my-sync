//
//  RecommendCell.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "defines.h"
#import "Product.h"


@interface RecommendCell : UITableViewCell {
	Product			*item;
	UILabel			*title;
	UIImageView		*imgView;
}

@property (nonatomic, retain) UILabel			*title;
@property (nonatomic, retain) UIImageView		*imgView;
@property (nonatomic, assign) Product					*item;
@end
