//
//  ProductsSortedByTypeDatasource.m
//  BabyBear
//
//  Created by luke on 10-9-2.
//  Copyright 2010 Luke. All rights reserved.
//

#import "ProductsSortedByTypeDatasource.h"
#import "AllProducts.h"
#import "ProductCell.h"


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
	return YES;
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
	NSString *aTypeName = [[AllProducts sharedAllProducts].elementTypeArray objectAtIndex:indexPath.section];
	NSArray *aProductArr = [[AllProducts sharedAllProducts] elementsWithType:aTypeName];
	return [aProductArr objectAtIndex:indexPath.row];
}

#pragma mark -
#pragma mark table data

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	static NSString *CellID = @"ProductCell";
	Product *theProduct = (Product *)[self productForIndexPath:indexPath];
	ProductCell *cell = (ProductCell *)[tableView dequeueReusableCellWithIdentifier:CellID];
	if (cell == nil) {
		cell = [[[ProductCell alloc] initWithStyle:UITableViewStylePlain 
								   reuseIdentifier:CellID 
										   product:theProduct 
							 cellForRowAtIndexPath:indexPath] autorelease];
	}
	
	if ([self showDisclosureIcon]) {
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
	}
	cell.product = (Product *)theProduct;
	
	return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return [[AllProducts sharedAllProducts].elementTypeArray count];
}

- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView
{
	return [AllProducts sharedAllProducts].elementTypeArray;
}

- (NSInteger)tableView:(UITableView *)tableView sectionForSectionIndexTitle:(NSString *)title
			   atIndex:(NSInteger)index
{
	return index;
}

- (NSInteger)tableView:(UITableView *)tableView  numberOfRowsInSection:(NSInteger)section
{
	NSString *aType = [[AllProducts sharedAllProducts].elementTypeArray objectAtIndex:section];
	NSArray *aTypedArr = [[AllProducts sharedAllProducts] elementsWithType:aType];
	return [aTypedArr count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return [[AllProducts sharedAllProducts].elementTypeArray objectAtIndex:section];
}


@end
