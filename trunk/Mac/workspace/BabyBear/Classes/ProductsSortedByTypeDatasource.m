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
#import "ProductsViewController.h"


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
	NSString *aTypeName = [[AllProducts sharedAllProducts].elementTypeArray objectAtIndex:indexPath.section];
	NSArray *aProductArr = [[AllProducts sharedAllProducts] elementsWithType:aTypeName];
	return [aProductArr objectAtIndex:indexPath.row];
}

- (NSInteger)allDatasCount
{
	return [AllProducts sharedAllProducts].allProductsCount;
}

#pragma mark -
#pragma mark table data

@synthesize controller;

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return [[AllProducts sharedAllProducts].elementTypeArray count];
}

- (NSInteger)tableView:(UITableView *)tableView  numberOfRowsInSection:(NSInteger)section
{
	NSString *aType = [[AllProducts sharedAllProducts].elementTypeArray objectAtIndex:section];
	NSArray *aTypedArr = [[AllProducts sharedAllProducts] elementsWithType:aType];
	return [aTypedArr count];
}

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
		cell.accessoryType = UITableViewCellAccessoryNone; //UITableViewCellAccessoryDisclosureIndicator;
	}
	cell.product = (Product *)theProduct;
	
	// Only load cached images; delay new downloads until scrolling ends
	if (!theProduct.productIcon) {
		if (((ProductsViewController *)controller).tableView.dragging == NO && ((ProductsViewController *)controller).tableView.decelerating == NO) {
			[(ProductsViewController *)controller startImgDownload:theProduct forIndexPath:indexPath];
		}
		// if a download is deferred or in progress, return a placeholder image
		cell.imageView.image = [UIImage imageNamed:@"Placeholder.png"];                
	}
	else {
		cell.imageView.image = theProduct.productIcon;
	}
	[cell refresh];
	return cell;
}


#pragma mark table sections

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	NSString *aType = [[AllProducts sharedAllProducts].elementTypeArray objectAtIndex:section];
	return aType;
}

- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView
{
	NSArray *types = [AllProducts sharedAllProducts].elementTypeArray;
	NSLog(@"types: %@", types);
	return types;
}

- (NSInteger)tableView:(UITableView *)tableView sectionForSectionIndexTitle:(NSString *)title
			   atIndex:(NSInteger)index
{
	if (index == 0) {
		// search item
		[tableView scrollRectToVisible:[[tableView tableHeaderView] bounds] animated:NO];
		return -1;
	}
	return index;
}

@end
