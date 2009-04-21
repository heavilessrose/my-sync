//
//  AppController.h
//  SpeakLine
//
//  Created by wang luke on 4/20/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Cocoa/Cocoa.h>


/*!
    @class       AppController
    @superclass  NSObject
    @abstract    两个按钮的target.
    @discussion  每个按钮都会触发一个不同的action方法
*/
@interface AppController : NSObject {
	IBOutlet NSTextField *textField;
	IBOutlet NSTextField *label;
	IBOutlet NSTableView *tableView;
	
	NSArray *voiceList;
	
	IBOutlet NSButton *startButton;
	IBOutlet NSButton *stopButton;
	NSSpeechSynthesizer *speechSynth;
}

- (IBAction)sayIt:(id)sender;
- (IBAction)stopIt:(id)sender;
- (IBAction)counting:(id)sender;

@end
