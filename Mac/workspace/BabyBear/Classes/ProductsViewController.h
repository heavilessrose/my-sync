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
#import "ProductsDatasourceProtocol.h"

@interface ProductsViewController : UITableViewController <UITableViewDelegate, 
UIScrollViewDelegate, XmlParseOperationDelegate, ImageDownloaderDelegate> {
	
    NSMutableDictionary *imageDownloadsInProgress;  // the set of IconDownloader objects for each product
	
	//NSMutableArray		*products; // the main data model for our UITableView which is arranged by product type
	NSOperationQueue	*queue;
	
	BOOL				isProductsFetched; // avoid fetch products info more than one times
	NSURLConnection		*productsFeedConnection;
	NSMutableData		*productsData;
	
	//ProductCell	*tmpProductCell;
	
	//NSMutableArray		*productTypeArr;
	
	UITableView			*tableView;
	id<ProductsDatasource, UITableViewDataSource> dataSource;
	NSIndexPath			*indexSelected;
}


@property (nonatomic, retain) NSMutableDictionary	*imageDownloadsInProgress;

//@property (nonatomic, retain) NSMutableArray		*products;
@property (nonatomic, retain) NSOperationQueue		*queue;

@property (nonatomic, retain) NSURLConnection		*productsFeedConnection;
@property (nonatomic, retain) NSMutableData			*productsData;

//@property (nonatomic, retain) NSMutableArray		*productTypeArr;

@property (nonatomic, retain) UITableView *tableView;
@property (nonatomic, retain) id<ProductsDatasource, UITableViewDataSource> dataSource;


- (id)initWithDataSource:(Class)aDataSourceClass;
@end
