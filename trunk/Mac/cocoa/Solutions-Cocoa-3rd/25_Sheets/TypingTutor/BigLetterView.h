//
//  BigLetterView.h
//  TypingTutor
//
//  Created by Aaron Hillegass on 10/19/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface BigLetterView : NSView {
	NSColor *bgColor;
	NSString *string;
	NSMutableDictionary *attributes;
	NSEvent *mouseDownEvent;
	BOOL highlighted;
}

@property (assign, readwrite) NSColor *bgColor;
@property (copy, readwrite) NSString *string;
- (IBAction)savePDF:(id)sender;
- (IBAction)cut:(id)sender;
- (IBAction)copy:(id)sender;
- (IBAction)paste:(id)sender;
@end
