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
	id<UITableViewDelegate> delegate;
}

@property (nonatomic, retain) Product	*product;
@property (nonatomic, assign) id<UITableViewDelegate> delegate;

- (void)refresh;
- (id)initWithStyle:(UITableViewCellStyle)style 
	reuseIdentifier:(NSString *)reuseIdentifier 
			product:(Product *)aProduct cellForRowAtIndexPath:(NSIndexPath *)indexPath;
@end
