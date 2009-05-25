//
//  CarArrayController.m
//  CarLot
//
//  Created by Aaron Hillegass on 9/13/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "CarArrayController.h"


@implementation CarArrayController

- (id)newObject
{
	id newCar = [super newObject];
	NSDate *d = [NSDate date];
	[newCar setValue:d forKey:@"datePurchased"];
	return newCar;
}
@end
