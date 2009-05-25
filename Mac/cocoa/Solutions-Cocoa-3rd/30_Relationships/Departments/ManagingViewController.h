//
//  ManagingViewController.h
//  Departments
//
//  Created by Aaron Hillegass on 10/24/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ManagingViewController : NSViewController {
	NSManagedObjectContext *managedObjectContext;
}
@property (assign) NSManagedObjectContext *managedObjectContext;

@end
