//
//  WindowSizerController.m
//  WindowSizer
//
//  Created by wang luke on 4/21/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "WindowSizerController.h"


@implementation WindowSizerController

- (id)init
{
	[super init];
	NSLog(@"init");
	
	window = [[NSWindow alloc] init];
	[window setDelegate:self];
	
	return self;
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
	NSSize newSize;
	newSize.width = 200.0;
	newSize.height = 200.0;
	NSLog(@"my size is 2 times bigger !!");
	return newSize;
}

@end
