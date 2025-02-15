/*============================================================================*
 * (C) 2001-2010 G.Ishiwata, All Rights Reserved.
 *
 *	Project		: IP Messenger for MacOS X
 *	File		: SendMessageView.m
 *	Module		: 送信メッセージ表示View		
 *============================================================================*/
 
#import "SendMessageView.h"
#import "AttachmentServer.h"
#import "SendControl.h"
#import "DebugLog.h"

@implementation SendMessageView

- (id)initWithFrame:(NSRect)frameRect {
	self = [super initWithFrame:frameRect];
	[self setRichText:NO];
	duringDragging = NO;
	// ファイルのドラッグを受け付ける
	if ([AttachmentServer isAvailable]) {
		[self registerForDraggedTypes:[NSArray arrayWithObjects:NSFilenamesPboardType, nil]];
	}
	return self;
}

/*----------------------------------------------------------------------------*
 * ファイルドロップ処理（添付ファイル）
 *----------------------------------------------------------------------------*/
 
- (unsigned int)draggingEntered:(id <NSDraggingInfo>)sender {
	if (![AttachmentServer isAvailable]) {
		return NSDragOperationNone;
	}
	duringDragging = YES;
	[self setNeedsDisplay:YES];
	return NSDragOperationGeneric;
}

- (unsigned int)draggingUpdated:(id <NSDraggingInfo>)sender {
	if (![AttachmentServer isAvailable]) {
		return NSDragOperationNone;
	}
	return NSDragOperationGeneric;
}

- (void)draggingExited:(id <NSDraggingInfo>)sender {
	if (![AttachmentServer isAvailable]) {
		return;
	}
	duringDragging = NO;
	[self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)aRect {
	[super drawRect:aRect];
	if (duringDragging) {
		[[NSColor selectedControlColor] set];
		NSFrameRectWithWidth([self visibleRect], 4.0);
	}
}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender {
	return [AttachmentServer isAvailable];
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {
	return [AttachmentServer isAvailable];
}

- (void)concludeDragOperation:(id <NSDraggingInfo>)sender {
	NSPasteboard* 	pBoard	= [sender draggingPasteboard];
	NSArray*		files	= [pBoard propertyListForType:NSFilenamesPboardType];
	id				control	= [[self window] delegate];
	int				i;
	for (i = 0; i < [files count]; i++) {
		[control appendAttachmentByPath:[files objectAtIndex:i]];
	}
	duringDragging = NO;
	[self setNeedsDisplay:YES];
}

- (void)keyDown:(NSEvent*)theEvent {
	// enterキーは送信に使うので無視（親Viewに中継）する
	if ([theEvent keyCode] == 52) {
		[[self superview] keyDown:theEvent];
	}
	else {
		[super keyDown:theEvent];
	}
}

- (void)insertText:(id)insertString {
	// タブ文字の入力時はフォーカス移動にする
	// （文字の入力にすることによって日本語変換中のタブキーには反応しなくなる）
	if ([insertString isEqualToString:@"\t"]) {
		[[self window] makeFirstResponder:[self nextValidKeyView]];
		return;
	}
	[super insertText:insertString];
}

@end