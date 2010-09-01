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
#import "ProductInfoView.h"


@interface ProductDetailViewConctroller : UIViewController 
<ScrollShowViewPageDelegate, ImageDownloaderDelegate, UITableViewDelegate, UITableViewDataSource> 
{
	BaseProduct			*product;
    NSMutableDictionary *imageDownloadsInProgress;  // the set of IconDownloader objects for each product
	NSMutableDictionary *downloadedPreImgs; // the set of UIImage objects that downloaded from server
	
	UIScrollView		*scrollView;
	ProductInfoView		*infoView;
	ScrollShowView		*scrollshow;
	UITableView			*tableView;
}

- (id)initWithProduct:(BaseProduct *)aProduct;
@end
