//
//  BaseProduct.m
//  BabyBear
//
//  Created by luke on 10-8-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import "BaseProduct.h"



@implementation ProductImageUrlWithIndex
@synthesize url;

- (id)initWithUrl:(NSString *)aUrl andIndex:(NSInteger)aIndex
{
	if (self = [super init]) {
		self.url = aUrl;
		index = aIndex;
	}
	return self;
}

- (void)dealloc
{
	[url release];
	[super dealloc];
}

@end



@implementation BaseProduct

@synthesize pid, pname, pprice, pallRating, pdesc, pUrlIcon, pgallary, 
/*pUrlPhoto,*/ previews, /*preview,*/ pstores; /*pstore,*/

@synthesize iconLoaded, gallaryLoaded, iconLocalPath, productIcon, productImgs;

- (id)init
{
	if (self = [super init]) {
		self.pgallary	= [NSMutableArray array];
		self.previews	= [NSMutableArray array];
		self.pstores	= [NSMutableArray array];
		self.productImgs = [NSMutableArray array];
	}
	
	return self;
}


- (void)dealloc
{
	[pid release];
	[pname release];
	[pprice release];
	[pallRating release];
	[pdesc release];
	[pUrlIcon release];
	[productImgs release];
	[pgallary release];
	[previews release];
	[pstores release];
	
	[super dealloc];
}

- (NSString *)description
{
	return [NSString stringWithFormat:@"pid: %@, pname: %@, pprice: %@, pallRating: %@", pid, pname, pprice, pallRating];
}

@end
