//
//  ProductsSortedByTypeDatasource.m
//  BabyBear
//
//  Created by luke on 10-9-2.
//  Copyright 2010 Luke. All rights reserved.
//

#import "ProductsSortedByTypeDatasource.h"


@implementation ProductsSortedByTypeDatasource


#pragma mark -
#pragma mark ProductsDatasource Protocol

- (NSString *)name
{
	return @"Products";
}

- (NSString *)navigationBarName
{
	return @"Sorted by Name";
}

- (BOOL)showDisclosureIcon
{
	return NO;
}
- (UIImage *)tabBarImage
{
	return nil;
}

- (UITableViewStyle)tableViewStyle
{
	return UITableViewStylePlain;
};

- (BaseProduct *)productForIndexPath:(NSIndexPath *)indexPath
{
	
}

@end
