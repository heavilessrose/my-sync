/*
=================================================
CEScriptManager
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

#import "CEScriptManager.h"
#import "CEDocument.h"
#import "NSEventAdditions.h"

//=======================================================
// Private method
//
//=======================================================

@interface CEScriptManager (Private)
- (void)setupMenuIcon;
- (NSString *)pathOfScriptDirectory;
- (void)addChildFileItemTo:(NSMenu *)inMenu fromDir:(NSString *)inPath;
- (void)removeAllMenuItemsFromParent:(NSMenu *)inMenu;
- (NSString *)menuTitleFromFileName:(NSString *)inFileName;
- (NSString *)keyEquivalentAndModifierMask:(unsigned int *)ioModMask fromFileName:(NSString *)inFileName;
- (void)showAlert:(NSString *)inMessage;
- (NSString *)stringOfScript:(NSString *)inPath;
- (void)doLaunchShellScript:(NSString *)inPath;
- (void)availableOutput:(NSNotification *)inNotification;
- (void)showScriptErrorLog:(NSString *)inLogString;
@end


//------------------------------------------------------------------------------------------




@implementation CEScriptManager

static CEScriptManager *sharedInstance = nil;

#pragma mark ===== Class method =====

//=======================================================
// Class method
//
//=======================================================

// ------------------------------------------------------
+ (CEScriptManager *)sharedInstance
// 共有インスタンスを返す
// ------------------------------------------------------
{
    return sharedInstance ? sharedInstance : [[self alloc] init];
}



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
    if (sharedInstance == nil) {
        self = [super init];
        [self setupMenuIcon];
        _outputType = k_noOutput;
        _outputHandle = nil;
        _errorHandle = nil;
        (void)[NSBundle loadNibNamed:@"ScriptManager" owner:self];
        // ノーティフィケーションセンタへデータ出力読み込み完了の通知を依頼
        [[NSNotificationCenter defaultCenter] addObserver:self 
            selector:@selector(availableOutput:) 
            name:NSFileHandleReadToEndOfFileCompletionNotification 
            object:nil];
        sharedInstance = self;
    }
    return sharedInstance;
}


// ------------------------------------------------------
- (void)dealloc
// あとかたづけ
// ------------------------------------------------------
{
    // ノーティフィケーションセンタから自身を排除
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    // NSBundle loadNibNamed: でロードされたオブジェクトを開放
    // 参考にさせていただきました > http://homepage.mac.com/mkino2/backnumber/2004_10.html#October%2012_1
    [[_errorTextView window] release]; // （コンテントビューは自動解放される）

    [_outputHandle release];
    [_errorHandle release];

    _errorTextView = nil;

    [super dealloc];
}


//------------------------------------------------------
- (void)buildScriptMenu:(id)sender
// Scriptメニューを生成
//------------------------------------------------------
{
    NSString *theDirPath = [self pathOfScriptDirectory]; // データディレクトリパス取得

    // ディレクトリの存在チェック
    NSFileManager *theFileManager = [NSFileManager defaultManager];
    BOOL theBoolIsDir = NO, theBoolCreated = NO;
    BOOL theExists = [theFileManager fileExistsAtPath:theDirPath isDirectory:&theBoolIsDir];
    if (!theExists) {
        // 0.6.3以前の古いディレクトリ名がある場合はそれをリネームして使う。古いドキュメントは削除する。
        NSString *theOldPath = [NSHomeDirectory( ) 
                stringByAppendingPathComponent:@"Library/Application Support/CotEditor/AppleScriptMenu"];
        BOOL theBoolOldIsDir = NO;
        BOOL theBoolOldExists = [theFileManager fileExistsAtPath:theOldPath isDirectory:&theBoolOldIsDir];
        if (theBoolOldExists && theBoolOldIsDir) {
            theBoolCreated = [theFileManager movePath:theOldPath toPath:theDirPath handler:nil];
            NSString *theOldAboutDocPath = [NSHomeDirectory( ) 
                stringByAppendingPathComponent:
                @"Library/Application Support/CotEditor/ScriptMenu/_aboutAppleScriptFolder.rtf"];
            (void)[theFileManager removeFileAtPath:theOldAboutDocPath handler:nil];
        } else {
            theBoolCreated = [theFileManager createDirectoryAtPath:theDirPath attributes:nil];
        }
    }
    if ((!theExists) && (!theBoolCreated)) {
        NSLog(@"Error. ScriptMenu directory could not found.");
        return;
    }

    // About 文書をコピー
    NSString *theSource = 
            [[[NSBundle mainBundle] bundlePath] 
            stringByAppendingPathComponent:@"/Contents/Resources/_aboutScriptFolder.rtf"];
    NSString *theDestination = [theDirPath stringByAppendingPathComponent:@"_aboutScriptFolder.rtf"];
    if (([theFileManager fileExistsAtPath:theSource]) && 
                (![theFileManager fileExistsAtPath:theDestination])) {
        if (![theFileManager copyPath:theSource toPath:theDestination handler:nil]) {
            NSLog(@"Error. AppleScriptFolder about document could not copy.");
        }
    }
    // メニューデータの読み込みとメニュー構成
    NSMenu *theASMenu = [[[NSApp mainMenu] itemAtIndex:k_scriptMenuIndex] submenu];
    [self removeAllMenuItemsFromParent:theASMenu];
    NSMenuItem *theMenuItem;

    [self addChildFileItemTo:theASMenu fromDir:theDirPath]; 
    if ([theASMenu numberOfItems] > 0) {
        [theASMenu addItem:[NSMenuItem separatorItem]];
    }
    theMenuItem = [[[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Open Scripts Folder",@"") 
                action:@selector(openScriptFolder:) 
                keyEquivalent:@""] autorelease];
    [theMenuItem setTarget:self];
    [theASMenu addItem:theMenuItem];
    theMenuItem = [[[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Update Script Menu",@"") 
                action:@selector(buildScriptMenu:) 
                keyEquivalent:@""] autorelease];
    [theMenuItem setTarget:self];
    [theASMenu addItem:theMenuItem];
}


//------------------------------------------------------
- (NSMenu *)contexualMenu
// コンテキストメニュー用のメニューを返す
//------------------------------------------------------
{
    NSMenu *theASMenu = [[[NSApp mainMenu] itemAtIndex:k_scriptMenuIndex] submenu];

    return [[theASMenu copy] autorelease];
}


//------------------------------------------------------
- (void)launchScript:(id)sender
// Script実行
//------------------------------------------------------
{
    NSString *thePath = nil;
    if ([sender isMemberOfClass:[NSMenuItem class]]) {
        thePath = [sender representedObject];
    }
    if (thePath == nil) { return; }

    // ファイルがない場合は警告して抜ける
    if (![[NSFileManager defaultManager] fileExistsAtPath:thePath]) {
        [self showAlert:[NSString stringWithFormat:
                NSLocalizedString(@"The Script \"%@\" NOT exists.\n\nCheck it and do \"Update Script Menu\".",@""), thePath]];
        return;
    }

    // Optキーが押されていたら、アプリでスクリプトを開く
    unsigned int theFlags = [NSEvent currentCarbonModifierFlags];
    NSString *theXtsn = [thePath pathExtension];
    NSString *theMessage = nil;
    BOOL theModifierPressed = NO;
    BOOL theResult = YES;
    if (theFlags == NSAlternateKeyMask) {
        theModifierPressed = YES;
        if (([theXtsn isEqualToString:@"applescript"]) || ([theXtsn isEqualToString:@"scpt"])) {
            theResult = [[NSWorkspace sharedWorkspace] openFile:thePath withApplication:@"Script Editor"];
        } else if (([theXtsn isEqualToString:@"sh"]) || ([theXtsn isEqualToString:@"pl"]) || 
                ([theXtsn isEqualToString:@"php"]) || ([theXtsn isEqualToString:@"rb"]) || 
                ([theXtsn isEqualToString:@"py"])) {
            theResult = [[NSWorkspace sharedWorkspace] openFile:thePath 
                        withApplication:[[NSBundle mainBundle] bundlePath]];
        }
        if (!theResult) {
            theMessage = [NSString stringWithFormat:
                    NSLocalizedString(@"The Script file \"%@\" could not open.",@""), thePath];
        }
    } else if (theFlags == (NSAlternateKeyMask | NSShiftKeyMask)) {
        theModifierPressed = YES;
        theResult = [[NSWorkspace sharedWorkspace] selectFile:thePath inFileViewerRootedAtPath:@""];
        if (!theResult) {
            theMessage = [NSString stringWithFormat:
                    NSLocalizedString(@"The Script file \"%@\" could not select.",@""), thePath];
        }
    }
    if ((!theResult) && (theMessage != nil)) {
        // 開けなかったり選択できなければその旨を表示
        [self showAlert:theMessage];
    }
    if (theModifierPressed) {
        return;
    }

    if (([theXtsn isEqualToString:@"applescript"]) || ([theXtsn isEqualToString:@"scpt"])) {
        NSAppleScript *theAppleScript = nil;
        NSDictionary *theErrorInfo = nil;
        NSAppleEventDescriptor *theDescriptor;
        if (([theXtsn isEqualToString:@"applescript"]) || 
                ([theXtsn isEqualToString:@"scpt"])) {
            NSURL *theURL = [NSURL fileURLWithPath:thePath];
            theAppleScript = [[[NSAppleScript alloc] initWithContentsOfURL:theURL 
                        error:&theErrorInfo] autorelease];
        }

        if (theAppleScript != nil) {
            theDescriptor = [theAppleScript executeAndReturnError:&theErrorInfo];
        }
        // エラーが発生したら、表示
        if (((theAppleScript == nil) || (theDescriptor == nil)) && (theErrorInfo != nil)) {
            [self showAlert:[NSString stringWithFormat:
                    NSLocalizedString(@"%@\nErrorNumber: %i",@""), 
                    [theErrorInfo valueForKey:NSAppleScriptErrorMessage], 
                    [[theErrorInfo valueForKey:NSAppleScriptErrorNumber] intValue]]];
        }
    } else if (([theXtsn isEqualToString:@"sh"]) || ([theXtsn isEqualToString:@"pl"]) || 
            ([theXtsn isEqualToString:@"php"]) || ([theXtsn isEqualToString:@"rb"]) || 
            ([theXtsn isEqualToString:@"py"])) {

        // 実行権限がない場合は警告して抜ける
        if (![[NSFileManager defaultManager] isExecutableFileAtPath:thePath]) {
            [self showAlert:[NSString stringWithFormat:
                    NSLocalizedString(@"The Script \"%@\" NOT be able to execute.\nShell scripts have to have execute permission.\n\nCheck it\'s permission.",@""), thePath]];
            return;
        }
        [self doLaunchShellScript:thePath];
    }
}


// ------------------------------------------------------
- (void)openScriptErrorWindow
// Scriptエラーウィンドウを表示
// ------------------------------------------------------
{
    [[_errorTextView window] orderFront:self];
}



#pragma mark ===== Protocol =====

//=======================================================
// NSNibAwaking Protocol
//
//=======================================================

// ------------------------------------------------------
- (void)awakeFromNib
// Nibファイル読み込み直後
// ------------------------------------------------------
{
    // 自動スペルチェックをオフ
    [_errorTextView setContinuousSpellCheckingEnabled:NO]; // nib での設定が有効にならないため、ここで設定している
    // フォント指定
    [_errorTextView setFont:[NSFont messageFontOfSize:10]];
}


//=======================================================
// NSMenuValidation Protocol
//
//=======================================================

// ------------------------------------------------------
- (BOOL)validateMenuItem:(NSMenuItem *)inMenuItem
// メニュー項目の有効・無効を制御
// ------------------------------------------------------
{
    return YES;
}



#pragma mark ===== Action messages =====

//=======================================================
// Action messages
//
//=======================================================

// ------------------------------------------------------
- (IBAction)openScriptFolder:(id)sender
// ScriptフォルダウィンドウをFinderで表示
// ------------------------------------------------------
{
    NSString *thePath = [[NSBundle mainBundle] pathForResource:@"openScriptMenu" ofType:@"applescript"];
    if (thePath == nil) { return; }
    NSURL *theURL = [NSURL fileURLWithPath:thePath];
    NSAppleScript *theAppleScript = [[[NSAppleScript alloc] initWithContentsOfURL:theURL error:nil] autorelease];

    if (theAppleScript != nil) {
        (void)[theAppleScript executeAndReturnError:nil];
    }
}


// ------------------------------------------------------
- (IBAction)cleanScriptError:(id)sender
// Scriptエラーログを削除
// ------------------------------------------------------
{
    [_errorTextView setString:@""];
}



@end


@implementation CEScriptManager (Private)

//=======================================================
// Private method
//
//=======================================================

//------------------------------------------------------
- (void)setupMenuIcon
// メニューバーにアイコンを表示
//------------------------------------------------------
{
    NSMenuItem *theASMenuItem = [[NSApp mainMenu] itemAtIndex:k_scriptMenuIndex];

    [theASMenuItem setTitle:NSLocalizedString(@"Script Menu",@"")];
    [theASMenuItem setImage:[NSImage imageNamed:@"scriptMenuIcon"]];
}


//------------------------------------------------------
- (NSString *)pathOfScriptDirectory
// Scriptファイル保存用ディレクトリを返す
//------------------------------------------------------
{
    NSString *outPath = [NSHomeDirectory( ) 
            stringByAppendingPathComponent:@"Library/Application Support/CotEditor/ScriptMenu"];

    return outPath;
}


//------------------------------------------------------
- (void)addChildFileItemTo:(NSMenu *)inMenu fromDir:(NSString *)inPath
// ファイルを読み込みメニューアイテムを生成／追加する
//------------------------------------------------------
{
    NSFileManager *theFileManager = [NSFileManager defaultManager];
    NSArray *theFiles = [theFileManager directoryContentsAtPath:inPath];
    NSString *thePath, *theMenuTitle;
    NSMenuItem *theMenuItem;
    int i;

    for (i = 0; i < [theFiles count]; i++) {
        NSString *theFileName = [theFiles objectAtIndex:i];
        thePath = [inPath stringByAppendingPathComponent:theFileName];
        NSString *theXtsn = [thePath pathExtension];
        NSDictionary *theAttrs = [theFileManager fileAttributesAtPath:thePath traverseLink:NO];
        if (!theAttrs) { continue; }
        if ([theAttrs valueForKey:NSFileType] == NSFileTypeDirectory) {
            theMenuTitle = [self menuTitleFromFileName:theFileName];
            if ([theMenuTitle isEqualToString:@"-"]) { // セパレータ
                [inMenu addItem:[NSMenuItem separatorItem]];
                continue;
            }
            NSMenu *theSubMenu = [[[NSMenu alloc] initWithTitle:theMenuTitle] autorelease];
            theMenuItem = [[[NSMenuItem alloc] initWithTitle:theMenuTitle 
                            action:nil keyEquivalent:@""] autorelease];
            [theMenuItem setTag:k_scriptMenuDirectoryTag];
            [inMenu addItem:theMenuItem];
            [theMenuItem setSubmenu:theSubMenu];
            [self addChildFileItemTo:theSubMenu fromDir:thePath];
        } else if (([theAttrs valueForKey:NSFileType] == NSFileTypeRegular) && 
                (([theXtsn isEqualToString:@"applescript"]) || 
                ([theXtsn isEqualToString:@"scpt"]) || 
                ([theXtsn isEqualToString:@"sh"]) || 
                ([theXtsn isEqualToString:@"pl"]) || 
                ([theXtsn isEqualToString:@"php"]) || 
                ([theXtsn isEqualToString:@"rb"]) || 
                ([theXtsn isEqualToString:@"py"]))) {
            unsigned int theMod = 0;
            NSString *theKeyEquivalent = [self keyEquivalentAndModifierMask:&theMod fromFileName:theFileName];
            theMenuTitle = [self menuTitleFromFileName:theFileName];
            theMenuItem = [[[NSMenuItem alloc] initWithTitle:theMenuTitle 
                            action:@selector(launchScript:) keyEquivalent:theKeyEquivalent] autorelease];
            [theMenuItem setKeyEquivalentModifierMask:theMod];
            [theMenuItem setRepresentedObject:thePath];
            [theMenuItem setTarget:self];
            [theMenuItem setToolTip:NSLocalizedString(@"\"Opt + click\" to open in Script Editor.",@"")];
            [inMenu addItem:theMenuItem];
        }
    }
}


//------------------------------------------------------
- (void)removeAllMenuItemsFromParent:(NSMenu *)inMenu
// すべてのメニューアイテムを削除
//------------------------------------------------------
{
    NSArray *theItems = [inMenu itemArray];
    NSMenuItem *theMenuItem;
    int i;

    for (i = ([theItems count] - 1); i >= 0; i--) {
        theMenuItem = [theItems objectAtIndex:i];
        if ((![theMenuItem isSeparatorItem]) && ([theMenuItem hasSubmenu])) {
            [self removeAllMenuItemsFromParent:[theMenuItem submenu]];
        }
        [inMenu removeItem:theMenuItem];
    }
}


//------------------------------------------------------
- (NSString *)menuTitleFromFileName:(NSString *)inFileName
// ファイル／フォルダ名からメニューアイテムタイトル名を生成
//------------------------------------------------------
{
    NSMutableString *outString = [NSMutableString stringWithString:[inFileName stringByDeletingPathExtension]];
    NSString *theExtnFirstChar = [[outString pathExtension] substringFromIndex:0];
    NSCharacterSet *theSpecSet = [NSCharacterSet characterSetWithCharactersInString:@"^~$@"];

    // 順番調整の冒頭の番号を削除
    [outString replaceOccurrencesOfRegularExpressionString:@"^[0-9]+\\)" 
                withString:@"" options:OgreNoneOption range:NSMakeRange(0, [outString length])];

    // キーボードショートカット定義があれば、削除して返す
    if (([theExtnFirstChar length] > 0) && 
            ([theSpecSet characterIsMember:[theExtnFirstChar characterAtIndex:0]])) {
        return [outString stringByDeletingPathExtension];
    }
    return outString;
}


//------------------------------------------------------
- (NSString *)keyEquivalentAndModifierMask:(unsigned int *)ioModMask fromFileName:(NSString *)inFileName
// ファイル名からキーボードショートカット定義を読み取る
//------------------------------------------------------
{
    NSString *theKeySpec = [[inFileName stringByDeletingPathExtension] pathExtension];

    return [[NSApp delegate] keyEquivalentAndModifierMask:ioModMask 
        fromString:theKeySpec includingCommandKey:YES];
}


//------------------------------------------------------
- (void)showAlert:(NSString *)inMessage
// エラーアラートを表示
//------------------------------------------------------
{
    NSAlert *theAleart = [NSAlert alertWithMessageText:NSLocalizedString(@"Script Error",@"") 
                defaultButton:nil 
                alternateButton:nil 
                otherButton:nil 
                informativeTextWithFormat:inMessage];
    [theAleart setAlertStyle:NSCriticalAlertStyle];
    (void)[theAleart runModal];
}


//------------------------------------------------------
- (NSString *)stringOfScript:(NSString *)inPath
// スクリプトの文字列を得る
//------------------------------------------------------
{
    NSString *outString = nil;
    NSData *theData = [NSData dataWithContentsOfFile:inPath];
    if ((theData == nil) || ([theData length] < 1)) { return outString; }
    id theValues = [[NSUserDefaultsController sharedUserDefaultsController] values];
    NSArray *theEncodings = [[[theValues valueForKey:k_key_encodingList] copy] autorelease];
    NSStringEncoding theEncoding;
    int i = 0;

    while (outString == nil) {
        theEncoding = 
                CFStringConvertEncodingToNSStringEncoding([[theEncodings objectAtIndex:i] unsignedLongValue]);
        if (theEncoding == NSProprietaryStringEncoding) {
            NSLog(@"theEncoding == NSProprietaryStringEncoding");
            break;
        }
        outString = [[[NSString alloc] initWithData:theData encoding:theEncoding] autorelease];
        if (outString != nil) { break; }
        i++;
    }
    if (outString != nil) {
        // 10.3.9 で、一部のバイナリファイルを開いたときにクラッシュする問題への暫定対応。
        // 10.4+ ではスルー（2005.12.25）
        // ＞＞ しかし「すべて2バイト文字で4096文字以上あるユニコードでない文書」は開けない（2005.12.25）
        // (下記の現象と同じ理由で発生していると思われる）
        // https://www.codingmonkeys.de/bugs/browse/HYR-529?page=all
        if ((floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_3) || // = 10.4+
                ([theData length] <= 8192) || 
                (([theData length] > 8192) && ([theData length] != ([outString length] * 2 + 1)) && 
                        ([theData length] != ([outString length] * 2)))) {

            return outString;
        }
    }
    return nil;
}


//------------------------------------------------------
- (void)doLaunchShellScript:(NSString *)inPath
// シェルスクリプト実行
//------------------------------------------------------
{
    NSString *theScript = [self stringOfScript:inPath];

    // スクリプトファイル内容を得られない場合は警告して抜ける
    if ((theScript == nil) || ([theScript length] < 1)) {
        [self showAlert:[NSString stringWithFormat:
                NSLocalizedString(@"The Script \"%@\" could NOT read.",@""), inPath]];
        return;
    }

    CEDocument *theDoc = nil;
    NSScanner *theScanner = [NSScanner scannerWithString:theScript];
    NSString *theInputType = nil, *theOutputType = nil;
    NSString *theInputStr = nil;
    NSData *theInputData = nil;
    NSTask *theTask = [[[NSTask alloc] init] autorelease];
    NSPipe *theOutPipe = [NSPipe pipe];
    NSPipe *theErrorPipe = [NSPipe pipe];
    BOOL theBoolDocExists = NO;
    BOOL theBoolIsError = NO;

    if ([[NSApp orderedDocuments] count] > 0) {
        theBoolDocExists = YES;
        theDoc = [[NSApp orderedDocuments] objectAtIndex:0];
    }
    _outputHandle = [[theOutPipe fileHandleForReading] retain]; // ===== retain
    _errorHandle = [[theErrorPipe fileHandleForReading] retain]; // ===== retain
    [theScanner setCaseSensitive:YES];
    while (![theScanner isAtEnd]) {
        (void)[theScanner scanUpToString:@"%%%{CotEditorXInput=" intoString:nil];
        if ([theScanner scanString:@"%%%{CotEditorXInput=" intoString:nil]) {
            if ([theScanner scanUpToString:@"}%%%" intoString:&theInputType]) {
                break;
            }
        }
    }
    if ((theInputType != nil) && ([theInputType isEqualToString:@"Selection"])) {
        if (theBoolDocExists) {
            NSRange theSelectedRange = [[[theDoc editorView] textView] selectedRange];
            theInputStr = [[[theDoc editorView] string] substringWithRange:theSelectedRange];
            // ([[theDoc editorView] string] は行末コードLFの文字列を返すが、[[theDoc editorView] selectedRange] は
            // 行末コードを反映させた範囲を返すので、「CR/LF」では使えない。そのため、
            // [[[theDoc editorView] textView] selectedRange] を使う必要がある。2009-04-12

        } else {
            theBoolIsError = YES;
        }
    } else if ((theInputType != nil) && ([theInputType isEqualToString:@"AllText"])) {
        if (theBoolDocExists) {
            theInputStr = [[theDoc editorView] string];
        } else {
            theBoolIsError = YES;
        }
    } else { // == "None"
    }
    if (theBoolIsError) {
        [self showScriptErrorLog:
                [NSString stringWithFormat:@"[ %@ ]\n%@", 
                    [[NSDate date] description], @"NO document, no Input."]];
        return;
    }
    if (theInputStr != nil) {
        theInputData = [theInputStr dataUsingEncoding:NSUTF8StringEncoding];
    }
    [theScanner setScanLocation:0];
    while (![theScanner isAtEnd]) {
        (void)[theScanner scanUpToString:@"%%%{CotEditorXOutput=" intoString:nil];
        if ([theScanner scanString:@"%%%{CotEditorXOutput=" intoString:nil]) {
            if ([theScanner scanUpToString:@"}%%%" intoString:&theOutputType]) {
                break;
            }
        }
    }
    if (theOutputType == nil) {
        _outputType = k_noOutput;
    } else if ([theOutputType isEqualToString:@"ReplaceSelection"]) {
        _outputType = k_replaceSelection;
    } else if ([theOutputType isEqualToString:@"ReplaceAllText"]) {
        _outputType = k_replaceAllText;
    } else if ([theOutputType isEqualToString:@"InsertAfterSelection"]) {
        _outputType = k_insertAfterSelection;
    } else if ([theOutputType isEqualToString:@"AppendToAllText"]) {
        _outputType = k_appendToAllText;
    } else if ([theOutputType isEqualToString:@"Pasteboard"]) {
        _outputType = k_pasteboard;
    } else if ([theOutputType isEqualToString:@"Pasteboard puts"]) { // 以前の定義文字列。互換性のため。(2007.05.26)
        _outputType = k_pasteboard;
    } else { // == "Discard"
        _outputType = k_noOutput;
    }

    // タスク実行準備
    // （theTask に引数をセットすると一部のスクリプトが誤動作する。例えば、Perl 5.8.xで「use encoding 'utf8'」のうえ
    // printコマンドを使用すると文字化けすることがある。2009-03-31）
    [theTask setLaunchPath:inPath];
    [theTask setCurrentDirectoryPath:NSHomeDirectory()];
    [theTask setStandardInput:[NSPipe pipe]];
    [theTask setStandardOutput:theOutPipe];
    [theTask setStandardError:theErrorPipe];
    // 出力をバックグラウンドで行うように指示
    [[[theTask standardOutput] fileHandleForReading] readToEndOfFileInBackgroundAndNotify];
    [[[theTask standardError] fileHandleForReading] readToEndOfFileInBackgroundAndNotify];

    [theTask launch];
    if ((theInputData != nil) && ([theInputData length] > 0)) {
        [[[theTask standardInput] fileHandleForWriting] writeData:theInputData];
        [[[theTask standardInput] fileHandleForWriting] closeFile];
    }
}


// ------------------------------------------------------
- (void)availableOutput:(NSNotification *)inNotification
// 標準出力を取得
// ------------------------------------------------------
{
    NSData *theOutputData = [[inNotification userInfo] objectForKey:NSFileHandleNotificationDataItem];
    CEDocument *theDoc = nil;
    NSString *theOutputStr = nil;
    NSPasteboard *thePb;
    BOOL theBoolDocExists = NO;

    if ([[NSApp orderedDocuments] count] > 0) {
        theBoolDocExists = YES;
        theDoc = [[NSApp orderedDocuments] objectAtIndex:0];
    }

    if (theOutputData == nil) { return; }
    if ([[inNotification object] isEqualTo:_outputHandle]) {
        theOutputStr = [[[NSString alloc] initWithData:theOutputData 
                encoding:NSUTF8StringEncoding] autorelease];
        if (theOutputStr != nil) {
            switch (_outputType) {
            case k_replaceSelection:
                [[theDoc editorView] replaceTextViewSelectedStringTo:theOutputStr scroll:NO];
                break;
            case k_replaceAllText:
                [[theDoc editorView] replaceTextViewAllStringTo:theOutputStr];
                break;
            case k_insertAfterSelection:
                [[theDoc editorView] insertTextViewAfterSelectionStringTo:theOutputStr];
                break;
            case k_appendToAllText:
                [[theDoc editorView] appendTextViewAfterAllStringTo:theOutputStr];
                break;
            case k_pasteboard:
                thePb = [NSPasteboard generalPasteboard];
                [thePb declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:nil];
                if (![thePb setString:theOutputStr forType:NSStringPboardType]) {
                    NSBeep();
                }
                break;
            }
        }
        _outputType = k_noOutput;
        [_outputHandle release];
        _outputHandle = nil;
    } else if ([[inNotification object] isEqualTo:_errorHandle]) {
        theOutputStr = [[[NSString alloc] initWithData:theOutputData 
                encoding:NSUTF8StringEncoding] autorelease];
        if ((theOutputStr != nil) && ([theOutputStr length] > 0)) {
            [self showScriptErrorLog:
                    [NSString stringWithFormat:@"[ %@ ]\n%@", [[NSDate date] description], theOutputStr]];
        }
        [_errorHandle release];
        _errorHandle = nil;
    }
}


// ------------------------------------------------------
- (void)showScriptErrorLog:(NSString *)inLogString
// スクリプトエラーを追記し、エラーログウィンドウを表示
// ------------------------------------------------------
{
    [_errorTextView setEditable:YES];
    [_errorTextView setSelectedRange:NSMakeRange([[_errorTextView string] length], 0)];
    [_errorTextView insertText:inLogString];
    [_errorTextView setEditable:NO];
    [self openScriptErrorWindow];
}

@end