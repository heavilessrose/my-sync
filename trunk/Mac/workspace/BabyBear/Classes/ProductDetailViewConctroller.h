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
	BaseProduct			*product;
    NSMutableDictionary *imageDownloadsInProgress;  // the set of IconDownloader objects for each product
	NSMutableDictionary *downloadedPreImgs; // the set of UIImage objects that downloaded from server
	
	IBOutlet ScrollShowView	*scrollshow;
	IBOutlet UITableView	*tableView;
}

@end
