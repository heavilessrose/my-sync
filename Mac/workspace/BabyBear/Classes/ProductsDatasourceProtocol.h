/*
 *  ProductsDatasourceProtocol.h
 *  BabyBear
 *
 *  Created by luke on 10-9-2.
 *  Copyright 2010 Luke. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import "BaseProduct.h"

@protocol ProductsDatasource <NSObject>

@required

// these properties are used by the view controller for the navigation and tab bar
@property (readonly) NSString *name;
@property (readonly) NSString *navigationBarName;
@property (readonly) UIImage *tabBarImage;

// as the C in MVC
@property (nonatomic, assign) id<UITableViewDelegate> controller;

// this property determines the style of table view displayed
@property (readonly) UITableViewStyle tableViewStyle;


- (BOOL)showDisclosureIcon;

/*
 provides a standardized means of asking for the element at the specific index path, 
 regardless of the sorting or display technique for the specific datasource
 */
- (BaseProduct *)productForIndexPath:(NSIndexPath *)indexPath;
- (NSInteger)allDatasCount;

@optional

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section;
@end
