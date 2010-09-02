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

@property (nonatomic, retain) NSArray *elementTypeIndexArray;

- (void)setupElementsArray;
- (NSArray *)elementsWithType:(NSString *)aKey;
@end



@implementation AllProducts

@synthesize allProductsByName, allProductsByType, allProductsByPrice;

// we use the singleton approach, one collection for the entire application
static AllProducts *sharedAllProductsInstance = nil;

+ (AllProducts *)sharedAllProducts {
    @synchronized(self) {
        if (sharedAllProductsInstance == nil) {
            [[self alloc] init]; // assignment not done here
        }
    }
    return sharedAllProductsInstance;
	// note: Xcode (3.2) static analyzer will report this singleton as a false positive
	// '(Potential leak of an object allocated')
}

+ (id)allocWithZone:(NSZone *)zone {
    @synchronized(self) {
        if (sharedAllProductsInstance == nil) {
            sharedAllProductsInstance = [super allocWithZone:zone];
            return sharedAllProductsInstance;  // assignment and return on first allocation
        }
    }
    return nil; //on subsequent allocation attempts return nil
}

- (id)copyWithZone:(NSZone *)zone {
    return self;
}

- (id)retain {
    return self;
}

- (unsigned)retainCount {
    return UINT_MAX;  // denotes an object that cannot be released
}

- (void)release {
    //do nothing
}

- (id)autorelease {
    return self;
}

// setup the data collection
- init {
	if (self = [super init]) {
		[self setupElementsArray];
	}
	return self;
}

- (void)dealloc
{
	[elementTypeIndexArray release];
	[allProductsByName release];
	[allProductsByType release];
	[allProductsByPrice release];
	
	[super dealloc];
}

#pragma mark -
#pragma mark func

@synthesize elementTypeIndexArray;

- (void)setupElementsArray
{
	self.allProductsByName = [NSMutableDictionary dictionary];
	self.allProductsByType = [NSMutableDictionary dictionary];
	self.allProductsByPrice = [NSMutableDictionary dictionary];
}

- (BaseProduct *)productElementForIndexPath:(NSIndexPath *)indexPath
{
	
}

// return an array of elements for an type (ie home, travel, gift, ...)
- (NSArray *)elementsWithType:(NSString *)aKey
{
	NSArray *typedArr = [allProductsByType objectForKey:aKey];
	return typedArr;
}

@end
