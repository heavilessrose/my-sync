//
//  EmployeeViewController.m
//  Departments
//
//  Created by Aaron Hillegass on 10/15/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "EmployeeViewController.h"


@implementation EmployeeViewController
- (id)init
{
	if (![super initWithNibName:@"EmployeeView"
						 bundle:nil]) 
		return nil;
	[self setTitle:@"Employees"];
	return self;
}

@end
