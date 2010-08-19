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

@interface ProductsViewController : UITableViewController <XmlParseOperationDelegate> {
	
	NSArray				*entries; // the main data model for our UITableView
	
	NSMutableArray		*products;
	NSOperationQueue	*queue;
	
	NSURLConnection		*productsFeedConnection;
	NSMutableData		*productsData;
}


@property (nonatomic, retain) NSArray			*entries;

@property (nonatomic, retain) NSMutableArray	*products;
@property (nonatomic, retain) NSOperationQueue	*queue;

@property (nonatomic, retain) NSURLConnection	*productsFeedConnection;
@property (nonatomic, retain) NSMutableData		*productsData;

@end
