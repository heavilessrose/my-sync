//
//  ProductCell.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Product.h"


@interface ProductCell : UITableViewCell {
	Product	*product;
}

@property (nonatomic, retain) Product	*product;
@end
