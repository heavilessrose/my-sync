//
//  FileTestAppController.m
//  FileTest
//
//  Created by wang luke on 5/4/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "FileTestAppController.h"

/*******************************************************************
 需要区分HFS file type， creator codes，和文件后缀的概念
 *******************************************************************/
@implementation FileTestAppController

#pragma mark -
#pragma mark 初始化
- (id)init
{
	if(![super init])
		return nil;
	
	return self;
}

#pragma mark -
#pragma mark UI按键响应
- (IBAction)HFSFileTypeTest:(id)sender
{
	OSType aFileType = 'TEXT';
	
	NSNumber *AFileType = [NSNumber numberWithUnsignedLong:'TEXT'];
	NSLog(@"%@", [AFileType stringValue]);
	
	NSString *afileTypeStr = [[NSString alloc] initWithFormat:@"%d",aFileType];
	[fileTypeCodeField setStringValue:afileTypeStr];
	[afileTypeStr release];
	
	printf("%d", FileValid(@"Untitled.txt"));
}

#pragma mark -
#pragma mark 实用方法
// 文件是否匹配给定的文件类型(根据HFS file type code 或后缀)
BOOL FileValid(NSString *fullFilePath)
{
    // Create an array of strings specifying valid extensions and HFS file types.
    NSArray *fileTypes = [NSArray arrayWithObjects:
						  @"txt",
						  @"text",
						  NSFileTypeForHFSTypeCode('TEXT'), // Returns a string encoding a file type code.
						  nil];
	
    // Try to get the HFS file type as a string.
    NSString *fileType = NSHFSTypeOfFile(fullFilePath);
    if ([fileType isEqualToString:@"''"] || fileType == nil)
    {
        // No HFS type; get the extension instead.
        fileType = [fullFilePath pathExtension];
    }
	
    return [fileTypes containsObject:fileType];
}
@end
