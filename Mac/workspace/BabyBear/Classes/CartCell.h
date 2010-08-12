//
//  CartCell.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "defines.h"


@interface CartCell : UITableViewCell {
	CartItem	item;
}
@property (nonatomic, assign) CartItem	item;
@end
