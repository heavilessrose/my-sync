//
//  AllProducts.h
//  BabyBear
//
//  Created by luke on 10-9-2.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface AllProducts : NSObject {
	NSMutableDictionary *allProductsByName;		// key: name
	NSMutableDictionary *allProductsByType;		// key: type
	NSMutableDictionary *allProductsByPrice;	// key: price
	
	NSArray				*elementTypeArray;		// type name string array indexed by "Ptype"
	NSArray				*typeNameIndexArray;
	NSMutableDictionary *typeIndexesDict;		// unique type characters (for the type index table)
	
	NSInteger			allProductsCount;
}

@property (nonatomic, retain) NSArray				*typeNameIndexArray;
@property (nonatomic, retain) NSArray				*elementTypeArray;
@property (nonatomic, retain) NSMutableDictionary	*allProductsByName;	
@property (nonatomic, retain) NSMutableDictionary	*allProductsByType;
@property (nonatomic, retain) NSMutableDictionary	*allProductsByPrice;

@property (nonatomic, assign) NSInteger				allProductsCount;

+ (AllProducts *)sharedAllProducts;
// return an array of elements for an type (ie home, travel, gift, ...)
- (NSArray *)elementsWithType:(NSString *)aKey;
@end
