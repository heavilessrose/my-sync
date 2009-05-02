/*
=================================================
CEDocument (AdditionalAppleScript)
(for CotEditor)

Copyright (C) 2004-2007 nakamuxu.
http://www.aynimac.com/
=================================================

encoding="UTF-8"
Created:2005.03.12

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

#import "CEDocumentAppleScript.h"

//=======================================================
// Private method
//
//=======================================================

@interface CEDocument (AdditionalAppleScriptPrivate)
- (BOOL)doFind:(NSString *)inSearchString range:(NSRange)inRange 
            option:(unsigned)inMask withRegularExpression:(BOOL)inRE;
@end


//------------------------------------------------------------------------------------------




@implementation CEDocument (AdditionalAppleScript)

#pragma mark ===== Public method =====

//=======================================================
// Public method
//
//=======================================================

// ------------------------------------------------------
- (void)cleanUpTextStorage:(NSTextStorage *)inTextStorage
// 生成した textStorage のデリゲートであることをやめる
// ------------------------------------------------------
{
    [inTextStorage setDelegate:nil];
}



#pragma mark === Delegate and Notification ===

//=======================================================
// Delegate method (NSTextStorage)
//  <== NSTextStorage
//=======================================================

// ------------------------------------------------------
- (void)textStorageDidProcessEditing:(NSNotification *)inNotification
// AppleScriptの返り値としてのtextStorageが更新された
// ------------------------------------------------------
{
    NSString *theNewString = [(NSTextStorage *)[inNotification object] string];

    [[_editorView textView] replaceAllStringTo:theNewString];
    [self cleanUpTextStorage:(NSTextStorage *)[inNotification object]];
}




#pragma mark ===== AppleScript accessor =====

//=======================================================
// AppleScript accessor
//
//=======================================================

// ------------------------------------------------------
- (NSTextStorage *)textStorage
// ドキュメントの文字列を返す(text型)
// ------------------------------------------------------
{
    NSTextStorage *outStorage = [[[NSTextStorage alloc] initWithString:[_editorView stringForSave]] autorelease];

    [outStorage setDelegate:self];
    // 0.5秒後にデリゲートをやめる（放置するとクラッシュの原因になる）
    [self performSelector:@selector(cleanUpTextStorage:) withObject:outStorage afterDelay:0.5];

    return outStorage;
}


// ------------------------------------------------------
- (void)setTextStorage:(id)inObject;
// ドキュメントの文字列をセット（全置換）
// ------------------------------------------------------
{
    if ([inObject isKindOfClass:[NSTextStorage class]]) {
        [[_editorView textView] replaceAllStringTo:[inObject string]];
    }
}


// ------------------------------------------------------
- (NSTextStorage *)contents
// ドキュメントの文字列を返す(text型)
// ------------------------------------------------------
{
    return [self textStorage];
}


// ------------------------------------------------------
- (void)setContents:(id)inObject
// ドキュメントの文字列をセット（全置換）
// ------------------------------------------------------
{
    [self setTextStorage:inObject];
}


// ------------------------------------------------------
- (NSNumber *)length
// ドキュメントの文字数を返す(integer型)
// ------------------------------------------------------
{
    int theLength = [[_editorView stringForSave] length];

    return [NSNumber numberWithInt:theLength];
}


// ------------------------------------------------------
- (CELineEnding)lineEnding
// 行末コードを返す(enum型)
// ------------------------------------------------------
{
    int theCode = [_editorView lineEndingCharacter];
    CELineEnding outLineEnding;

    switch (theCode) {
    case 1:
        outLineEnding = CELineEndingCR;
        break;
    case 2:
        outLineEnding = CELineEndingCRLF;
        break;
    default:
        outLineEnding = CELineEndingLF;
        break;
    }
    return outLineEnding;
}


// ------------------------------------------------------
- (void)setLineEnding:(CELineEnding)inEnding
// 行末コードをセット
// ------------------------------------------------------
{
    int theCode;

    switch (inEnding) {
    case CELineEndingCR:
        theCode = 1;
        break;
    case CELineEndingCRLF:
        theCode = 2;
        break;
    default:
        theCode = 0;
        break;
    }
    [self doSetNewLineEndingCharacterCode:theCode];
}


// ------------------------------------------------------
- (NSString *)encoding
// エンコーディング名を返す(Unicode text型)
// ------------------------------------------------------
{
    return [NSString localizedNameOfStringEncoding:_encoding];
}


// ------------------------------------------------------
- (NSString *)IANACharSetName
// エンコーディング名の IANA Charset 名を返す(Unicode text型)
// ------------------------------------------------------
{
    NSString *outName = [self currentIANACharSetName];

    // 得られなければ空文字を返す
    if (outName == nil) {
        return @"";
    }
    return outName;
}


// ------------------------------------------------------
- (NSString *)coloringStyle
// カラーリングスタイル名を返す(Unicode text型)
// ------------------------------------------------------
{
    return [_editorView syntaxStyleNameToColoring];
}


// ------------------------------------------------------
- (void)setColoringStyle:(NSString *)inStyleName
// カラーリングスタイル名をセット
// ------------------------------------------------------
{
    [self doSetSyntaxStyle:inStyleName];
}


// ------------------------------------------------------
- (CETextSelection *)selection
// selection-object を返す
// ------------------------------------------------------
{
    return _selection;
}


// ------------------------------------------------------
- (void)setSelection:(id)inObject
// 選択範囲へテキストを設定
// ------------------------------------------------------
{
    if ([inObject isKindOfClass:[NSString class]]) {
        [_selection setContents:inObject];
    }
}


// ------------------------------------------------------
- (NSNumber *)alphaOnlyTextView
// 透明度をtextViewだけに適用するかどうかを返す
// ------------------------------------------------------
{
    return [NSNumber numberWithBool:_alphaOnlyTextViewInThisWindow];
}


// ------------------------------------------------------
- (void)setAlphaOnlyTextView:(NSNumber *)inBoolNum
// 透明度をtextViewだけに適用するかどうかを設定
// ------------------------------------------------------
{
    [self setAlphaOnlyTextViewInThisWindow:[inBoolNum boolValue]];
    [self setAlphaToWindowAndTextView];
}


// ------------------------------------------------------
- (NSNumber *)transparency
// ウィンドウまたはtextViewの透明度を返す
// ------------------------------------------------------
{
    float theAlpha = [self alpha];

    return [NSNumber numberWithFloat:theAlpha];
    // AppleScript で値を得てみると、0.50000000113 とかになってしまう **** 2005.03.04
}


// ------------------------------------------------------
- (void)setTransparency:(NSNumber *)inAlpha
// ウィンドウまたはtextViewの透明度を設定
// ------------------------------------------------------
{
    [self setAlpha:[inAlpha floatValue]];
}


// ------------------------------------------------------
- (NSNumber *)wrapLines
// ワードラップの状態を返す
// ------------------------------------------------------
{
    BOOL theBOOL = [_editorView wrapLines];

    return [NSNumber numberWithBool:theBOOL];
}


// ------------------------------------------------------
- (void)setWrapLines:(NSNumber *)inValue
// ワードラップを切り替える
// ------------------------------------------------------
{
    [_editorView setWrapLines:[inValue boolValue]];
}


// ------------------------------------------------------
- (NSNumber *)lineSpacing
// 行間値を返す
// ------------------------------------------------------
{
    float theSpacing = [self lineSpacingInTextView];

    return [NSNumber numberWithFloat:theSpacing];
}


// ------------------------------------------------------
- (void)setLineSpacing:(NSNumber *)inSpacing
// 行間値をセット
// ------------------------------------------------------
{
    [self setCustomLineSpacingToTextView:[inSpacing floatValue]];
}


#pragma mark ===== AppleScript handler =====

//=======================================================
// AppleScript handler
//
//=======================================================

// ------------------------------------------------------
- (NSNumber *)handleConvert:(NSScriptCommand *)inCommand
// エンコーディングを変更し、テキストをコンバートする
// ------------------------------------------------------
{
    NSDictionary *theArg = [inCommand evaluatedArguments];
    NSString *theEncodingName = [theArg valueForKey:@"newEncoding"];
    NSStringEncoding theEncoding = [[NSApp delegate] encodingFromName:theEncodingName];
    BOOL theResult = NO;

    if (theEncoding == NSNotFound) {
        theResult = NO;
    } else if (theEncoding == _encoding) {
        theResult = YES;
    } else {
        NSString *theActionName = [NSString stringWithString:@"TEST"];
        BOOL theLossy = NO;

        theLossy = [[theArg valueForKey:@"Lossy"] boolValue];
        theResult = [self doSetEncoding:theEncoding updateDocument:YES 
                askLossy:NO lossy:theLossy asActionName:theActionName];
    }

    return [NSNumber numberWithBool:theResult];
}


// ------------------------------------------------------
- (NSNumber *)handleReinterpret:(NSScriptCommand *)inCommand
// エンコーディングを変更し、テキストを再解釈する
// ------------------------------------------------------
{
    NSDictionary *theArg = [inCommand evaluatedArguments];
    NSString *theEncodingName = [theArg valueForKey:@"newEncoding"];
    NSString *theFileName = [self fileName];
    NSStringEncoding theEncoding = [[NSApp delegate] encodingFromName:theEncodingName];
    BOOL theResult = NO;

    if ((theEncoding == NSNotFound) || (theFileName == nil)) {
        theResult = NO;
    } else if (theEncoding == _encoding) {
        theResult = YES;
    } else if ([self stringFromData:[NSData dataWithContentsOfFile:theFileName] 
                encoding:theEncoding xattr:NO]) {
        [self setStringToEditorView];
        // ダーティーフラグをクリア
        [self updateChangeCount:NSChangeCleared];
        // ツールバーアイテムの選択状態をセット
        [[_windowController toolbarController] setSelectEncoding:_encoding];
        theResult = YES;
    }

    return [NSNumber numberWithBool:theResult];
}


// ------------------------------------------------------
- (NSNumber *)handleFind:(NSScriptCommand *)inCommand
// 検索
// ------------------------------------------------------
{
    NSDictionary *theArg = [inCommand evaluatedArguments];
    NSString *theSearch = [theArg valueForKey:@"targetString"];
    if ((theSearch == nil) || ([theSearch length] < 1)) { return [NSNumber numberWithBool:NO]; }
    BOOL theBoolIsRE = ([theArg valueForKey:@"regularExpression"] != nil) ? 
               [[theArg valueForKey:@"regularExpression"] boolValue] : NO;
    BOOL theBoolIgnoreCase = ([theArg valueForKey:@"ignoreCase"] != nil) ? 
               [[theArg valueForKey:@"ignoreCase"] boolValue] : NO;
    BOOL theBoolBackwards = ([theArg valueForKey:@"backwardsSearch"] != nil) ? 
               [[theArg valueForKey:@"backwardsSearch"] boolValue] : NO;
    BOOL theBoolWrapSearch = ([theArg valueForKey:@"wrapSearch"] != nil) ? 
               [[theArg valueForKey:@"wrapSearch"] boolValue] : NO;
    NSString *theWholeStr = [_editorView stringForSave];
    int theWholeLength = [theWholeStr length];
    if (theWholeLength < 1) { return [NSNumber numberWithBool:NO]; }
    NSRange theSelectionRange = [_editorView selectedRange];
    NSRange theTargetRange;

    if (theBoolBackwards) {
        theTargetRange = NSMakeRange(0, theSelectionRange.location);
    } else {
        theTargetRange = NSMakeRange(NSMaxRange(theSelectionRange), 
                            theWholeLength - NSMaxRange(theSelectionRange));
    }
    unsigned theMask = 0;
    if (theBoolIgnoreCase) {
        theMask |= (theBoolIsRE) ? OgreIgnoreCaseOption : NSCaseInsensitiveSearch;
    }
    if (theBoolBackwards) {
        theMask |= NSBackwardsSearch;
    }

    BOOL theBoolResult = [self doFind:theSearch range:theTargetRange 
                option:theMask withRegularExpression:theBoolIsRE];
    if ((theBoolResult == NO) && theBoolWrapSearch) {
        theTargetRange = NSMakeRange(0, theWholeLength);
        theBoolResult = [self doFind:theSearch range:theTargetRange 
                option:theMask withRegularExpression:theBoolIsRE];
    }
    
    return [NSNumber numberWithBool:theBoolResult];
}


// ------------------------------------------------------
- (NSNumber *)handleReplace:(NSScriptCommand *)inCommand
// 置換
// ------------------------------------------------------
{
    NSDictionary *theArg = [inCommand evaluatedArguments];
    NSString *theSearch = [theArg valueForKey:@"targetString"];
    if ((theSearch == nil) || ([theSearch length] < 1)) { return [NSNumber numberWithBool:NO]; }
    BOOL theBoolIsRE = ([theArg valueForKey:@"regularExpression"] != nil) ? 
               [[theArg valueForKey:@"regularExpression"] boolValue] : NO;
    BOOL theBoolIgnoreCase = ([theArg valueForKey:@"ignoreCase"] != nil) ? 
               [[theArg valueForKey:@"ignoreCase"] boolValue] : NO;
    BOOL theBoolAll = ([theArg valueForKey:@"all"] != nil) ? 
               [[theArg valueForKey:@"all"] boolValue] : NO;
    BOOL theBoolBackwards = ([theArg valueForKey:@"backwardsSearch"] != nil) ? 
               [[theArg valueForKey:@"backwardsSearch"] boolValue] : NO;
    BOOL theBoolWrapSearch = ([theArg valueForKey:@"wrapSearch"] != nil) ? 
               [[theArg valueForKey:@"wrapSearch"] boolValue] : NO;
    NSString *theWholeStr = [_editorView stringForSave];
    int theWholeLength = [theWholeStr length];
    if (theWholeLength < 1) { return [NSNumber numberWithInt:0]; }
    NSString *theNewString = [theArg valueForKey:@"newString"];
    if ([theSearch isEqualToString:theNewString]) { return [NSNumber numberWithBool:NO]; }
    if (theNewString == nil) { theNewString = @""; }
    NSRange theSelectionRange, theTargetRange;

    if (theBoolAll) {
        theTargetRange = NSMakeRange(0, theWholeLength);
    } else {
        theSelectionRange = [_editorView selectedRange];
        if (theBoolBackwards) {
            theTargetRange = NSMakeRange(0, theSelectionRange.location);
        } else {
            theTargetRange = NSMakeRange(NSMaxRange(theSelectionRange), 
                                theWholeLength - NSMaxRange(theSelectionRange));
        }
    }
    unsigned theMask = 0;
    if (theBoolIgnoreCase) {
        theMask |= (theBoolIsRE) ? OgreIgnoreCaseOption : NSCaseInsensitiveSearch;
    }
    if (theBoolBackwards) {
        theMask |= NSBackwardsSearch;
    }

    BOOL theBoolResult = NO;
    int theResult = 0;
    if (theBoolAll) {
        NSMutableString *theTmpStr = [theWholeStr mutableCopy]; // ===== copy
        if (theBoolIsRE) {
            theResult = [theTmpStr replaceOccurrencesOfRegularExpressionString:theSearch 
                            withString:theNewString options:theMask range:theTargetRange];
        } else {
            theResult = [theTmpStr replaceOccurrencesOfString:theSearch 
                            withString:theNewString options:theMask range:theTargetRange];
        }
        if (theResult > 0) {
            [[_editorView textView] replaceAllStringTo:theTmpStr];
            [[_editorView textView] setSelectedRange:NSMakeRange(0,0)];
        }
        [theTmpStr release]; // ===== release

    } else {
        theBoolResult = [self doFind:theSearch range:theTargetRange 
                    option:theMask withRegularExpression:theBoolIsRE];
        if ((theBoolResult == NO) && theBoolWrapSearch) {
            theTargetRange = NSMakeRange(0, theWholeLength);
            theBoolResult = [self doFind:theSearch range:theTargetRange 
                    option:theMask withRegularExpression:theBoolIsRE];
        }
        if (theBoolResult) {
            [_selection setContents:theNewString]; // （CETextSelection の setContents: の引数は NSString も可）
            theResult = 1;
        }
    }

    return [NSNumber numberWithInt:theResult];
}


// ------------------------------------------------------
- (void)handleScroll:(NSScriptCommand *)inCommand
// スクロール実行
// ------------------------------------------------------
{
    [self scrollToCenteringSelection];
}


// ------------------------------------------------------
- (NSString *)handleString:(NSScriptCommand *)inCommand
// 指定された範囲の文字列を返す
// ------------------------------------------------------
{
    NSDictionary *theArg = [inCommand evaluatedArguments];
    NSArray *theArray = [theArg valueForKey:@"range"];
    int theLocation, theLength;
    NSRange theRange;

    if ((theArray == nil) || ([theArray count] < 1)) { return [NSString string]; }
    theLocation = [[theArray objectAtIndex:0] intValue];
    theLength = ([theArray count] > 1) ? 
            [[theArray objectAtIndex:1] intValue] : 1;
    theRange = [self rangeInTextViewWithLocation:theLocation withLength:theLength];

    if (NSEqualRanges(NSMakeRange(0, 0), theRange)) {
        return @"";
    }
    return [[[_editorView textView] string] substringWithRange:theRange];
}



@end




@implementation CEDocument (AdditionalAppleScriptPrivate)

//=======================================================
// Private method
//
//=======================================================

// ------------------------------------------------------
- (BOOL)doFind:(NSString *)inSearchString range:(NSRange)inRange 
            option:(unsigned)inMask withRegularExpression:(BOOL)inRE
// 文字列を検索し、見つかったら選択して結果を返す
// ------------------------------------------------------
{
    NSString *theWholeStr = [_editorView stringForSave];
    NSRange theSearchedRange;

    if (inRE) {
        theSearchedRange = 
                [theWholeStr rangeOfRegularExpressionString:inSearchString options:inMask range:inRange];
    } else {
        theSearchedRange = [theWholeStr rangeOfString:inSearchString options:inMask range:inRange];
    }
    if (theSearchedRange.location != NSNotFound) {
        [_editorView setSelectedRange:theSearchedRange];
        return YES;
    }
    return NO;
}
@end