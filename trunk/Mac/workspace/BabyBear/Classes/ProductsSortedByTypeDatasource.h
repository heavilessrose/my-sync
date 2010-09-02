//
//  ProductsSortedByTypeDatasource.h
//  BabyBear
//
//  Created by luke on 10-9-2.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProductsDatasourceProtocol.h"

// product tableview datasource 重构
@interface ProductsSortedByTypeDatasource : NSObject <UITableViewDataSource, ProductsDatasource> {

}

@end
