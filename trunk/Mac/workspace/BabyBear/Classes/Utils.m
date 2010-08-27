//
//  Utils.m
//  BabyBear
//
//  Created by luke on 10-8-17.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Utils.h"


@implementation Utils

+ (BOOL)isMultitasking
{
	UIDevice* device = [UIDevice currentDevice];
	BOOL backgroundSupported = NO;
	if ([device respondsToSelector:@selector(isMultitaskingSupported)])
		backgroundSupported = device.multitaskingSupported;
	return backgroundSupported;
}

+ (NSString *)writeData:(NSData *)data toFile:(NSString *)fName type:(FileType)type replaceExists:(BOOL)replace
{
    NSArray	*documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDir = [documentPaths objectAtIndex:0];
    NSString *path = nil;
	NSString *filePath = nil;
	NSError *err = nil;
	switch (type) {
		case FILE_IMG:
			path = [documentsDir stringByAppendingPathComponent:kPath_img];
			break;
		case FILE_OTHER:
			path = documentsDir;
			break;
		default:
			break;
	}
	filePath = [path stringByAppendingPathComponent:fName];
	if (path) {
        if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
            [[NSFileManager defaultManager] createDirectoryAtPath:path attributes:nil];
        }
		
		if ([[NSFileManager defaultManager] fileExistsAtPath:filePath] && !replace) {
			NSLog(@"file existed already!!");
			return nil;
		}
		
		//if(![portrait writeToFile:portraitfile atomically:YES]){
		if(![data writeToFile:filePath options:NSAtomicWrite error:&err]) {
			NSLog(@"write file err: %@", [err description]);
			return nil;
		}
    }
	
    return filePath;
}


@end
