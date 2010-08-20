//
//  BaseProduct.m
//  BabyBear
//
//  Created by luke on 10-8-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import "BaseProduct.h"


@implementation BaseProduct

@synthesize pid, pname, pprice, pallRating, pdesc, pUrlIcon, pgallary, 
/*pUrlPhoto,*/ previews, /*preview,*/ pstores; /*pstore,*/

@synthesize iconLoaded, gallaryLoaded, iconLocalPath, productIcon;

- (id)init
{
	if (self = [super init]) {
		self.pgallary	= [NSMutableArray array];
		self.previews	= [NSMutableArray array];
		self.pstores	= [NSMutableArray array];
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
	[pgallary release];
	[previews release];
	[pstores release];
	
	[super dealloc];
}


@end
