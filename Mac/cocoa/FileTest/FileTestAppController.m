//
//  FileTestAppController.m
//  FileTest
//
//  Created by wang luke on 5/4/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "FileTestAppController.h"


@implementation FileTestAppController

- (IBAction)HFSFileTypeTest:(id)sender
{
	OSType aFileType = [[fileTypeField stringValue] UTF8String];
	[fileTypeCodeField setStringValue:[[NSString alloc] initWithFormat:@"%d",aFileType]];
}

#pragma mark -
#pragma mark 初始化
- (id)init
{
	if(![super init])
		return nil;
	
	return self;
}
@end
