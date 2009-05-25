//
//  PreferenceController.m
//  RaiseMan
//
//  Created by Aaron Hillegass on 10/15/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "PreferenceController.h"


@implementation PreferenceController

- (id)init
{
	if (![super initWithWindowNibName:@"Preferences"]) 
		return nil;
	
	return self;
}

- (void)windowDidLoad
{
	NSLog(@"nib file loaded");
}

- (IBAction)changeBackgroundColor:(id)sender
{
	NSLog(@"Color changed: %@", [colorWell color]);
}

- (IBAction)changeNewEmptyDoc:(id)sender
{
	NSLog(@"Checkbox changed: %d", [checkbox state]);
}

@end
