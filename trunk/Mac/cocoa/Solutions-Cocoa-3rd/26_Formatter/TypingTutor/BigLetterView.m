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
	string = @"";
	[self registerForDraggedTypes:[NSArray arrayWithObject:NSStringPboardType]];
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
	// Is it an empty string?
	if ([string length] == 0) {
		return;
	}
	
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
	if (highlighted) {
		[[NSColor whiteColor] set];
	} else {
		[bgColor set];
	}
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
	[self setString:@""];
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
	[self setString:@""];
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
#pragma mark Dragging Source

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)isLocal
{
	return NSDragOperationCopy | NSDragOperationDelete;
}

- (void)mouseDown:(NSEvent *)event
{
	[event retain];
	[mouseDownEvent release];
	mouseDownEvent = event;
}

- (void)mouseDragged:(NSEvent *)event
{
	NSPoint down = [mouseDownEvent locationInWindow];
	NSPoint drag = [event locationInWindow];
	float distance = hypot(down.x - drag.x, down.y - drag.y);
	if (distance < 5) {
		NSLog(@"Not far enough: %f", distance);
		return;
	}
	
	if ([string length] == 0) {
		return;
	}
	
	NSSize s = [string sizeWithAttributes:attributes];
	NSRect imageBounds;
	imageBounds.origin = NSZeroPoint;
	imageBounds.size = s;
	
	NSImage *anImage = [[NSImage alloc] initWithSize:s];

	[anImage lockFocus];
	[self drawStringCenteredIn:imageBounds];
	[anImage unlockFocus];
	
	NSPoint p = [self convertPoint:down fromView:nil]; 
	p.x = p.x - s.width / 2.0;
	p.y = p.y - s.height / 2.0;
	
	NSPasteboard *pb = [NSPasteboard pasteboardWithName:NSDragPboard];
	[self writeToPasteboard:pb];
	[self dragImage:anImage
				 at:p
			 offset:NSMakeSize(0,0)
			  event:mouseDownEvent
		 pasteboard:pb
			 source:self
		  slideBack:YES];
	[anImage release];
}

- (void)draggedImage:(NSImage *)image 
			 endedAt:(NSPoint)screenPoint 
		   operation:(NSDragOperation)operation
{
	if (operation == NSDragOperationDelete) {
		[self setString:@""];
	}
}
#pragma mark Dragging Destination


- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>) sender
{
	if ([sender draggingSource] == self) {
		return NSDragOperationNone;
	}
	
	highlighted = YES;
	[self setNeedsDisplay:YES];
	return NSDragOperationCopy;
}

- (NSDragOperation)draggingUpdated:(id <NSDraggingInfo>)sender
{
	NSLog(@"draggingUpdated: %d", [sender draggingSourceOperationMask]);
	return NSDragOperationCopy;
}
- (void)draggingExited:(id <NSDraggingInfo>)sender
{
	NSLog(@"draggingExited");
	highlighted = NO;
	[self setNeedsDisplay:YES];}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender
{
	return YES;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
	NSPasteboard *pb = [sender draggingPasteboard];
	if (![self readFromPasteboard:pb]) {
		NSLog(@"Unable to read from pasteboard");
		return NO;
	}
	return YES;
}
- (void)concludeDragOperation:(id <NSDraggingInfo>)sender
{
	highlighted = NO;
	[self setNeedsDisplay:YES];
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
