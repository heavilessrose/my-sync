//
//  Utils.m
//  Picoffee
//
//  Created by luke on 10-9-9.
//  Copyright 2010 Luke.cn. All rights reserved.
//

#import "Utils.h"


@implementation Utils


- (void)diskSpace
{
	// Get access to a file manager as our means
	// to perform file operations
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	// Using the application home directory, get dictionary of attributes
	NSDictionary *attributesDict = [fileManager attributesOfFileSystemForPath:NSHomeDirectory() error:NULL];
	
	// Print total file system size and available space  
	NSLog(@"File system size: %lld", [[attributesDict objectForKey:NSFileSystemSize] longLongValue]);
	NSLog(@"File system free space: %lld", [[attributesDict objectForKey:NSFileSystemFreeSize] longLongValue]);
	
}
@end
