//
//  PreferenceController.m
//  RaiseMan
//
//  Created by Aaron Hillegass on 10/15/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "PreferenceController.h"

NSString *BNRTableBgColorKey = @"TableBackgroundColor";
NSString *BNREmptyDocKey = @"EmptyDocumentFlag";
NSString *BNRColorChangedNotification = @"BNRColorChanged";


@implementation PreferenceController

- (id)init
{
	if (![super initWithWindowNibName:@"Preferences"]) 
		return nil;
	
	return self;
}
- (NSColor *)tableBgColor
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSData *colorAsData = [defaults objectForKey:BNRTableBgColorKey];
	return [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData];
}
- (BOOL)emptyDoc
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	return [defaults boolForKey:BNREmptyDocKey];
}

- (void)windowDidLoad
{
	[colorWell setColor:[self tableBgColor]];
	[checkbox setState:[self emptyDoc]];
}

- (IBAction)changeBackgroundColor:(id)sender
{
	NSColor *color = [colorWell color];
	NSData *colorAsData;
	colorAsData = [NSKeyedArchiver archivedDataWithRootObject:color];
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	[defaults setObject:colorAsData forKey:BNRTableBgColorKey];
	
	NSLog(@"sending color changed notification");
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];	
	NSDictionary *d = [NSDictionary dictionaryWithObject:color
												  forKey:@"color"];
	[nc postNotificationName:BNRColorChangedNotification 
					  object:self
					userInfo:d];
}

- (IBAction)changeNewEmptyDoc:(id)sender
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	[defaults setBool:[checkbox state] forKey:BNREmptyDocKey];}

@end
