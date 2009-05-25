//
//  Employee.h
//  Departments
//
//  Created by Aaron Hillegass on 10/24/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>
@class Department;

@interface Employee :  NSManagedObject  
{
}

@property (retain) NSString *firstName;
@property (retain) NSString *lastName;
@property (retain) Department *department;
@property (readonly) NSString *fullName;
@end


