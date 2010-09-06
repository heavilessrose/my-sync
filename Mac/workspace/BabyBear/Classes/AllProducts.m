//
//  AllProducts.m
//  BabyBear
//
//  Created by luke on 10-9-2.
//  Copyright 2010 Luke. All rights reserved.
//

#import "AllProducts.h"
#import "BaseProduct.h"

@interface AllProducts ()


- (void)setupElementsArray;
- (NSArray *)elementsWithType:(NSString *)aKey;
@end



@implementation AllProducts

@synthesize elementTypeArray, typeNameIndexArray;
@synthesize allProductsByName, allProductsByType, allProductsByPrice;

// we use the singleton approach, one collection for the entire application
static AllProducts *sharedAllProductsInstance = nil;

+ (AllProducts *)sharedAllProducts
{
    @synchronized(self) {
        if (sharedAllProductsInstance == nil) {
            [[self alloc] init]; // assignment not done here
        }
    }
    return sharedAllProductsInstance;
	// note: Xcode (3.2) static analyzer will report this singleton as a false positive
	// '(Potential leak of an object allocated')
}

+ (id)allocWithZone:(NSZone *)zone
{
    @synchronized(self) {
        if (sharedAllProductsInstance == nil) {
            sharedAllProductsInstance = [super allocWithZone:zone];
            return sharedAllProductsInstance;  // assignment and return on first allocation
        }
    }
    return nil; //on subsequent allocation attempts return nil
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain
{
    return self;
}

- (unsigned)retainCount
{
    return UINT_MAX;  // denotes an object that cannot be released
}

- (void)release
{
    //do nothing
}

- (id)autorelease
{
    return self;
}

// setup the data collection
- init
{
	if (self = [super init]) {
		[self setupElementsArray];
	}
	return self;
}

- (void)dealloc
{
	[elementTypeArray release];
	[allProductsByName release];
	[allProductsByType release];
	[allProductsByPrice release];
	
	[super dealloc];
}

#pragma mark -
#pragma mark func


- (void)setupElementsArray
{
	self.allProductsByName = [NSMutableDictionary dictionary];
	self.allProductsByPrice = [NSMutableDictionary dictionary];
	
	self.elementTypeArray = [NSArray arrayWithObjects:type_home, type_office, type_travel, 
							 type_clothing, type_gift, type_tuan, type_other, nil];
	self.allProductsByType = [[NSMutableDictionary alloc] init];
	for (int i = 0; i < [elementTypeArray count]; i++) {
		[allProductsByType setObject:[NSMutableArray array] forKey:[NSNumber numberWithInt:i]];
	}
	[allProductsByType release];
}

// return an array of elements for an type (ie home, travel, gift, ...)
- (NSArray *)elementsWithType:(NSString *)aKey
{
	int aTypeIndex = Ptype_Other;
	for (NSString *typeName in elementTypeArray) {
		if ([typeName isEqualToString:aKey]) {
			aTypeIndex = [elementTypeArray indexOfObject:typeName];
		}
	}
	NSArray *typedArr = [allProductsByType objectForKey:[NSNumber numberWithInt:aTypeIndex]];
	return typedArr;
}

@end
