//
//  ProductDetailViewConctroller.h
//  BabyBear
//
//  Created by luke on 10-8-20.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseProduct.h"
#import "ScrollShowView.h"
#import "ImageDownloader.h"


@interface ProductDetailViewConctroller : UITableViewController <ScrollShowViewPageDelegate, ImageDownloaderDelegate> {
	BaseProduct	*product;
}

@end
