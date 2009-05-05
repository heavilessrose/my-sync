//
//  FileTestAppController.h
//  FileTest
//
//  Created by wang luke on 5/4/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface FileTestAppController : NSObject {
	IBOutlet NSTextField *fileTypeField;
	IBOutlet NSTextField *fileTypeCodeField;
	IBOutlet NSButton *showFileType;
}

- (IBAction)HFSFileTypeTest:(id)sender;
@end
