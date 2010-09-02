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
	
	NSArray				*elementTypeIndexArray;
	NSMutableDictionary *typeIndexesDict;		// unique type characters (for the type index table)
}

@property (nonatomic, retain) NSMutableDictionary *allProductsByName;	
@property (nonatomic, retain) NSMutableDictionary *allProductsByType;
@property (nonatomic, retain) NSMutableDictionary *allProductsByPrice;

@end
