//
//  AppController.m
//  SpeakLine
//
//  Created by wang luke on 4/20/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "AppController.h"


@implementation AppController

- (void)awakeFromNib
{
    NSString *defaultVoice = [NSSpeechSynthesizer defaultVoice];
    int defaultRow = [voiceList indexOfObject:defaultVoice];
    [tableView selectRow:defaultRow byExtendingSelection:NO];
    [tableView scrollRowToVisible:defaultRow];
}

- (id)init
{
	[super init];
	
	NSLog(@"init");
	
	speechSynth = [[NSSpeechSynthesizer alloc] initWithVoice:nil];
	
	// 设置speech synthesizer的delegate
	[speechSynth setDelegate:self];
	
	voiceList = [[NSSpeechSynthesizer availableVoices] retain];
	return self;
}


- (IBAction)sayIt:(id)sender
{
	NSString *string = [textField stringValue];
	
	if([string length] == 0){
		NSLog(@"String from %@ is of zero-length", textField);
		return;
	}
	[speechSynth startSpeakingString:string];
	NSLog(@"have started to say: %a", string);
	
	[startButton setEnabled:NO];
	[stopButton setEnabled:YES];
	[tableView setEnabled:NO];
}


- (IBAction)stopIt:(id)sender
{
	NSLog(@"stopping");
	[speechSynth stopSpeaking];
}

- (IBAction)counting:(id)sender
{
	NSString *string = [textField stringValue];
	NSString *ss = [NSString stringWithFormat:@"\"%@\" has %d characters", string, [string length]];
	NSLog(@"string = %@", string);
	NSLog(@"ss = %@", ss);
	[label setStringValue:ss];
}

// ------delegate
// 添加delegate方法
- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender didFinishSpeaking:(BOOL)complete
{
	NSLog(@"complete = %d", complete);
	[startButton setEnabled:YES];
	[stopButton setEnabled:NO];
	[tableView setEnabled:YES];
}

// tableView的dataSouce方法
- (int)numberOfRowsInTableView:(NSTableView *)tv;
{
	return [voiceList count];
}

- (id)tableView:(NSTableView *)tv objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row
{
	NSString *v = [voiceList objectAtIndex:row]; 
	NSDictionary *dict = [NSSpeechSynthesizer attributesForVoice:v];
    return [dict objectForKey:NSVoiceName];
}

- (void)tableViewSelectionDidChange:(NSNotification *)notification
{
    int row = [tableView selectedRow];
    if (row == -1) {
        return;
    }
    NSString *selectedVoice = [voiceList objectAtIndex:row];
    [speechSynth setVoice:selectedVoice];
    NSLog(@"new voice = %@", selectedVoice);
}

@end
