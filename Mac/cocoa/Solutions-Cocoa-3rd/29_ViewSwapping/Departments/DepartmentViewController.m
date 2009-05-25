//
//  DepartmentViewController.m
//  Departments
//
//  Created by Aaron Hillegass on 10/15/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "DepartmentViewController.h"


@implementation DepartmentViewController

- (id)init
{
	if (![super initWithNibName:@"DepartmentView"
						 bundle:nil]) 
		return nil;
	[self setTitle:@"Departments"];
	return self;
}

@end
