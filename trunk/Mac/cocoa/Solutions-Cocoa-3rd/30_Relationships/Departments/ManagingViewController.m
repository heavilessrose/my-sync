//
//  ManagingViewController.m
//  Departments
//
//  Created by Aaron Hillegass on 10/24/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "ManagingViewController.h"


@implementation ManagingViewController

- (void)setManagedObjectContext:(NSManagedObjectContext *)moc
{
	[moc retain];
	[managedObjectContext release];
	managedObjectContext = moc;
}

- (NSManagedObjectContext *)managedObjectContext
{
	return managedObjectContext;
}

- (void)dealloc
{
	[managedObjectContext release];
	[super dealloc];
}

@end
