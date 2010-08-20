//
//  ProductsViewController.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "XmlParseOperation.h"
#import "Product.h"
#import "ProductCell.h"
#import "ImageDownloader.h"

@interface ProductsViewController : UITableViewController <UIScrollViewDelegate, XmlParseOperationDelegate, ImageDownloaderDelegate> {
	
	NSArray				*entries; // the main data model for our UITableView
    NSMutableDictionary *imageDownloadsInProgress;  // the set of IconDownloader objects for each product
	
	NSMutableArray		*products;
	NSOperationQueue	*queue;
	
	BOOL				isProductsFetched; // avoid fetch products info more than one times
	NSURLConnection		*productsFeedConnection;
	NSMutableData		*productsData;
	
	IBOutlet ProductCell	*tmpProductCell;
	IBOutlet UITableView	*tableView;
}


@property (nonatomic, retain) NSArray				*entries;
@property (nonatomic, retain) NSMutableDictionary	*imageDownloadsInProgress;

@property (nonatomic, retain) NSMutableArray		*products;
@property (nonatomic, retain) NSOperationQueue		*queue;

@property (nonatomic, retain) NSURLConnection		*productsFeedConnection;
@property (nonatomic, retain) NSMutableData			*productsData;

@end
