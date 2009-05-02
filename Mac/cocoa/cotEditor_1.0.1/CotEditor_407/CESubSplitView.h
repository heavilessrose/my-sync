/*
=================================================
CESubSplitView
(for CotEditor)

Copyright (C) 2004-2007 nakamuxu.
http://www.aynimac.com/
=================================================

encoding="UTF-8"
Created:2006.03.18

-------------------------------------------------

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. 


=================================================
*/

#import <Cocoa/Cocoa.h>
#import "CEEditorView.h"
#import "CELineNumView.h"
#import "CENavigationBarView.h"
#import "CETextViewCore.h"
#import "CESyntax.h"
#import "constants.h"


@interface CESubSplitView : NSView
{
    CEEditorView *_editorView;
    NSTextStorage *_textStorage;
    NSScrollView *_scrollView;
    CETextViewCore *_textViewCore;
    CELineNumView *_lineNumView;
    CENavigationBarView *_navigationBar;
    CESyntax *_syntax;
    NSDictionary *_highlightBracesColorDict;
    NSTimer *_lineNumUpdateTimer;
    NSTimer *_outlineMenuTimer;
    NSTimeInterval _lineNumUpdateInterval;
    NSTimeInterval _outlineMenuInterval;
    NSRange _hilightedLineRange;
    NSRect _hilightedLineRect;

    int _lastCursorLocation;
    BOOL _highlightCurrentLine;
    BOOL _setHiliteLineColorToIMChars;
    BOOL _hadMarkedText;
}

// Public method
- (void)releaseEditorView;
- (CEEditorView *)editorView;
- (void)setEditorView:(CEEditorView *)inEditorView;
- (NSScrollView *)scrollView;
- (CETextViewCore *)textView;
- (CELineNumView *)lineNumView;
- (CENavigationBarView *)navigationBar;
- (CESyntax *)syntax;
- (NSString *)string;
- (void)viewDidEndLiveResize;
- (void)replaceTextStorage:(NSTextStorage *)inTextStorage;
- (BOOL)isWritable;
- (BOOL)isAlertedNotWritable;
- (void)setTextViewToEditorView:(CETextViewCore *)inTextView;
- (void)setShowLineNumWithNumber:(NSNumber *)inNumber;
- (void)setShowNavigationBarWithNumber:(NSNumber *)inNumber;
- (void)setWrapLinesWithNumber:(NSNumber *)inNumber;
- (void)setShowInvisiblesWithNumber:(NSNumber *)inNumber;
- (void)setUseAntialiasWithNumber:(NSNumber *)inNumber;
- (BOOL)showPageGuide;
- (void)setCaretToBeginning;
- (void)setSyntaxStyleNameToSyntax:(NSString *)inName;
- (void)recoloringAllTextViewString;
- (void)updateOutlineMenu;
- (void)updateOutlineMenuSelection;
- (void)stopUpdateLineNumberTimer;
- (void)stopUpdateOutlineMenuTimer;
- (NSCharacterSet *)completionsFirstLetterSet;
- (NSDictionary *)highlightBracesColorDict;
- (void)setBackgroundColorAlphaWithNumber:(NSNumber *)inNumber;

@end
