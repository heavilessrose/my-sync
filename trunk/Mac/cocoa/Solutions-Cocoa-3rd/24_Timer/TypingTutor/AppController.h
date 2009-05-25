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
	int count;
	NSTimer *timer;
	NSArray *letters;
	int lastIndex;
}
- (void)showAnotherLetter;
- (IBAction)stopGo:(id)sender;
@end
