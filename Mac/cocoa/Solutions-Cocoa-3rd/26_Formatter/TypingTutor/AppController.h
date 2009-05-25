//
//  AppController.h
//  TypingTutor
//
//  Created by Aaron Hillegass on 10/22/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class BigLetterView;

@interface AppController : NSObject {
	IBOutlet BigLetterView *inLetterView;
	IBOutlet BigLetterView *outLetterView;
	
	// Data
	NSArray *letters;
	int lastIndex;
	
	// Speed
	NSTimer *timer;
	IBOutlet NSWindow *speedSheet;
	int stepSize;
	int count;
}
- (void)showAnotherLetter;
- (IBAction)stopGo:(id)sender;
- (IBAction)showSpeedSheet:(id)sender;
- (IBAction)endSpeedSheet:(id)sender;

@end
