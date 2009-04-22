//
//  PracticeController.m
//  practice
//
//  Created by wang luke on 4/22/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "PracticeController.h"


@implementation PracticeController

- (id)init
{
	if(![super init])
		return nil;
	result = [[NSTextField alloc] init];
	[inputField setDelegate:self];
	
	return self;
}

- (void)dealloc
{
	//[sth release]; or self.sth = nil;
	[super dealloc];
}


///// delegate
- (void)textDidChange:(NSNotification *)aNotification
{
	[result setStringValue:[inputField 
}
@end
