//
//  CartCell.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "defines.h"
#import "CartProduct.h"


@interface CartCell : UITableViewCell {
	CartProduct	*item;
}
@property (nonatomic, assign) CartProduct	*item;
@end
