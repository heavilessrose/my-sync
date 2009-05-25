//
//  AppController.m
//  SpeakLine
//
//  Created by Aaron Hillegass on 9/13/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "AppController.h"


@implementation AppController

- (id)init
{
	[super init];
	NSLog(@"init");
	speechSynth = [[NSSpeechSynthesizer alloc] initWithVoice:nil];
	[speechSynth setDelegate:self];
	
	voiceList = [[NSSpeechSynthesizer availableVoices] retain];
	
	return self;
}

- (void)awakeFromNib
{
	NSString *defaultVoice = [NSSpeechSynthesizer defaultVoice];
	int defaultRow = [voiceList indexOfObject:defaultVoice];
	[tableView selectRow:defaultRow byExtendingSelection:NO];
	[tableView scrollRowToVisible:defaultRow];
}

#pragma mark Action Methods

- (IBAction)sayIt:(id)sender
{
	NSString *string = [textField stringValue];
	
	// Is 'string' zero-length?
	if ([string length] == 0) {
		return;
	}
	
	[speechSynth startSpeakingString:string];
	NSLog(@"Have started to say: %@", string);
	[stopButton setEnabled:YES];
	[startButton setEnabled:NO];
}
- (IBAction)stopIt:(id)sender
{
	NSLog(@"stopping");
	[speechSynth stopSpeaking];
}

#pragma mark Delegate Methods

- (BOOL)selectionShouldChangeInTableView:(NSTableView *)tv
{
	if ([speechSynth isSpeaking]) {
		NSBeep();
		return NO;
	} else {
		return YES;
	}
}

- (void)tableViewSelectionDidChange:(NSNotification *)note
{
	int row = [tableView selectedRow];
	
	// Is the selection empty?
	if (row == -1) {
		return;
	}
	
	NSString *selectedVoice = [voiceList objectAtIndex:row];
	[speechSynth setVoice:selectedVoice];
	NSLog(@"new voice = %@", selectedVoice);
}

- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender
        didFinishSpeaking:(BOOL)finishedNormally
{
	NSLog(@"didFinish:%d", finishedNormally);
	[stopButton setEnabled:NO];
	[startButton setEnabled:YES];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
	return YES;
}

#pragma mark TableView datasource methods

- (int)numberOfRowsInTableView:(NSTableView *)tv
{
	return [voiceList count];
}

- (id)tableView:(NSTableView *)tv
        objectValueForTableColumn:(NSTableColumn *)tc
                              row:(int)row
{
	NSString *voice = [voiceList objectAtIndex:row];
	NSDictionary *attribs = [NSSpeechSynthesizer attributesForVoice:voice];
	return [attribs objectForKey:NSVoiceName];
}


@end
