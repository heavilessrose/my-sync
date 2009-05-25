//
//  AppController.m
//  ImageFun
//
//  Created by Aaron Hillegass on 10/18/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "AppController.h"
#import "StretchView.h"

@implementation AppController

- (void)openPanelDidEnd:(NSOpenPanel *)op
			 returnCode:(int)returnCode
			contextInfo:(void *)ci
{
	if (returnCode == NSOKButton) {
		NSString *path = [op filename];
		NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
		[stretchView setImage:image];
	}
}

- (IBAction)showOpenPanel:(id)sender
{
	NSOpenPanel *op = [NSOpenPanel openPanel];
	
	[op beginSheetForDirectory:nil
						  file:nil
						 types:[NSImage imageFileTypes]
				modalForWindow:[stretchView window]
				 modalDelegate:self
				didEndSelector:@selector(openPanelDidEnd:returnCode:contextInfo:)
				   contextInfo:nil];
	
}
@end
