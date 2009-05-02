/*
=================================================
CEDocumentController
(for CotEditor)

Copyright (C) 2004-2007 nakamuxu.
http://www.aynimac.com/
=================================================

encoding="UTF-8"
Created:2004.12.14

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
#import "CEAppController.h"
#import "CEDocument.h"

@interface CEDocumentController : NSDocumentController
{
    IBOutlet id _appController;
    IBOutlet id _openPanelAccessoryView;
    IBOutlet id _accessoryEncodingMenu;
    IBOutlet id _transparencyController;
    IBOutlet id _windowAlphaSlider;
    IBOutlet id _windowAlphaField;
    IBOutlet id _windowAlphaTextViewOnlyButton;
    IBOutlet id _windowAlphaSetButton;
    IBOutlet id _gotoIndexField;
    IBOutlet id _gotoSelectButton;
    IBOutlet id _gotoCharLineMatrix;
    IBOutlet id _lineSpacingField;

    BOOL _isOpenHidden;
}

// Public method
- (id)accessoryEncodingMenu;
- (NSStringEncoding)accessorySelectedEncoding;
- (void)setSelectAccessoryEncoding:(NSStringEncoding)inEncoding;
- (void)setFontToAllDocuments;
- (float)windowAlphaControllerValue;
- (void)setWindowAlphaControllerDictionary:(NSMutableDictionary *)inDict;
- (void)setWindowAlphaControllerValueDefault;
- (void)setTransparencyPanelControlsEnabledWithDecrement:(BOOL)inValue;
- (void)setGotoPanelControlsEnabledWithDecrement:(BOOL)inValue;
- (void)rebuildAllToolbarsEncodingItem;
- (void)rebuildAllToolbarsSyntaxItem;
- (void)setRecolorFlagToAllDocumentsWithStyleName:(NSDictionary *)inDict;
- (void)setNoneAndRecolorFlagToAllDocumentsWithStyleName:(NSString *)inStyleName;

// Action Message
- (IBAction)openHiddenDocument:(id)sender;
- (IBAction)openTransparencyPanel:(id)sender;
- (IBAction)setAllWindowAlpha:(id)sender;
- (IBAction)openGotoPanel:(id)sender;
- (IBAction)gotoCharacterOrLine:(id)sender;
- (IBAction)setSmartInsertAndDeleteToAllTextView:(id)sender;
- (IBAction)setSelectAccessoryEncodingMenuToDefault:(id)sender;
- (IBAction)openLineSpacingPanel:(id)sender;
- (IBAction)closeLineSpacingPanel:(id)sender;
- (IBAction)setCustomLineSpacing:(id)sender;

@end
