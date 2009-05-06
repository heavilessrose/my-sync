//
//  FileOPUtils.m
//  FileOP
//
//  Created by wang luke on 5/5/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "FileOPUtils.h"


@implementation FileOPUtils

//@synthesize attributes;
//- (id)init
//{
//	path = [dir stringByAppendingPathComponent:fileName];
//	attributes = [[NSFileManager defaultManager] fileAttributesAtPath:path traverseLink:NO];
//}

+ (NSString *)dataFileDirectory
{
	
}

+ (NSString *)dataFileFullPath
{
	
}

+ (BOOL)write:(NSData *)data asFile:(NSString *)fileName toPath:(NSString *)directory
{
	
	///////////// test
	NSString *home = [[NSString alloc] initWithCString:getenv("HOME")];
	NSLog(@"Home----> %@", home);
	
	NSString *file= @"test.dat";
	NSString *path = [home stringByAppendingPathComponent:file];
	NSLog(@"path ---> %@", path);
	
	
	//////////// test~
	
	int i = 100;
	float f = 98.3333f;
	NSMutableData *writer = [[NSMutableData alloc] init];
	
	[writer appendData:[temp dataUsingEncoding:NSUTF8StringEncoding]];
	[writer appendBytes:&amp;i length:sizeof(i)];
	[writer appendBytes:&amp;f length:sizeof(f)];
	
	[writer writeToFile:path atomically:YES];
	[writer release];
}

+ (BOOL)read:(NSString *)fileName inDirectory:(NSString *)directory to:(NSData *)data
{
	
}

+ (BOOL)exists:(NSString *)fileName inDirectory:(NSString *)directory
{
	
}

+ (NSDate *)modificationDate:(NSString *)fileName inDirectory:(NSString *)directory
{
	//return [self.attributes objectForKey:NSFileModificationDate];
}

+ (NSString *)fileType:(NSString *)fileName inDirectory:(NSString *)directory
{
	
}

@end
