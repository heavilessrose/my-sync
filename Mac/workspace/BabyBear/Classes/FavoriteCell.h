//
//  FavoriteCell.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "defines.h"
#import "FavoriteProduct.h"


@interface FavoriteCell : UITableViewCell {
	FavoriteProduct	*item;
}

@property (nonatomic, assign) FavoriteProduct	*item;
@end
