//
//  BigLetterView.m
//  TypingTutor
//
//  Created by Aaron Hillegass on 10/19/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "BigLetterView.h"
#import "FirstLetter.h"

@implementation BigLetterView
- (void)prepareAttributes
{
	attributes = [[NSMutableDictionary alloc] init];
	[attributes setObject:[NSFont fontWithName:@"Helvetica"
										  size:75]
				   forKey:NSFontAttributeName];
	
	[attributes setObject:[NSColor redColor]
				   forKey:NSForegroundColorAttributeName];
	 
}
- (id)initWithFrame:(NSRect)frame {
    [super initWithFrame:frame];
	NSLog(@"initWithFrame:");
	[self prepareAttributes];
	bgColor = [[NSColor yellowColor] retain];
	string = @" ";
	return self;
}
- (void)dealloc
{
	[bgColor release];
	[string release];
	[attributes release];
	[super dealloc];
}

#pragma mark Drawing

- (BOOL)isOpaque
{
	return YES;
}

- (void)drawStringCenteredIn:(NSRect)r
{
	NSSize strSize = [string sizeWithAttributes:attributes];
	NSPoint strOrigin;
	strOrigin.x = r.origin.x + (r.size.width - strSize.width) / 2;
	strOrigin.y = r.origin.y + (r.size.height - strSize.height) /2;
	[string drawAtPoint:strOrigin
		 withAttributes:attributes];
}
- (void)drawRect:(NSRect)rect 
{
    NSRect bounds = [self bounds];
	[bgColor set];
	[NSBezierPath fillRect:bounds];

	[self drawStringCenteredIn:bounds];
	if (([[self window] firstResponder] == self) &&
		([NSGraphicsContext currentContextDrawingToScreen])){
		[NSGraphicsContext saveGraphicsState];
		NSSetFocusRingStyle(NSFocusRingOnly);
		[NSBezierPath fillRect:bounds];
		[NSGraphicsContext restoreGraphicsState];
	}
}

#pragma mark Keyboard Events

- (BOOL)acceptsFirstResponder
{
	NSLog(@"Accepting");
	return YES;
}

- (BOOL)resignFirstResponder
{
	NSLog(@"Resigning");
	[self setNeedsDisplay:YES];
	return YES;
}

- (BOOL)becomeFirstResponder
{
	NSLog(@"Becoming");
	[self setNeedsDisplay:YES];
	return YES;
}

- (void)keyDown:(NSEvent *)e
{
	[self interpretKeyEvents:[NSArray arrayWithObject:e]];
}

- (void)insertText:(NSString *)s
{
	[self setString:s];
}

- (void)insertTab:(id)sender
{
	[[self window] selectNextKeyView:nil];
}

- (void)insertBacktab:(id)sender
{
	[[self window] selectPreviousKeyView:nil];
}

- (void)deleteBackward:(id)sender
{
	[self setString:@" "];
}

#pragma mark Pasteboard

- (void)writeToPasteboard:(NSPasteboard *)pb
{
	[pb declareTypes:[NSArray arrayWithObject:NSStringPboardType]
			   owner:self];
	[pb setString:string forType:NSStringPboardType];
}

- (BOOL)readFromPasteboard:(NSPasteboard *)pb
{
	NSArray *types = [pb types];
	if ([types containsObject:NSStringPboardType]) {
		NSString *value = [pb stringForType:NSStringPboardType];
		[self setString:[value BNR_firstLetter]];
		return YES;
	}
	return NO;
}

- (IBAction)cut:(id)sender
{
	[self copy:nil];
	[self setString:@" "];
}

- (IBAction)copy:(id)sender
{
	NSPasteboard *pb = [NSPasteboard generalPasteboard];
	[self writeToPasteboard:pb];
}

- (IBAction)paste:(id)sender
{
	NSPasteboard *pb = [NSPasteboard generalPasteboard];
	if (![self readFromPasteboard:pb]){
		NSBeep();
	}
}
#pragma mark Saving as PDF

- (IBAction)savePDF:(id)sender
{
	NSSavePanel *panel = [NSSavePanel savePanel];
	[panel setRequiredFileType:@"pdf"];
	[panel beginSheetForDirectory:nil
							 file:nil
				   modalForWindow:[self window]
					modalDelegate:self
				   didEndSelector:@selector(didEnd:returnCode:contextInfo:)
					  contextInfo:nil];
}

- (void)didEnd:(NSSavePanel *)sheet
	returnCode:(int)code
   contextInfo:(void *)context
{
	if (code != NSOKButton) 
		return;
	
	NSRect r = [self bounds];
	NSData *data = [self dataWithPDFInsideRect:r];
	NSString *path = [sheet filename];
	NSError *error;
	BOOL successful = [data writeToFile:path options:YES error:&error];
	if (!successful) {
		NSAlert *a = [NSAlert alertWithError:error];
		[a runModal];
	}
}

#pragma mark Accessors

- (NSColor *)bgColor
{
	return bgColor;
}

- (void)setBgColor:(NSColor *)c
{
	[c retain];
	[bgColor release];
	bgColor = c;
	[self setNeedsDisplay:YES];
}

- (NSString *)string
{
	return string;
}

- (void)setString:(NSString *)s
{
	s = [s copy];
	[string release];
	string = s;
	NSLog(@"the string is now %@", string);
	[self setNeedsDisplay:YES];
}

@end
