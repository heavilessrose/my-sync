/*
=================================================
CESyntax
(for CotEditor)

Copyright (C) 2004-2007 nakamuxu.
http://www.aynimac.com/
=================================================

encoding="UTF-8"
Created:2004.12.22

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

#import "CESyntax.h"
#import "CEEditorView.h"

//=======================================================
// Private method
//
//=======================================================

@interface CESyntax (Private)
- (void)setAttrToSimpleWordsArrayDict:(NSMutableDictionary*)inWordsDict 
                withCharString:(NSMutableString *)inCharString;
- (NSArray *)rangesSimpleWordsArrayDict:(NSMutableDictionary*)inWordsDict 
            withCharString:(NSMutableString *)inCharString;
- (NSArray *)rangesBeginString:(NSString *)inBeginString withEndString:(NSString *)inEndString 
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind;
- (NSArray *)rangesRegularExpressionString:(NSString *)inRegexStr withIgnoreCase:(BOOL)inIgnoreCase 
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind;
- (NSArray *)checkRegularExpressionString:(NSString *)inRegexStr withIgnoreCase:(BOOL)inIgnoreCase 
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind;
- (NSArray *)rangesRegularExpressionBeginString:(NSString *)inBeginString withEndString:(NSString *)inEndString 
        withIgnoreCase:(BOOL)inIgnoreCase 
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind;
- (NSArray *)checkRegularExpressionBeginString:(NSString *)inBeginString withEndString:(NSString *)inEndString 
        withIgnoreCase:(BOOL)inIgnoreCase 
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind;
- (void)setAttrToCommentsWithSyntaxArray:(NSArray *)inArray 
        withSingleQuotes:(BOOL)inValueSingleQuotes withDoubleQuotes:(BOOL)inValueDoubleQuotes 
        updateIndicator:(BOOL)inValueUpdateIndicator;
- (unsigned int)numberOfEscapeSequenceInString:(NSString *)inString;
- (void)setOtherInvisibleCharsAttrs;
- (void)doColoring;
- (double)doubleValueOfIndicator;
- (void)setDoubleIndicator:(double)inValue;
@end


//------------------------------------------------------------------------------------------




@implementation CESyntax

#pragma mark ===== Public method =====

//=======================================================
// Public method
//
//=======================================================

// ------------------------------------------------------
- (id)init
// 初期化
// ------------------------------------------------------
{
    self = [super init];
    if (self) {
        id theValues = [[NSUserDefaultsController sharedUserDefaultsController] values];

        (void)[NSBundle loadNibNamed:@"Indicator" owner:self];
        _wholeString = nil;
        _localString = nil;
        _syntaxStyleName = nil;
        _coloringDictionary = nil;
        _singleQuotesAttrs = nil;
        _doubleQuotesAttrs = nil;
        _completeWordsArray = nil;
        _completeFirstLetterSet = nil;
        _updateRange = NSMakeRange(0, 0);
        _isIndicatorShown = NO;
        _isPrinting = NO;
        _isPanther = (floor(NSAppKitVersionNumber) <=  NSAppKitVersionNumber10_3);
        _showColoringIndicatorTextLength = 
                [[theValues valueForKey:k_key_showColoringIndicatorTextLength] unsignedIntValue];
        [_coloringIndicator setIndeterminate:NO];
    }
    return self;
}


// ------------------------------------------------------
- (void)dealloc
// あとかたづけ
// ------------------------------------------------------
{
    // NSBundle loadNibNamed: でロードされたオブジェクトを開放
    // 参考にさせていただきました > http://homepage.mac.com/mkino2/backnumber/2004_10.html#October%2012_1
    [[_coloringIndicator window] release];
    // （_coloringIndicator はパネルのコンテントビューなのでパネルの解放後に自動開放される）

    // （_textColor, _currentAttrs, _singleQuotesAttrs, _doubleQuotesAttrs は保持していない）
    [_layoutManager release];
    [_wholeString release];
    [_localString release];
    [_syntaxStyleName release];
    [_coloringDictionary release];
    [_completeWordsArray release];
    [_completeFirstLetterSet release];

    [super dealloc];
}


// ------------------------------------------------------
- (void)setWholeString:(NSString *)inString
// 全文字列をセット
// ------------------------------------------------------
{
    [inString retain];
    [_wholeString release];
    _wholeString = inString;
}


// ------------------------------------------------------
- (unsigned int)wholeStringLength
// 全文字列の長さを返す
// ------------------------------------------------------
{
    return [_wholeString length];
}


// ------------------------------------------------------
- (void)setLocalString:(NSString *)inString
// カラーリング対象文字列 をセット
// ------------------------------------------------------
{
    [inString retain];
    [_localString release];
    _localString = inString;
}


// ------------------------------------------------------
- (void)setLayoutManager:(CELayoutManager *)inLayoutManager
// layoutManagerをセット
// ------------------------------------------------------
{
    [inLayoutManager retain];
    [_layoutManager release];
    _layoutManager = inLayoutManager;
}


// ------------------------------------------------------
- (NSString *)syntaxStyleName
// style名を返す
// ------------------------------------------------------
{
    return _syntaxStyleName;
}


// ------------------------------------------------------
- (void)setSyntaxStyleName:(NSString *)inStyleName
// 保持するstyle名をセット
// ------------------------------------------------------
{
    CESyntaxManager *theManager = [CESyntaxManager sharedInstance];
    NSArray *theNames = [theManager styleNames];

    if (([theNames containsObject:inStyleName]) || 
            ([inStyleName isEqualToString:NSLocalizedString(@"None",@"")])) {

        NSDictionary *theTmpDict = [[theManager syntaxWithStyleName:inStyleName] retain];
        [_coloringDictionary release];
        _coloringDictionary = theTmpDict;

        [self setCompleteWordsArrayFromColoringDictionary];

        [inStyleName retain];
        [_syntaxStyleName release];
        _syntaxStyleName = inStyleName;
    }
}


// ------------------------------------------------------
- (BOOL)setSyntaxStyleNameFromExtension:(NSString *)inExtension
// 拡張子からstyle名をセット
// ------------------------------------------------------
{
    NSString *theName = [[CESyntaxManager sharedInstance] syntaxNameFromExtension:inExtension];

    if ((theName) && (![[self syntaxStyleName] isEqualToString:theName])) {
        [self setSyntaxStyleName:theName];
        return YES;
    }
    return NO;
}


// ------------------------------------------------------
- (NSArray *)completeWordsArray
// 保持している入力補完文字列配列を返す
// ------------------------------------------------------
{
    return _completeWordsArray;
}


// ------------------------------------------------------
- (NSCharacterSet *)completeFirstLetterSet
// 保持している入力補完の最初の1文字のセットを返す
// ------------------------------------------------------
{
    return _completeFirstLetterSet;
}


// ------------------------------------------------------
- (void)setCompleteWordsArrayFromColoringDictionary
// 保持しているカラーリング辞書から補完文字列配列を生成
// ------------------------------------------------------
{
    if (_coloringDictionary == nil) { return; }

    NSMutableArray *theTmpArray = [NSMutableArray array];
    NSArray *theCompleteArray = [_coloringDictionary objectForKey:k_SCKey_completionsArray];
    NSMutableString *theTmpString = [NSMutableString string];
    NSString *theStr = nil;
    NSCharacterSet *theCharSet;
    int i, theCount;

    if (theCompleteArray) {

        theCount = [theCompleteArray count];

        for (i = 0; i < theCount; i++) {
            theStr = [[theCompleteArray objectAtIndex:i] valueForKey:k_SCKey_arrayKeyString];
            [theTmpArray addObject:theStr];
            [theTmpString appendString:[theStr substringToIndex:1]];
        }

    } else {

        NSArray *theSyntaxArray = [NSArray arrayWithObjects:k_SCKey_allColoringArrays, nil];
        NSArray *theArray;
        NSString *theEndStr = nil;
        NSDictionary *theStrDict;
        int j;

        theCount = [theSyntaxArray count];

        NSAutoreleasePool *thePool = [[NSAutoreleasePool alloc] init]; // ===== alloc
        for (i = 0; i < theCount; i++) {
            theArray = [_coloringDictionary objectForKey:[theSyntaxArray objectAtIndex:i]];
            int theArrayCount = [theArray count];
            for (j = 0; j < theArrayCount; j++) {
                theStrDict = [theArray objectAtIndex:j];
                theStr = [[theStrDict objectForKey:k_SCKey_beginString] stringByTrimmingCharactersInSet:
                                    [NSCharacterSet whitespaceAndNewlineCharacterSet]];
                theEndStr = [[theStrDict objectForKey:k_SCKey_endString] stringByTrimmingCharactersInSet:
                                    [NSCharacterSet whitespaceAndNewlineCharacterSet]];
                if (([theStr length] > 0) && 
                        ((theEndStr == nil) || ([theEndStr length] < 1)) && 
                        (![[theStrDict objectForKey:k_SCKey_regularExpression] boolValue])) {
                    [theTmpArray addObject:theStr];
                    [theTmpString appendString:[theStr substringToIndex:1]];
                }
            }
            if (i % 100 == 0) {
                [thePool release]; // ===== release
                thePool = [[NSAutoreleasePool alloc] init]; // ===== alloc
            }
        }
        [thePool release]; // ===== release
        // ソート
        [theTmpArray sortedArrayUsingSelector:@selector(compare:)];
    }
    // _completeWordsArray を保持する
    [theTmpArray retain]; // ===== retain
    [_completeWordsArray release];
    _completeWordsArray = theTmpArray;

    // _completeFirstLetterSet を保持する
    [_completeFirstLetterSet release];
    if ([theTmpString length] > 0) {
        theCharSet = [[NSCharacterSet characterSetWithCharactersInString:theTmpString] retain]; // ===== retain
        _completeFirstLetterSet = theCharSet;
    } else {
        _completeFirstLetterSet = nil;
    }
}


// ------------------------------------------------------
- (void)colorAllString:(NSString *)inWholeString
// 全体をカラーリング
// ------------------------------------------------------
{
    if ((inWholeString == nil) || ([inWholeString length] < 1) || 
            ([[self syntaxStyleName] length] < 1)) { return; }

    [self setWholeString:inWholeString];
    _updateRange = NSMakeRange(0, [self wholeStringLength]);

    if (_coloringDictionary == nil) {
        _coloringDictionary  = [[[CESyntaxManager sharedInstance] syntaxWithStyleName:
                    [self syntaxStyleName]] retain]; // ===== retain
        [self setCompleteWordsArrayFromColoringDictionary];
    }
    if (_coloringDictionary == nil) { return; }

    [self doColoring];
    [self setWholeString:nil];
}


// ------------------------------------------------------
- (void)colorVisibleRange:(NSRange)inRange withWholeString:(NSString *)inWholeString
// 表示されている部分をカラーリング
// ------------------------------------------------------
{
    if ((inWholeString == nil) || ([inWholeString length] < 1) || 
            ([[self syntaxStyleName] length] < 1)) { return; }
    [self setWholeString:inWholeString];

    NSRange theEffectiveRange;
    unsigned int theStart = inRange.location;
    unsigned int theEnd = NSMaxRange(inRange) - 1;
    unsigned int theWholeLength = [self wholeStringLength];

    // 直前／直後が同色ならカラーリング範囲を拡大する（10.5+で実行するならより正確に拡大）
    if (floor(NSAppKitVersionNumber) >= 949) { // 949 = LeopardのNSAppKitVersionNumber
        (void)[_layoutManager temporaryAttributesAtCharacterIndex:theStart 
                longestEffectiveRange:&theEffectiveRange inRange:NSMakeRange(0, [self wholeStringLength])];
    } else {
        (void)[_layoutManager temporaryAttributesAtCharacterIndex:theStart 
                effectiveRange:&theEffectiveRange];
    }
    theStart = theEffectiveRange.location;
    if (floor(NSAppKitVersionNumber) >= 949) { // 949 = LeopardのNSAppKitVersionNumber
        (void)[_layoutManager temporaryAttributesAtCharacterIndex:theEnd 
                longestEffectiveRange:&theEffectiveRange inRange:NSMakeRange(0, [self wholeStringLength])];
    } else {
        (void)[_layoutManager temporaryAttributesAtCharacterIndex:theEnd 
                effectiveRange:&theEffectiveRange];
    }
    theEnd = (NSMaxRange(theEffectiveRange) < theWholeLength) ? 
                NSMaxRange(theEffectiveRange) : theWholeLength;

    _updateRange = NSMakeRange(theStart, theEnd - theStart);
    if (_coloringDictionary == nil) {
        _coloringDictionary  = [[[CESyntaxManager sharedInstance] syntaxWithStyleName:
                    [self syntaxStyleName]] retain]; // ===== retain
        [self setCompleteWordsArrayFromColoringDictionary];
    }
    if (_coloringDictionary == nil) { return; }

    [self doColoring];
    [self setWholeString:nil];
}


// ------------------------------------------------------
- (NSArray *)outlineMenuArrayWithWholeString:(NSString *)inWholeString
// アウトラインメニュー用の配列を生成し、返す
// ------------------------------------------------------
{
// （outlineMenuは、過去の定義との互換性保持のためもあってOgreKitを使っている 2008.05.16）
    NSMutableArray *outArray = [NSMutableArray array];
    if ((inWholeString == nil) || ([inWholeString length] < 1) || 
            ([[self syntaxStyleName] length] < 1)) {
        return outArray;
    }
    [self setWholeString:inWholeString];

    id theValues = [[NSUserDefaultsController sharedUserDefaultsController] values];
    NSArray *theREStringArray = [_coloringDictionary objectForKey:k_SCKey_outlineMenuArray];
    NSDictionary *theDict;
    NSMutableString *thePattern; 
    NSString *theTitle, *theMatchedIndexString;
    NSRange theMatchRange;
    int i, j, theCount = [theREStringArray count];
    unsigned int theIndex, theLines, theCurLine, theWholeLength = [inWholeString length];
    unsigned int theMenuTitleMaxLength = [[theValues valueForKey:k_key_outlineMenuMaxLength] unsignedIntValue];

    for (i = 0; i < theCount; i++) {
        theDict = [theREStringArray objectAtIndex:i];
        unsigned int theOption = ([[theDict objectForKey:k_SCKey_ignoreCase] boolValue]) ? 
                        OgreIgnoreCaseOption : OgreNoneOption;
        NSDictionary *theMatchDict;
        OGRegularExpression *theRegex;
        NSEnumerator *theEnumerator;
        OGRegularExpressionMatch *theMatch;

        NS_DURING
            theRegex = [OGRegularExpression regularExpressionWithString:
                        [theDict objectForKey:k_SCKey_beginString] options:theOption];
        NS_HANDLER
            // 何もしない
            NSLog(@"ERROR in \"outlineMenuArrayWithWholeString:\"");
            continue;
        NS_ENDHANDLER

        theEnumerator = [theRegex matchEnumeratorInString:_wholeString];
        while (theMatch = [theEnumerator nextObject]) {
            // マッチした範囲
            theMatchRange = [theMatch rangeOfMatchedString];
            // メニュー項目タイトル
            thePattern = [[[theDict objectForKey:k_SCKey_arrayKeyString] mutableCopy] autorelease];
            if ([thePattern isEqualToString:k_outlineMenuSeparatorSymbol]) {
                // セパレータのとき
                theMatchDict = [NSDictionary dictionaryWithObjectsAndKeys:
                            [NSValue valueWithRange:theMatchRange], k_outlineMenuItemRange, 
                            k_outlineMenuSeparatorSymbol, k_outlineMenuItemTitle, 
                            [NSNumber numberWithUnsignedInt:theMatchRange.location], k_outlineMenuItemSortKey, 
                            nil];
                [outArray addObject:theMatchDict];
                continue;
            } else if ((thePattern == nil) || ([thePattern length] < 1)) {
                // パターン定義なし
                thePattern = [[[theMatch matchedString] mutableCopy] autorelease];
            } else {
                // マッチ文字列（$0, $&）置換
                (void)[thePattern replaceOccurrencesOfRegularExpressionString:@"(?<!\\\\)\\$0" 
                        withString:[theMatch matchedString] options:0 
                        range:NSMakeRange(0, [thePattern length])];
                (void)[thePattern replaceOccurrencesOfRegularExpressionString:@"(?<!\\\\)\\$&" 
                        withString:[theMatch matchedString] options:0 
                        range:NSMakeRange(0, [thePattern length])];
                // マッチ部分文字列（$1-9）置換
                for (j = 1; j < 10; j++) {
                    theMatchedIndexString = [theMatch substringAtIndex:j];
                    if (theMatchedIndexString != nil) {
                        (void)[thePattern replaceOccurrencesOfRegularExpressionString:
                                    [NSString stringWithFormat:@"(?<!\\\\)\\$%i", j] 
                                withString:theMatchedIndexString options:0 
                                range:NSMakeRange(0, [thePattern length])];
                    }
                }
                // マッチした範囲の開始位置の行
                theCurLine = 1;
                for (theIndex = 0, theLines = 0; theIndex < theWholeLength; theLines++) {
                    if (theIndex <= theMatchRange.location) {
                        theCurLine = theLines + 1;
                    } else {
                        break;
                    }
                    theIndex = NSMaxRange([inWholeString lineRangeForRange:NSMakeRange(theIndex, 0)]);
                }
                //行番号（$LN）置換
                (void)[thePattern replaceOccurrencesOfRegularExpressionString:
                            [NSString stringWithFormat:@"(?<!\\\\)\\$LN", j] 
                        withString:[NSString stringWithFormat:@"%i", theCurLine] options:0 
                        range:NSMakeRange(0, [thePattern length])];
            }
            // 改行またはタブをスペースに置換
            (void)[thePattern replaceOccurrencesOfRegularExpressionString:@"[\n\t]" 
                    withString:@" " options:0 range:NSMakeRange(0, [thePattern length])];
            // エスケープされた「$」を置換
            (void)[thePattern replaceOccurrencesOfRegularExpressionString:@"\\\\\\$(?=([0-9&]|LN))" 
                    withString:@"$" options:0 range:NSMakeRange(0, [thePattern length])];
            // タイトル確定
            if ([thePattern length] > theMenuTitleMaxLength) {
                theTitle = [NSString stringWithFormat:@"%@ ...", 
                        [thePattern substringToIndex:theMenuTitleMaxLength]];
            } else {
                theTitle = [NSString stringWithString:thePattern];
            }
            // ボールド
            BOOL theBoolIsBold = [[theDict valueForKey:k_SCKey_bold] boolValue];
            // イタリック
            BOOL theBoolIsItalic = [[theDict valueForKey:k_SCKey_italic] boolValue];
            // アンダーライン
            unsigned int theUnderlineMask = ([[theDict valueForKey:k_SCKey_underline] boolValue]) ? 
                    (NSUnderlineByWordMask | NSUnderlinePatternSolid | NSUnderlineStyleThick) : 0;
            // 辞書生成
            theMatchDict = [NSDictionary dictionaryWithObjectsAndKeys:
                        [NSValue valueWithRange:theMatchRange], k_outlineMenuItemRange, 
                        theTitle, k_outlineMenuItemTitle, 
                        [NSNumber numberWithUnsignedInt:theMatchRange.location], k_outlineMenuItemSortKey, 
                        [NSNumber numberWithBool:theBoolIsBold], k_outlineMenuItemFontBold, 
                        [NSNumber numberWithBool:theBoolIsItalic], k_outlineMenuItemFontItalic, 
                        [NSNumber numberWithUnsignedInt:theUnderlineMask], k_outlineMenuItemUnderlineMask, 
                        nil];
            [outArray addObject:theMatchDict];
        }
    }
    if ([outArray count] > 0) {
        NSSortDescriptor *theDescriptor = [[[NSSortDescriptor alloc] initWithKey:k_outlineMenuItemSortKey 
                    ascending:YES selector:@selector(compare:)] autorelease];
        [outArray sortUsingDescriptors:[NSArray arrayWithObject:theDescriptor]];
        // ソート後に、冒頭のアイテムを追加
        [outArray insertObject:
                [NSDictionary dictionaryWithObjectsAndKeys:
                    [NSValue valueWithRange:NSMakeRange(0, 0)], k_outlineMenuItemRange, 
                    NSLocalizedString(@"< Outline Menu >",@""), k_outlineMenuItemTitle, 
                    [NSNumber numberWithUnsignedInt:0], k_outlineMenuItemSortKey, 
                    nil]
                atIndex:0];
    }
    return outArray;
}


// ------------------------------------------------------
- (BOOL)isPrinting
// プリンタ中かどうかを返す
// ------------------------------------------------------
{
    // （[NSGraphicsContext currentContextDrawingToScreen] は真を返す時があるため、専用フラグを使う）
    return _isPrinting;
}


// ------------------------------------------------------
- (void)setIsPrinting:(BOOL)inValue
// プリンタ中かどうかを設定
// ------------------------------------------------------
{
    // （[NSGraphicsContext currentContextDrawingToScreen] は真を返す時があるため、専用フラグを使う）
    _isPrinting = inValue;
}



#pragma mark ===== Action messages =====

//=======================================================
// Action messages
//
//=======================================================

// ------------------------------------------------------
- (IBAction)cancelColoring:(id)sender
// カラーリング中止、インジケータシートのモーダルを停止
// ------------------------------------------------------
{
    [NSApp abortModal];
}

@end



@implementation CESyntax (Private)

//=======================================================
// Private method
//
//=======================================================

// ------------------------------------------------------
- (void)setAttrToSimpleWordsArrayDict:(NSMutableDictionary*)inWordsDict 
                withCharString:(NSMutableString *)inCharString
// 指定された文字列をそのまま検索し、カラーリング
// ------------------------------------------------------
{
    NSArray *theArray = [self rangesSimpleWordsArrayDict:inWordsDict withCharString:inCharString];
    NSRange theRange;
    int i, theCount = [theArray count];

    for (i = 0; i < theCount; i++) {
        theRange = [[theArray objectAtIndex:i] rangeValue];
        theRange.location += _updateRange.location;

        if ([self isPrinting]) {
            [[_layoutManager firstTextView] setTextColor:_textColor range:theRange];
        } else {
            [_layoutManager addTemporaryAttributes:_currentAttrs forCharacterRange:theRange];
        }
    }
}


// ------------------------------------------------------
- (NSArray *)rangesSimpleWordsArrayDict:(NSMutableDictionary*)inWordsDict 
            withCharString:(NSMutableString *)inCharString
// 指定された文字列をそのまま検索し、位置を返す
// ------------------------------------------------------
{
    NSScanner *theScanner = [NSScanner scannerWithString:_localString];
    NSString *theScanStr = nil;
    CEPrivateMutableArray *outArray = [[[CEPrivateMutableArray alloc] initWithCapacity:10] autorelease];
    NSCharacterSet *theCharSet;
    NSRange theAttrRange;
    id wordsArray;
    unsigned int theLocation = 0, theLength = 0;

    // 改行、タブ、スペースは無視
    [inCharString chomp];
    (void)[inCharString replaceOccurrencesOfString:@"\t"
                withString:@"" options:0 range:NSMakeRange(0, [inCharString length])];
    (void)[inCharString replaceOccurrencesOfString:@" "
                withString:@"" options:0 range:NSMakeRange(0, [inCharString length])];

    theCharSet = [NSCharacterSet characterSetWithCharactersInString:inCharString];
    [theScanner setCharactersToBeSkipped:[NSCharacterSet characterSetWithCharactersInString:@"\n\t "]];
    [theScanner setCaseSensitive:YES];

    NS_DURING
        while (![theScanner isAtEnd]) {
            (void)[theScanner scanUpToCharactersFromSet:theCharSet intoString:NULL];
            if ([theScanner scanCharactersFromSet:theCharSet intoString:&theScanStr]) {
                theLength = [theScanStr length];
                if (theLength > 0) {
                    theLocation = [theScanner scanLocation];
                    wordsArray = [inWordsDict objectForKey:[NSNumber numberWithInt:theLength]];
                    if ([wordsArray containsObject:theScanStr]) {
                        theAttrRange = NSMakeRange(theLocation - theLength, theLength);
                        [outArray addObject:[NSValue valueWithRange:theAttrRange]];
                    }
                }
            }
        }
    NS_HANDLER
        // 何もしない
        NSLog(@"ERROR in \"rangesSimpleWordsArrayDict:withCharString:\"");
        return nil;
    NS_ENDHANDLER

    return outArray;
}


// ------------------------------------------------------
- (NSArray *)rangesBeginString:(NSString *)inBeginString withEndString:(NSString *)inEndString 
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind
// 指定された開始／終了ペアの文字列を検索し、位置を返す
// ------------------------------------------------------
{
    NSString *theESCheckStr = nil;
    NSScanner *theScanner = [NSScanner scannerWithString:_localString];
    unsigned int theLocalLength = [_localString length];
    unsigned int theStart = 0, theESNum = 0, theEnd = 0;
    unsigned int theBeginLength = 0, theEndLength = 0, theESCheckLength;
    unsigned int theStartEnd = 0;
    NSRange theAttrRange, theTmpRange;

    theBeginLength = [inBeginString length];
    if (theBeginLength < 1) { return nil; }
    theEndLength = [inEndString length];
    [theScanner setCharactersToBeSkipped:nil];
    [theScanner setCaseSensitive:YES];
    CEPrivateMutableArray *outArray =  [[[CEPrivateMutableArray alloc] initWithCapacity:10] autorelease];
    int i = 0;

    while (![theScanner isAtEnd]) {
        (void)[theScanner scanUpToString:inBeginString intoString:nil];
        theStart = [theScanner scanLocation];
        if (theStart + theBeginLength < theLocalLength) {
            [theScanner setScanLocation:(theStart + theBeginLength)];
            theESCheckLength = (theStart < k_ESCheckLength) ? theStart : k_ESCheckLength;
            theTmpRange = NSMakeRange(theStart - theESCheckLength, theESCheckLength);
            theESCheckStr = [_localString substringWithRange:theTmpRange];
            theESNum = [self numberOfEscapeSequenceInString:theESCheckStr];
            if (theESNum % 2 == 1) {
                continue;
            }
            if (!inValueDoColoring) {
                theStartEnd = (inPairKind >= k_QC_CommentBaseNum) ? k_QC_Start : k_notUseStartEnd;
                [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                        [NSNumber numberWithUnsignedInt:theStart], k_QCPosition, 
                        [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                        [NSNumber numberWithUnsignedInt:theStartEnd], k_QCStartEnd, 
                        [NSNumber numberWithUnsignedInt:theBeginLength], k_QCStrLength, 
                        nil]];
            }
        } else {
            break;
        }
        while (1) {
            i++;
            if ((_isIndicatorShown) && ((i % 10) == 0) && 
                    ([NSApp runModalSession:_modalSession] != NSRunContinuesResponse)) {
                return nil;
            }
            (void)[theScanner scanUpToString:inEndString intoString:nil];
            theEnd = [theScanner scanLocation] + theEndLength;
            if (theEnd <= theLocalLength) {
                [theScanner setScanLocation:theEnd];
                theESCheckLength = ((theEnd - theEndLength) < k_ESCheckLength) ? 
                        (theEnd - theEndLength) : k_ESCheckLength;
                theTmpRange = NSMakeRange(theEnd - theEndLength - theESCheckLength, theESCheckLength);
                theESCheckStr = [_localString substringWithRange:theTmpRange];
                theESNum = [self numberOfEscapeSequenceInString:theESCheckStr];
                if (theESNum % 2 == 1) {
                    continue;
                } else {
                    if (theStart < theEnd) {
                        if (inValueDoColoring) {
                            theAttrRange = NSMakeRange(theStart, theEnd - theStart);
                            [outArray addObject:[NSValue valueWithRange:theAttrRange]];
                        } else {
                            theStartEnd = (inPairKind >= k_QC_CommentBaseNum) ? k_QC_End : k_notUseStartEnd;
                            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithUnsignedInt:(theEnd - theEndLength)], k_QCPosition, 
                                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                                    [NSNumber numberWithUnsignedInt:theStartEnd], k_QCStartEnd, 
                                    [NSNumber numberWithUnsignedInt:theEndLength], k_QCStrLength, 
                                    nil]];
                        }
                        break;
                    }
                }
            } else {
                break;
            }
        } // end-while (1)
    } // end-while (![theScanner isAtEnd])
    return outArray;
}


// ------------------------------------------------------
- (NSArray *)rangesRegularExpressionString:(NSString *)inRegexStr withIgnoreCase:(BOOL)inIgnoreCase 
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind
// 指定された文字列を正規表現として検索し、位置を返す
// ------------------------------------------------------
{
    uint32_t theOption = (inIgnoreCase) ? (RKLCaseless | RKLMultiline) : RKLMultiline;
    NSArray *theMatchArray;
    NSEnumerator *theEnumerator;
    CEPrivateMutableArray *outArray = nil;
    NSRange theAttrRange;
    int i, theCount = 0;
    unsigned int theQCStart = 0, theQCEnd = 0;

    NS_DURING
        theEnumerator = [_localString matchEnumeratorWithRegex:inRegexStr options:theOption];
        theMatchArray = [theEnumerator allObjects];
    NS_HANDLER
        // 何もしない
        NSLog(@"ERROR in \"rangesRegularExpressionString:withIgnoreCase:doColoring:pairStringKind:\"");
        return nil;
    NS_ENDHANDLER

    if (inValueDoColoring) {
        return theMatchArray;
    } else {
        theCount = [theMatchArray count];
        if (theCount > 0) {
            outArray = [[[CEPrivateMutableArray alloc] initWithCapacity:10] autorelease];
        }
        for (i = 0; i < theCount; i++) {
            if ((_isIndicatorShown) && ((i % 10) == 0) && 
                    ([NSApp runModalSession:_modalSession] != NSRunContinuesResponse)) {
                return nil;
            }
            theAttrRange = [[theMatchArray objectAtIndex:i] rangeValue];
            if (inPairKind >= k_QC_CommentBaseNum) {
                theQCStart = k_QC_Start;
                theQCEnd = k_QC_End;
            } else {
                theQCStart = theQCEnd = k_notUseStartEnd;
            }
            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                    [NSNumber numberWithUnsignedInt:(theAttrRange.location)], k_QCPosition, 
                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                    [NSNumber numberWithUnsignedInt:theQCStart], k_QCStartEnd, 
                    [NSNumber numberWithUnsignedInt:0], k_QCStrLength, 
                    nil]];
            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                    [NSNumber numberWithUnsignedInt:NSMaxRange(theAttrRange)], k_QCPosition, 
                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                    [NSNumber numberWithUnsignedInt:theQCEnd], k_QCStartEnd, 
                    [NSNumber numberWithUnsignedInt:0], k_QCStrLength, 
                    nil]];
        }
        return outArray;
    }
}


// ------------------------------------------------------
- (NSArray *)checkRegularExpressionString:(NSString *)inRegexStr withIgnoreCase:(BOOL)inIgnoreCase 
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind
// 指定された文字列を正規表現として検索し、位置を返す
// ------------------------------------------------------
{
    unsigned int theOption = (inIgnoreCase) ? OgreIgnoreCaseOption : OgreNoneOption;
    OGRegularExpression *theRegex;
    NSEnumerator *theEnum;
    OGRegularExpressionMatch *theMatch;
    NSMutableArray *outArray = [NSMutableArray array];
    NSRange theAttrRange;
    unsigned int theQCStart = 0, theQCEnd = 0;

    NS_DURING
        theRegex = [OGRegularExpression regularExpressionWithString:inRegexStr options:theOption];
    NS_HANDLER
        // 例外処理 (OgreKit付属のRegularExpressionTestのコードを参考にしています)
        // 何もしない
        return nil;
    NS_ENDHANDLER

    theEnum = [theRegex matchEnumeratorInString:_localString];

    while (theMatch = [theEnum nextObject]) {
        if ((_isIndicatorShown) && 
                ([NSApp runModalSession:_modalSession] != NSRunContinuesResponse)) {
            return nil;
        }
        NSAutoreleasePool *thePool = [[NSAutoreleasePool alloc] init]; // ===== alloc
        theAttrRange = [theMatch rangeOfMatchedString];
        theAttrRange.location += _updateRange.location;
        if (inValueDoColoring) {
            [outArray addObject:[NSValue valueWithRange:theAttrRange]];
        } else {
            if (inPairKind >= k_QC_CommentBaseNum) {
                theQCStart = k_QC_Start;
                theQCEnd = k_QC_End;
            } else {
                theQCStart = theQCEnd = k_notUseStartEnd;
            }
            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                    [NSNumber numberWithUnsignedInt:(theAttrRange.location)], k_QCPosition, 
                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                    [NSNumber numberWithUnsignedInt:theQCStart], k_QCStartEnd, 
                    [NSNumber numberWithUnsignedInt:0], k_QCStrLength, 
                    nil]];
            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                    [NSNumber numberWithUnsignedInt:NSMaxRange(theAttrRange)], k_QCPosition, 
                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                    [NSNumber numberWithUnsignedInt:theQCEnd], k_QCStartEnd, 
                    [NSNumber numberWithUnsignedInt:0], k_QCStrLength, 
                    nil]];
        }
        [thePool release]; // ===== release
    }
    return outArray;
}


// ------------------------------------------------------
- (NSArray *)rangesRegularExpressionBeginString:(NSString *)inBeginString withEndString:(NSString *)inEndString 
        withIgnoreCase:(BOOL)inIgnoreCase
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind
// 指定された開始／終了文字列を正規表現として検索し、位置を返す
// ------------------------------------------------------
{
    uint32_t theOption = (inIgnoreCase) ? (RKLCaseless | RKLMultiline) : RKLMultiline;
    NSEnumerator *theEnumerator;
    NSArray *theMatchArray;
    NSRange theBeginRange, theEndRange, theAttrRange;
    CEPrivateMutableArray *outArray = nil;
    int i, theCount = 0;
    unsigned int theQCStart = 0, theQCEnd = 0;

    NS_DURING
        theEnumerator = [_localString matchEnumeratorWithRegex:inBeginString options:theOption];
        theMatchArray = [theEnumerator allObjects];
    NS_HANDLER
        // 何もしない
        NSLog(@"ERROR in \"rangesRegularExpressionBeginString:withEndString:withIgnoreCase:doColoring:pairStringKind:\" first NS_DURING");
        return nil;
    NS_ENDHANDLER

    theCount = [theMatchArray count];
    if (theCount > 0) {
        outArray = [[[CEPrivateMutableArray alloc] initWithCapacity:10] autorelease];
    }
    for (i = 0; i < theCount; i++) {
        if ((_isIndicatorShown) && ((i % 10) == 0) && 
                ([NSApp runModalSession:_modalSession] != NSRunContinuesResponse)) {
            return nil;
        }
        theBeginRange = [[theMatchArray objectAtIndex:i] rangeValue];
        NS_DURING
            theEndRange = [_localString rangeOfRegex:inEndString 
                        options:theOption 
                        inRange:NSMakeRange(NSMaxRange(theBeginRange), 
                            [_localString length] - NSMaxRange(theBeginRange))
                        capture:0 error:NULL];
        NS_HANDLER
            // 何もしない
            NSLog(@"ERROR in \"rangesRegularExpressionBeginString:withEndString:withIgnoreCase:doColoring:pairStringKind:\" second NS_DURING");
            return nil;
        NS_ENDHANDLER

        if (theEndRange.location != NSNotFound) {
            theAttrRange = NSUnionRange(theBeginRange, theEndRange);
        } else {
            continue;
        }
        if (inValueDoColoring) {
            [outArray addObject:[NSValue valueWithRange:theAttrRange]];
        } else {
            if (inPairKind >= k_QC_CommentBaseNum) {
                theQCStart = k_QC_Start;
                theQCEnd = k_QC_End;
            } else {
                theQCStart = theQCEnd = k_notUseStartEnd;
            }
            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                    [NSNumber numberWithUnsignedInt:(theAttrRange.location)], k_QCPosition, 
                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                    [NSNumber numberWithUnsignedInt:theQCStart], k_QCStartEnd, 
                    [NSNumber numberWithUnsignedInt:0], k_QCStrLength, 
                    nil]];
            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                    [NSNumber numberWithUnsignedInt:NSMaxRange(theAttrRange)], k_QCPosition, 
                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                    [NSNumber numberWithUnsignedInt:theQCEnd], k_QCStartEnd, 
                    [NSNumber numberWithUnsignedInt:0], k_QCStrLength, 
                    nil]];
        }
    }
    return outArray;
}
// ------------------------------------------------------
- (NSArray *)checkRegularExpressionBeginString:(NSString *)inBeginString withEndString:(NSString *)inEndString 
        withIgnoreCase:(BOOL)inIgnoreCase
        doColoring:(BOOL)inValueDoColoring pairStringKind:(unsigned int)inPairKind
// 指定された開始／終了文字列を正規表現として検索し、位置を返す
// ------------------------------------------------------
{
    unsigned int theOption = (inIgnoreCase) ? OgreIgnoreCaseOption : OgreNoneOption;
    OGRegularExpression *theRegex;
    NSEnumerator *theEnum;
    OGRegularExpressionMatch *theMatch;
    NSRange theBeginRange, theEndRange, theAttrRange;
    NSMutableArray *outArray = [NSMutableArray array];
    unsigned int theQCStart = 0, theQCEnd = 0;

    NS_DURING
        theRegex = [OGRegularExpression regularExpressionWithString:inBeginString options:theOption];
    NS_HANDLER
        // 構文エラーなど例外は無視 (OgreKit付属のRegularExpressionTestのコードを参考にしています)
        // 何もしない
        return nil;
    NS_ENDHANDLER

    theEnum = [theRegex matchEnumeratorInString:_localString];

    while (theMatch = [theEnum nextObject]) {
        if ((_isIndicatorShown) && 
                ([NSApp runModalSession:_modalSession] != NSRunContinuesResponse)) {
            return nil;
        }
        NSAutoreleasePool *thePool = [[NSAutoreleasePool alloc] init]; // ===== alloc
        theBeginRange = [theMatch rangeOfMatchedString];
        NS_DURING
            theEndRange = [_localString rangeOfRegularExpressionString:inEndString 
                        options:theOption 
                        range:NSMakeRange(NSMaxRange(theBeginRange), 
                            [_localString length] - NSMaxRange(theBeginRange))];
        NS_HANDLER
            [thePool release]; // ===== release
            return nil;
        NS_ENDHANDLER
        if (theEndRange.location != NSNotFound) {
            theAttrRange = NSUnionRange(theBeginRange, theEndRange);
            theAttrRange.location += _updateRange.location;
        } else {
            continue;
        }
        if (inValueDoColoring) {
            [outArray addObject:[NSValue valueWithRange:theAttrRange]];
        } else {
            if (inPairKind >= k_QC_CommentBaseNum) {
                theQCStart = k_QC_Start;
                theQCEnd = k_QC_End;
            } else {
                theQCStart = theQCEnd = k_notUseStartEnd;
            }
            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                    [NSNumber numberWithUnsignedInt:(theAttrRange.location)], k_QCPosition, 
                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                    [NSNumber numberWithUnsignedInt:theQCStart], k_QCStartEnd, 
                    [NSNumber numberWithUnsignedInt:0], k_QCStrLength, 
                    nil]];
            [outArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:
                    [NSNumber numberWithUnsignedInt:NSMaxRange(theAttrRange)], k_QCPosition, 
                    [NSNumber numberWithUnsignedInt:inPairKind], k_QCPairKind, 
                    [NSNumber numberWithUnsignedInt:theQCEnd], k_QCStartEnd, 
                    [NSNumber numberWithUnsignedInt:0], k_QCStrLength, 
                    nil]];
        }
        [thePool release]; // ===== release
    }
    return outArray;
}


// ------------------------------------------------------
- (void)setAttrToCommentsWithSyntaxArray:(NSArray *)inArray 
        withSingleQuotes:(BOOL)inValueSingleQuotes withDoubleQuotes:(BOOL)inValueDoubleQuotes 
        updateIndicator:(BOOL)inValueUpdateIndicator
// コメントをカラーリング
// ------------------------------------------------------
{
    NSMutableArray *thePosArray = [NSMutableArray array];
    NSMutableDictionary *theSimpleWordsDict = [NSMutableDictionary dictionaryWithCapacity:40];
    NSArray *theTmpArray = nil;
    NSDictionary *theStrDict, *theCurRecord, *theCheckRecord, *theAttrs;
    NSString *theBeginStr = nil, *theEndStr = nil;
    NSMutableString *theSimpleWordsChar = [NSMutableString string];
    NSRange theColoringRange;
    int i, j, theIndex = 0, theSyntaxCount = [inArray count], theColoringCount;
    unsigned int theQCKind, theStart, theEnd, theCheckStartEnd;
    double indicatorValue, theOldValue = 0.0, theBeginDouble = [self doubleValueOfIndicator];
    BOOL theBoolHasEnd = NO;

    // コメント定義の位置配列を生成
    for (i = 0; i < theSyntaxCount; i++) {
        if ((_isIndicatorShown) && ((i % 10) == 0) && 
                ([NSApp runModalSession:_modalSession] != NSRunContinuesResponse)) { return; }
        theStrDict = [inArray objectAtIndex:i];
        theBeginStr = [theStrDict objectForKey:k_SCKey_beginString];

        if ([theBeginStr length] < 1) { continue; }

        theEndStr = [theStrDict objectForKey:k_SCKey_endString];

        if ([[theStrDict objectForKey:k_SCKey_regularExpression] boolValue]) {
            if ((theEndStr != nil) && ([theEndStr length] > 0)) {
                if (_isPanther) {
                    theTmpArray = [self checkRegularExpressionBeginString:theBeginStr 
                            withEndString:theEndStr 
                            withIgnoreCase:[[theStrDict objectForKey:k_SCKey_ignoreCase] boolValue] 
                            doColoring:NO pairStringKind:(k_QC_CommentBaseNum + i)];
                } else {
                    theTmpArray = [self rangesRegularExpressionBeginString:theBeginStr 
                            withEndString:theEndStr 
                            withIgnoreCase:[[theStrDict objectForKey:k_SCKey_ignoreCase] boolValue] 
                            doColoring:NO pairStringKind:(k_QC_CommentBaseNum + i)];
                }
                [thePosArray addObjectsFromArray:theTmpArray];
            } else {
                if (_isPanther) {
                    theTmpArray = [self checkRegularExpressionString:theBeginStr 
                            withIgnoreCase:[[theStrDict objectForKey:k_SCKey_ignoreCase] boolValue] 
                            doColoring:NO pairStringKind:(k_QC_CommentBaseNum + i)];
                } else {
                    theTmpArray = [self rangesRegularExpressionString:theBeginStr 
                            withIgnoreCase:[[theStrDict objectForKey:k_SCKey_ignoreCase] boolValue] 
                            doColoring:NO pairStringKind:(k_QC_CommentBaseNum + i)];
                }
                [thePosArray addObjectsFromArray:theTmpArray];
            }
        } else {
            if ((theEndStr != nil) && ([theEndStr length] > 0)) {
                theTmpArray = [self rangesBeginString:theBeginStr withEndString:theEndStr 
                        doColoring:NO pairStringKind:(k_QC_CommentBaseNum + i)];
                [thePosArray addObjectsFromArray:theTmpArray];
            } else {
                NSNumber *len = [NSNumber numberWithInt:[theBeginStr length]];
                id wordsArray = [theSimpleWordsDict objectForKey:len];
                if (wordsArray) {
                    [wordsArray addObject:theBeginStr];
                } else {
                    wordsArray = [NSMutableArray arrayWithObject:theBeginStr];
                    [theSimpleWordsDict setObject:wordsArray forKey:len];
                }
                [theSimpleWordsChar appendString:theBeginStr];
            }
        }
    } // end-for
    // シングルクォート定義があれば位置配列を生成、マージ
    if (inValueSingleQuotes) {
        [thePosArray addObjectsFromArray:
                [self rangesBeginString:@"\'" withEndString:@"\'" 
                    doColoring:NO pairStringKind:k_QC_SingleQ]];
    }
    // ダブルクォート定義があれば位置配列を生成、マージ
    if (inValueDoubleQuotes) {
        [thePosArray addObjectsFromArray:
                [self rangesBeginString:@"\"" withEndString:@"\"" 
                    doColoring:NO pairStringKind:k_QC_DoubleQ]];
    }
    // コメントもクォートもなければ、もどる
    if (([thePosArray count] < 1) && ([theSimpleWordsDict count] < 1)) { return; }

    // まず、開始文字列だけのコメント定義があればカラーリング
    if (([theSimpleWordsDict count]) > 0) {
        [self setAttrToSimpleWordsArrayDict:theSimpleWordsDict withCharString:theSimpleWordsChar];
    }

    // カラーリング対象がなければ、もどる
    if ([thePosArray count] < 1) { return; }
    NSSortDescriptor *theDescriptor = 
            [[[NSSortDescriptor alloc] initWithKey:k_QCPosition ascending:YES] autorelease];
    [thePosArray sortUsingDescriptors:[NSArray arrayWithObject:theDescriptor]];
    theColoringCount = [thePosArray count];

    theQCKind = k_notUseKind;
    while (theIndex < theColoringCount) {
        // インジケータ更新
        if ((inValueUpdateIndicator) && ((theIndex % 10) == 0)) {
            indicatorValue = theBeginDouble + (double)(theIndex / (double)theColoringCount * 200);
            [self setDoubleIndicator:(double)indicatorValue];
        }
        theCurRecord = [thePosArray objectAtIndex:theIndex];
        if (theQCKind == k_notUseKind) {
            if ([[theCurRecord objectForKey:k_QCStartEnd] unsignedIntValue] == k_QC_End) {
                theIndex++;
                continue;
            }
            theQCKind = [[theCurRecord objectForKey:k_QCPairKind] unsignedIntValue];
            theStart = [[theCurRecord objectForKey:k_QCPosition] unsignedIntValue];
            theIndex++;
            continue;
        }
        if (theQCKind == [[theCurRecord objectForKey:k_QCPairKind] unsignedIntValue]) {
            if (theQCKind == k_QC_SingleQ) {
                theAttrs = _singleQuotesAttrs;
            } else if (theQCKind == k_QC_DoubleQ) {
                theAttrs = _doubleQuotesAttrs;
            } else if (theQCKind >= k_QC_CommentBaseNum) {
                theAttrs = _currentAttrs;
            } else {
                NSLog(@"setAttrToCommentsWithSyntaxArray:withSyngleQuotes::... \n Can not set Attrs.");
                break;
            }
            theEnd = [[theCurRecord objectForKey:k_QCPosition] unsignedIntValue] + 
                    [[theCurRecord objectForKey:k_QCStrLength] unsignedIntValue];
            theColoringRange = NSMakeRange(theStart + _updateRange.location, theEnd - theStart);
            if ([self isPrinting]) {
                [[_layoutManager firstTextView] setTextColor:
                        [theAttrs objectForKey:NSForegroundColorAttributeName] range:theColoringRange];
            } else {
                [_layoutManager addTemporaryAttributes:theAttrs forCharacterRange:theColoringRange];
            }
            theQCKind = k_notUseKind;
            theIndex++;
        } else {
            // 「終わり」があるか調べる
            for (j = (theIndex + 1); j < theColoringCount; j++) {
                theCheckRecord = [thePosArray objectAtIndex:j];
                if (theQCKind == [[theCheckRecord objectForKey:k_QCPairKind] unsignedIntValue]) {
                    theCheckStartEnd = [[theCheckRecord objectForKey:k_QCStartEnd] unsignedIntValue];
                    if ((theCheckStartEnd == k_notUseStartEnd) || (theCheckStartEnd == k_QC_End)) {
                        theBoolHasEnd = YES;
                        break;
                    }
                }
                theBoolHasEnd = NO;
            }
            // 「終わり」があればそこへジャンプ、なければ最後までカラーリングして、抜ける
            if (theBoolHasEnd) {
                theIndex = j;
            } else {
                if (theQCKind == k_QC_SingleQ) {
                    theAttrs = _singleQuotesAttrs;
                } else if (theQCKind == k_QC_DoubleQ) {
                    theAttrs = _doubleQuotesAttrs;
                } else if (theQCKind >= k_QC_CommentBaseNum) {
                    theAttrs = _currentAttrs;
                } else {
                    NSLog(@"setAttrToCommentsWithSyntaxArray:withSyngleQuotes::... \n Can not set Attrs.");
                    break;
                }
                theColoringRange = 
                        NSMakeRange(theStart + _updateRange.location, NSMaxRange(_updateRange) - theStart);
                if ([self isPrinting]) {
                    [[_layoutManager firstTextView] setTextColor:
                            [theAttrs objectForKey:NSForegroundColorAttributeName] range:theColoringRange];
                } else {
                    [_layoutManager addTemporaryAttributes:theAttrs forCharacterRange:theColoringRange];
                }
                break;
            }
        }
    }
}


// ------------------------------------------------------
- (unsigned int)numberOfEscapeSequenceInString:(NSString *)inString
// 与えられた文字列の末尾にエスケープシーケンス（バックスラッシュ）がいくつあるかを返す
// ------------------------------------------------------
{
    unsigned int outCount = 0, theLength = [inString length];
    int i;

    for (i = (theLength - 1); i >= 0; i--) {
        if ([inString characterAtIndex:i] == '\\') {
            outCount++;
        } else {
            break;
        }
    }
    return outCount;
}


// ------------------------------------------------------
- (void)setOtherInvisibleCharsAttrs
// 不可視文字表示時に文字色を変更する
// ------------------------------------------------------
{
    if (![_layoutManager showOtherInvisibles]) { return; }
    id theValues = [[NSUserDefaultsController sharedUserDefaultsController] values];
    NSColor *theColor = 
            [NSUnarchiver unarchiveObjectWithData:[theValues valueForKey:k_key_invisibleCharactersColor]];
    if ([[_layoutManager firstTextView] textColor] == theColor) { return; }
    NSDictionary *theAttrs;
    NSMutableArray *theTargetArray = [NSMutableArray array];
    NSScanner *theScanner = [NSScanner scannerWithString:_localString];
    NSString *theControlStr;
    NSRange theColoringRange;
    int theStart, i, theMax;

    if (![self isPrinting]) {
        theAttrs = [NSDictionary dictionaryWithObjectsAndKeys:theColor, NSForegroundColorAttributeName,  nil];
    }

    while (![theScanner isAtEnd]) {
        (void)[theScanner scanUpToCharactersFromSet:[NSCharacterSet controlCharacterSet] intoString:nil];
        theStart = [theScanner scanLocation];
        if ([theScanner scanCharactersFromSet:[NSCharacterSet controlCharacterSet] 
                    intoString:&theControlStr]) {
            [theTargetArray addObject:
                        [NSValue valueWithRange:
                            NSMakeRange(theStart, [theControlStr length])]];
        }
    }
    theMax = [theTargetArray count];
    if ([self isPrinting]) {
        for (i = 0; i < theMax; i++) {
            theColoringRange = [[theTargetArray objectAtIndex:i] rangeValue];
            theColoringRange.location += _updateRange.location;
            [[_layoutManager firstTextView] setTextColor:theColor range:theColoringRange];
        }
    } else {
        for (i = 0; i < theMax; i++) {
            theColoringRange = [[theTargetArray objectAtIndex:i] rangeValue];
            theColoringRange.location += _updateRange.location;
            [_layoutManager addTemporaryAttributes:theAttrs forCharacterRange:theColoringRange];
        }
    }
}


// ------------------------------------------------------
- (void)doColoring
// カラーリングを実行
// ------------------------------------------------------
{
    id theValues = [[NSUserDefaultsController sharedUserDefaultsController] values];

    unsigned int theLength = [self wholeStringLength];
    if (theLength < 1) { return; }
    [self setLocalString:[_wholeString substringWithRange:_updateRange]]; // カラーリング対象文字列を保持
    if ([_localString length] < 1) { return; }

    // 現在あるカラーリングを削除、カラーリング不要なら不可視文字のカラーリングだけして戻る
    [_layoutManager removeTemporaryAttribute:NSForegroundColorAttributeName forCharacterRange:_updateRange];
    if (([[_coloringDictionary objectForKey:k_SCKey_numOfObjInArray] intValue] == 0) || 
            ([[self syntaxStyleName] isEqualToString:NSLocalizedString(@"None",@"")])) {
        [self setOtherInvisibleCharsAttrs];
        return;
    }

    // 規定の文字数以上の場合にはカラーリングインジケータシートを表示
    // （ただし、k_key_showColoringIndicatorTextLength が「0」の時は表示しない）
    NSWindow *theDocWindow = nil;
    NSWindow *theSheet = nil;
    if ((_showColoringIndicatorTextLength > 0) && 
                (_updateRange.length > _showColoringIndicatorTextLength)) {
        _isIndicatorShown = YES;
        [self setDoubleIndicator:0];
        if ([self isPrinting]) {
            theDocWindow = [NSApp mainWindow];
            [_coloringCaption setStringValue:NSLocalizedString(@"Print text Coloring ...",@"")];
        } else {
            theDocWindow = [[_layoutManager firstTextView] window];
            [_coloringCaption setStringValue:NSLocalizedString(@"Text Coloring ...",@"")];
        }
        theSheet = [_coloringIndicator window];
        [NSApp beginSheet:theSheet 
                modalForWindow:theDocWindow 
                modalDelegate:self 
                didEndSelector:NULL 
                contextInfo:NULL];
        _modalSession = [NSApp beginModalSessionForWindow:theSheet];
    }

    NSArray *theColorArray = [NSArray arrayWithObjects:k_key_allSyntaxColors, nil];
    NSArray *theSyntaxArray = [NSArray arrayWithObjects:k_SCKey_allColoringArrays, nil];
    NSArray *theArray, *theInArray;
    NSMutableDictionary *theSimpleWordsDict = [NSMutableDictionary dictionaryWithCapacity:40];
    NSMutableString *theSimpleWordsChar = [NSMutableString stringWithString:k_allAlphabetChars];
    NSString *theBeginStr = nil, *theEndStr = nil;
    NSDictionary *theStrDict;
    NSRange theColoringRange;
    int i, j, theCount, theSyntaxArrayCount = [theSyntaxArray count];
    int k,l, theTargetCount, theInArrayCount;
    BOOL theBoolIsSingleQuotes = NO, theBoolIsDoubleQuotes = NO;
    double indicatorValue, theOldValue = 0.0, theBeginDouble = 0.0;

    NS_DURING
        // Keywords > Commands > Values > Numbers > Strings > Characters > Comments
        for (i = 0; i < theSyntaxArrayCount; i++) {

            if ((_isIndicatorShown) && ([NSApp runModalSession:_modalSession] != NSRunContinuesResponse)) {
                // キャンセルされたら、現在あるカラーリング（途中まで色づけられたもの）を削除して戻る
                if ([self isPrinting]) {
                    [[_layoutManager firstTextView] setTextColor:[[_layoutManager firstTextView] textColor] 
                                range:_updateRange];
                } else {
                    [_layoutManager removeTemporaryAttribute:NSForegroundColorAttributeName 
                                forCharacterRange:_updateRange];
                    [[[CEDocumentController sharedDocumentController] documentForWindow:theDocWindow] 
                                    doSetSyntaxStyle:NSLocalizedString(@"None",@"") delay:YES];
                }
                break;
            }

            theArray = [_coloringDictionary objectForKey:[theSyntaxArray objectAtIndex:i]];
            theCount = [theArray count];
            if (theCount < 1) {
                if (_isIndicatorShown) {
                    [self setDoubleIndicator:((i + 1) * 100.0)];
                }
                continue;
            }
            _textColor = [[NSUnarchiver unarchiveObjectWithData:
                    [theValues valueForKey:[theColorArray objectAtIndex:i]]] retain]; // ===== retain
            _currentAttrs = [[NSDictionary dictionaryWithObjectsAndKeys:_textColor, 
                    NSForegroundColorAttributeName, nil] retain]; // ===== retain

            // シングル／ダブルクォートのカラーリングがあったら、コメントとともに別メソッドでカラーリングする
            if ([[theSyntaxArray objectAtIndex:i] isEqualToString:k_SCKey_commentsArray]) {
                [self setAttrToCommentsWithSyntaxArray:theArray withSingleQuotes:theBoolIsSingleQuotes 
                        withDoubleQuotes:theBoolIsDoubleQuotes updateIndicator:_isIndicatorShown];
                [_textColor release]; // ===== release
                [_currentAttrs release]; // ===== release
                _currentAttrs = nil;
                break;
            }

            if (_isIndicatorShown) {
                theBeginDouble = [self doubleValueOfIndicator];
            }
            CEPrivateMutableArray *theTargetArray = 
                        [[[CEPrivateMutableArray alloc] initWithCapacity:10] autorelease];
            NSArray *theTmpArray = nil;
            NSAutoreleasePool *thePool = [[NSAutoreleasePool alloc] init]; // ===== alloc
            for (j = 0; j < theCount; j++) {
                theStrDict = [theArray objectAtIndex:j];
                theBeginStr = [theStrDict objectForKey:k_SCKey_beginString];

                if ([theBeginStr length] < 1) { continue; }

                theEndStr = [theStrDict objectForKey:k_SCKey_endString];

                if ([[theStrDict objectForKey:k_SCKey_regularExpression] boolValue]) {
                    if ((theEndStr != nil) && ([theEndStr length] > 0)) {
                        if (_isPanther) {
                            theTmpArray = 
                                [self checkRegularExpressionBeginString:theBeginStr withEndString:theEndStr 
                                    withIgnoreCase:[[theStrDict objectForKey:k_SCKey_ignoreCase] boolValue] 
                                    doColoring:YES pairStringKind:k_notUseKind];
                        } else {
                            theTmpArray = 
                                [self rangesRegularExpressionBeginString:theBeginStr withEndString:theEndStr 
                                    withIgnoreCase:[[theStrDict objectForKey:k_SCKey_ignoreCase] boolValue] 
                                    doColoring:YES pairStringKind:k_notUseKind];
                        }
                        if (theTmpArray != nil) {
                            [theTargetArray addObject:theTmpArray];
                        }
                    } else {
                        if (_isPanther) {
                            theTmpArray = [self checkRegularExpressionString:theBeginStr 
                                        withIgnoreCase:[[theStrDict objectForKey:k_SCKey_ignoreCase] boolValue] 
                                        doColoring:YES pairStringKind:k_notUseKind];
                        } else {
                            theTmpArray = [self rangesRegularExpressionString:theBeginStr 
                                        withIgnoreCase:[[theStrDict objectForKey:k_SCKey_ignoreCase] boolValue] 
                                        doColoring:YES pairStringKind:k_notUseKind];
                        }
                        if (theTmpArray != nil) {
                            [theTargetArray addObject:theTmpArray];
                        }
                    }
                } else {
                    if ((theEndStr != nil) && ([theEndStr length] > 0)) {
                        // 開始／終了ともに入力されていたらクォートかどうかをチェック、最初に出てきたクォートのみを把握
                        if ([theBeginStr isEqualToString:@"\'"] && [theEndStr isEqualToString:@"\'"]) {
                            if (!theBoolIsSingleQuotes) {
                                theBoolIsSingleQuotes = YES;
                                _singleQuotesAttrs = [_currentAttrs retain]; // ===== retain
                            }
                            continue;
                        }
                        if ([theBeginStr isEqualToString:@"\""] && [theEndStr isEqualToString:@"\""]) {
                            if (!theBoolIsDoubleQuotes) {
                                theBoolIsDoubleQuotes = YES;
                                _doubleQuotesAttrs = [_currentAttrs retain]; // ===== retain
                            }
                            continue;
                        }
                        theTmpArray = [self rangesBeginString:theBeginStr withEndString:theEndStr 
                                    doColoring:YES pairStringKind:k_notUseKind];
                        if (theTmpArray != nil) {
                            [theTargetArray addObject:theTmpArray];
                        }
                    } else {
                        NSNumber *len = [NSNumber numberWithInt:[theBeginStr length]];
                        id wordsArray = [theSimpleWordsDict objectForKey:len];
                        if (wordsArray) {
                            [wordsArray addObject:theBeginStr];
                        } else {
                            wordsArray = [NSMutableArray arrayWithObject:theBeginStr];
                            [theSimpleWordsDict setObject:wordsArray forKey:len];
                        }
                        [theSimpleWordsChar appendString:theBeginStr];
                    }
                }
                // インジケータ更新
                if ((_isIndicatorShown) && ((j % 10) == 0)) {
                    indicatorValue = theBeginDouble + (double)(j / (double)theCount * k_perCompoIncrement);
                    [self setDoubleIndicator:(double)indicatorValue];
                    [_coloringIndicator displayIfNeeded];
                }
                if (j % 100 == 0) {
                    [thePool release]; // ===== release
                    thePool = [[NSAutoreleasePool alloc] init]; // ===== alloc
                }
            } // end-for (j)
            [thePool release]; // ===== release
            if (([theSimpleWordsDict count]) > 0) {
                theTmpArray = [self rangesSimpleWordsArrayDict:theSimpleWordsDict 
                            withCharString:theSimpleWordsChar];
                if (theTmpArray != nil) {
                    [theTargetArray addObject:theTmpArray];
                }
                [theSimpleWordsDict removeAllObjects];
                [theSimpleWordsChar setString:k_allAlphabetChars];
            }
            // カラーリング実行
            theTargetCount = [theTargetArray count]; // = 配列内の配列の数
            for (k = 0; k < theTargetCount; k++) {
                theInArray = [theTargetArray objectAtIndex:k];
                theInArrayCount = [theInArray count];

                // IMP を使ってメソッド呼び出しを高速化
                // http://www.mulle-kybernetik.com/artikel/Optimization/opti-3.html
                // http://homepage.mac.com/mkino2/spec/optimize/methodCall.html
                if ([self isPrinting]) {
                    IMP impSetTextColor = [[_layoutManager firstTextView] methodForSelector:
                                            @selector(setTextColor:range:)];
                    for (l = 0; l < theInArrayCount; l++) {
                        theColoringRange = [[theInArray objectAtIndex:l] rangeValue];
                        theColoringRange.location += _updateRange.location;
                        impSetTextColor([_layoutManager firstTextView], 
                                    @selector(setTextColor:range:), 
                                    _textColor, theColoringRange);
                    }
                } else {
                    IMP impAddTempAttrs = [_layoutManager methodForSelector:
                                            @selector(addTemporaryAttributes:forCharacterRange:)];
                    for (l = 0; l < theInArrayCount; l++) {
                        theColoringRange = [[theInArray objectAtIndex:l] rangeValue];
                        theColoringRange.location += _updateRange.location;
                        impAddTempAttrs(_layoutManager, 
                                    @selector(addTemporaryAttributes:forCharacterRange:), 
                                    _currentAttrs, theColoringRange);
                    }
                }
            }
            if (_isIndicatorShown) {
                [self setDoubleIndicator:((i + 1) * 100.0)];
            }
            [_textColor release]; // ===== release
            [_currentAttrs release]; // ===== release
            _currentAttrs = nil;
        } // end-for (i)
        [self setOtherInvisibleCharsAttrs];
    NS_HANDLER
        // 何もしない
        NSLog(@"ERROR in \"doColoring\"");
    NS_ENDHANDLER

    // インジーケータシートを片づける
    if (_isIndicatorShown) {
        [NSApp endModalSession:_modalSession];
        [NSApp endSheet:theSheet];
        [theSheet orderOut:self];
        _isIndicatorShown = NO;
        _modalSession = nil;
    }
    // 不要な変数を片づける
    if (_singleQuotesAttrs != nil) {
        [_singleQuotesAttrs release];
        _singleQuotesAttrs = nil;
    }
    if (_doubleQuotesAttrs != nil) {
        [_doubleQuotesAttrs release];
        _doubleQuotesAttrs = nil;
    }
    [self setLocalString:nil];
}


// ------------------------------------------------------
- (double)doubleValueOfIndicator
// カラーリングインジケータの値を返す
// ------------------------------------------------------
{
    return [_coloringIndicator doubleValue];
}


// ------------------------------------------------------
- (void)setDoubleIndicator:(double)inValue
// カラーリングインジケータの値を設定
// ------------------------------------------------------
{
    [_coloringIndicator setDoubleValue:inValue];
}



@end
