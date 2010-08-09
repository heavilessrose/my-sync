/*
 *  TapImage.h
 *  Picoffee
 *
 *  Created by luke on 8/6/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

enum _SaleType {
    SaleType_DEFAULT = 0,
    SaleType_HOT,
    SaleType_TUAN,
    SaleType_DISCOUNT,
    SaleType_NEW,
}SaleType;

@interface TapImage : UIImageView {
    int product_id;
    int rank;
    NSString *name;
    NSString *price;
    NSString *briefInfo;
    NSString *contentInfo;
    NSString *avatarUrl;
}

@property (nonatomic, assign) int product_id;
@property (nonatomic, assign) int rank;
@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *price;
@property (nonatomic, retain) NSString *briefInfo;
@property (nonatomic, retain) NSString *contentInfo;
@property (nonatomic, retain) NSString *avatarUrl;

@end