/*
* ZDic.c
*
* main file for ZDic
*
* This wizard-generated code is based on code adapted from the
* stationery files distributed as part of the Palm OS SDK 4.0.
*
* Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
* All rights reserved.
*/

#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <CtlGlue.h>
#include <LstGlue.h>        // for LstGlueSetFont
#include <Window.h>         // for WinScreenGetAttribute
#include <SonyChars.h>
#include <PalmOneNavigator.h>	// for 5-way Navigator

/* palmOne */
#include <palmOneResources.h>

/* palmOne Treo */
#include <HsKeyCodes.h>
#include <HsExt.h>
#include <HsKeyTypes.h>

#include "ZDic.h"
#include "ZDicConfig.h"
#include "ZDicDB.h"
#include "ZDicDIA.h"
#include "ZDicTools.h"
#include "ZDicExport.h"
#include "ZDicRegister.h"
#include "ZDicVoice.h"

#include "ZDic_Rsc.h"
#include "Decode.h"

/*********************************************************************
 * Entry Points
 *********************************************************************/

/*********************************************************************
 * Global variables
 *********************************************************************/

static Err ToolsPlayVoice ( void );
static Err PrefFormPopupPrefsDialog( void );
static Int16 ZDicDBInitDictList( UInt32 type, UInt32 creator );
static void ZDicDBInitShortcutList();
static void ZDicDBInitPopupList();
static Boolean PrefFormHandleEvent( FormType *frmP, EventType *eventP, Boolean *exit );
static Boolean DAFormHandleEvent( EventType * eventP );
static Err MainFormSearch( Boolean putinHistory, Boolean updateWordList, Boolean highlightWordField, Boolean updateDescField, Boolean bEnableBreak, Boolean bEnableAutoSpeech );
static Err MainFormChangeWordFieldCase( void );
static Err MainFormUpdateWordList( void );
static void MainFormWordListUseAble( Boolean turnOver, Boolean redraw );
static Boolean MainFormHandleEvent( EventType * eventP );
static void CustomKey(UInt8 num);
static void ShortcutMoveDictionary( WinDirectionType direction );
static void PopupMoveDictionary( WinDirectionType direction );
static Err FormPopupDictList( UInt16 formId );
static Err FormHistorySeekBack( UInt16 formId );

/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

//#pragma mark -
/*********************************************************************
 * Internal Functions
 *********************************************************************/
//--------------------------------------------------------------------------
//���option���Ƿ񱻰���
static Boolean hasOptionPressed(UInt16 modifiers)
{
	Boolean capsLock		= false;
	Boolean	numLock			= false;
	Boolean optLock			= false;
	Boolean autoShifted		= false;
	Boolean	optionPressed	= false;
	UInt16	tempShift		= 0;
	AppGlobalType 			* global;

    global = AppGetGlobal();
    
	if ((modifiers & optionKeyMask)) //����option����
	{
		optionPressed = true;
	}
	else
	{
		if (global->prefs.isTreo) //HSϵ��״̬����
		{
			HsGrfGetStateExt(&capsLock, &numLock, &optLock, &tempShift, &autoShifted);
		}
		else //��׼״̬����
		{
			GrfGetState(&capsLock, &numLock, &tempShift, &autoShifted);
		}
		
		if (tempShift == grfTempShiftUpper || tempShift == hsGrfTempShiftOpt)
		{
			optionPressed = true;
		}
	}
	
	return (optionPressed | optLock);
}
//--------------------------------------------------------------------------
//��������
static void CustomKey(UInt8 num)
{
	FormType	*frmP;
	EventType	event;
	WChar		chr = chrNull,keyCode = chrNull;
	AppGlobalType	*global;
	
	FrmPopupForm(frmSetKeyTips);
	
	global = AppGetGlobal();
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if(event.data.ctlSelect.controlID == btnClearKey)
		{
			switch(num)
			{
				case 1:
				{
					global->prefs.keyPlaySoundChr = chrNull;
					global->prefs.keyPlaySoundKeycode = chrNull;
					break;
				}
				case 2:
				{
					global->prefs.keyWordListChr = chrNull;
					global->prefs.keyWordListKeycode = chrNull;
					break;
				}case 3:
				{
					global->prefs.keyHistoryChr = chrNull;
					global->prefs.keyHistoryKeycode = chrNull;
					break;
				}
				case 4:
				{
					global->prefs.keyEnlargeDAChr = chrNull;
					global->prefs.keyEnlargeDAKeycode = chrNull;
					break;
				}
				case 5:
				{
					global->prefs.keyOneKeyChgDicChr = chrNull;
					global->prefs.keyOneKeyChgDicKeycode = chrNull;
					break;
				}
				case 6:
				{
					global->prefs.keyExportChr = chrNull;
					global->prefs.keyExportKeycode = chrNull;
					break;
				}
				case 7:
				{
					global->prefs.keyClearFieldChr = chrNull;
					global->prefs.keyClearFieldKeycode = chrNull;
					break;
				}
				case 8:
				{
					global->prefs.keyShortcutChr = chrNull;
					global->prefs.keyShortcutKeycode = chrNull;
					break;
				}
				case 9:
				{
					global->prefs.keyPopupChr = chrNull;
					global->prefs.keyPopupKeycode = chrNull;
					break;
				}
				case 10:
				{
					global->prefs.keyGobackChr = chrNull;
					global->prefs.keyGobackKeycode = chrNull;
					break;
				}
				case 11:
				{
					global->prefs.keySearchAllChr = chrNull;
					global->prefs.keySearchAllKeycode = chrNull;
					break;
				}
				
			}
			if(global->prefs.SelectKeyUsed == num) global->prefs.SelectKeyUsed = 0;
			event.eType = appStopEvent;
		}
		else if (event.eType == keyDownEvent)// || event.eType == keyUpEvent)
		{
			chr = (WChar)event.data.keyDown.chr;
			keyCode = (WChar)event.data.keyDown.keyCode;
			switch(num)
			{
				case 1:
				{
					global->prefs.keyPlaySoundChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyPlaySoundKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 2:
				{
					global->prefs.keyWordListChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyWordListKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 3:
				{
					global->prefs.keyHistoryChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyHistoryKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 4:
				{
					global->prefs.keyEnlargeDAChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyEnlargeDAKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 5:
				{
					global->prefs.keyOneKeyChgDicChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyOneKeyChgDicKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 6:
				{
					global->prefs.keyExportChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyExportKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 7:
				{
					global->prefs.keyClearFieldChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyClearFieldKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 8:
				{
					global->prefs.keyShortcutChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyShortcutKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 9:
				{
					global->prefs.keyPopupChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyPopupKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 10:
				{
					global->prefs.keyGobackChr = (WChar)event.data.keyDown.chr;
					global->prefs.keyGobackKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
				case 11:
				{
					global->prefs.keySearchAllChr = (WChar)event.data.keyDown.chr;
					global->prefs.keySearchAllKeycode = (WChar)event.data.keyDown.keyCode;
					break;
				}
			}
			event.eType = appStopEvent;
		}
		
		if(global->prefs.keyPlaySoundChr == chr && global->prefs.keyPlaySoundKeycode == keyCode && num != 1)
		{
			global->prefs.keyPlaySoundChr = chrNull;
			global->prefs.keyPlaySoundKeycode = chrNull;
		}
		else if(global->prefs.keyWordListChr == chr && global->prefs.keyWordListKeycode == keyCode && num != 2 && num != 4)
		{
			global->prefs.keyWordListChr = chrNull;
			global->prefs.keyWordListKeycode = chrNull;
		}
		else if(global->prefs.keyHistoryChr == chr && global->prefs.keyHistoryKeycode == keyCode && num != 3 && num != 4)
		{
			global->prefs.keyHistoryChr = chrNull;
			global->prefs.keyHistoryKeycode = chrNull;
		}
		else if(global->prefs.keyEnlargeDAChr == chr && global->prefs.keyEnlargeDAKeycode == keyCode && num != 4 && num != 3 && num != 2 )//�Ա���DA��ָ����ڿ�ݼ��Ĺ���
		{
			global->prefs.keyEnlargeDAChr = chrNull;
			global->prefs.keyEnlargeDAKeycode = chrNull;
		}
		else if(global->prefs.keyOneKeyChgDicChr == chr && global->prefs.keyOneKeyChgDicKeycode == keyCode && num != 5)
		{
			global->prefs.keyOneKeyChgDicChr = chrNull;
			global->prefs.keyOneKeyChgDicKeycode = chrNull;
		}
		else if(global->prefs.keyExportChr == chr && global->prefs.keyExportKeycode == keyCode && num != 6)
		{
			global->prefs.keyExportChr = chrNull;
			global->prefs.keyExportKeycode = chrNull;
		}
		else if(global->prefs.keyClearFieldChr == chr && global->prefs.keyClearFieldKeycode == keyCode && num != 7)
		{
			global->prefs.keyClearFieldChr = chrNull;
			global->prefs.keyClearFieldKeycode = chrNull;
		}
		else if(global->prefs.keyClearFieldChr == chr && global->prefs.keyClearFieldKeycode == keyCode && num != 8)
		{
			global->prefs.keyShortcutChr = chrNull;
			global->prefs.keyShortcutKeycode = chrNull;
		}
		else if(global->prefs.keyPopupChr == chr && global->prefs.keyPopupKeycode == keyCode && num != 9)
		{
			global->prefs.keyPopupChr = chrNull;
			global->prefs.keyPopupKeycode = chrNull;
		}
		else if(global->prefs.keyGobackChr == chr && global->prefs.keyGobackKeycode == keyCode && num != 10)
		{
			global->prefs.keyGobackChr = chrNull;
			global->prefs.keyGobackKeycode = chrNull;
		}
		else if(global->prefs.keySearchAllChr == chr && global->prefs.keySearchAllKeycode == keyCode && num != 11)
		{
			global->prefs.keySearchAllChr = chrNull;
			global->prefs.keySearchAllKeycode = chrNull;
		}
		
		if(NavSelectPressed( &event )
                          || event.data.keyDown.chr == vchrRockerCenter
                          || event.data.keyDown.chr == vchrThumbWheelPush
                          || event.data.keyDown.chr == vchrJogRelease)
        {
            global->prefs.SelectKeyUsed = num;
		}
		
		if (! SysHandleEvent(&event))
		{
			if (event.eType == frmOpenEvent && event.data.frmOpen.formID == frmSetKeyTips)
			{
				frmP = FrmInitForm(frmSetKeyTips);
				FrmSetActiveForm(frmP);
				FrmDrawForm(frmP);
			}
			else
			{
				FrmDispatchEvent(&event);
			}
		}
		
	}while (event.eType != appStopEvent);
	
	//�˳�����
	FrmReturnToForm(0);
}

//--------------------------------------------------------------------------
//Custom Shortcut Key
static void CustomShortcutKey(UInt8 num)
{
	FormType	*frmP;
	EventType	event;
	WChar		chr = chrNull,keyCode = chrNull;
	AppGlobalType	*global;
	UInt8	i;
	
	ZDicDBDictInfoType	*dictInfo;
	
	FrmPopupForm(frmSetKeyTips);
	
	global = AppGetGlobal();
	
	dictInfo = &global->prefs.dictInfo;
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if(event.data.ctlSelect.controlID == btnClearKey)
		{
			dictInfo->keyShortcutChr[ num ] = chrNull;
			dictInfo->keyShortcutKeycode[ num ] = chrNull;
			event.eType = appStopEvent;
		}
		else if (event.eType == keyDownEvent)
		{
			dictInfo->keyShortcutChr[ num ] = (WChar)event.data.keyDown.chr;
			dictInfo->keyShortcutKeycode[ num ] = (WChar)event.data.keyDown.keyCode;
			event.eType = appStopEvent;
		}
		
		for(i = 0; i < MAX_DICT_NUM; i++)
		{
			if (dictInfo->keyShortcutChr[ i ] == (WChar)event.data.keyDown.chr && dictInfo->keyShortcutKeycode[ i ] == (WChar)event.data.keyDown.keyCode && i != num)
			{
				dictInfo->keyShortcutChr[ i ] = chrNull;
				dictInfo->keyShortcutKeycode[ i ] = chrNull;
				dictInfo->showInShortcut[ i ] = false;
				break;
			}
		}
		
		if (! SysHandleEvent(&event))
		{
			if (event.eType == frmOpenEvent && event.data.frmOpen.formID == frmSetKeyTips)
			{
				frmP = FrmInitForm(frmSetKeyTips);
				FrmSetActiveForm(frmP);
				FrmDrawForm(frmP);
			}
			else
			{
				FrmDispatchEvent(&event);
			}
		}
		
	}while (event.eType != appStopEvent);
	
	//�˳�����
	FrmReturnToForm(0);
}

/*
 *
 * FUNCTION:    HideObject
 *
 * DESCRIPTION: This routine set an object not-usable and erases it
 *              if the form it is in is visible.
 *
 * PARAMETERS:  frm      - pointer to a form
 *              objectID - id of the object to set not usable
 *
 * RETURNED:    nothing
 *
 *
 */
static void HideObject ( const FormPtr frm, UInt16 objectID )
{

    FrmHideObject ( frm, FrmGetObjectIndex ( frm, objectID ) );
}

/*
 *
 * FUNCTION:    ShowObject
 *
 * DESCRIPTION: This routine set an object usable and draws the object if
 *              the form it is in is visible.
 *
 * PARAMETERS:  frm      - pointer to a form
 *              objectID - id of the object to set usable
 *
 * RETURNED:    nothing
 *
 *
 */
static void ShowObject ( const FormPtr frm, UInt16 objectID )
{
    FrmShowObject ( frm, FrmGetObjectIndex ( frm, objectID ) );
}

/*
 *
 * FUNCTION:    IsInside
 *
 * DESCRIPTION: Check the point whether in the rectangle range.
 *
 * RETURNED:    true if inside else false
 *
 *
 */

static Boolean IsInside( RectanglePtr r, UInt16 x, UInt16 y )
{
    return ( 0 <= x && x <= r->extent.x && 0 <= y && y <= r->extent.y );
}

/*
 *
 * FUNCTION:    IsOutside
 *
 * DESCRIPTION: Check the point whether out side the rectangle range.
 *
 * RETURNED:    true if outside else true
 *
 *
 */

static Boolean IsOutside( RectanglePtr r, UInt16 x, UInt16 y )
{
    return ( !IsInside( r, x, y ) );
}

#pragma mark -

/***********************************************************************
 *
 * FUNCTION:	ToolsQuitApp
 *
 * DESCRIPTION: Send appStopEvent to event queue to stop application.
 *
 * PARAMETERS:
 *				nothing.
 *
 * RETURN:
 *				nothing
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static void ToolsQuitApp( void )
{
    EventType	newEvent;

    MemSet( &newEvent, sizeof( newEvent ), 0 );
    newEvent.eType = appStopEvent;
    EvtAddEventToQueue( &newEvent );
    return ;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsFormatExplain
 *
 * DESCRIPTION: Format explain for easy display.
 *
 * PARAMETERS:
 *				->	p Pointer of explain string.
 *
 * RETURN:
 *				errNone if success else fail.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static Err ToolsFormatExplain( UInt8 *p )
{
    UInt16	i, j;

    if ( p == NULL )
        return errNone;

    // get key word.
    i = 0;
    while ( p[ i ] != chrHorizontalTabulation && p[ i ] != chrNull )
        i++;
    if ( p[ i ] == chrHorizontalTabulation )
        p[ i++ ] = chrLineFeed;

    // get explain, translate "\n" to charLineFeed;
    j = i;
    while ( p[ i ] != chrNull )
    {
        if ( p[ i ] == '\\' && p[ i + 1 ] == chrSmall_N )
        {
            if ( j != 0 )
                p[ j++ ] = chrLineFeed;
            i++;
        }
        else
        {
            if ( i != j )
                p[ j ] = p[ i ];
            j++;
        }

        i++;
    }
    p[ j ] = chrNull;

    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsGMXTranslate
 *
 * DESCRIPTION: This routine translate lazy or efan phonetic to GMX phonetic.
 *
 * PARAMETERS:
 *				->	p Pointer of explain string.
 *				->	index Source phonetic index.
 *
 * RETURN:
 *				errNone if success else fail.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static Err ToolsGMXTranslate( Char *p, phoneticEnum index )
{
    MemHandle transH;
    Char *trans;
    UInt16	i, j, offset, strID;
    WChar ch;

    if ( index == eGmxPhonetic || index == eNonePhonetic || p == NULL )
        return errNone;

    // get GMX to efan phonetic table and translate it.
    switch ( index )
    {
    case eEfanPhonetic:
        strID = efan2GMXString;
        break;
    case eLazywormPhonetic:
        strID = lazy2GMXString;
        break;
    case eMutantPhonetic:
        strID = mutant2GMXString;
        break;
    default:
        return errNone;
    }

    transH = DmGetResource( strRsc, strID );
    if ( transH == NULL )
    {
        return DmGetLastErr();
    }

    trans = ( Char * ) MemHandleLock( transH );

    // seek  phonetic string.
    i = 0;

NextPhoneticStr:
    while ( true )
    {
        offset = TxtGetNextChar( p, i, &ch );
        if ( ch == chrNull
                || ch == chrSolidus
                || ch == chrLeftSquareBracket
                || ch == chrLessThanSign )
            break;

        i += offset;
    }

    if ( ch == chrSolidus || ch == chrLeftSquareBracket || ch == chrLessThanSign )
    {
        i += offset;
        offset = TxtGetNextChar( p, i, &ch );

        while ( ch != chrSolidus && ch != chrRightSquareBracket
                && ch != chrGreaterThanSign && ch != chrNull )
        {
            j = 0;
            while ( ( WChar ) * ( trans + j ) != ch && ( WChar ) * ( trans + j ) != chrNull )
            {
                j++;
                j++;
            }

            if ( ( WChar ) * ( trans + j ) == ch )
                TxtSetNextChar( p, i, ( WChar ) * ( trans + j + 1 ) );

            i += offset;
            offset = TxtGetNextChar( p, i, &ch );
        }

        if ( ch != chrNull )
        {
            i += offset;
            goto NextPhoneticStr;
        }
    }

    MemHandleUnlock( transH );
    DmReleaseResource( transH );

    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsSetFieldHandle
 *
 * DESCRIPTION:	Set new handle for a filed by id and free old handle.
 *
 * PARAMETERS:	->	objID Field resource id.
 *				->	newTxtH New handle that should be set the field.
 *				->	redraw True if need to redraw the field.
 *
 * RETURNED:	errNone if success else fail.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	14/Aug/04	Initial Revision 
 *
 ***********************************************************************/

static Err ToolsSetFieldHandle( UInt16 objID, MemHandle newTxtH, Boolean redraw )
{
    FieldType * field;
    MemHandle oldTxtH;

    field = ( FieldType * ) GetObjectPtr( objID );

    // change the text and update the display
    oldTxtH = FldGetTextHandle( field );
    FldSetTextHandle( field, newTxtH );

    // Reset insert point.
    FldSetInsertionPoint ( field, 0 );

    if ( redraw )
        FldDrawField( field );

    // free the old text handle
    if ( oldTxtH != NULL )
        MemHandleFree( oldTxtH );

    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsSetFieldPtr
 *
 * DESCRIPTION:	Set new text for a filed by id and free old handle.
 *
 * PARAMETERS:	->	objID Field resource id.
 *				->	text Pointer of the sting.
 *				->	textLen Length of the string.
 *				->	redraw True if need to redraw the field.
 *
 * RETURNED:	errNone if success else fail.
 *
 * Note:		we must use ToolsSetFieldPtr to replace FldInsert,
 *				because ToolsSetFieldPtr sets the field's dirty attribute and
 *				posts a fldChangedEvent to the event queue. But we
 *				have a search when receive a fldChangedEvent.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	14/Aug/04	Initial Revision 
 *
 ***********************************************************************/

static Err ToolsSetFieldPtr( UInt16 objID, const Char *text, Int16 textLen, Boolean redraw )
{
    MemHandle	bufH;
    Char	*str;

    bufH = MemHandleNew( textLen + 1 );
    if ( bufH == NULL )
        return memErrNotEnoughSpace;

    // Build new text handle.
    str = MemHandleLock( bufH );
    MemMove( str, text, textLen );
    str[ textLen ] = chrNull;
    MemHandleUnlock( bufH );

    // Set new handle.
    ToolsSetFieldHandle( objID, bufH, redraw );

    return errNone;
}

/*
 * FUNCTION: ToolsHighlightField
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */

static void ToolsHighlightField( UInt16 fieldID )
{
    Char * wordStr;
    FieldType *field;

    // select all the text in word field, then use can clear it easy.
    field = GetObjectPtr( fieldID );
    wordStr = FldGetTextPtr ( field );

    if ( wordStr != NULL && *wordStr != chrNull )
    {
        // Reset insert point. and select it all.
        FldSetInsertionPoint ( field, 0 );
        FldSetSelection ( field, 0, StrLen( wordStr ) );
    }
}


/***********************************************************************
 *
 * FUNCTION:	ToolsUpdateScrollBar
 *
 * DESCRIPTION:	Update scroll bar by special field.
 *
 * PARAMETERS:	->	objID Field resource id.
 *				->	scrollBarID Scrollbar resource id.
 *
 * RETURNED:	nothing.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	14/Aug/04	Initial Revision 
 *
 ***********************************************************************/

static void ToolsUpdateScrollBar ( UInt16 fieldID, UInt16 scrollBarID )
{
    UInt16 scrollPos;
    UInt16 textHeight;
    UInt16 fieldHeight;
    Int16 maxValue;
    FieldPtr fld;
    ScrollBarPtr bar;

    fld = GetObjectPtr ( fieldID );
    bar = GetObjectPtr ( scrollBarID );

    FldGetScrollValues ( fld, &scrollPos, &textHeight, &fieldHeight );

    if ( textHeight > fieldHeight )
    {
        // On occasion, such as after deleting a multi-line selection of text,
        // the display might be the last few lines of a field followed by some
        // blank lines.  To keep the current position in place and allow the user
        // to "gracefully" scroll out of the blank area, the number of blank lines
        // visible needs to be added to max value.  Otherwise the scroll position
        // may be greater than maxValue, get pinned to maxvalue in SclSetScrollBar
        // resulting in the scroll bar and the display being out of sync.
        maxValue = ( textHeight - fieldHeight ) + FldGetNumberOfBlankLines ( fld );
    }
    else if ( scrollPos )
        maxValue = scrollPos;
    else
        maxValue = 0;

    SclSetScrollBar ( bar, scrollPos, 0, maxValue, fieldHeight - 1 );
}

/***********************************************************************
 *
 * FUNCTION:	ToolsScroll
 *
 * DESCRIPTION:	Scroll the field and update the scrollbar.
 *
 * PARAMETERS:	->	linesToScroll Number of scroll line.
 *				->	updateScrollbar True if update the scrollbar else not.
 *				->	objID Field resource id.
 *				->	scrollBarID Scrollbar resource id.
 *
 * RETURNED:	nothing.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	14/Aug/04	Initial Revision 
 *
 ***********************************************************************/

static void ToolsScroll ( Int16 linesToScroll, Boolean updateScrollbar, UInt16 fieldID, UInt16 scrollBarID )
{
    UInt16	blankLines;
    FieldPtr	fld;

    fld = GetObjectPtr ( fieldID );
    blankLines = FldGetNumberOfBlankLines ( fld );

    if ( linesToScroll < 0 )
        FldScrollField ( fld, -linesToScroll, winUp );
    else if ( linesToScroll > 0 )
        FldScrollField ( fld, linesToScroll, winDown );

    // If there were blank lines visible at the end of the field
    // then we need to update the scroll bar.
    if ( blankLines || updateScrollbar )
    {
        ToolsUpdateScrollBar( fieldID, scrollBarID );
    }
}

/***********************************************************************
 *
 * FUNCTION:	ToolsScrollWord
 *
 * DESCRIPTION:	scroll to next/prev word.
 *
 * PARAMETERS:	->	direction Direction of page scroll.
 *				->	objID Field resource id.
 *				->	scrollBarID Scrollbar resource id.
 *				->  inputFieldID Input field resource id.
 *				->	highlightWordField True if highlight inputFieldID.
 *
 * RETURNED:	nothing.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	14/Aug/04	Initial Revision 
 *		JemyZhang		04/Apr/08	Add hidePlayerBtonID 
 *
 ***********************************************************************/

static void ToolsScrollWord ( WinDirectionType direction, UInt16 fieldID,
                              UInt16 scrollBarID, UInt16 inputFieldID, 
                              UInt16 playerBtonID, Boolean highlightWordField )
{
    UInt8	* explainPtr;
    UInt32	explainLen;
    ZDicDBDictInfoType	*dictInfoP;
    AppGlobalType	*global;
    Err	err;

    global = AppGetGlobal();
    dictInfoP = &global->prefs.dictInfo;

    if ( direction == winUp )
    {
        err = ZDicLookupBackward( &explainPtr, &explainLen, &global->descript );

    }
    else
    {
        err = ZDicLookupForward( &explainPtr, &explainLen, &global->descript );
    }

    if ( err == errNone )
    {
        FormType * frmP;
        MemHandle	bufH;
        Char	*str;
        Int16	len;

        bufH = MemHandleNew( explainLen + 1 );
        if ( bufH == NULL )
            return ;

        // format string.
        str = MemHandleLock( bufH );
        MemMove( str, explainPtr, explainLen );
        str[ explainLen ] = chrNull;
        ToolsFormatExplain( ( UInt8* ) str );
        if ( dictInfoP->phonetic[ dictInfoP->curMainDictIndex ] != 0 )
            ToolsGMXTranslate( str, dictInfoP->phonetic[ dictInfoP->curMainDictIndex ] );
        MemHandleUnlock( bufH );

        // update display.
        ToolsSetFieldHandle( fieldID, bufH, true );
        ToolsUpdateScrollBar ( fieldID, scrollBarID );

        // update input field
        // get key word.
        len = 0;
        while ( explainPtr[ len ] != chrHorizontalTabulation && explainPtr[ len ] != chrNull && len < MAX_WORD_LEN )
        {
            global->data.readBuf[ len ] = explainPtr[ len ];
            len++;
        }
        global->data.readBuf[ len ] = chrNull;
        ToolsSetFieldPtr( inputFieldID, ( char * ) global->data.readBuf, len, true );

        if ( highlightWordField )
            ToolsHighlightField( inputFieldID );

        // Display or hide the player button
        frmP = FrmGetActiveForm ();
        if ( ZDicVoiceIsExist ( ( Char * ) explainPtr ) )
        {
            ShowObject ( frmP, playerBtonID );
            if ( global->prefs.enableAutoSpeech )
                ToolsPlayVoice ();
        }
        else
            HideObject ( frmP, playerBtonID );
        }

}

/***********************************************************************
 *
 * FUNCTION:	ToolsPageScroll
 *
 * DESCRIPTION:	Page scroll the field and update the scrollbar.
 *
 * PARAMETERS:	->	direction Direction of page scroll.
 *				->	objID Field resource id.
 *				->	scrollBarID Scrollbar resource id.
 *				->  inputFieldID Input field resource id.
 *				->	highlightWordField True if highlight inputFieldID.
 *
 * RETURNED:	nothing.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	14/Aug/04	Initial Revision
 *		JemyZhang		04/Apr/08	Add hidePlayerBtonID 
 *
 ***********************************************************************/

static void ToolsPageScroll ( WinDirectionType direction, UInt16 fieldID,
                              UInt16 scrollBarID, UInt16 inputFieldID, 
                              UInt16 playerBtonID, Boolean highlightWordField )
{
    UInt16	linesToScroll;
    FieldPtr	fld;

    fld = GetObjectPtr ( fieldID );

    if ( FldScrollable ( fld, direction ) )
    {
        linesToScroll = FldGetVisibleLines ( fld ) - 1;

        if ( direction == winUp )
            linesToScroll = -linesToScroll;

        ToolsScroll( linesToScroll, true, fieldID, scrollBarID );

        return ;
    }

    ToolsScrollWord ( direction, fieldID, scrollBarID, inputFieldID, playerBtonID, highlightWordField );
	MainFormUpdateWordList( );
    return ;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsChangeCase
 *
 * DESCRIPTION: This routine change the string case.
 *
 * PARAMETERS:
 *				<->	newStr Pass the pointer of string and ret result.
 *
 * RETURN:
 *				errNone if success else fail.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static Err ToolsChangeCase( Char* newStr )
{
    Int16 strLen, i;
    UInt16 offset;
    WChar ch;

    // if it is not ascii sting then do not change case.
    i = 0;
    while ( true )
    {
        offset = TxtGetNextChar( newStr, i, &ch );
        if ( ch == chrNull )
            break;

        if ( offset != 1 )
            return ~errNone;

        i += offset;
    }

    strLen = StrLen( newStr );

    if ( TxtCharIsLower ( newStr[ 0 ] ) )
    {
        // "word" -> "WORD"
        for ( i = 0; i < strLen; i++ )
        {
            if ( TxtCharIsLower( newStr[ i ] ) )
                newStr[ i ] -= 'a' - 'A';
        }
    }
    else if ( TxtCharIsUpper ( newStr[ 0 ] ) )
    {
        if ( strLen > 1 && TxtCharIsLower ( newStr[ 1 ] ) )
        {
            // "Word" -> "word"
            StrToLower ( newStr, newStr );
        }
        else
        {
            if ( strLen > 1 )
            {
                // "WORD" -> "Word"
                for ( i = 1; i < strLen; i++ )
                {
                    if ( TxtCharIsUpper( newStr[ i ] ) )
                        newStr[ i ] += 'a' - 'A';
                }
            }
            else
            {
                // "W" -> "w"
                if ( TxtCharIsUpper( newStr[ 0 ] ) )
                    newStr[ 0 ] += 'a' - 'A';
            }
        }
    }
    else
        return ~errNone;

    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsWordNotYuanYin
 *
 * DESCRIPTION: This routine change the string by word assay.
 *
 * PARAMETERS:
 *				<->	newStr Pass the pointer of string and ret result.
 *
 * RETURN:
 *				errNone if success else fail.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/
__inline static Int16 ToolsWordSameAs( const Char *s1, const Char *s2 )
{
    Int16	idx;

    // compare key string.
    idx = 0;
    while ( *( s1 + idx ) == *( s2 + idx )
            && *( s1 + idx ) != chrNull )
    {
        idx++;
    }

    return *( s1 + idx ) - *( s2 + idx );
}

__inline static Boolean ToolsWordNotYuanYin ( Char c )
{
    return ( c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' );
}

static Err ToolsWordAssay ( Char* newStr )
{
    Int16 tail;
    Boolean err = ~errNone;

    tail = StrLen ( newStr );
    if ( tail <= 4 )
        return false;

    // n.
    if ( newStr[ tail - 1 ] == 's' )
    {
        // fu yin + "oes" -> fu yin + "o" : "tomatoes" -> "tomato"
        if ( ToolsWordSameAs ( &newStr[ tail - 3 ], "oes" ) == 0
                && ToolsWordNotYuanYin ( newStr[ tail - 4 ] ) )
        {
            newStr[ tail - 2 ] = chrNull;
        }

        // fu yin + "ies" -> fu yin + "y": "babies" -> "baby"
        else if ( ToolsWordSameAs ( &newStr[ tail - 3 ], "ies" ) == 0
                  && ToolsWordNotYuanYin ( newStr[ tail - 4 ] ) )
        {
            newStr[ tail - 3 ] = 'y';
            newStr[ tail - 2 ] = chrNull;
        }

        else
        {
            newStr[ tail - 1 ] = chrNull;
        }

        err = errNone;
    }

    // adj/adv : "busy" "busier" "busiest"
    else if ( ToolsWordSameAs ( &newStr[ tail - 3 ], "ier" ) == 0
              && ToolsWordNotYuanYin ( newStr[ tail - 4 ] ) )
    {
        newStr[ tail - 3 ] = 'y';
        newStr[ tail - 2 ] = chrNull;
        err = errNone;
    }
    else if ( ToolsWordSameAs ( &newStr[ tail - 4 ], "iest" ) == 0
              && ToolsWordNotYuanYin ( newStr[ tail - 5 ] ) )
    {
        newStr[ tail - 4 ] = 'y';
        newStr[ tail - 3 ] = chrNull;
        err = errNone;
    }

    // v. + ing
    else if ( ToolsWordSameAs ( &newStr[ tail - 3 ], "ing" ) == 0 )
    {
        newStr[ tail - 3 ] = chrNull;
        err = errNone;
    }

    // v. + ed "selected" -> "select"
    else if ( ToolsWordSameAs ( &newStr[ tail - 2 ], "ed" ) == 0 )
    {
        newStr[ tail - 2 ] = chrNull;
        err = errNone;
    }

    return err;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsSearch
 *
 * DESCRIPTION: This routine search the word that in the special field.
 *
 * PARAMETERS:
 *				->	fieldID Filed resource ID.
 *				<-	matchsP Ret the matchs character number,
 *					LOOK_UP_FULL_PARITY means match nicety.
 *				<-	explainPP Ret the pointer of explain.
 *				<-	explainLenP Ret the explain length.
 *
 * RETURN:
 *				errNone if success else fail.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static Err ToolsSearch( UInt16 fieldID, UInt16 *matchsP, UInt8 **explainPP, UInt32 *explainLenP )
{
    Char	* wordStr;
    UInt8	*explainPtr;
    UInt32	explainLen;
    UInt16	matchs1, matchs2, matchs3, matchsTemp, len;
    AppGlobalType	*global;
    ZDicDBDictInfoType	*dictInfo;
    UInt8	newStr[ 4 ][ MAX_WORD_LEN + 1 ];
    Boolean	dirty1, dirty2;
    Err	err;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    *matchsP = 0;
    *explainPP = NULL;
    *explainLenP = 0;

    wordStr = FldGetTextPtr ( GetObjectPtr( fieldID ) );
    if ( wordStr == NULL )
        return ~errNone;

    // switch case search string.
    StrNCopy( ( Char * ) & newStr[ 0 ][ 0 ], ( Char * ) wordStr, MAX_WORD_LEN );
    newStr[ 0 ][ MAX_WORD_LEN ] = chrNull;

    // Clear the tail space.
    len = StrLen ( ( Char * ) & newStr[ 0 ][ 0 ] );
    while ( len > 0 && newStr[ 0 ][ len - 1 ] == chrSpace )
        len--;
    newStr[ 0 ][ len ] = chrNull;

    matchs1 = matchs2 = matchs3 = 0;
    dirty1 = dirty2 = false;

    err = ZDicLookup( &newStr[ 0 ][ 0 ], &matchs1, &explainPtr, &explainLen, &global->descript );
    *matchsP = matchs1;
    len = StrLen( ( Char * ) & newStr[ 0 ][ 0 ] );
    if ( matchs1 == LOOK_UP_FULL_PARITY )
        goto FIND_OUT;

    if ( ToolsWordAssay ( ( Char* ) & newStr[ 0 ][ 0 ] ) == errNone )
    {
        ZDicLookup( &newStr[ 0 ][ 0 ], &matchsTemp, &explainPtr, &explainLen, &global->descript );
        *matchsP = matchsTemp;
        len = StrLen( ( Char * ) & newStr[ 0 ][ 0 ] );
        if ( matchsTemp == LOOK_UP_FULL_PARITY )
            goto FIND_OUT;

        // restore the old word.
        StrNCopy( ( Char * ) & newStr[ 0 ][ 0 ], ( Char * ) wordStr, MAX_WORD_LEN );
        newStr[ 0 ][ MAX_WORD_LEN ] = chrNull;

        // Clear the tail space.
        len = StrLen ( ( Char * ) & newStr[ 0 ][ 0 ] );
        while ( len > 0 && newStr[ 0 ][ len - 1 ] == chrSpace )
            len--;
        newStr[ 0 ][ len ] = chrNull;

        dirty1 = true;
    }

    if ( ( global->prefs.enableTryLowerSearch && matchs1 < len ) || err )
    {
        dirty1 = true;
        StrCopy( ( Char * ) & newStr[ 1 ][ 0 ], ( Char * ) & newStr[ 0 ][ 0 ] );
        err = ToolsChangeCase( ( Char * ) & newStr[ 1 ][ 0 ] );
        if ( err == errNone )
        {
            err = ZDicLookup( ( UInt8* ) & newStr[ 1 ][ 0 ], &matchs2, &explainPtr, &explainLen, &global->descript );
            *matchsP = matchs2;
            if ( matchs2 == LOOK_UP_FULL_PARITY )
                goto FIND_OUT;
            len = StrLen( ( Char * ) & newStr[ 1 ][ 0 ] );
            if ( matchs2 < len || err )
            {
                dirty2 = true;
                StrCopy( ( Char * ) & newStr[ 2 ][ 0 ], ( Char * ) & newStr[ 1 ][ 0 ] );
                err = ToolsChangeCase( ( Char * ) & newStr[ 2 ][ 0 ] );
                if ( err == errNone )
                {
                    err = ZDicLookup( &newStr[ 2 ][ 0 ], &matchs3, &explainPtr, &explainLen, &global->descript );
                    *matchsP = matchs3;
                    if ( matchs3 == LOOK_UP_FULL_PARITY )
                        goto FIND_OUT;
                }
            }
        }
    }

    err = errNone;
    if ( matchs1 >= matchs2 && matchs1 >= matchs3 )
    {
        if ( dirty1 )
            err = ZDicLookup( &newStr[ 0 ][ 0 ], &matchs1, &explainPtr, &explainLen, &global->descript );
        *matchsP = matchs1;
    }
    else if ( matchs2 >= matchs1 && matchs2 >= matchs3 )
    {
        if ( dirty2 )
            err = ZDicLookup( ( UInt8* ) & newStr[ 1 ][ 0 ], &matchs2, &explainPtr, &explainLen, &global->descript );
        *matchsP = matchs2;
    }
    else
    {
        // it always not dirty, so we do not search again.
        *matchsP = matchs3;
    }

FIND_OUT:
    *explainPP = explainPtr;
    *explainLenP = explainLen;

    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:    ToolsMemAppend
 *
 * DESCRIPTION: This routine append a stirng to the end of memory block.
 *
 * PARAMETERS:  -> srcHPtr Pointer of the memory handle.
 *				-> str String pointer.
 *				-> strLen String length.
 *				-> phoneticIdx Index of phonetic.
 *
 * RETURNED:    errNone if no error
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	14/Aug/04	Initial Revision 
 *
 ***********************************************************************/
static Err ToolsAppendExplain( MemHandle *srcHPtr, UInt8 *str, Int16 strLen, phoneticEnum phoneticIdx )
{
    MemHandle	desH;
    Char	*srcP, *desP;
    UInt32	srcSize, desSize;

    ErrNonFatalDisplayIf ( str == NULL, "Bad parameter" );

    if ( strLen == 0 )
        return errNone;

    if ( *srcHPtr == NULL )
    {
        desH = MemHandleNew( strLen + 1 );
        if ( desH == NULL )
        {
            FrmAlert ( NoEnoughMemoryAlert );
            return memErrNotEnoughSpace;
        }

        desP = MemHandleLock( desH );
        MemMove( desP, str, strLen );
        desP[ strLen ] = chrNull;

        // format string.
        ToolsFormatExplain( ( UInt8* ) desP );
        ToolsGMXTranslate( desP, phoneticIdx );

        MemHandleUnlock( desH );
    }
    else
    {
        srcP = MemHandleLock( *srcHPtr );
        srcSize = StrLen( srcP );
        desSize = srcSize + strLen + 1;

        desH = MemHandleNew( desSize );
        if ( desH == NULL )
        {
            MemHandleUnlock( *srcHPtr );
            FrmAlert ( NoEnoughMemoryAlert );
            return memErrNotEnoughSpace;
        }
        desP = MemHandleLock( desH );

        MemMove( desP, srcP, srcSize );
        MemMove( desP + srcSize, str, strLen );
        desP[ desSize - 1 ] = chrNull;

        // format string.
        ToolsFormatExplain( ( UInt8* ) ( desP + srcSize ) );
        ToolsGMXTranslate( ( desP + srcSize ), phoneticIdx );

        MemHandleUnlock( desH );
        MemHandleUnlock( *srcHPtr );

        MemHandleFree( *srcHPtr );
    }

    *srcHPtr = desH;

    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsAllDictionaryCommand
 *
 * DESCRIPTION: Get explain of all dictionary.
 *
 * PARAMETERS:
 *				->	wordFieldID Key word field resource id.
 *				->	descFieldID Description field resource id.
 *				->	scrollBarID Scroll bar resource id.
 *
 * RETURN:
 *				errNone if success else fail.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static Err ToolsAllDictionaryCommand( UInt16 wordFieldID,
                                      UInt16 descFieldID, UInt16 scrollBarID )
{
    AppGlobalType	* global;
    ZDicDBDictInfoType	*dictInfo;
    ZDicDBDictShortcutInfoType	*shortcutInfo;
    ZDicDBDictPopupInfoType	*popupInfo;
    Int16	oldMainDictIndex,dicIndex = 0;
    UInt8	newStr[ MAX_WORD_LEN + 1 ];
    UInt8	dbName[ dmDBNameLength + 31 ];	// +31, 5 for linefreed character and 26 for "=".
    Char	*wordStr;
    MemHandle	bufH;
    UInt16	matchs, len;
    UInt8	*explainPtr;//,i;
    UInt32	explainLen;
    Err	err;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    shortcutInfo = &global->prefs.shortcutInfo;
    popupInfo = &global->prefs.popupInfo;
    bufH = NULL;

    // Get key string for search.
    wordStr = FldGetTextPtr ( GetObjectPtr( wordFieldID ) );
    if ( wordStr == NULL )
        return errNone;

    StrNCopy( ( Char * ) newStr, wordStr, MAX_WORD_LEN );
    newStr[ MAX_WORD_LEN ] = chrNull;
    len = StrLen ( ( Char * ) newStr );
    if ( len == 0 )
        return errNone;

    // Save current dictioary index and close it.
    oldMainDictIndex = dictInfo->curMainDictIndex;
    ZDicCloseCurrentDict();
    
    /*
    if(global->prefs.dictMenu == 0)
	{
		dicIndex = 0;
	}
	else if(global->prefs.dictMenu == 1)
	{
		dicIndex = shortcutInfo->dictIndex[0];

	}
	else if(global->prefs.dictMenu == 2)
	{
	    dicIndex = popupInfo->dictIndex[0];
	}
    */
    //dictInfo->curMainDictIndex = 0;

	/*
    // Go through all dictionary and get it explain.
    while ( dictInfo->curMainDictIndex < dictInfo->totalNumber )
    {
        err = ZDicOpenCurrentDict();
        if ( err != errNone )
        {
            ToolsQuitApp();
        }

        // Get dictionary name.
        if ( dictInfo->curMainDictIndex == 0 )
        {
            StrCopy( ( Char* ) dbName, &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
        }
        else
        {
            dbName[ 0 ] = '\n';
            dbName[ 1 ] = '\n';
            StrCopy( ( Char* ) & dbName[ 2 ], &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
        }

        // Append linefreed character at the end of dictionary name.
        StrCat( ( Char* ) dbName, "\n\n" );

        err = ToolsAppendExplain ( &bufH, dbName, StrLen( ( Char* ) dbName ), eNonePhonetic );
        if ( err != errNone )
            break;

        err = ToolsSearch( wordFieldID, &matchs, &explainPtr, &explainLen );
        if ( err == errNone && matchs >= len && explainPtr != NULL && explainLen != 0 )
        {
            err = ToolsAppendExplain ( &bufH, explainPtr, explainLen, dictInfo->phonetic[ dictInfo->curMainDictIndex ] );
            if ( err != errNone )
                break;
        }

        ZDicCloseCurrentDict();
        dictInfo->curMainDictIndex++;
    }
	*/
	
	if(global->prefs.dictMenu == 0)
	{
		dictInfo->curMainDictIndex = dicIndex;
		while ( dictInfo->curMainDictIndex < dictInfo->totalNumber )
	    {
	        err = ZDicOpenCurrentDict();
	        if ( err != errNone )
	        {
	            ToolsQuitApp();
	        }

			err = ToolsSearch( wordFieldID, &matchs, &explainPtr, &explainLen );
	        if ( err == errNone && matchs >= len && explainPtr != NULL && explainLen != 0 )
	        {

		        // Get dictionary name.
		        if ( dictInfo->curMainDictIndex == 0 )
		        {
		            StrCopy( ( Char* ) dbName, &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
		        }
		        else
		        {
		            StrCopy( ( Char* ) & dbName[ 0 ], "\n\n============\n\n" );
		            StrCopy( ( Char* ) & dbName[ 16 ], &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
		        }

		        // Append linefreed character at the end of dictionary name.
		        StrCat( ( Char* ) dbName, "\n-------------\n" );

		        err = ToolsAppendExplain ( &bufH, dbName, StrLen( ( Char* ) dbName ), eNonePhonetic );
		        if ( err != errNone )
		            break;

		        err = ToolsSearch( wordFieldID, &matchs, &explainPtr, &explainLen );
		        if ( err == errNone && matchs >= len && explainPtr != NULL && explainLen != 0 )
		        {
		            err = ToolsAppendExplain ( &bufH, explainPtr, explainLen, dictInfo->phonetic[ dictInfo->curMainDictIndex ] );
		            if ( err != errNone )
		                break;
		        }
		    }

	        ZDicCloseCurrentDict();
	        dictInfo->curMainDictIndex++;
	    }
	}
	else if(global->prefs.dictMenu == 1)
	{
		while ( dicIndex < shortcutInfo->totalNumber )
	    {
	    	dictInfo->curMainDictIndex = shortcutInfo->dictIndex[dicIndex];
	        err = ZDicOpenCurrentDict();
	        if ( err != errNone )
	        {
	            ToolsQuitApp();
	        }
	        
			err = ToolsSearch( wordFieldID, &matchs, &explainPtr, &explainLen );
	        if ( err == errNone && matchs >= len && explainPtr != NULL && explainLen != 0 )
	        {

		        // Get dictionary name.
		        if ( dictInfo->curMainDictIndex == 0 )
		        {
		            StrCopy( ( Char* ) dbName, &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
		        }
		        else
		        {
		            StrCopy( ( Char* ) & dbName[ 0 ], "\n\n============\n\n" );
		            StrCopy( ( Char* ) & dbName[ 16 ], &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
		        }

		        // Append linefreed character at the end of dictionary name.
		        StrCat( ( Char* ) dbName, "\n-------------\n" );

		        err = ToolsAppendExplain ( &bufH, dbName, StrLen( ( Char* ) dbName ), eNonePhonetic );
		        if ( err != errNone )
		            break;

		        err = ToolsSearch( wordFieldID, &matchs, &explainPtr, &explainLen );
		        if ( err == errNone && matchs >= len && explainPtr != NULL && explainLen != 0 )
		        {
		            err = ToolsAppendExplain ( &bufH, explainPtr, explainLen, dictInfo->phonetic[ dictInfo->curMainDictIndex ] );
		            if ( err != errNone )
		                break;
		        }
		        
		    }

	        ZDicCloseCurrentDict();
	        dicIndex++;
	    }
	}
	else if(global->prefs.dictMenu == 2)
	{
	    while ( dicIndex < popupInfo->totalNumber )
	    {
	    	dictInfo->curMainDictIndex = popupInfo->dictIndex[dicIndex];
	        err = ZDicOpenCurrentDict();
	        if ( err != errNone )
	        {
	            ToolsQuitApp();
	        }

			err = ToolsSearch( wordFieldID, &matchs, &explainPtr, &explainLen );
	        if ( err == errNone && matchs >= len && explainPtr != NULL && explainLen != 0 )
	        {

		        // Get dictionary name.
		        if ( dictInfo->curMainDictIndex == 0 )
		        {
		            StrCopy( ( Char* ) dbName, &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
		        }
		        else
		        {
		            StrCopy( ( Char* ) & dbName[ 0 ], "\n\n============\n\n" );
		            StrCopy( ( Char* ) & dbName[ 16 ], &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
		        }

		        // Append linefreed character at the end of dictionary name.
		        StrCat( ( Char* ) dbName, "\n-------------\n" );

		        err = ToolsAppendExplain ( &bufH, dbName, StrLen( ( Char* ) dbName ), eNonePhonetic );
		        if ( err != errNone )
		            break;

		        err = ToolsSearch( wordFieldID, &matchs, &explainPtr, &explainLen );
		        if ( err == errNone && matchs >= len && explainPtr != NULL && explainLen != 0 )
		        {
		            err = ToolsAppendExplain ( &bufH, explainPtr, explainLen, dictInfo->phonetic[ dictInfo->curMainDictIndex ] );
		            if ( err != errNone )
		                break;
		        }
			
			}
			
	        ZDicCloseCurrentDict();
	        dicIndex++;
	    }
	}
	
	
    // Clear the menu status from the display
    MenuEraseStatus( 0 );

    // update display.
    ToolsSetFieldHandle( descFieldID, bufH, true );
    ToolsUpdateScrollBar ( descFieldID, scrollBarID );

    // Restore current dictionary.
    dictInfo->curMainDictIndex = oldMainDictIndex;
    err = ZDicOpenCurrentDict();
    if ( err != errNone )
    {
        ToolsQuitApp();
    }

    // Restore decode buffer.
    err = ToolsSearch( wordFieldID, &matchs, &explainPtr, &explainLen );

    return true;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsChangeDictionaryCommand
 *
 * DESCRIPTION: Change dictionary if use select a dict form menu.
 *
 * PARAMETERS:
 *				->	formId Form ID that need to update.
 *				->	command Menu item id.
 *
 * RETURN:
 *				true if handled else false.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static Boolean ToolsChangeDictionaryCommand( UInt16 formId, UInt16 command )
{
    AppGlobalType	* global;
    ZDicDBDictInfoType	*dictInfo;
    ZDicDBDictShortcutInfoType	*shortcutInfo;
    ZDicDBDictPopupInfoType	*popupInfo;
    Boolean	handled = false;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    shortcutInfo = &global->prefs.shortcutInfo;
    popupInfo = &global->prefs.popupInfo;

	if(global->prefs.dictMenu == 0)
	{
	    if ( command >= ZDIC_DICT_MENUID && command < ZDIC_DICT_MENUID + dictInfo->totalNumber )
	    {
	        ZDicCloseCurrentDict();
	        dictInfo->curMainDictIndex = command - ZDIC_DICT_MENUID;
	        FrmUpdateForm( formId, updateDictionaryChanged );

	        handled = true;
	    }
	}
	if(global->prefs.dictMenu == 1)
	{
	    if ( command >= ZDIC_DICT_MENUID && command < ZDIC_DICT_MENUID + shortcutInfo->totalNumber )
	    {
	        ZDicCloseCurrentDict();
	        dictInfo->curMainDictIndex = shortcutInfo->dictIndex[command - ZDIC_DICT_MENUID];
	        shortcutInfo->curIndex = command - ZDIC_DICT_MENUID;
	        FrmUpdateForm( formId, updateDictionaryChanged );

	        handled = true;
	    }
	}
	if(global->prefs.dictMenu == 2)
	{
	    if ( command >= ZDIC_DICT_MENUID && command < ZDIC_DICT_MENUID + popupInfo->totalNumber )
	    {
	        ZDicCloseCurrentDict();
	        dictInfo->curMainDictIndex = popupInfo->dictIndex[command - ZDIC_DICT_MENUID];
	        popupInfo->curIndex = command - ZDIC_DICT_MENUID;
	        FrmUpdateForm( formId, updateDictionaryChanged );

	        handled = true;
	    }
	}

    return handled;
}

static Boolean ToolsChangeListDictionaryCommand( UInt16 formId, UInt16 command )
{
    AppGlobalType	* global;
    ZDicDBDictInfoType	*dictInfo;
    Boolean	handled = false;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;

	if(global->prefs.dictMenu == 1)
		global->prefs.shortcutInfo.curIndex = command;
	if(global->prefs.dictMenu == 2)
		global->prefs.popupInfo.curIndex = command;
	
    if ( command >= 0 && command < dictInfo->totalNumber )
    {
        ZDicCloseCurrentDict();
        dictInfo->curMainDictIndex = command;
        FrmUpdateForm( formId, updateDictionaryChanged );
		
        handled = true;
    }

    return handled;
}

/***********************************************************************
 *
 * FUNCTION:	ToolsPrevDictionaryCommand
 *
 * DESCRIPTION: Change dictionary to previous dictionary with hardkey
 *
 * PARAMETERS:
 *
 * RETURN:
 *				
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		Jemyzhang	02/Apr/08	Initial Revision
 *
 ***********************************************************************/
static void ToolsPrevDictionaryCommand(void)
{
    AppGlobalType	* global = AppGetGlobal();

    UInt16 dicIndex,i;
    
    if(global->prefs.dictMenu == 0)
	{
		dicIndex = global->prefs.dictInfo.curMainDictIndex;
	}
	else if(global->prefs.dictMenu == 1)
	{
		for(i =0;i<global->prefs.shortcutInfo.totalNumber;i++)
	    {
	    	if(global->prefs.shortcutInfo.dictIndex[i] == global->prefs.dictInfo.curMainDictIndex)
	    	{
	    		dicIndex = i;
	    		break;
	    	}
	    }

	}
	else if(global->prefs.dictMenu == 2)
	{
	    for(i =0;i<global->prefs.popupInfo.totalNumber;i++)
	    {
	    	if(global->prefs.popupInfo.dictIndex[i] == global->prefs.dictInfo.curMainDictIndex)
	    	{
	    		dicIndex = i;
	    		break;
	    	}
	    }
	}
    
    if(global->prefs.dictMenu == 0)
	{
	    if(dicIndex > 0)
	    {
	        dicIndex --;
	    }
	    else
	    {
	        dicIndex = global->prefs.dictInfo.totalNumber - 1;
	    }
	}
	if(global->prefs.dictMenu == 1)
	{
		if(dicIndex > 0)
	    {
	        dicIndex --;
	    }
	    else
	    {
	        dicIndex = global->prefs.shortcutInfo.totalNumber - 1;
	    }
	}
	if(global->prefs.dictMenu == 2)
	{
	    if(dicIndex > 0)
	    {
	        dicIndex --;
	    }
	    else
	    {
	        dicIndex = global->prefs.popupInfo.totalNumber - 1;
	    }
	}
	
    ToolsChangeDictionaryCommand(MainForm,dicIndex + ZDIC_DICT_MENUID);
}

/***********************************************************************
 *
 * FUNCTION:	ToolsNextDictionaryCommand
 *
 * DESCRIPTION: Change dictionary to next dictionary with hardkey
 *
 * PARAMETERS:
 *
 * RETURN:
 *				
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		Jemyzhang	02/Apr/08	Initial Revision
 *
 ***********************************************************************/
static void ToolsNextDictionaryCommand(void){
    AppGlobalType	* global = AppGetGlobal();
    
    UInt16 dicIndex,i;
    
    if(global->prefs.dictMenu == 0)
	{
		dicIndex = global->prefs.dictInfo.curMainDictIndex;
	}
	else if(global->prefs.dictMenu == 1)
	{
		for(i =0;i<global->prefs.shortcutInfo.totalNumber;i++)
	    {
	    	if(global->prefs.shortcutInfo.dictIndex[i] == global->prefs.dictInfo.curMainDictIndex)
	    	{
	    		dicIndex = i;
	    		break;
	    	}
	    }

	}
	else if(global->prefs.dictMenu == 2)
	{
	    for(i =0;i<global->prefs.popupInfo.totalNumber;i++)
	    {
	    	if(global->prefs.popupInfo.dictIndex[i] == global->prefs.dictInfo.curMainDictIndex)
	    	{
	    		dicIndex = i;
	    		break;
	    	}
	    }
	}
	
    if(global->prefs.dictMenu == 0)
	{
        if(dicIndex < global->prefs.dictInfo.totalNumber - 1)
		{
		    dicIndex ++;
		}
		else
		{
		    dicIndex = 0;
		}
	}
	if(global->prefs.dictMenu == 1)
	{
		if(dicIndex < global->prefs.shortcutInfo.totalNumber - 1)
		{
		    dicIndex ++;
		}
		else
		{
		    dicIndex = 0;
		}
	}
	if(global->prefs.dictMenu == 2)
	{
		if(dicIndex < global->prefs.popupInfo.totalNumber - 1)
		{
		    dicIndex ++;
		}
		else
		{
		    dicIndex = 0;
		}
	}
	
    ToolsChangeDictionaryCommand(MainForm,dicIndex + ZDIC_DICT_MENUID);
}
/***********************************************************************
 *
 * FUNCTION:	ToolsCreatDictionarysMenu
 *
 * DESCRIPTION: Initial dictionary popmenu.
 *
 * PARAMETERS:
 *				nothing.
 *
 * RETURN:
 *				nothing.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static void ToolsCreatDictionarysMenu( void )
{

    AppGlobalType	* global;
    ZDicDBDictInfoType	*dictInfo;
    ZDicDBDictShortcutInfoType	*shortcutInfo;
    ZDicDBDictPopupInfoType	*popupInfo;
    Int16	i;
    UInt16	baseID;
    Err	err;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    shortcutInfo = &global->prefs.shortcutInfo;
    popupInfo = &global->prefs.popupInfo;

    baseID = DictionarysDictAll;
    if(global->prefs.menuType == 0)
    {
    	if(global->prefs.dictMenu == 0)
   		{
		    for ( i = 0; i < dictInfo->totalNumber; i++ )
		    {
	        	err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, '1' + i, &dictInfo->displayName[ i ][ 0 ] );
	       		baseID = ZDIC_DICT_MENUID + i;
		    }
		}
		if(global->prefs.dictMenu == 1)
   		{
		    for ( i = 0; i < shortcutInfo->totalNumber; i++)
		    {
	        	err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, '1' + i, &dictInfo->displayName[ shortcutInfo->dictIndex[i] ][ 0 ] );
	        	baseID = ZDIC_DICT_MENUID + i;
		    }
		}
		if(global->prefs.dictMenu == 2)
   		{
		    for ( i = 0; i < popupInfo->totalNumber; i++)
		    {
	        	err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, '1' + i, &dictInfo->displayName[ popupInfo->dictIndex[i] ][ 0 ] );
	        	baseID = ZDIC_DICT_MENUID + i;
		    }
		}
    }
    else if(global->prefs.menuType == 1)
   	{
   		if(global->prefs.dictMenu == 0)
   		{
		    for ( i = 0; i < dictInfo->totalNumber  && i < 26; i++)
		    {
		        err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, 'A' + i, &dictInfo->displayName[ i ][ 0 ] );
		        baseID = ZDIC_DICT_MENUID + i;
		    }
		}
		if(global->prefs.dictMenu == 1)
   		{
		    for ( i = 0; i < shortcutInfo->totalNumber  && i < 26; i++)
		    {
		        err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, 'A' + i, &dictInfo->displayName[ shortcutInfo->dictIndex[i] ][ 0 ] );
		        baseID = ZDIC_DICT_MENUID + i;
		    }
		}
		if(global->prefs.dictMenu == 2)
   		{
		    for ( i = 0; i < popupInfo->totalNumber  && i < 26; i++)
		    {
		        err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, 'A' + i, &dictInfo->displayName[ popupInfo->dictIndex[i] ][ 0 ] );
		        baseID = ZDIC_DICT_MENUID + i;
		    }
		}
	}
	
    else if(global->prefs.menuType == 2)
    {
    	if(global->prefs.dictMenu == 0)
   		{
		    for ( i = 0; i < dictInfo->totalNumber; i++ )
		    {
	    		err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, dictInfo->menuShortcut[i], &dictInfo->displayName[ i ][ 0 ] );
	       		baseID = ZDIC_DICT_MENUID + i;
		    }
		}
		if(global->prefs.dictMenu == 1)
   		{
		    for ( i = 0; i < shortcutInfo->totalNumber; i++)
		    {
	    		err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, dictInfo->menuShortcut[ shortcutInfo->dictIndex[i] ], &dictInfo->displayName[ shortcutInfo->dictIndex[i] ][ 0 ] );
	        	baseID = ZDIC_DICT_MENUID + i;
		    }
		}
		if(global->prefs.dictMenu == 2)
   		{
		    for ( i = 0; i < popupInfo->totalNumber; i++)
		    {
	    		err = MenuAddItem ( baseID, ZDIC_DICT_MENUID + i, dictInfo->menuShortcut[ popupInfo->dictIndex[i] ], &dictInfo->displayName[ popupInfo->dictIndex[i] ][ 0 ] );
	        	baseID = ZDIC_DICT_MENUID + i;
		    }
		}
    }
	
    return ;
}

/***********************************************************************
 *
 * FUNCTION:    ToolsGetFieldHighlightText
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:	buf	<-> a point of buffer.
 *		size	 -> Buffer size.
 *		nearWord -> true if no highlight text then ret nearest text.
 *
 * RETURNED:	Err - zero if no error, else the error
 *
 *
 ***********************************************************************/
static Err ToolsGetFieldHighlightText( FieldPtr fieldP, Char *buf, UInt32 size, Boolean nearWord )
{
    UInt16 startPosition;
    UInt16 endPosition;
    Char * fieldText;
    Err err = ~errNone;

    if ( fieldP == NULL )
        return err;

    fieldText = FldGetTextPtr ( fieldP );
    if ( fieldText == NULL )
        return err;

    FldGetSelection ( fieldP, &startPosition, &endPosition );
    if ( startPosition == endPosition )
    {
        Boolean foundWord;
        UInt32 start, end;

        if ( !nearWord )
            return err;

        // Get the word that near by insertion position.
        // Is there a word to select? We'll go for the word following the passed
        // offset (better would be to use offset + leading edge flag), and if that
        // doesn't work, check the word _before_ the offset.
        foundWord = TxtWordBounds( fieldText, StrLen( fieldText ), startPosition, &start, &end );
        if ( ( !foundWord ) && ( startPosition > 0 ) )
        {
            UInt16 prevPos = startPosition - TxtPreviousCharSize( fieldText, startPosition );
            foundWord = TxtWordBounds( fieldText, StrLen( fieldText ), prevPos, &start, &end );
        }

        if ( !foundWord )
            return err;

        startPosition = start;
        endPosition = end;
    }

    if ( size != 0 && buf != NULL )
    {
        if ( size >= endPosition - startPosition + 1 )
            size = endPosition - startPosition;
        MemMove( buf, fieldText + startPosition, size );
        MemSet( buf + size, 1, 0 );
    }

    err = errNone;
    return err;
}

/***********************************************************************
 *
 * FUNCTION:    ToolsGetFormHighlightText
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:	
 *
 * RETURNED:	Err - zero if no error, else the error
 *
 *
 ***********************************************************************/
static Err ToolsGetFormHighlightText( FormPtr frmPtr, Char *buf, UInt32 size )
{
    FieldType	* fieldP;
    TableType	*tableP;
    UInt16	numObjects, focusIdx, i;

    if ( frmPtr == NULL )
        return ~errNone;

    numObjects = FrmGetNumberOfObjects( frmPtr );
    focusIdx = FrmGetFocus ( frmPtr );

    // check focus field at first, if it is nothing then go through all fields.
    i = focusIdx;
    while ( i < numObjects )
    {
        switch ( FrmGetObjectType( frmPtr, i ) )
        {
        case frmFieldObj:
            fieldP = FrmGetObjectPtr( frmPtr, i );
            break;
        case frmTableObj:
            tableP = FrmGetObjectPtr ( frmPtr, i );
            fieldP = TblGetCurrentField( tableP );
            break;
        default:
            fieldP = NULL;
            break;
        }

        if ( fieldP != NULL )
        {
            if ( ToolsGetFieldHighlightText( fieldP, buf, size, true ) == errNone )
                return errNone;
        }

        if ( i == focusIdx )
        {
            // there is nothing in active field, so we go through all fields.
            focusIdx = 0xffff;
            i = 0;
        }
        else
            i++;
    }

    return ~errNone;
}

/***********************************************************************
 *
 * FUNCTION: 	 ToolsFontTruncateName
 *
 * DESCRIPTION: This routine trunctated a category name such that it
 *              is short enough to display.
 *
 * PARAMETERS:	name    - pointer to the name of the new category
 *				maxWidth	- maximum pixel width that name can occupy.
 *				font - font will to be displayed.
 *
 * RETURNED:	 nothing
 *
 *
 ***********************************************************************/
static void ToolsFontTruncateName ( Char * name, FontID font, UInt16 maxWidth )
{
    UInt32 bytesThatFit;
    UInt32 charEnd;
    Int16 truncWidth;
    UInt32 length;
    FontID curFont;
    AppGlobalType* global;

    global = AppGetGlobal();

    curFont = ZDicFontSet ( global->fontLibrefNum, &global->font, font );

    length = StrLen( name );
    bytesThatFit = FntWidthToOffset( name, length, maxWidth, NULL, &truncWidth );

    // If we need to truncate, then figure out how far back to trim to
    // get enough horizongal width for the ellipsis character.

    if ( bytesThatFit < length )
    {
        Int16 ellipsisWidth = FntCharWidth( chrEllipsis );

        // Not enough space for a single ellipsis, so return an empty string.

        if ( ellipsisWidth > maxWidth )
        {
            name[ 0 ] = '\0';
            return ;
        }
        else if ( ellipsisWidth > maxWidth - truncWidth )
        {
            bytesThatFit = FntWidthToOffset( name, length, maxWidth - ellipsisWidth, NULL, &truncWidth );
        }

        // Should never happen, but make sure we don't create a string that's
        // longer than the max category length (leave room for ellipsis & null)
        bytesThatFit = bytesThatFit < dmDBNameLength - 2
                       ? bytesThatFit : dmDBNameLength - 2;

        // Also make sure we don't truncate in the middle of a character.
        TxtCharBounds( name, bytesThatFit, &bytesThatFit, &charEnd );
        bytesThatFit += TxtSetNextChar( name, bytesThatFit, chrEllipsis );
        name[ bytesThatFit ] = '\0';
    }
    else if ( length > dmCategoryLength - 1 )
    {
        TxtCharBounds( name, dmCategoryLength - 1, &bytesThatFit, &charEnd );
        name[ bytesThatFit ] = '\0';
    }

    ZDicFontSet ( global->fontLibrefNum, &global->font, curFont );
}


/*
 * FUNCTION: ToolsPutWordFieldToHistory
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */

static Err ToolsPutWordFieldToHistory( UInt16 fieldID )
{
    Char * wordStr;
    AppGlobalType *	global;
    Boolean exist = false;
    Int16	i;

    global = AppGetGlobal();

    wordStr = FldGetTextPtr ( GetObjectPtr( fieldID ) );
    if ( wordStr == NULL || wordStr[ 0 ] == chrNull )
        return errNone;


    for ( i = 0; i < MAX_HIS_FAR && global->prefs.history[ i ][ 0 ] != chrNull; i++ )
    {
        if ( StrCompare ( &global->prefs.history[ i ][ 0 ], wordStr ) == 0 )
        {
            // if the word already in history then move it to top;
            if ( i != 0 )
            {
                MemMove( &global->prefs.history[ 1 ][ 0 ], &global->prefs.history[ 0 ][ 0 ], ( MAX_WORD_LEN + 1 ) * i );
                StrCopy( &global->prefs.history[ 0 ][ 0 ], wordStr );
            }
            exist = true;
        }
    }

    if ( !exist )
    {
        // push current key string to history.
        MemMove( &global->prefs.history[ 1 ][ 0 ],
                 &global->prefs.history[ 0 ][ 0 ], ( MAX_HIS_FAR - 1 ) * ( MAX_WORD_LEN + 1 ) );
        StrNCopy( &global->prefs.history[ 0 ][ 0 ], wordStr, MAX_WORD_LEN );
    }

    // reset the history seek.
    global->historySeekIdx = 0;

    return errNone;
}

/*
 * FUNCTION: ToolsGetStartWord
 *
 * DESCRIPTION:
 *
 * PARAMETERS: 
 *
 * RETURNED:    nothing
 *
 */

static void ToolsGetStartWord( AppGlobalType *global )
{
    UInt16	textLen;
    MemHandle	textH;
    Char	*textPtr;

    global->initKeyWord[ 0 ] = chrNull;

    // Get text from clipboard if it is enable.
    if ( global->prefs.getClipBoardAtStart )
    {
        textH = ClipboardGetItem ( clipboardText, &textLen );
        if ( ( textH != NULL ) && ( textLen != 0 ) )
        {
            textPtr = MemHandleLock( textH );
            if ( textLen > MAX_WORD_LEN )
                textLen = MAX_WORD_LEN;
            MemMove( &global->initKeyWord[ 0 ], textPtr, textLen );
            global->initKeyWord[ textLen ] = chrNull;
            MemHandleUnlock( textH );
        }
    }

    // if the clip board is empty then get highlight string.
    if ( global->initKeyWord[ 0 ] == chrNull )
    {
        ToolsGetFormHighlightText( global->prvActiveForm, &global->initKeyWord[ 0 ], MAX_WORD_LEN );
    }

    if ( global->initKeyWord[ 0 ] == chrNull && global->prefs.history[ 0 ][ 0 ] != chrNull )
    {
        MemMove( &global->initKeyWord[ 0 ], &global->prefs.history[ 0 ][ 0 ], MAX_WORD_LEN );
        global->initKeyWord[ MAX_WORD_LEN ] = chrNull;
    }

    // clear head space character and tail space.
    if ( global->initKeyWord[ 0 ] != chrNull )
    {
        Int16	i;

        i = 0;
        while ( global->initKeyWord[ i ] == chrSpace )
            i++;

        if ( i != 0 )
            StrCopy ( &global->initKeyWord[ 0 ], &global->initKeyWord[ i ] );

        i = StrLen ( &global->initKeyWord[ 0 ] );
        while ( i > 0 && global->initKeyWord[ i - 1 ] == chrSpace )
            i--;
        global->initKeyWord[ i ] = chrNull;
    }

    return ;
}

/*
 * FUNCTION: ToolsClearInput
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 */

static Boolean ToolsClearInput( UInt16 fieldID )
{
    FormType	* frmP;
    UInt16	idx;

    ToolsSetFieldHandle( fieldID, NULL, true );
    frmP = FrmGetActiveForm();
    idx = FrmGetObjectIndex( frmP, fieldID );
    FrmSetFocus ( frmP, idx );

    return true;
}

/*
 * FUNCTION: ToolsPlayVoice
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */

static Err ToolsPlayVoice ( void )
{
    UInt8 * explainPtr;
    UInt32 explainLen;
    AppGlobalType *global;
    Err err = errNone;

    global = AppGetGlobal();

    err = ZDicLookupCurrent( &explainPtr, &explainLen, &global->descript );
    if ( err == errNone )
    {
        ZDicVoicePlayWord ( ( Char* ) explainPtr );
    }

    return err;
}

/*
 * FUNCTION: ToolsSendMenuCmd
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */

static void ToolsSendMenuCmd ( UInt16 chr )
{
    EventType event;
    Err error;

    event.eType = keyDownEvent;
    event.penDown = 0;
    event.tapCount = 1;
    event.data.keyDown.chr = vchrCommand;
    event.data.keyDown.keyCode = vchrCommand;
    event.data.keyDown.modifiers = commandKeyMask;
    if ( MenuHandleEvent( 0, &event, &error ) )
    {
        EvtGetEvent( &event, evtNoWait );
        EvtGetEvent( &event, evtNoWait );
        EvtGetEvent( &event, evtNoWait );

        event.eType = menuCmdBarOpenEvent;
        FrmDispatchEvent( &event );
        event.eType = keyDownEvent;
        event.penDown = 0;
        event.tapCount = 1;
        event.data.keyDown.chr = chr;
        event.data.keyDown.keyCode = 0;
        event.data.keyDown.modifiers = 0;
        EvtAddEventToQueue ( &event );
    }

    return ;
}

/*
 * FUNCTION: ToolsHomeKeyChr
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */
 
static WChar ToolsHomeKeyChr(void) {
  UInt32 company = 0;
  UInt32 device = 0;
  WChar res = vchrHard1;

  FtrGet(sysFtrCreator, sysFtrNumOEMCompanyID, &company);
  
  if (company == 'hspr' || company == 'Palm' || company == 'palm') {
    FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &device);

    if (device == kPalmOneDeviceIDTreo650 || device == kPalmOneDeviceIDTreo650Sim ) 
    {
	res = vchrLaunch;
    }
    if (device == 'D053'|| device == 'D061')// 'D053' treo680,'D061' Centro
    {
	res = vchrHard4;
    }
  }

  return res;
}

static void ToolsSetOptKeyStatus(Char s){
    AppGlobalType	*global;

    global = AppGetGlobal();

	global->optflag = s;
	FtrSet( appFileCreator, AppGlobalFtr, ( UInt32 ) global );

}

static char ToolsGetOptKeyStatus(void){
    AppGlobalType	*global;

    global = AppGetGlobal();

	return global->optflag;

}

#pragma mark -

/***********************************************************************
 *
 * FUNCTION:     AppReigsterWinDisplayChangedNotification
 *
 * DESCRIPTION:  Register for NotifyMgr notifications for WinDisplayChanged.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     nothing
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static void AppReigsterWinDisplayChangedNotification(void)
{
	UInt16 cardNo;
	LocalID dbID;
	Err err;

	err = SysCurAppDatabase(&cardNo, &dbID);
	ErrNonFatalDisplayIf(err != errNone, "can't get app db info");
	if(err == errNone)
	{
		err = SysNotifyRegister(cardNo, dbID, sysNotifyDisplayResizedEvent,
								NULL, sysNotifyNormalPriority, NULL);

#if EMULATION_LEVEL == EMULATION_NONE
		ErrNonFatalDisplayIf((err != errNone) && (err != sysNotifyErrDuplicateEntry), "can't register");
#endif

	}

	return;
}
/*
 * FUNCTION: AppGetGlobal
 *
 * DESCRIPTION: 
 *
 * This routine return global pointer.
 *
 * PARAMETERS:
 *
 * nothing.
 *
 * RETURNED:
 *     return global pointer if success else return NULL. 
 */

AppGlobalType * AppGetGlobal()
{
    UInt32	global;
    Err	err;

    err = FtrGet( appFileCreator, AppGlobalFtr, &global );
    return err == errNone ? ( AppGlobalType * ) global : NULL;
}

/*
 * FUNCTION: AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.
 */

static void AppEventLoop()
{
    UInt16 error;
    EventType event;
    Int32 ticksPreHalfSecond;
    FormType *form, *originalForm;
    AppGlobalType *global;
    
    global = AppGetGlobal();


    ticksPreHalfSecond = SysTicksPerSecond() / TIMES_PRE_SECOND;

    // Remember the original form
    originalForm = FrmGetActiveForm();
    form = FrmInitForm( MainForm );
    FrmSetActiveForm( form );

    event.eType = frmOpenEvent;
    MainFormHandleEvent( &event );

    do
    {
        /* change timeout if you need periodic nilEvents */
        EvtGetEvent( &event, ticksPreHalfSecond /*evtWaitForever*/ );
    	if ( event.eType == keyDownEvent)/*quit silently. jz080401*/
    	{
    		if (event.data.keyDown.modifiers & commandKeyMask
				&& event.data.keyDown.chr == ToolsHomeKeyChr() )
			{
    			break;
    		}
    		else if (event.data.keyDown.chr == ToolsHomeKeyChr() )
			{
    			break;
    		}
			if(event.data.keyDown.modifiers & optionKeyMask	//enable option key
				&& event.data.keyDown.chr == 0x160d)
			{
				Char s = ToolsGetOptKeyStatus();
				s = s > 1 ? 0 : s+1;
				ToolsSetOptKeyStatus(s);
			}
    	}
        if ( event.eType != nilEvent )
            error = errNone;

		/*if(event.data.keyDown.chr == 0x160D)
		{
			if(SysHandleEvent(&event)) continue;
		}
		else */
		if(event.data.keyDown.chr == 0x160D)
		{
			if(( event.data.keyDown.keyCode != (WChar)global->prefs.keyPlaySoundKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyExportKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyWordListKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyHistoryKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyOneKeyChgDicKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyClearFieldKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyShortcutKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyPopupKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyGobackKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keySearchAllKeycode))
			{
				if(SysHandleEvent(&event)) continue;
			}
		}
		else if(( event.data.keyDown.chr != (WChar)global->prefs.keyPlaySoundChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyExportChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyWordListChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyHistoryChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyOneKeyChgDicChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyClearFieldChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyShortcutChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyPopupChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyGobackChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keySearchAllChr))
		{
			if(SysHandleEvent(&event)) continue;
		}

        if ( MenuHandleEvent( NULL, &event, &error ) )
            continue;

        if ( MainFormHandleEvent( &event ) )
            continue;

        FrmHandleEvent( FrmGetActiveForm(), &event );

    }
    while ( event.eType != appStopEvent );

    event.eType = frmCloseEvent;
    MainFormHandleEvent( &event );

    FrmEraseForm( form );
    FrmDeleteForm( form );

    FrmSetActiveForm ( originalForm );
}

/*
 * FUNCTION: AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for da sub launch.
 */

static void AppDAEventLoop( Boolean enableSmallDA, Boolean *bGotoMainForm )
{
    EventType event;
    Boolean done = false;
    UInt16 error;
    Int32 ticksPreHalfSecond;
    AppGlobalType *global;
    
    global = AppGetGlobal();
	
    ticksPreHalfSecond = SysTicksPerSecond() / TIMES_PRE_SECOND;
    *bGotoMainForm = false;

    // Initial form
    event.eType = frmOpenEvent;
    if ( enableSmallDA )
        DAFormHandleEvent( &event );
    else
        MainFormHandleEvent( &event );

    do
    {
        EvtGetEvent( &event, ticksPreHalfSecond );
    	if ( event.eType == keyDownEvent)/*quit silently. jz080401*/
    	{
    		if (event.data.keyDown.modifiers & commandKeyMask)
    		{
    			const UInt16 chr = event.data.keyDown.chr;
    			if(chr == ToolsHomeKeyChr())
    			break;
    		}
    		else
    		{
    			const UInt16 chr = event.data.keyDown.chr;
    			if(chr == ToolsHomeKeyChr())
    				break;
    		}
			if(event.data.keyDown.modifiers & optionKeyMask	//enable option key
				&& event.data.keyDown.chr == 0x160d)
			{
				Char s = ToolsGetOptKeyStatus();
				s = s > 1 ? 0 : s+1;
				ToolsSetOptKeyStatus(s);
			}
    	}

        if ( event.eType == keyDownEvent
                && EvtKeydownIsVirtual( &event )
                && event.data.keyDown.chr == vchrJogBack)
        {
            break;
        }
        
		// volup,voldown,sidekey
		
		if(event.data.keyDown.chr == 0x160D)
		{
			if(( event.data.keyDown.keyCode != (WChar)global->prefs.keyPlaySoundKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyExportKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyWordListKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyHistoryKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyOneKeyChgDicKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyClearFieldKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyEnlargeDAKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyShortcutKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyPopupKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keyGobackKeycode) &&
			(event.data.keyDown.keyCode != (WChar)global->prefs.keySearchAllKeycode))
			{
				if(SysHandleEvent(&event)) continue;
			}
		}
		else if(( event.data.keyDown.chr != (WChar)global->prefs.keyPlaySoundChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyExportChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyWordListChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyHistoryChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyOneKeyChgDicChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyClearFieldChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyEnlargeDAChr)&&
			(event.data.keyDown.chr != (WChar)global->prefs.keyShortcutChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyPopupChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keyGobackChr) &&
			(event.data.keyDown.chr != (WChar)global->prefs.keySearchAllChr))
		{
			if(SysHandleEvent(&event)) continue;
		}
        if ( MenuHandleEvent( NULL, &event, &error ) )
            continue;

        if ( enableSmallDA )
        {
            // Handle More button. we exit da form and goto main form.
            if ((event.eType == keyDownEvent && event.data.keyDown.chr == (WChar)global->prefs.keyEnlargeDAChr && event.data.keyDown.keyCode == (WChar)global->prefs.keyEnlargeDAKeycode) ||
            	(event.eType == ctlSelectEvent && event.data.ctlSelect.controlID == DAMoreButton) )
            {
                FieldType	* field;
                Char *str;

                // pass content of word field to Main form.
                field = GetObjectPtr( DAWordField );
                str = FldGetTextPtr( field );
                if ( str != NULL && *str != chrNull )
                {
                    AppGlobalType * global;

                    // Get highlight text.
                    global = AppGetGlobal();
                    StrNCopy( &global->initKeyWord[ 0 ], str, MAX_WORD_LEN );
                    global->initKeyWord[ MAX_WORD_LEN ] = chrNull;
                }

                *bGotoMainForm = true;
                break;
            }

            // pen outside da form then exit.
            else if ( event.eType == penDownEvent )
            {
                RectangleType bounds;

                FrmGetFormBounds( FrmGetActiveForm(), &bounds );
                RctInsetRectangle( &bounds, 2 );
                if ( IsOutside( &bounds, event.screenX, event.screenY ) )
                    break;
            }

        }
        else
        {
            // Handle exit button.
            if ( event.eType == ctlSelectEvent && event.data.ctlSelect.controlID == MainExitButton )
                break;
        }


        if ( enableSmallDA )
        {
            if ( DAFormHandleEvent( &event ) )
                continue;
        }
        else
        {
            if ( MainFormHandleEvent( &event ) )
                continue;
        }

        FrmHandleEvent( FrmGetActiveForm(), &event );

    }
    while ( event.eType != appStopEvent );

    // pass appstop event to background form.
    if ( event.eType == appStopEvent )
    {
        ToolsQuitApp();
    }

    event.eType = frmCloseEvent;
    if ( enableSmallDA )
        DAFormHandleEvent( &event );
    else
    {    
        MainFormHandleEvent( &event );        
	}

}

/*
 * FUNCTION: AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * RETURNED:
 *     errNone - if nothing went wrong
 */

static Err AppStart( Boolean subLaunch ,UInt8 dictMenu)
{
    AppGlobalType *global, *oldGlobal;
    UInt16 prefsSize;
    Int16 prefsVersion, itemNum;
    Err	err = ~errNone;
    UInt32			company_id;
	UInt32			device_id;

    // global already exist, so only double it and return.
    oldGlobal = AppGetGlobal();//��ȡ������saved prefs���Zdic����

    // We force to only two lever deep and only have one DA.
    if ( oldGlobal != NULL//������ھ�����
        && ( oldGlobal->prvGlobalP != NULL || oldGlobal->subLaunch ) )
        return memErrNotEnoughSpace;

    global = ( AppGlobalType * ) MemPtrNew( sizeof( AppGlobalType ) );//Ϊ��ǰ��global�����ڴ�ռ�
    if ( global == NULL )
        return memErrNotEnoughSpace;

    MemSet( global, sizeof( AppGlobalType ), 0 );//�ѵ�ǰ��globalд�뵽��̬��ջ

    err = FtrSet( appFileCreator, AppGlobalFtr, ( UInt32 ) global );//�Ѷ�ջ���globalд��feature
    if ( err != errNone )
        goto exit;

    // Initial global.
    global->prvGlobalP = oldGlobal;
    global->subLaunch = subLaunch;

    /* Read the saved preferences / saved-state information. */
    prefsSize = sizeof( global->prefs );//��ȡ��ǰglobal->prefs�Ĵ�С
    prefsVersion = PrefGetAppPreferences( appFileCreator, appPrefID, &global->prefs, &prefsSize, true );
    										//��ȡ֮ǰ���ݵİ汾�����Ұ�֮ǰ�����ݱ��浽global->prefs��
    
    if ( prefsVersion != appPrefVersionNum || prefsSize != sizeof( global->prefs ) )//�Ƚϰ汾���Ƚϵ�ǰglobal->prefs��֮ǰglobal->prefs�Ĵ�С
    {
        prefsVersion = noPreferenceFound;
    }

    if ( prefsVersion != appPrefVersionNum )
    {
        /* no prefs; initialize pref struct with default values */
        MemSet( &global->prefs, sizeof( global->prefs ), 0 );
        
		//�����ͺ�
		FtrGet(sysFtrCreator, sysFtrNumOEMCompanyID, &company_id);
		FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &device_id);
		
		if ((company_id == 'Palm' || company_id == 'hspr') && 
			(device_id == 'H102' || device_id == 'H202' ||
			device_id == 'D053'|| device_id == 'D061' || device_id == 'D062' ||
			device_id == 'D052'|| device_id == 'H101' || device_id == 'H201'))
		{
			global->prefs.isTreo = true;	
		}
		else global->prefs.isTreo = false;
		
        global->prefs.font = stdFont;
        global->prefs.fontDA = stdFont;
        global->prefs.getClipBoardAtStart = false;
        global->prefs.enableIncSearch = true;
        global->prefs.enableSingleTap = true;
        global->prefs.enableWordList = false;
        global->prefs.enableHighlightWord = true;
        global->prefs.enableTryLowerSearch = true;
        global->prefs.useSystemFont = false;
        global->prefs.enableJumpSearch = true;
        global->prefs.enableAutoSpeech = false;
        //global->prefs.daEditable = false;
        //global->prefs.enableTinyFont = false;
        global->prefs.menuType = 0;
        global->prefs.daSize = 0;
        global->prefs.dictMenu = 0;
        global->prefs.daFormLocation.x = ResLoadConstant( DAFormOriginX );
        global->prefs.daFormLocation.y = ResLoadConstant( DAFormOriginY );
        global->prefs.incSearchDelay = ResLoadConstant( DefaultIncSearchDelay );
        global->prefs.exportCagetoryIndex = dmUnfiledCategory;
        global->prefs.exportPrivate = false;
        global->prefs.exportAppCreatorID = sysFileCMemo;

        global->prefs.OptUD = false;
        global->prefs.UD = false;
        global->prefs.OptLR = false;
        global->prefs.LR = false;
        global->prefs.SwitchUDLR = false;
        global->prefs.SelectKeyUsed = 0;
        global->prefs.SelectKeyFunc = 3;
		global->prefs.OptSelectKeyFunc = 0;
		
        global->prefs.keyPlaySoundChr = chrNull;
        global->prefs.keyWordListChr = chrNull;
        global->prefs.keyHistoryChr = chrNull;
        global->prefs.keyOneKeyChgDicChr = chrNull;
        global->prefs.keyEnlargeDAChr = chrNull;
        global->prefs.keyExportChr = chrNull;
        global->prefs.keyClearFieldChr = chrNull;
        global->prefs.keyShortcutChr = chrNull;
        global->prefs.keyPopupChr = chrNull;
        global->prefs.keyGobackChr = chrNull;
        global->prefs.keySearchAllChr = chrNull;
	
        global->prefs.keyPlaySoundKeycode = chrNull;
        global->prefs.keyWordListKeycode = chrNull;
        global->prefs.keyHistoryKeycode = chrNull;
        global->prefs.keyOneKeyChgDicKeycode = chrNull;
        global->prefs.keyEnlargeDAKeycode = chrNull;
        global->prefs.keyExportKeycode = chrNull;
        global->prefs.keyClearFieldKeycode = chrNull;
		global->prefs.keyShortcutKeycode = chrNull;
		global->prefs.keyPopupKeycode = chrNull;
		global->prefs.keyGobackKeycode = chrNull;
		global->prefs.keySearchAllKeycode = chrNull;

        // Do not initial dictionary list at start every time. it maybe slowly.
        // We initial dictionary when
        // 1. fist time to run.
        // 2. open preferences dialog in menu.
        // 3. open dictionary fail.

        // Initial dictionary list.
        itemNum = ZDicDBInitDictList( appDBType, appKDicCreator );
        if ( itemNum == 0 )
        {
            FrmAlert ( DictNotFoundAlert );
            err = ~errNone;
            goto exit;
        }
		
        // Initial index record of dictionary database that in dictionary list.
        ZDicDBInitIndexRecord();
    }

	if(dictMenu == 0)
    {
    	global->prefs.dictMenu = 0;
    }
    else if(dictMenu == 1)
    {
    	global->prefs.dictMenu = 1;
    	global->prefs.dictInfo.curMainDictIndex = global->prefs.shortcutInfo.dictIndex[global->prefs.shortcutInfo.curIndex];
    }
    else if(dictMenu == 2)
    {
    	global->prefs.dictMenu = 2;
    	global->prefs.dictInfo.curMainDictIndex = global->prefs.popupInfo.dictIndex[global->prefs.popupInfo.curIndex];
    }

    // Open current dictionary.
    err = ZDicOpenCurrentDict ();
    if ( err != errNone )
        goto exit;

    // Initial font share library.
    err = ZDicToolsLibInitial ( ZDicFontTypeID, ZDicFontCreatorID, ZDicFontName,
                                &global->fontLibrefNum, &global->fontLibLoad );
    if ( err != errNone )
    {
        FrmAlert ( NoFontAlert );
        goto exit;
    }
    ZDicFontInit ( global->fontLibrefNum, &global->font, global->prefs.useSystemFont );

	if(global->prefs.font == stdFont)
	{
		global->prefs.font = global->font.smallFontID;
	}
	if(global->prefs.fontDA == stdFont)
	{
		global->prefs.fontDA = global->font.smallFontID;
	}
	
    // Initial dia.
    ZDicDIALibInitial ( global );

    // Get initial word;
    global->prvActiveForm = FrmGetActiveForm();
    ToolsGetStartWord(global);

    return err;
    
exit:
    ZDicCloseCurrentDict();

    // Release the phonic font resource when app quits.
    if ( global->fontLibrefNum )
    {
        ZDicFontRelease ( global->fontLibrefNum, &global->font );
        ZDicToolsLibRelease( global->fontLibrefNum, global->fontLibLoad );
    }

    MemPtrFree( global );
    FtrSet( appFileCreator, AppGlobalFtr, ( UInt32 ) NULL );
    return err;
}

/*
 * FUNCTION: AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 */

static void AppStop(void)
{
    AppGlobalType * global;

    global = AppGetGlobal();
    if ( global != NULL )
    {
    	global->optflag = 0;	//reset optflag
        ZDicCloseCurrentDict();

        // Release the phonic font resource when app quits.
        ZDicFontRelease ( global->fontLibrefNum, &global->font );

        ZDicToolsLibRelease( global->fontLibrefNum, global->fontLibLoad );

        /*
         * Write the saved preferences / saved-state information.  This
         * data will be saved during a HotSync backup. 
         */
        PrefSetAppPreferences(
            appFileCreator, appPrefID, appPrefVersionNum,
            &global->prefs, sizeof( global->prefs ), true );

        // Set previous  global to current global.
        FtrSet( appFileCreator, AppGlobalFtr, ( UInt32 ) global->prvGlobalP );
        MemPtrFree( global );
    }
    
    // Release all memory that used by ZDicSpeech.
    ZDicVoiceRelease();
            
    return;
}

#pragma mark -

/*
 * FUNCTION: RomVersionCompatible
 *
 * DESCRIPTION: 
 *
 * This routine checks that a ROM version is meet your minimum 
 * requirement.
 *
 * PARAMETERS:
 *
 * requiredVersion
 *     minimum rom version required
 *     (see sysFtrNumROMVersion in SystemMgr.h for format)
 *
 * launchFlags
 *     flags that indicate if the application UI is initialized
 *     These flags are one of the parameters to your app's PilotMain
 *
 * RETURNED:
 *     error code or zero if ROM version is compatible
 */

static Err RomVersionCompatible( UInt32 requiredVersion, UInt16 launchFlags )
{
    UInt32 romVersion;

    /* See if we're on in minimum required version of the ROM or later. */
    FtrGet( sysFtrCreator, sysFtrNumROMVersion, &romVersion );
    if ( romVersion < requiredVersion )
    {
        if ( ( launchFlags &
                ( sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp ) ) ==
                ( sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp ) )
        {
            FrmAlert ( RomIncompatibleAlert );

            /* Palm OS versions before 2.0 will continuously relaunch this
             * app unless we switch to another safe one. */
            if ( romVersion < kPalmOS20Version )
            {
                AppLaunchWithCommand(
                    sysFileCDefaultApp,
                    sysAppLaunchCmdNormalLaunch, NULL );
            }
        }

        return sysErrRomIncompatible;
    }

    return errNone;
}

/*
 * FUNCTION: PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 * 
 * PARAMETERS:
 *
 * cmd
 *     word value specifying the launch code. 
 *
 * cmdPB
 *     pointer to a structure that is associated with the launch code
 *
 * launchFlags
 *     word value providing extra information about the launch.
 *
 * RETURNED:
 *     Result of launch, errNone if all went OK
 */

UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags )
{
    Err error;
	UInt8 dictMenu = -1;
	
    error = RomVersionCompatible ( ourMinVersion, launchFlags );
    if ( error )
        return ( error );
	
	if(cmd == 50001)
	{
		dictMenu = 0;
		cmd = sysAppLaunchCmdNormalLaunch;
	}
	else if(cmd == 50002)
	{
		dictMenu = 1;
		cmd = sysAppLaunchCmdNormalLaunch;
	}
	else if(cmd == 50003)
	{
		dictMenu = 2;
		cmd = sysAppLaunchCmdNormalLaunch;
	}
	else if(cmd == 50011)
	{
		dictMenu = 0;
		cmd = sysZDicCmdDALaunch;
	}
	else if(cmd == 50012)
	{
		dictMenu = 1;
		cmd = sysZDicCmdDALaunch;
	}
	else if(cmd == 50013)
	{
		dictMenu = 2;
		cmd = sysZDicCmdDALaunch;
	}
	
    switch ( cmd )
    {
	    case sysAppLaunchCmdNormalLaunch:
		{
	        error = AppStart( false ,dictMenu);
	        if ( error )
	            return error;
	        /*
	         * start application by opening the main form
	         * and then entering the main event loop 
	         */
	        AppEventLoop();

	        AppStop();
	        break;
		}
	    case sysZDicCmdTibrProLaunch:
	    case sysZDicCmdDALaunch:
       	{
            FormPtr form, originalForm;
            AppGlobalType *	global;
            Boolean bGotoMainForm;

            error = AppStart( true ,dictMenu);
            if ( error )
                return error;

            global = AppGetGlobal();

            // Remember the original form
            originalForm = FrmGetActiveForm();
            if(global->prefs.daSize == 0)
            {
            	form = FrmInitForm( DAForm );
            }
            else if(global->prefs.daSize == 1)
            {
            	form = FrmInitForm( DAFormLarge );
            }
            else if(global->prefs.daSize == 2)
            {
            	form = FrmInitForm( MainForm );
            }
            
            FrmSetActiveForm( form );
			
			if(global->prefs.daSize == 0)
            {
            	AppDAEventLoop( true, &bGotoMainForm );
            }
            else if(global->prefs.daSize == 1)
            {
            	AppDAEventLoop( true, &bGotoMainForm );
            }
            else if(global->prefs.daSize == 2)
            {
            	AppDAEventLoop( false, &bGotoMainForm );
            }

            FrmEraseForm( form );
            FrmDeleteForm( form );
            FrmSetActiveForm ( originalForm );
			
            if ( bGotoMainForm && global->prvGlobalP == NULL )
            {
                // Remember the original form
                originalForm = FrmGetActiveForm();
                form = FrmInitForm( MainForm );
                FrmSetActiveForm( form );

                AppDAEventLoop( false, &bGotoMainForm );

                FrmEraseForm( form );
                FrmDeleteForm( form );
                FrmSetActiveForm ( originalForm );
            }

            AppStop();
            break;
        }

	    case sysAppLaunchCmdSystemReset:
	        //AppReigsterWinDisplayChangedNotification();
	        break;
	        
	    case sysAppLaunchCmdNotify:
	        //ZDicDIACmdNotify ( cmdPBP );
	        break;
    
    }

    return errNone;
}

#pragma mark -

///////////////////////////////////////////////////////////////////////////

/*SetPopupEventHandler*/
static void SetPopupEventHandler()
{
	EventType		event;
	FormType		*frmP;
	ZDicDBDictInfoType	*dictInfo;
	ZDicDBDictPopupInfoType	*popupInfo;
	Boolean			exit = false;
	ListType		*lstP,*lstP2;
	AppGlobalType 	*global;
	UInt16 i;
	
    global = AppGetGlobal();
	dictInfo = &global->prefs.dictInfo;
	popupInfo = &global->prefs.popupInfo;
	
	FrmPopupForm(DictShortcutForm);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		switch (event.eType)
		{
			case frmLoadEvent:
			{
				frmP = FrmInitForm(DictPopupForm);
		
				break;
			}
			case frmOpenEvent:
			{
				FrmSetActiveForm(frmP);
				
				ZDicDBInitDictList( appDBType, appKDicCreator );
				ZDicDBInitPopupList();

			    lstP = ( ListType* ) GetObjectPtr( DictList );
			    lstP2 = ( ListType* ) GetObjectPtr( lstPopup );
			    
			    
			    // initial the list.
			    LstSetListChoices ( lstP, global->listItem, dictInfo->totalNumber );
			    LstSetSelection ( lstP, dictInfo->curMainDictIndex );
			    
			    LstSetListChoices ( lstP2, global->popuplistItem, popupInfo->totalNumber );
			    if( popupInfo->totalNumber != 0 )
			    {
				    for(i =0;i<popupInfo->totalNumber;i++)
				    {
				    	if(popupInfo->dictIndex[i] == dictInfo->curMainDictIndex)
				    	{
				    		popupInfo->curIndex = i;
				    		break;
				    	}
				    	else
				    	{
				    		popupInfo->curIndex = 0;
				    	}
				    }
				    LstSetSelection ( lstP2, popupInfo->curIndex );
				}
				
			    CtlSetValue ( GetObjectPtr ( cbPopupShow ), dictInfo->showInPopup[ dictInfo->curMainDictIndex ] );
				
				FrmDrawForm(frmP);
				
				break;
			}
			case frmUpdateEvent:
			{
				FrmDrawForm(frmP);
				break;
			}
			case ctlSelectEvent:
			{
				switch (event.data.ctlSelect.controlID)
				{
					case btnPopupOK:
						exit = true;
						break;
					case cbPopupShow:
						dictInfo->showInPopup[ LstGetSelection( lstP ) ] = !dictInfo->showInPopup[ LstGetSelection( lstP ) ];
						ZDicDBInitPopupList();
						LstSetListChoices ( lstP2, global->popuplistItem, popupInfo->totalNumber );
					    if( popupInfo->totalNumber != 0 )
					    {
						    for(i =0;i<popupInfo->totalNumber;i++)
						    {
						    	if(popupInfo->dictIndex[i] == dictInfo->curMainDictIndex)
						    	{
						    		popupInfo->curIndex = i;
						    		break;
						    	}
						    	else
						    	{
						    		popupInfo->curIndex = 0;
						    	}
						    }
						    LstSetSelection ( lstP2, popupInfo->curIndex );
						}
						break;
					case btnSetLaunchPopupKey:
						CustomKey(9);
						break;
					case PopupUpButton:
						PopupMoveDictionary(winUp);
						ZDicDBInitPopupList();
						LstSetListChoices ( lstP2, global->popuplistItem, popupInfo->totalNumber );
					    LstSetSelection ( lstP2, popupInfo->curIndex );
						break;
					case PopupDownButton:
						PopupMoveDictionary(winDown);
						ZDicDBInitPopupList();
						LstSetListChoices ( lstP2, global->popuplistItem, popupInfo->totalNumber );
					    LstSetSelection ( lstP2, popupInfo->curIndex );
						break;
				}
				FrmUpdateForm(DictPopupForm, 0);
				break;
			}
			case lstSelectEvent:
			{
		        if (event.data.lstSelect.selection >= 0 && event.data.lstSelect.listID == DictList )
		        {
		        	CtlSetValue ( GetObjectPtr ( cbPopupShow ), dictInfo->showInPopup[ event.data.lstSelect.selection ] );
		        	dictInfo->curMainDictIndex = LstGetSelection( lstP );
		        	if( popupInfo->totalNumber != 0 )
				    {
					    for(i =0;i<popupInfo->totalNumber;i++)
					    {
					    	if(popupInfo->dictIndex[i] == dictInfo->curMainDictIndex)
					    	{
					    		popupInfo->curIndex = i;
					    		break;
					    	}
					    	else
					    	{
					    		popupInfo->curIndex = 0;
					    	}
					    }
					    LstSetSelection ( lstP2, popupInfo->curIndex );
					}
		        }
		        if (event.data.lstSelect.selection >= 0 && event.data.lstSelect.listID == lstPopup )
		        {
		        	LstSetSelection ( lstP, popupInfo->dictIndex[event.data.lstSelect.selection] );
		        	dictInfo->curMainDictIndex = popupInfo->dictIndex[event.data.lstSelect.selection];
		        	CtlSetValue ( GetObjectPtr ( cbPopupShow ), dictInfo->showInPopup[ dictInfo->curMainDictIndex ] );
		        	popupInfo->curIndex = event.data.lstSelect.selection;
		        }
		        break;
		    }
			default:
			{
				if (! SysHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
				break;
			}
		}

	}while (! (event.eType == appStopEvent || exit));
	
	FrmReturnToForm(0);
}

///////////////////////////////////////////////////////////////////////////

/*Popup Shortcut Form EventHandler*/
static void ShortcutPopEventHandler()
{
	EventType			event;
	FormType			*frmP;
	ZDicDBDictInfoType	*dictInfo;
	ZDicDBDictShortcutInfoType	*shortcutInfo;
	Boolean				exit = false;
	ListType			*lstP;
	//TableType			*tableP;
	AppGlobalType 		*global;
	UInt8				i,j;
	
    global = AppGetGlobal();
	dictInfo = &global->prefs.dictInfo;
	shortcutInfo = &global->prefs.shortcutInfo;
	
	FrmPopupForm(DictShortcutForm);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if(event.eType == keyDownEvent)
	    {
	    	for(i = 0;i<shortcutInfo->totalNumber; i++)
			{
	    		if( event.data.keyDown.chr == (WChar)dictInfo->keyShortcutChr[shortcutInfo->dictIndex[i]] && 
	    			event.data.keyDown.keyCode == (WChar)dictInfo->keyShortcutKeycode[shortcutInfo->dictIndex[i]] )
	    		{
	    			ToolsChangeListDictionaryCommand(MainForm,shortcutInfo->dictIndex[i]);
					exit = true;
					goto pupopformexit;
	    		}
	    		else if(event.data.keyDown.chr == chrBackspace )
	    		{
	    			exit = true;
					goto pupopformexit;
	    		}
		    }
	    }
		
		switch (event.eType)
		{
			case frmLoadEvent:
			{
				frmP = FrmInitForm(DictShortcutPopForm);
		
				break;
			}
			case frmOpenEvent:
			{
				FrmSetActiveForm(frmP);
				
				ZDicDBInitDictList( appDBType, appKDicCreator );
				ZDicDBInitShortcutList();
				
				// Initial index record of dictionary database that in dictionary list.
			    //ZDicDBInitIndexRecord();

			    lstP = ( ListType* ) GetObjectPtr( PopShortcutList );
			    
			    //tableP =  GetObjectPtr( PopShortcutTable );
			    
		        if( shortcutInfo->totalNumber != 0 )
			    {
				    for(i =0;i<shortcutInfo->totalNumber;i++)
				    {
				    	if(shortcutInfo->dictIndex[i] == dictInfo->curMainDictIndex)
				    	{
				    		j = i;
				    		break;
				    	}
				    	else
				    	{
				    		j = 0;
				    	}
				    }
				}
			    
			    if ( lstP )
			    {
			        // initial the list.
			        for(i =0;i<shortcutInfo->totalNumber;i++)
			        {
			        	StrCopy(global->shortcutlistShowTemp[i], global->shortcutlistItem[i]);
			        	if(StrLen(dictInfo->noteShortcut[shortcutInfo->dictIndex[i]]) > 0)
			        	{
				        	StrCat(global->shortcutlistShowTemp[i], " (" );
				        	StrCat(global->shortcutlistShowTemp[i], dictInfo->noteShortcut[shortcutInfo->dictIndex[i]] );
				        	StrCat(global->shortcutlistShowTemp[i], ")" );
				        }
			        }
			        
			        for(i =0;i<shortcutInfo->totalNumber;i++)
			        {
			        	global->shortcutlistShow[i] = global->shortcutlistShowTemp[i];
			        }
			        
			        LstSetListChoices ( lstP, global->shortcutlistShow, shortcutInfo->totalNumber );
			        LstSetSelection ( lstP, j );
			        
			    }
			    
			    
			    
				FrmDrawForm(frmP);
			    FrmSetFocus (frmP,FrmGetObjectIndex( frmP, PopShortcutList ));
				break;
			}
			case frmUpdateEvent:
			{
				FrmDrawForm(frmP);
				break;
			}
			case lstSelectEvent:
			{
		        if ( event.data.lstSelect.listID == PopShortcutList )
		        {
		            ToolsChangeListDictionaryCommand(MainForm,shortcutInfo->dictIndex[LstGetSelection(lstP)] );
		            exit = true;
		        }
		        break;
		    }
		    
			default:
			{
				if (! SysHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
				break;
			}
		}
		
pupopformexit:if (event.eType == penDownEvent)
        {
        	RectangleType bounds;
            FrmGetFormBounds( FrmGetActiveForm(), &bounds );
            RctInsetRectangle( &bounds, 2 );
            
            if ( IsOutside( &bounds, event.screenX, event.screenY ) ) exit = true;
        }

	}while (! (event.eType == appStopEvent || exit));

	FrmReturnToForm(0);
}

///////////////////////////////////////////////////////////////////////////

/*SetShortcutEventHandler*/
static void SetShortcutEventHandler()
{
	EventType		event;
	FormType		*frmP;
	ZDicDBDictInfoType	*dictInfo;
	ZDicDBDictShortcutInfoType	*shortcutInfo;
	Boolean			exit = false;
	ListType		*lstP,*lstP2;
	AppGlobalType 	*global;
	UInt16 i;
	
    global = AppGetGlobal();
	dictInfo = &global->prefs.dictInfo;
	shortcutInfo = &global->prefs.shortcutInfo;
	
	FrmPopupForm(DictShortcutForm);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		switch (event.eType)
		{
			case frmLoadEvent:
			{
				frmP = FrmInitForm(DictShortcutForm);
		
				break;
			}
			case frmOpenEvent:
			{
				FrmSetActiveForm(frmP);
				
				ZDicDBInitDictList( appDBType, appKDicCreator );
				ZDicDBInitShortcutList();

			    lstP = ( ListType* ) GetObjectPtr( DictList );
			    lstP2 = ( ListType* ) GetObjectPtr( ShortcutList );
			    
			    
			    // initial the list.
			    LstSetListChoices ( lstP, global->listItem, dictInfo->totalNumber );
			    LstSetSelection ( lstP, dictInfo->curMainDictIndex );
			    
			    LstSetListChoices ( lstP2, global->shortcutlistItem, shortcutInfo->totalNumber );
			    if( shortcutInfo->totalNumber != 0 )
			    {
				    for(i =0;i<shortcutInfo->totalNumber;i++)
				    {
				    	if(shortcutInfo->dictIndex[i] == dictInfo->curMainDictIndex)
				    	{
				    		shortcutInfo->curIndex = i;
				    		break;
				    	}
				    	else
				    	{
				    		shortcutInfo->curIndex = 0;
				    	}
				    }
				    LstSetSelection ( lstP2, shortcutInfo->curIndex );
				}
				
				if(dictInfo->noteShortcut[dictInfo->curMainDictIndex][0] != NULL)
			    {
			    	FldDelete(GetObjectPtr(fldShortcutNote), 0, FldGetTextLength(GetObjectPtr(fldShortcutNote)) );
					FldInsert(GetObjectPtr(fldShortcutNote), dictInfo->noteShortcut[dictInfo->curMainDictIndex],StrLen(dictInfo->noteShortcut[dictInfo->curMainDictIndex]) );
				}
				else
				{
					FldDelete(GetObjectPtr(fldShortcutNote), 0, FldGetTextLength(GetObjectPtr(fldShortcutNote)) );
				}
			    
			    CtlSetValue ( GetObjectPtr ( cbShortcutShow ), dictInfo->showInShortcut[ dictInfo->curMainDictIndex ] );
				
				FrmDrawForm(frmP);
				
				break;
			}
			case frmUpdateEvent:
			{
				FrmDrawForm(frmP);
				break;
			}
			case ctlSelectEvent:
			{
				switch (event.data.ctlSelect.controlID)
				{
					case btnShortcutOK:
						exit = true;
						break;
					case cbShortcutShow:
						dictInfo->showInShortcut[ LstGetSelection( lstP ) ] = !dictInfo->showInShortcut[ LstGetSelection( lstP ) ];
						//ZDicDBInitDictList( appDBType, appKDicCreator );
						ZDicDBInitShortcutList();
						LstSetListChoices ( lstP2, global->shortcutlistItem, shortcutInfo->totalNumber );
					    if( shortcutInfo->totalNumber != 0 )
					    {
						    for(i =0;i<shortcutInfo->totalNumber;i++)
						    {
						    	if(shortcutInfo->dictIndex[i] == dictInfo->curMainDictIndex)
						    	{
						    		shortcutInfo->curIndex = i;
						    		break;
						    	}
						    	else
						    	{
						    		shortcutInfo->curIndex = 0;
						    	}
						    }
						    LstSetSelection ( lstP2, shortcutInfo->curIndex );
						}
						break;
					case btnSetShortcutKey:
						CustomShortcutKey(shortcutInfo->dictIndex[LstGetSelection( lstP2 )]);
						break;
					case btnSetLaunchShortcutKey:
						CustomKey(8);
						break;
					case ShortcutUpButton:
						ShortcutMoveDictionary(winUp);
						ZDicDBInitShortcutList();
						LstSetListChoices ( lstP2, global->shortcutlistItem, shortcutInfo->totalNumber );
					    LstSetSelection ( lstP2, shortcutInfo->curIndex );
						break;
					case ShortcutDownButton:
						ShortcutMoveDictionary(winDown);
						ZDicDBInitShortcutList();
						LstSetListChoices ( lstP2, global->shortcutlistItem, shortcutInfo->totalNumber );
					    LstSetSelection ( lstP2, shortcutInfo->curIndex );
						break;
					case btnSetShortcutNote:
						if(FldGetTextLength(GetObjectPtr(fldShortcutNote)) == 0)
						{
							for(i=0;i<MAX_SHORTCUTNOTE_LEN;i++)
							{
								dictInfo->noteShortcut[ dictInfo->curMainDictIndex ][i] = chrNull;
							}
						}
						else
						{
							StrCopy( &dictInfo->noteShortcut[ dictInfo->curMainDictIndex ][ 0 ] , FldGetTextPtr(GetObjectPtr(fldShortcutNote)));
						}
						break;
				}
				FrmUpdateForm(DictShortcutForm, 0);
				break;
			}
			case lstSelectEvent:
			{
		        if (event.data.lstSelect.selection >= 0 && event.data.lstSelect.listID == DictList )
		        {
		        	CtlSetValue ( GetObjectPtr ( cbShortcutShow ), dictInfo->showInShortcut[ event.data.lstSelect.selection ] );
		        	
		        	dictInfo->curMainDictIndex = LstGetSelection( lstP );
		        	
		        	if(dictInfo->noteShortcut[dictInfo->curMainDictIndex][0] != NULL)
				    {
				    	FldDelete(GetObjectPtr(fldShortcutNote), 0, FldGetTextLength(GetObjectPtr(fldShortcutNote)) );
						FldInsert(GetObjectPtr(fldShortcutNote), dictInfo->noteShortcut[event.data.lstSelect.selection],StrLen(dictInfo->noteShortcut[event.data.lstSelect.selection]) );
					}
					else
					{
						FldDelete(GetObjectPtr(fldShortcutNote), 0, FldGetTextLength(GetObjectPtr(fldShortcutNote)) );
					}
			    	
			    	if( shortcutInfo->totalNumber != 0 )
				    {
					    for(i =0;i<shortcutInfo->totalNumber;i++)
					    {
					    	if(shortcutInfo->dictIndex[i] == dictInfo->curMainDictIndex)
					    	{
					    		shortcutInfo->curIndex = i;
					    		break;
					    	}
					    	else
					    	{
					    		shortcutInfo->curIndex = 0;
					    	}
					    }
					    LstSetSelection ( lstP2, shortcutInfo->curIndex );
					}
		        }
		        if (event.data.lstSelect.selection >= 0 && event.data.lstSelect.listID == ShortcutList )
		        {
		        	LstSetSelection ( lstP, shortcutInfo->dictIndex[event.data.lstSelect.selection] );
		        	dictInfo->curMainDictIndex = shortcutInfo->dictIndex[event.data.lstSelect.selection];
		        	CtlSetValue ( GetObjectPtr ( cbShortcutShow ), dictInfo->showInShortcut[ dictInfo->curMainDictIndex ] );
		        	
		        	if(dictInfo->noteShortcut[dictInfo->curMainDictIndex][0] != NULL)
				    {
				    	FldDelete(GetObjectPtr(fldShortcutNote), 0, FldGetTextLength(GetObjectPtr(fldShortcutNote)) );
						FldInsert(GetObjectPtr(fldShortcutNote), dictInfo->noteShortcut[dictInfo->curMainDictIndex],StrLen(dictInfo->noteShortcut[dictInfo->curMainDictIndex]) );
					}
					else
					{
						FldDelete(GetObjectPtr(fldShortcutNote), 0, FldGetTextLength(GetObjectPtr(fldShortcutNote)) );
					}
			    	
		        	shortcutInfo->curIndex = event.data.lstSelect.selection;
		        }
		        break;
		    }
			default:
			{
				if (! SysHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
				break;
			}
		}

	}while (! (event.eType == appStopEvent || exit));
	
	FrmReturnToForm(0);
}

/*
 * FUNCTION: PrefFormInit
 *
 * DESCRIPTION: This routine initializes the PrefsForm form.
 *
 * PARAMETERS:
 *
 * frm
 *     pointer to the MainForm form.
 */

static void PrefFormInit( FormType *frmP )
{
    AppGlobalType	* global;
    ZDicDBDictInfoType	*dictInfo;
    ListType	*lstP,*lstP2,*lstP3;
    ControlType	*triP2,*triP3;
    FieldType *fldP;
    Int16	itemNum;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    
    triP3 = GetObjectPtr(triggerPhonetic);
    lstP3 = GetObjectPtr( PrefsPhoneticList );
    
    triP2 = GetObjectPtr(triggerDictMenu);
    lstP2 = GetObjectPtr(lstDictMenu);
    
    lstP = GetObjectPtr( PrefsDictList );
    
    fldP = GetObjectPtr(fldMenuShortcut);
   	
    if(dictInfo->menuShortcut[dictInfo->curMainDictIndex] != NULL)
    {
    	FldDelete(fldP, 0, FldGetTextLength(fldP) );
    	FldInsert(fldP, &dictInfo->menuShortcut[dictInfo->curMainDictIndex], 1);
	}
	else
	{
		FldDelete (fldP, 0, FldGetTextLength(fldP) );
	}
	
    // Initial dictionary list.
    itemNum = ZDicDBInitDictList( appDBType, appKDicCreator );
    if ( itemNum == 0 )
    {
        FrmAlert ( DictNotFoundAlert );
        ToolsQuitApp();
        return ;
    }

    // Initial index record of dictionary database that in dictionary list.
    ZDicDBInitIndexRecord();

    if ( lstP )
    {
        // initial the list.
        LstSetListChoices ( lstP, global->listItem, dictInfo->totalNumber );
        LstSetSelection ( lstP, dictInfo->curMainDictIndex );
    }
    
    // Set the phonetic list
	CtlSetLabel(triP3,LstGetSelectionText(lstP3,dictInfo->phonetic[ dictInfo->curMainDictIndex ]));
	
	//Set font
	CtlGlueSetFont( GetObjectPtr ( PrefsFontSmallTinyPushButton ), global->font.smallTinyFontID);
	CtlGlueSetFont( GetObjectPtr ( PrefsFontLargeTinyPushButton ), global->font.largeTinyFontID);
	CtlGlueSetFont( GetObjectPtr ( PrefsFontSmallPushButton ), global->font.smallFontID);
	CtlGlueSetFont( GetObjectPtr ( PrefsFontLargePushButton ), global->font.largeFontID);
	
	CtlGlueSetFont( GetObjectPtr ( PrefsFontSmallTinyDAPushButton ), global->font.smallTinyFontID);
	CtlGlueSetFont( GetObjectPtr ( PrefsFontLargeTinyDAPushButton ), global->font.largeTinyFontID);
	CtlGlueSetFont( GetObjectPtr ( PrefsFontSmallDAPushButton ), global->font.smallFontID);
	CtlGlueSetFont( GetObjectPtr ( PrefsFontLargeDAPushButton ), global->font.largeFontID);
	
    // Set font group selection.
    if(global->prefs.font == global->font.smallTinyFontID)
    	FrmSetControlGroupSelection ( frmP, 1,PrefsFontSmallTinyPushButton );
    if(global->prefs.font == global->font.largeTinyFontID)
    	FrmSetControlGroupSelection ( frmP, 1,PrefsFontLargeTinyPushButton );
    if(global->prefs.font == global->font.smallFontID)
    	FrmSetControlGroupSelection ( frmP, 1,PrefsFontSmallPushButton );
    if(global->prefs.font == global->font.largeFontID)
    	FrmSetControlGroupSelection ( frmP, 1,PrefsFontLargePushButton );
    	
    if(global->prefs.fontDA == global->font.smallTinyFontID)
    	FrmSetControlGroupSelection ( frmP, 2,PrefsFontSmallTinyDAPushButton );
    if(global->prefs.fontDA == global->font.largeTinyFontID)
    	FrmSetControlGroupSelection ( frmP, 2,PrefsFontLargeTinyDAPushButton );
    if(global->prefs.fontDA == global->font.smallFontID)
    	FrmSetControlGroupSelection ( frmP, 2,PrefsFontSmallDAPushButton );
    if(global->prefs.fontDA == global->font.largeFontID)
    	FrmSetControlGroupSelection ( frmP, 2,PrefsFontLargeDAPushButton );
    
    LstSetSelection(lstP2,global->prefs.dictMenu);
	CtlSetLabel(triP2,LstGetSelectionText(lstP2,global->prefs.dictMenu));

    return ;
}

static void ShortcutMoveDictionary( WinDirectionType direction )
{
    AppGlobalType	* global;
    ZDicDBDictShortcutInfoType	*shortcutInfo, *dictBuf;
    ZDicDBDictInfoType	*dictInfo;
    ListType	*lstP;
    Int16	idx,offset;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    shortcutInfo = &global->prefs.shortcutInfo;
    dictBuf = &global->data.shortcutInfoList;

    if ( shortcutInfo->totalNumber <= 1 )
        return ;

    offset = 0;
    switch ( direction )
    {
	    case winLeft:
	    case winUp:
	        if ( shortcutInfo->curIndex > 0 )
	            offset = -1;
	        break;
	    case winRight:
	    case winDown:
	        if ( shortcutInfo->curIndex < shortcutInfo->totalNumber - 1 )
	            offset = 1;
	        break;
	    default:
	        ErrNonFatalDisplay( "Bad parameter" );
    }
    
    if ( offset == 0 )
        return ;

    idx = shortcutInfo->curIndex;

    // Switch dictionary information.
    dictBuf->dictIndex[ 0 ] = shortcutInfo->dictIndex[ idx ];
	shortcutInfo->dictIndex[ idx ] = shortcutInfo->dictIndex[ idx + offset ];
	shortcutInfo->dictIndex[ idx + offset ] = dictBuf->dictIndex[ 0 ];

    shortcutInfo->curIndex += offset;

    // Update dictionary list.
    lstP = ( ListType* ) GetObjectPtr( ShortcutList );
    if ( lstP )
    {
    	ZDicDBInitShortcutList();
		LstSetListChoices ( lstP, global->shortcutlistItem, shortcutInfo->totalNumber );
        LstSetSelection ( lstP, shortcutInfo->curIndex );
        LstDrawList ( lstP );
    }

    return ;
}

static void PopupMoveDictionary( WinDirectionType direction )
{
    AppGlobalType	* global;
    ZDicDBDictPopupInfoType	*popupInfo, *dictBuf;
    ZDicDBDictInfoType	*dictInfo;
    ListType	*lstP;
    Int16	idx,offset;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    popupInfo = &global->prefs.popupInfo;
    dictBuf = &global->data.popupInfoList;

    if ( popupInfo->totalNumber <= 1 )
        return ;

    offset = 0;
    switch ( direction )
    {
	    case winLeft:
	    case winUp:
	        if ( popupInfo->curIndex > 0 )
	            offset = -1;
	        break;
	    case winRight:
	    case winDown:
	        if ( popupInfo->curIndex < popupInfo->totalNumber - 1 )
	            offset = 1;
	        break;
	    default:
	        ErrNonFatalDisplay( "Bad parameter" );
    }
    
    if ( offset == 0 )
        return ;

    idx = popupInfo->curIndex;

    // Switch dictionary information.
    dictBuf->dictIndex[ 0 ] = popupInfo->dictIndex[ idx ];
	popupInfo->dictIndex[ idx ] = popupInfo->dictIndex[ idx + offset ];
	popupInfo->dictIndex[ idx + offset ] = dictBuf->dictIndex[ 0 ];

    popupInfo->curIndex += offset;

    // Update dictionary list.
    lstP = ( ListType* ) GetObjectPtr( lstPopup );
    if ( lstP )
    {
    	ZDicDBInitShortcutList();
		LstSetListChoices ( lstP, global->popuplistItem, popupInfo->totalNumber );
        LstSetSelection ( lstP, popupInfo->curIndex );
        LstDrawList ( lstP );
    }

    return ;
}

/***********************************************************************
 *
 * FUNCTION:	PrefMoveDictionary
 *
 * DESCRIPTION:	Handler of move dictionary if user tap up or down button.
 *
 * PARAMETERS:	->	direction Direction of page scroll.
 *
 * RETURNED:	nothing.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	14/Aug/04	Initial Revision 
 *
 ***********************************************************************/

static void PrefMoveDictionary( WinDirectionType direction )
{
    AppGlobalType	* global;
    ZDicDBDictInfoType	*dictInfo, *dictBuf;
    ZDicDBDictShortcutInfoType *shortcutInfo;
    ListType	*lstP;
    Int16	idx,offset,i,j;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    dictBuf = &global->data.dictInfoList;
    shortcutInfo = &global->prefs.shortcutInfo;

    if ( dictInfo->totalNumber <= 1 )
        return ;

    offset = 0;
    switch ( direction )
    {
    case winLeft:
    case winUp:
        if ( dictInfo->curMainDictIndex > 0 )
            offset = -1;
        break;
    case winRight:
    case winDown:
        if ( dictInfo->curMainDictIndex < dictInfo->totalNumber - 1 )
            offset = 1;
        break;
    default:
        ErrNonFatalDisplay( "Bad parameter" );
    }
    if ( offset == 0 )
        return ;

    idx = dictInfo->curMainDictIndex;

    // Switch dictionary information.
    StrCopy( &dictBuf->dictName[ 0 ][ 0 ], &dictInfo->dictName[ idx ][ 0 ] );
    StrCopy( &dictBuf->displayName[ 0 ][ 0 ], &dictInfo->displayName[ idx ][ 0 ] );
    dictBuf->volRefNum[ 0 ] = dictInfo->volRefNum[ idx ];
    dictBuf->phonetic[ 0 ] = dictInfo->phonetic[ idx ];
    dictBuf->showInShortcut[ 0 ] = dictInfo->showInShortcut[ idx ];
    dictBuf->keyShortcutChr[ 0 ] = dictInfo->keyShortcutChr[ idx ];
    dictBuf->keyShortcutKeycode[ 0 ] = dictInfo->keyShortcutKeycode[ idx ];

    StrCopy( &dictInfo->dictName[ idx ][ 0 ], &dictInfo->dictName[ idx + offset ][ 0 ] );
    StrCopy( &dictInfo->displayName[ idx ][ 0 ], &dictInfo->displayName[ idx + offset ][ 0 ] );
    dictInfo->volRefNum[ idx ] = dictInfo->volRefNum[ idx + offset ];
    dictInfo->phonetic[ idx ] = dictInfo->phonetic[ idx + offset ];
    dictInfo->showInShortcut[ idx ] = dictInfo->showInShortcut[ idx + offset ];
    dictInfo->keyShortcutChr[ idx ] = dictInfo->keyShortcutChr[ idx + offset ];
    dictInfo->keyShortcutKeycode[ idx ] = dictInfo->keyShortcutKeycode[ idx + offset ];

    StrCopy( &dictInfo->dictName[ idx + offset ][ 0 ], &dictBuf->dictName[ 0 ][ 0 ] );
    StrCopy( &dictInfo->displayName[ idx + offset ][ 0 ], &dictBuf->displayName[ 0 ][ 0 ] );
    dictInfo->volRefNum[ idx + offset ] = dictBuf->volRefNum[ 0 ];
    dictInfo->phonetic[ idx + offset ] = dictBuf->phonetic[ 0 ];
    dictInfo->showInShortcut[ idx + offset ] = dictBuf->showInShortcut[ 0 ];
    dictInfo->keyShortcutChr[ idx + offset ] = dictBuf->keyShortcutChr[ 0 ];
    dictInfo->keyShortcutKeycode[ idx + offset ] = dictBuf->keyShortcutKeycode[ 0 ];

	for(i =0;i<shortcutInfo->totalNumber;i++)
    {
    	if(shortcutInfo->dictIndex[i] == dictInfo->curMainDictIndex)
    	{
    		shortcutInfo->dictIndex[i] += offset;
    		j = i;
    		break;
    	}
    }
    
    for(i =0;i<shortcutInfo->totalNumber;i++)
    {
    	if(shortcutInfo->dictIndex[i] == (dictInfo->curMainDictIndex + offset) && i!= j)
    	{
    		shortcutInfo->dictIndex[i] -= offset;
    		break;
    	}
	}

    dictInfo->curMainDictIndex += offset;

    // Update dictionary list.
    lstP = ( ListType* ) GetObjectPtr( PrefsDictList );
    if ( lstP )
    {
        LstSetSelection ( lstP, dictInfo->curMainDictIndex );
        LstDrawList ( lstP );
    }

    return ;
}

/*
 * FUNCTION: PrefFormHandleEvent
 *
 * DESCRIPTION:
 *
 * This routine is the event handler for the "PrefsForm" of this 
 * application.
 *
 * PARAMETERS:
 *
 * eventP
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed to
 *     FrmHandleEvent
 */

static Boolean PrefFormHandleEvent( FormType *frmP, EventType *eventP, Boolean *exit )
{
    Boolean	handled = false;
    ListType	*lstP,*lstP2;
    ControlType	*triP,*triP2;
    FieldType *fldP;
    AppGlobalType	*global;
    ZDicDBDictInfoType	*dictInfo;
	Char *chrP;
	
	fldP = GetObjectPtr(fldMenuShortcut);
	
    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    *exit = false;
	
	triP =  GetObjectPtr(triggerDictMenu);
	lstP = GetObjectPtr(lstDictMenu);
	
	triP2 = GetObjectPtr(triggerPhonetic);
	lstP2 = GetObjectPtr( PrefsPhoneticList );
	
    switch ( eventP->eType )
	{
	    case frmUpdateEvent:
	        /*
	         * To do any custom drawing here, first call
	         * FrmDrawForm(), then do your drawing, and
	         * then set handled to true. 
	         */
	        FrmDrawForm( frmP );
	        break;

	    case ctlSelectEvent:
        {
        	switch(eventP->data.ctlSelect.controlID)
        	{
        		case PrefsDoneButton:
        			*exit = true;
	                handled = true;
	                break;
	            case PrefsUpButton:
	                PrefMoveDictionary ( winUp );
	                handled = true;
	                break;
	            case PrefsDownButton:
	                PrefMoveDictionary ( winDown );
	                handled = true;
	                break;
	            case btnShortcut:
	            	SetShortcutEventHandler();
	            	handled = true;
	            	break;
	            case btnPopup:
	            	SetPopupEventHandler();
	            	handled = true;
	            	break;
	            case triggerDictMenu:
					LstPopupList(lstP);
					CtlSetLabel(triP,LstGetSelectionText(lstP,LstGetSelection(lstP)));
					global->prefs.dictMenu = LstGetSelection (lstP);
					if(global->prefs.dictMenu == 1)
				    {
				    	global->prefs.dictInfo.curMainDictIndex = global->prefs.shortcutInfo.dictIndex[global->prefs.shortcutInfo.curIndex];
				    }
				    else if(global->prefs.dictMenu == 2)
				    {
				    	global->prefs.dictInfo.curMainDictIndex = global->prefs.popupInfo.dictIndex[global->prefs.popupInfo.curIndex];
				    }
					handled = true;
					break;
				case triggerPhonetic:
					LstPopupList(lstP2);
					CtlSetLabel(triP2,LstGetSelectionText(lstP2,LstGetSelection(lstP2)));
					dictInfo->phonetic[ dictInfo->curMainDictIndex ] = LstGetSelection(lstP2);
					handled = true;
					break;
				
				case btnSetMenuShortcut:
					chrP = FldGetTextPtr(fldP);
					dictInfo->menuShortcut[dictInfo->curMainDictIndex] = *chrP;
					handled = true;
					break;
            }
            break;
        }

	    case lstSelectEvent:
	    {
	        if ( eventP->data.lstSelect.listID == PrefsDictList )
	        {
	            // set current dictionary.
	            dictInfo->curMainDictIndex = eventP->data.lstSelect.selection;

	            // Set the phonetic list
	            CtlSetLabel(triP2,LstGetSelectionText(lstP2,dictInfo->phonetic[ dictInfo->curMainDictIndex ]));
	            
	            if(dictInfo->menuShortcut[dictInfo->curMainDictIndex] != NULL)
				{
					FldDelete(fldP, 0, FldGetTextLength(fldP) );
					FldInsert(fldP, &dictInfo->menuShortcut[dictInfo->curMainDictIndex], 1);
				}
				else
				{
					FldDelete (fldP, 0, FldGetTextLength(fldP) );
				}
				
	            handled = true;
	        }
	        /*
	        else if ( eventP->data.lstSelect.listID == PrefsPhoneticList )
	        {
	            dictInfo->phonetic[ LstGetSelection( lstP ) ] = eventP->data.lstSelect.selection;
	            handled = true;
	        }
			*/
	        break;
		}

	}

    return handled;
}

/*
 * FUNCTION: PrefFormPopupPrefsDialog
 *
 * DESCRIPTION: This routine popup "PrefsForm" dialog.
 *
 * PARAMETERS: nothing.
 *
 * RETURNED: nothing.
 */

static Err PrefFormPopupPrefsDialog( void )
{
    EventType	event;
    FormPtr	frmP, originalForm;
    Boolean	exit, handled;
    UInt16	originalFormID;
    AppGlobalType * global;

    global = AppGetGlobal();

    ZDicCloseCurrentDict();

    // Display the preference from the display

    // Remember the original form
    originalForm = FrmGetActiveForm();
    originalFormID = FrmGetFormId ( originalForm );

    frmP = FrmInitForm ( PrefsForm );
    FrmSetActiveForm( frmP );
    PrefFormInit( frmP );
    FrmDrawForm ( frmP );

    do
    {
        EvtGetEvent( &event, evtWaitForever );

        if ( SysHandleEvent ( &event ) )
            continue;

        if ( event.eType == appStopEvent )
            EvtAddEventToQueue( &event );

        handled = PrefFormHandleEvent( frmP, &event, &exit );

        // Check if the form can handle the event
        if ( !handled )
            FrmHandleEvent ( frmP, &event );

    }
    while ( event.eType != appStopEvent && !exit );


    // update user change.
    {
        FontID newFont,newFontDA;
        AppGlobalType	*global;

        global = AppGetGlobal();

        switch(FrmGetObjectId ( frmP,FrmGetControlGroupSelection ( frmP, 1 )))
	    {
	    	case PrefsFontSmallTinyPushButton:
	    		newFont = global->font.smallTinyFontID;
	    		break;
	    	case PrefsFontLargeTinyPushButton:
	    		newFont = global->font.largeTinyFontID;
	    		break;
	    	case PrefsFontSmallPushButton:
	    		newFont = global->font.smallFontID;
	    		break;
	    	case PrefsFontLargePushButton:
	    		newFont = global->font.largeFontID;
	    		break;
	    }
	    switch(FrmGetObjectId ( frmP,FrmGetControlGroupSelection ( frmP, 2 )))
	    {
	    	case PrefsFontSmallTinyDAPushButton:
	    		newFontDA = global->font.smallTinyFontID;
	    		break;
	    	case PrefsFontLargeTinyDAPushButton:
	    		newFontDA = global->font.largeTinyFontID;
	    		break;
	    	case PrefsFontSmallDAPushButton:
	    		newFontDA = global->font.smallFontID;
	    		break;
	    	case PrefsFontLargeDAPushButton:
	    		newFontDA = global->font.largeFontID;
	    		break;
	    }
	    
        if ( newFont != global->prefs.font || newFontDA != global->prefs.fontDA )
        {
            global->prefs.font = newFont;
            global->prefs.fontDA = newFontDA;
            FrmUpdateForm( originalFormID, updateDictionaryChanged | updateFontChanged );
        }
        else
            FrmUpdateForm( originalFormID, updateDictionaryChanged );
    }

    FrmEraseForm ( frmP );
    FrmDeleteForm ( frmP );
    FrmSetActiveForm ( originalForm );

    return errNone;
}

#pragma mark -

///////////////////////////////////////////////////////////////////////////

/*SetCustomKeyEventHandler*/
static void SetCustomKeyEventHandler()
{
	EventType		event;
	FormType		*frmP;
	Boolean			exit = false;
	ListType			*lstP,*lstP2;
	ControlType			*triP,*triP2;
	
	AppGlobalType * global;

    global = AppGetGlobal();

	FrmPopupForm(CustomKeyForm);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		switch (event.eType)
		{
			case frmLoadEvent:
			{
				frmP = FrmInitForm(CustomKeyForm);
		
				break;
			}
			case frmOpenEvent:
			{
				FrmSetActiveForm(frmP);
				
				if(global->prefs.SwitchUDLR)
				{
					CtlSetLabel(GetObjectPtr(lblUD),":Change Wrds");
					CtlSetLabel(GetObjectPtr(lblLR),":Change Dics");
				}
				else
				{
					CtlSetLabel(GetObjectPtr(lblUD),":Change Dics");
					CtlSetLabel(GetObjectPtr(lblLR),":Change Wrds");
				}
				
				FrmDrawForm(frmP);
				
				
				lstP = GetObjectPtr(listSelectKey);
				triP = GetObjectPtr(triggerSelectKey);
				lstP2 = GetObjectPtr(listOptSelectKey);
				triP2 = GetObjectPtr(triggerOptSelectKey);
				LstSetSelection(lstP,global->prefs.SelectKeyFunc);
				LstSetSelection(lstP2,global->prefs.OptSelectKeyFunc);
				CtlSetLabel(triP,LstGetSelectionText(lstP,global->prefs.SelectKeyFunc));
				CtlSetLabel(triP2,LstGetSelectionText(lstP2,global->prefs.OptSelectKeyFunc));
				
				
				CtlSetValue(GetObjectPtr(cbOptUD), global->prefs.OptUD);
				CtlSetValue(GetObjectPtr(cbUD), global->prefs.UD);
				CtlSetValue(GetObjectPtr(cbOptLR), global->prefs.OptLR);
				CtlSetValue(GetObjectPtr(cbLR), global->prefs.LR);
				CtlSetValue(GetObjectPtr(cbSwitchUDLR), global->prefs.SwitchUDLR);
				
				break;
			}
			case frmUpdateEvent:
			{
				FrmDrawForm(frmP);
				break;
			}
			case ctlSelectEvent:
			{
				switch (event.data.ctlSelect.controlID)
				{
					case triggerSelectKey:
					{
						LstPopupList(lstP);
						CtlSetLabel(triP,LstGetSelectionText(lstP,LstGetSelection(lstP)));
						global->prefs.SelectKeyFunc = LstGetSelection (lstP);
						break;
					}
					case triggerOptSelectKey:
					{
						LstPopupList(lstP2);
						CtlSetLabel(triP2,LstGetSelectionText(lstP2,LstGetSelection(lstP2)));
						global->prefs.OptSelectKeyFunc = LstGetSelection (lstP2);
						break;
					}
					case btnPlaySound:
					{
						CustomKey(1);
						break;
					}
					case btnWordList:
					{
						CustomKey(2);
						break;
					}
					case btnHistory:
					{
						CustomKey(3);
						break;
					}
					case btnEnlargeDA:
					{
						CustomKey(4);
						break;
					}
					case btnOneKeyChgDic:
					{
						CustomKey(5);
						break;
					}
					case btnExport:
					{
						CustomKey(6);
						break;
					}
					case btnClearField:
					{
						CustomKey(7);
						break;
					}
					case btnGoBack:
					{
						CustomKey(10);
						break;
					}
					case btnSearchAll:
					{
						CustomKey(11);
						break;
					}
					case cbOptUD:
					{
						global->prefs.OptUD = ! global->prefs.OptUD;
						break;
					}
					case cbUD:
					{
						global->prefs.UD = ! global->prefs.UD;
						break;
					}
					case cbOptLR:
					{
						global->prefs.OptLR = ! global->prefs.OptLR;
						break;
					}
					case cbLR:
					{
						global->prefs.LR = ! global->prefs.LR;
						break;
					}
					case cbSwitchUDLR:
					{
						global->prefs.SwitchUDLR = ! global->prefs.SwitchUDLR;
						if(global->prefs.SwitchUDLR)
						{
							CtlSetLabel(GetObjectPtr(lblUD),":Change Wrds");
							CtlSetLabel(GetObjectPtr(lblLR),":Change Dics");
						}
						else
						{
							CtlSetLabel(GetObjectPtr(lblUD),":Change Dics");
							CtlSetLabel(GetObjectPtr(lblLR),":Change Wrds");
						}
						FrmDrawForm(frmP);
						break;
					}
					case btnCustomKeyOK:
					{
						exit = true;
						break;
					}
				}
				FrmUpdateForm(CustomKeyForm, 0);
				break;
			}
			default:
			{
				if (! SysHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
				break;
			}
		}

	}while (! (event.eType == appStopEvent || exit));
	
	FrmReturnToForm(0);
}

///////////////////////////////////////////////////////////////////////////

/*
 * FUNCTION: DetailsFormInit
 *
 * DESCRIPTION: This routine initializes the DetailsForm form.
 *
 * PARAMETERS:
 *
 * frm
 *     pointer to the MainForm form.
 */

static void DetailsFormInit( FormType *frmP )
{
    AppGlobalType * global;

    global = AppGetGlobal();

    // Set the get clipboard at normal launch.
    CtlSetValue ( GetObjectPtr ( DetailsGetClipBoardAtStart ), global->prefs.getClipBoardAtStart );

    // Set the incremental search.
    CtlSetValue ( GetObjectPtr ( DetailsEnableIncSearch ), global->prefs.enableIncSearch );

    // Set the enable jump search for single tap.
    CtlSetValue ( GetObjectPtr ( DetailsEnableSingleTap ), global->prefs.enableSingleTap );

    // Set the enable highlight word field.
    CtlSetValue ( GetObjectPtr ( DetailsEnableHighlightWord ), global->prefs.enableHighlightWord );

    // Set the enable try lower case on search failed..
    CtlSetValue ( GetObjectPtr ( DetailsEnableTryLowerSearch ), global->prefs.enableTryLowerSearch );

    // Set the disable phonetic font support inside.
    CtlSetValue ( GetObjectPtr ( DetailsDisablePhoneticFont ), global->prefs.useSystemFont );

    // Set the slider values of incremental search delay.
    CtlSetSliderValues ( GetObjectPtr ( DetailSearchDelay ), NULL, NULL, NULL, &global->prefs.incSearchDelay );

    // Set the enable automatic speech.
    CtlSetValue ( GetObjectPtr ( DetailsEnableAutoSpeech ), global->prefs.enableAutoSpeech );

	//CtlSetValue ( GetObjectPtr ( cbDAeditable ), global->prefs.daEditable );

	LstSetSelection(GetObjectPtr(lstMenuType),global->prefs.menuType);
	CtlSetLabel(GetObjectPtr(triggerMenuType),LstGetSelectionText(GetObjectPtr(lstMenuType),global->prefs.menuType));
	
	LstSetSelection(GetObjectPtr(lstDAsize),global->prefs.daSize);
	CtlSetLabel(GetObjectPtr(triggerDAsize),LstGetSelectionText(GetObjectPtr(lstDAsize),global->prefs.daSize));
	
    return ;
}

/*
 * FUNCTION: DetailsfFormHandleEvent
 *
 * DESCRIPTION:
 *
 * This routine is the event handler for the "DetailsForm" of this 
 * application.
 *
 * PARAMETERS:
 *
 * eventP
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed to
 *     FrmHandleEvent
 */

static Boolean DetailsFormHandleEvent( FormType * frmP, EventType * eventP, Boolean *exit )
{
    Boolean	handled = false;
    AppGlobalType	*global;
    ListType		*lstP,*lstP2;
	ControlType		*triP,*triP2;
	
	lstP = GetObjectPtr(lstMenuType);
	triP = GetObjectPtr(triggerMenuType);
	lstP2 = GetObjectPtr(lstDAsize);
	triP2 = GetObjectPtr(triggerDAsize);
	
    global = AppGetGlobal();
    *exit = false;

    switch ( eventP->eType )
    {
	    case frmOpenEvent:
	        {
	            frmP = FrmGetActiveForm();
	            DetailsFormInit( frmP );
	            FrmDrawForm( frmP );
	            handled = true;
	            break;
	        }

	    case frmUpdateEvent:
	        frmP = FrmGetActiveForm();
	        FrmDrawForm( frmP );
	        break;

	    case ctlSelectEvent:
        {
            switch(eventP->data.ctlSelect.controlID)
            {
            	case DetailsDoneButton:
            	{
	                // Get the get clipboard at normal launch setting.
	                global->prefs.getClipBoardAtStart = ( CtlGetValue ( GetObjectPtr ( DetailsGetClipBoardAtStart ) ) != 0 );

	                // Get the incremental search setting.
	                global->prefs.enableIncSearch = ( CtlGetValue ( GetObjectPtr ( DetailsEnableIncSearch ) ) != 0 );

	                // Get the incremental search setting.
	                global->prefs.enableSingleTap = ( CtlGetValue ( GetObjectPtr ( DetailsEnableSingleTap ) ) != 0 );

	                // Get the enable highlight word field.
	                global->prefs.enableHighlightWord = ( CtlGetValue ( GetObjectPtr ( DetailsEnableHighlightWord ) ) != 0 );

	                // Get the enable highlight word field.
	                global->prefs.enableTryLowerSearch = ( CtlGetValue ( GetObjectPtr ( DetailsEnableTryLowerSearch ) ) != 0 );

	                // Get the disable phonetic font support inside.
	                global->prefs.useSystemFont = ( CtlGetValue ( GetObjectPtr ( DetailsDisablePhoneticFont ) ) != 0 );

	                // Get the enable automatic speech.
	                global->prefs.enableAutoSpeech = ( CtlGetValue ( GetObjectPtr ( DetailsEnableAutoSpeech ) ) != 0 );
					
					//global->prefs.daEditable = ( CtlGetValue ( GetObjectPtr ( cbDAeditable ) ) != 0 );
					
	                *exit = true;
	                handled = true;
	                break;
	            }
	            case DetailSearchDelay:
	            {
	                global->prefs.incSearchDelay = eventP->data.ctlSelect.value;
	                handled = true;
	                break;
	            }
	            case DetailsCustomKeys:
	            {
	            	SetCustomKeyEventHandler();
	            	handled = true;
	            	break;
	            }
	            case triggerMenuType:
				{
					LstPopupList(lstP);
					CtlSetLabel(triP,LstGetSelectionText(lstP,LstGetSelection(lstP)));
					global->prefs.menuType = LstGetSelection (lstP);
					handled = true;
	            	break;
				}
				case triggerDAsize:
				{
					LstPopupList(lstP2);
					CtlSetLabel(triP2,LstGetSelectionText(lstP2,LstGetSelection(lstP2)));
					global->prefs.daSize = LstGetSelection (lstP2);
					handled = true;
	            	break;
				}
            break;
            }
        break;
        }
    }

    return handled;
}

/*
 * FUNCTION: DetailsFormPopupDetailsDialog
 *
 * DESCRIPTION: This routine popup "DetailsForm" dialog.
 *
 * PARAMETERS: nothing.
 *
 * RETURNED: nothing.
 */

static Err DetailsFormPopupDetailsDialog( void )
{
    EventType	event;
    FormPtr	frmP, originalForm;
    Boolean	exit, handled;

    // Remember the original form
    originalForm = FrmGetActiveForm();

    frmP = FrmInitForm ( DetailsForm );
    FrmSetActiveForm( frmP );
    DetailsFormInit( frmP );
    FrmDrawForm ( frmP );

    do
    {
        EvtGetEvent( &event, evtWaitForever );

        if ( SysHandleEvent ( &event ) )
            continue;

        if ( event.eType == appStopEvent )
            EvtAddEventToQueue( &event );

        handled = DetailsFormHandleEvent( frmP, &event, &exit );

        // Check if the form can handle the event
        if ( !handled )
            FrmHandleEvent ( frmP, &event );

    }
    while ( event.eType != appStopEvent && !exit );


    FrmEraseForm ( frmP );
    FrmDeleteForm ( frmP );
    FrmSetActiveForm ( originalForm );

    return errNone;
}

#pragma mark -


/*
 * FUNCTION: DAFormSearch
 *
 * DESCRIPTION: This routine search the word that in word field.
 *
 * PARAMETERS: putinHistory -> if be ture then put the word to history list.
 *
 * RETURNED: errNone if success else fail.
 *
 */

static Err DAFormSearch( Boolean putinHistory, Boolean highlight, Boolean bEnableBreak, Boolean bEnableAutoSpeech )
{
    UInt16	matchs;
    UInt8	*explainPtr;
    UInt32	explainLen;
    AppGlobalType	*global;
    ZDicDBDictInfoType	*dictInfo;
    Err	err;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;

    if ( bEnableBreak )
    {
        Int32 ticksPreHalfSecond;
        Int16 delayTimes;

        ticksPreHalfSecond = SysTicksPerSecond() / TIMES_PRE_SECOND;
        delayTimes = global->prefs.incSearchDelay;
        while ( !EvtSysEventAvail ( true ) && delayTimes > 0 )
        {
            Int16 x, y;
            Boolean penDown;

            EvtGetPen ( &x, &y, &penDown );
            if ( penDown )
                break;

            SysTaskDelay ( ticksPreHalfSecond );
            delayTimes--;
        }

        if ( delayTimes > 0 )
        {
            global->needSearch = true;
            global->putinHistory = putinHistory;
            global->highlightWordField = highlight;
            return errNone;
        }
        else
            global->needSearch = false;
    }

    err = ToolsSearch( DAWordField, &matchs, &explainPtr, &explainLen );
    if ( err == errNone && explainPtr != NULL && explainLen != 0 )
    {
        FormType * frmP;
        MemHandle	bufH;
        Char *str;

        // Clear the menu status from the display
        MenuEraseStatus( 0 );

        if ( putinHistory )
        {
            ToolsPutWordFieldToHistory( DAWordField );
        }

        bufH = MemHandleNew( explainLen + 1 );
        if ( bufH == NULL )
            return memErrNotEnoughSpace;

        // format string.
        str = MemHandleLock( bufH );
        MemMove( str, explainPtr, explainLen );
        str[ explainLen ] = chrNull;
        ToolsFormatExplain( ( UInt8* ) str );
        ToolsGMXTranslate( str, dictInfo->phonetic[ dictInfo->curMainDictIndex ] );
        MemHandleUnlock( bufH );

        // update display.
        ToolsSetFieldHandle( DADescriptionField, bufH, true );
        ToolsUpdateScrollBar ( DADescriptionField, DADescriptionScrollBar );

        // select all the text in word field, then use can clear it easy.
        if ( highlight )
            ToolsHighlightField( DAWordField );

        // Display or hide the player button
        frmP = FrmGetActiveForm ();
        if ( ZDicVoiceIsExist ( ( Char * ) explainPtr ) )
        {
            ShowObject ( frmP, DAPlayVoice );
            if ( bEnableAutoSpeech )
                ToolsPlayVoice ();
        }
        else
            HideObject ( frmP, DAPlayVoice );

    }

    return errNone;
}

/*
 * FUNCTION: DAFormHandleNilEvent
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED: true if the field handled the event
 *
 */

static Boolean DAFormHandleNilEvent( void )
{
    AppGlobalType	* global;

    global = AppGetGlobal();

    return false;
}

/*
 * FUNCTION: DAFormAdjustFormBounds
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:  -> curBounds Input current bounds.
 *
 * RETURNED:
 */
#define DAFormBorder	3

void DAFormAdjustFormBounds ( AppGlobalType	*global, FormPtr frmP,
                              RectangleType curBounds, RectangleType displayBounds )
{
    // adjust the bround and set new form bround
    if ( curBounds.topLeft.x < DAFormBorder )
        curBounds.topLeft.x = DAFormBorder;
    if ( curBounds.topLeft.y < DAFormBorder )
        curBounds.topLeft.y = DAFormBorder;
    displayBounds.extent.x -= DAFormBorder;
    displayBounds.extent.y -= DAFormBorder;
    if ( curBounds.topLeft.x + curBounds.extent.x > displayBounds.extent.x )
        curBounds.topLeft.x = displayBounds.extent.x - curBounds.extent.x;
    if ( curBounds.topLeft.y + curBounds.extent.y > displayBounds.extent.y )
        curBounds.topLeft.y = displayBounds.extent.y - curBounds.extent.y;
    global->prefs.daFormLocation = curBounds.topLeft;

    // Use new bround for da form.
    curBounds.topLeft = global->prefs.daFormLocation;
    WinSetBounds( FrmGetWindowHandle ( frmP ), &curBounds );

    return ;
}

/*
 * FUNCTION: DAFormMoveForm
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * RETURNED:
 */

static Boolean DAFormMoveForm( EventType * eventP )
{
    FormType	* frmP;
    Coord	x, y, dx, dy, oldx, oldy;
    Boolean	penDown = true;
    RectangleType	r, displayBounds;
    WinHandle	winH;
    AppGlobalType	*global;
    WinDrawOperation	oldMode;
    WinHandle	oldDrawWinH, oldActiveWinH;

    global = AppGetGlobal();

    frmP = FrmGetActiveForm();
    FrmGetObjectBounds ( frmP, FrmGetObjectIndex ( frmP, DAMoveRepeatButton ), &r );
    //r.topLeft.x = 0;
    //r.topLeft.y = 0;
    //r.extent.x = 151;
    //r.extent.y = 13;
    
    if ( !RctPtInRectangle ( eventP->screenX, eventP->screenY, &r ) )
        return false;

    // Set the draw and acitve windows to fixe bugs of chinese os.
    WinSetDrawWindow ( FrmGetWindowHandle ( frmP ) );
    WinSetActiveWindow ( FrmGetWindowHandle ( frmP ) );

    // Hide DA form.
    oldDrawWinH = WinGetDrawWindow ();
    oldActiveWinH = WinGetActiveWindow ();
    FrmEraseForm( frmP );

    // get the new display window bounds
    winH = WinGetDisplayWindow ();
    WinSetDrawWindow ( winH );
    ZDicToolsWinGetBounds ( winH, &displayBounds );

    FrmGetFormBounds( frmP, &r );
    RctInsetRectangle( &r, 2 );

    oldMode = WinSetDrawMode ( winInvert );

    PenGetPoint ( &x, &y, &penDown );
    WinWindowToDisplayPt ( &x, &y );
    dx = x - r.topLeft.x;
    dy = y - r.topLeft.y;
    oldx = x;
    oldy = y;
    WinPaintRectangleFrame ( roundFrame, &r );

    // Trace the pen until it move enough to constitute a move operation or until
    // the pen it released.
    while ( true )
    {
        PenGetPoint ( &x, &y, &penDown );
        WinWindowToDisplayPt ( &x, &y );

        if ( ! penDown )
            break;

        if ( x != oldx || y != oldy )
        {
            WinPaintRectangleFrame ( roundFrame, &r );
            r.topLeft.x = x - dx;
            r.topLeft.y = y - dy;
            oldx = x;
            oldy = y;
            WinPaintRectangleFrame ( roundFrame, &r );
        }
    }

    WinPaintRectangleFrame ( roundFrame, &r );

    // adjust the bround and set new form bround
    DAFormAdjustFormBounds ( global, frmP, r, displayBounds );

    WinSetDrawMode ( oldMode );

    // Show DA form.
    WinSetDrawWindow ( oldDrawWinH );
    WinSetActiveWindow ( oldActiveWinH );
    FrmDrawForm( frmP );

    return true;
}

/*
 * FUNCTION: MainFormIncrementalSearch
 *
 * DESCRIPTION: Adds a character to word field and looks up the word.
 *
 * PARAMETERS: event - EventType* containing character to add to word field.
 *
 * RETURNED: true if the field handled the event
 *
 */

static Boolean DAFormIncrementalSearch( EventType * event )
{

    FormType	* frmP;
    UInt16	fldIndex;
    FieldPtr	fldP;
    AppGlobalType	*global;

    global = AppGetGlobal();

    frmP = FrmGetActiveForm();
    fldIndex = FrmGetObjectIndex( frmP, DAWordField );
    FrmSetFocus( frmP, fldIndex );
    fldP = FrmGetObjectPtr ( frmP, fldIndex );

    if ( FldHandleEvent ( fldP, event ) || event->eType == fldChangedEvent )
    {
        if ( global->prefs.enableIncSearch )
        {
            DAFormSearch( false, false, true, global->prefs.enableAutoSpeech );
        }

        return true;
    }

    return false;
}

/*
 * FUNCTION: DAFormChangeWordFieldCase
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */
static Err DAFormChangeWordFieldCase( void )
{
    FieldType	* fieldP;
    Char	*wordStr, newStr[ MAX_WORD_LEN + 1 ];
    Err	err;
    AppGlobalType *	global;

    global = AppGetGlobal();

    fieldP = GetObjectPtr( DAWordField );
    wordStr = FldGetTextPtr ( fieldP );
    if ( wordStr == NULL || wordStr[ 0 ] == chrNull )
        return errNone;

    MemSet( newStr, sizeof( newStr ), 0 );
    StrNCopy( newStr, wordStr, MAX_WORD_LEN );

    err = ToolsChangeCase ( newStr );
    if ( err == errNone )
    {
        ToolsSetFieldPtr ( DAWordField, newStr, StrLen( newStr ), true );
        DAFormSearch( false, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
    }

    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:	DaFormJumpSearch
 *
 * DESCRIPTION: Jump search a word if user select a word.
 *
 * PARAMETERS:
 *				->	event The Event that we received.
 *
 * RETURN:
 *				true if handled else false.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		JEMYZHANG	06/Apr/08	Initial Revision
 *
 ***********************************************************************/

static Boolean DAFormJumpSearch( EventType * event )
{
    FormType	* frmP;
    Char	*buf;
    FieldType	*field;
    AppGlobalType	*global;
    Boolean	handled = false;

    if ( event->data.fldEnter.fieldID != DADescriptionField )
        return false;

    field = ( FieldType * ) GetObjectPtr( DADescriptionField );
    if ( FldHandleEvent ( field, event ) )
    {
        global = AppGetGlobal();
        buf = ( Char * ) global->data.readBuf;

        if ( global->prefs.enableJumpSearch )
        {
            // put old word into history and search new word and put it into history.
            ToolsPutWordFieldToHistory( DAWordField );
            if ( ToolsGetFieldHighlightText( field, buf, MAX_WORD_LEN, global->prefs.enableSingleTap ) == errNone )
            {
                ToolsSetFieldPtr( DAWordField, buf, StrLen( buf ), true );
 				DAFormSearch( false, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
                // Set force to input field.
                frmP = FrmGetActiveForm ();
                FrmSetFocus( frmP, FrmGetObjectIndex( frmP, DAWordField ) );
            }
        }
        handled = true;
    }

    return handled;
}


/*
 * FUNCTION: MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:
 *
 * command
 *     menu item id
 */

static Boolean DAFormDoCommand( UInt16 command )
{
    Boolean handled = false;
    AppGlobalType *	global;

    global = AppGetGlobal();

    if ( //command == DictionarysDictAll || 
         command >= ZDIC_DICT_MENUID )
    {
    	if(global->prefs.daSize == 0)
    	{
        	handled = ToolsChangeDictionaryCommand( DAForm, command );
        }
    	else if(global->prefs.daSize == 1)
    	{
    		handled = ToolsChangeDictionaryCommand( DAFormLarge, command );
    	}
        return handled;
    }

    switch ( command )
    {
    case OptionsAboutZDic:
        {
            FormType * frmP;

            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            // Display the About Box.
            frmP = FrmInitForm ( AboutForm );
            FrmDoDialog ( frmP );
            FrmDeleteForm ( frmP );

            handled = true;
            break;
        }
    case OptionsDictOptions:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            // Display the preference from the display
            PrefFormPopupPrefsDialog();
            handled = true;

            // Open new dictionary database.
            if ( ZDicOpenCurrentDict() != errNone )
            {
                ToolsQuitApp();
            }
            else
            {
                DAFormSearch( false, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
            }

            break;
        }
    case OptionsPreferences:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            // Display the details from the display
            DetailsFormPopupDetailsDialog();
	        
            handled = true;
            break;
        }
    case OptionsExportOptions:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            // Display exorpt options from.
            ExportPopupDialog();
            handled = true;
            break;
        }
    case OptionsGotoWord:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            DAFormSearch( true, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
            handled = true;
            break;
        }
    case OptionsClearWord:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            handled = ToolsClearInput ( DAWordField );
            break;
        }
    case OptionsClearHistory:
        {
            global->prefs.history[ 0 ][ 0 ] = chrNull;
            global->historySeekIdx = 0;
            handled = true;
            break;
        }
    case OptionsChangeCase:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            DAFormChangeWordFieldCase();
            handled = true;
            break;
        }
    case OptionsExportMemo:
        {
            ExportToMemo( DADescriptionField );
            handled = true;
            break;
        }
    case OptionsExportSugarMemo:
        {
            ExportToSugarMemo( DADescriptionField );
            handled = true;
            break;
        }
    case DictionarysDictAll:
        {
            ToolsAllDictionaryCommand( DAWordField, DADescriptionField, DADescriptionScrollBar );
            handled = true;
            break;
        }
    }

    return handled;
}

/***********************************************************************
 *
 * FUNCTION:	DAFormUpdateDisplay
 *
 * DESCRIPTION: This routine redraw the DAForm form.
 *
 * PARAMETERS:
 *				->	updateCode Update code.
 *
 * RETURN:
 *				nothing.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static void DAFormUpdateDisplay( UInt16 updateCode )
{
    AppGlobalType	* global;
    Err	err = errNone;
    //FormType *frmP;

    global = AppGetGlobal();

    if ( updateCode & updateDictionaryChanged )
    {
        // Open new dictionary database.
        err = ZDicOpenCurrentDict();
        if ( err != errNone )
        {
            ToolsQuitApp();
            return ;
        }
		
        DAFormSearch( false, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
        
		// initial dictionary name
	    StrCopy( global->dictTriggerName, &global->prefs.dictInfo.displayName[ global->prefs.dictInfo.curMainDictIndex ][ 0 ] );
	    //ToolsFontTruncateName( global->dictTriggerName, stdFont, ResLoadConstant( DictionaryNameMaxLenInPixels ) );
	    //frmP = FrmGetActiveForm ();
	    //FrmCopyTitle ( frmP, global->dictTriggerName );
	    CtlSetLabel(GetObjectPtr(lblDATitle),global->dictTriggerName);
	    FrmDrawForm( FrmGetActiveForm() );
    }

    if ( updateCode & updateFontChanged )
    {
        FieldType * field;
        field = ( FieldType* ) GetObjectPtr( DADescriptionField );
        FldSetFont( field, global->prefs.fontDA );
        ToolsUpdateScrollBar ( DADescriptionField, DADescriptionScrollBar );
    }

    if ( updateCode & frmRedrawUpdateCode )
    {
    	FrmDrawForm( FrmGetActiveForm() );
    }

    return ;
}

/*
 * FUNCTION: DAFormInit
 *
 * DESCRIPTION: This routine initializes the DAForm form.
 *
 * PARAMETERS:
 *
 * frm
 *     pointer to the MainForm form.
 */

static void DAFormInit( FormType *frmP )
{
    AppGlobalType	* global;
    FieldType	*field;

    global = AppGetGlobal();
    
    // initial description and word field font.
    field = GetObjectPtr( DADescriptionField );
    FldSetFont( field, global->prefs.fontDA );
    field = GetObjectPtr( DAWordField );
    FldSetFont( field, global->font.smallFontID );
    
    // Initial word for word field.
    if ( global->initKeyWord[ 0 ] != chrNull )
    {
        ToolsSetFieldPtr( DAWordField, &global->initKeyWord[ 0 ], StrLen( &global->initKeyWord[ 0 ] ), false );
    }
    else
    {
        ToolsSetFieldPtr( DAWordField, &global->initKeyWord[ 0 ], 2, false );
    }

    //FrmSetFocus( frmP, FrmGetObjectIndex( frmP, DAWordField ) );

    // Set get word source.
    //FrmSetControlValue(frmP,DAClipboardPushButton,global->prefs.getClipBoardAtStart);
    CtlSetValue (GetObjectPtr(DAClipboardPushButton),global->prefs.getClipBoardAtStart);
    //FrmSetControlValue(frmP,DAJumpPushButton,global->prefs.enableJumpSearch);
	CtlSetValue (GetObjectPtr(DAJumpPushButton),global->prefs.enableJumpSearch);
    
    // set da form location
    {
        WinHandle	winH;
        RectangleType	r;

        // Set the draw and acitve windows to fixe bugs of chinese os.
        WinSetDrawWindow ( FrmGetWindowHandle ( frmP ) );
        WinSetActiveWindow ( FrmGetWindowHandle ( frmP ) );
		
        WinGetDrawWindowBounds ( &r );
        r.topLeft = global->prefs.daFormLocation;
        winH = FrmGetWindowHandle ( frmP );
        WinSetBounds( winH, &r );
    }
    
    // initial dictionary name
    StrCopy( global->dictTriggerName, &global->prefs.dictInfo.displayName[ global->prefs.dictInfo.curMainDictIndex ][ 0 ] );
    //ToolsFontTruncateName( global->dictTriggerName, stdFont, ResLoadConstant( DictionaryNameMaxLenInPixels ) );
    //FrmCopyTitle ( frmP, global->dictTriggerName );
	CtlSetLabel(GetObjectPtr(lblDATitle),global->dictTriggerName);
	FrmDrawForm( FrmGetActiveForm() );

    return ;
}

/*
 * FUNCTION: DAFormHandleEvent
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * eventP
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed to
 *     FrmHandleEvent
 */

static Boolean DAFormHandleEvent( EventType * eventP )
{
    Boolean	handled = false;
    FormType	*frmP;
    AppGlobalType	*global;

    global = AppGetGlobal();

    switch ( eventP->eType )
    {
    case menuEvent:
        return DAFormDoCommand( eventP->data.menu.itemID );

    case menuOpenEvent:
        {
            ToolsCreatDictionarysMenu ();
            // don't set handled = true
            break;
        }

    case frmOpenEvent:

        // Do not draw anything before FrmDrawForm because
        // FrmDrawForm will save current screen.
        frmP = FrmGetActiveForm();
        
        ZDicDIAFormLoadInitial ( global, frmP );
        DAFormInit( frmP );
        ZDicDIADisplayChange ( global );
        
        FrmDrawForm ( frmP );
        
        DAFormSearch( true, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
        handled = true;
        break;

    case frmUpdateEvent:
        /*
         * To do any custom drawing here, first call
         * FrmDrawForm(), then do your drawing, and
         * then set handled to true. 
         */
        DAFormUpdateDisplay ( eventP->data.frmUpdate.updateCode );
        handled = true;
        break;

    case frmCloseEvent:
        ToolsSetFieldHandle( DADescriptionField, NULL, false );
        ToolsSetFieldHandle( DAWordField, NULL, false );
        ZDicDIAFormClose ( global );
        break;

    case winDisplayChangedEvent:
        handled = ZDicDIADisplayChange ( global );
        if ( handled )
        {
            frmP = FrmGetActiveForm ();
            FrmDrawForm ( frmP );
        }
        break;

    case winEnterEvent:
        ZDicDIAWinEnter ( global, eventP );
        handled = true;
        break;

    case penDownEvent:
        handled = DAFormMoveForm( eventP );
        break;

    case keyDownEvent:
        {
            if ( eventP->data.keyDown.chr == chrLineFeed )
            {
                DAFormSearch( true, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
                handled = true;
            }
			else if (eventP->data.keyDown.chr == ToolsHomeKeyChr())
            {
            	//eventP->eType = frmCloseEvent;
            	FrmCloseAllForms();
            	handled = true;
            }
            else if ( EvtKeydownIsVirtual( eventP ) )
            {
                if ( NavDirectionPressed( eventP, Up ) || eventP->data.keyDown.chr == vchrRockerUp || eventP->data.keyDown.chr == vchrPageUp  || eventP->data.keyDown.chr == vchrThumbWheelUp/* || (eventP->data.keyDown.chr) == vchrJogUp*/)
                {
                	Char s = ToolsGetOptKeyStatus();
					if(( global->prefs.OptUD && hasOptionPressed(eventP->data.keyDown.modifiers)) ||
						(global->prefs.OptUD && s > 0))
					{
						if(global->prefs.UD)
						{
							if(global->prefs.SwitchUDLR)
							{
								ToolsScrollWord ( winUp, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
								MainFormUpdateWordList( );
							}
							else
							{
								ToolsPrevDictionaryCommand();
							}
						}
						else
						{
							ToolsPageScroll ( winUp, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
                    	}
                    	if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else if((!global->prefs.OptUD) && (!hasOptionPressed(eventP->data.keyDown.modifiers)))
					{
                    	if(global->prefs.UD)
						{
							if(global->prefs.SwitchUDLR)
							{
								ToolsScrollWord ( winUp, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
								MainFormUpdateWordList( );
							}
							else
							{
								ToolsPrevDictionaryCommand();
							}
						}
						else
						{
							ToolsPageScroll ( winUp, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
                    	}
					}
					else
					{
						ToolsPageScroll ( winUp, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
                    }
                    handled = true;
                }
                else if ( NavDirectionPressed( eventP, Down )  || eventP->data.keyDown.chr == vchrRockerDown || eventP->data.keyDown.chr == vchrPageDown || eventP->data.keyDown.chr == vchrThumbWheelDown/* || (eventP->data.keyDown.chr) == vchrJogDown*/)
                {
                	Char s = ToolsGetOptKeyStatus();
                	if( (global->prefs.OptUD && hasOptionPressed(eventP->data.keyDown.modifiers)) ||
						(global->prefs.OptUD && s > 0))
					{
						if(global->prefs.UD)
						{
							if(global->prefs.SwitchUDLR)
							{
								ToolsScrollWord ( winDown, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
							}
							else
							{
								ToolsNextDictionaryCommand();
							}
						}
						else
						{
							ToolsPageScroll ( winDown, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
                    	}
                    	if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else if((!global->prefs.OptUD) && (!hasOptionPressed(eventP->data.keyDown.modifiers)))
					{
                    	if(global->prefs.UD)
						{
							if(global->prefs.SwitchUDLR)
							{
								ToolsScrollWord ( winDown, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
							}
							else
							{
								ToolsNextDictionaryCommand();
							}
						}
						else
						{
							ToolsPageScroll ( winDown, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
                    	}
					}
					else
					{
						ToolsPageScroll ( winDown, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
                    }
                    handled = true;
                }
                else if ( NavDirectionPressed( eventP, Left ) && global->prefs.LR)
                {
                	Char s = ToolsGetOptKeyStatus();
                	if((global->prefs.OptLR && hasOptionPressed(eventP->data.keyDown.modifiers)) ||
						(global->prefs.OptLR && s > 0))
					{
						if(global->prefs.SwitchUDLR)
						{
							ToolsPrevDictionaryCommand();
						}
						else
						{
							ToolsScrollWord ( winUp, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
						}
						if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else if((!global->prefs.OptLR) && (!hasOptionPressed(eventP->data.keyDown.modifiers)))
					{
						if(global->prefs.SwitchUDLR)
						{
							ToolsPrevDictionaryCommand();
						}
						else
						{
							ToolsScrollWord ( winUp, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
						}
					}
                }
                else if ( NavDirectionPressed( eventP, Right ) && global->prefs.LR)
                {
                	Char s = ToolsGetOptKeyStatus();
                	if( (global->prefs.OptLR && hasOptionPressed(eventP->data.keyDown.modifiers)) ||
						(global->prefs.OptLR && s > 0))
					{
						if(global->prefs.SwitchUDLR)
						{
							ToolsNextDictionaryCommand();
						}
						else
						{
							ToolsScrollWord ( winDown, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
						}
						if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else if((!global->prefs.OptLR) && (!hasOptionPressed(eventP->data.keyDown.modifiers)))
					{
						if(global->prefs.SwitchUDLR)
						{
							ToolsNextDictionaryCommand();
						}
						else
						{
							ToolsScrollWord ( winDown, DADescriptionField, DADescriptionScrollBar, DAWordField, DAPlayVoice, global->prefs.enableHighlightWord );
						}
					}
                }                
                else if ( (NavSelectPressed( eventP )
                          || ( eventP->data.keyDown.chr ) == vchrRockerCenter
                          || ( eventP->data.keyDown.chr ) == vchrThumbWheelPush
                          || ( eventP->data.keyDown.chr ) == vchrJogRelease)
                          && (global->prefs.SelectKeyUsed == 0)
                        )
                {
                	Char s = ToolsGetOptKeyStatus();
                	if(hasOptionPressed(eventP->data.keyDown.modifiers) || s > 0)
					{
						switch(global->prefs.OptSelectKeyFunc)
	                	{
	                		case 0:
	                		{
	                			ToolsPlayVoice();
	                			handled = true;
	                			break;
	                		}
	                		case 3:
	                		{
	                			ToolsNextDictionaryCommand();
	                			handled = true;
	                			break;
	                		}
	                		case 4:
	                		{
	                			ToolsClearInput ( DAWordField );
	                			handled = true;
	                			break;
	                		}
	                		case 5:
	                		{
	                			FormHistorySeekBack (DAForm);
	                			handled = true;
	                			break;
	                		}
	                		case 6:
	                		{
	                			ToolsSendMenuCmd( global->prefs.exportAppCreatorID == sysFileCMemo ? chrCapital_M : chrCapital_Z );
	                			handled = true;
	                			break;
	                		}
	                		case 7:
	                		{
	                			ShortcutPopEventHandler();
	                			handled = true;
	                			break;
	                		}
	                		case 8:
	                		{
	                			FormPopupDictList(DAForm);
	                			handled = true;
	                			break;
	                		}
	                		case 9:
	                		{
	                			ToolsAllDictionaryCommand( DAWordField, DADescriptionField, DADescriptionScrollBar );
                				handled = true;
	                			break;
	                		}
	                    }
	                    if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else
					{
						switch(global->prefs.SelectKeyFunc)
	                	{
	                		case 0:
	                		{
	                			ToolsPlayVoice();
	                			handled = true;
	                			break;
	                		}
	                		case 3:
	                		{
	                			ToolsNextDictionaryCommand();
	                			handled = true;
	                			break;
	                		}
	                		case 4:
	                		{
	                			ToolsClearInput ( DAWordField );
	                			handled = true;
	                			break;
	                		}
	                		case 5:
	                		{
	                			FormHistorySeekBack (DAForm);
	                			handled = true;
	                			break;
	                		}
	                		case 6:
	                		{
	                			ToolsSendMenuCmd( global->prefs.exportAppCreatorID == sysFileCMemo ? chrCapital_M : chrCapital_Z );
	                			handled = true;
	                			break;
	                		}
	                		case 7:
	                		{
	                			ShortcutPopEventHandler();
	                			handled = true;
	                			break;
	                		}
	                		case 8:
	                		{
	                			FormPopupDictList(DAForm);
	                			handled = true;
	                			break;
	                		}
	                		case 9:
	                		{
	                			ToolsAllDictionaryCommand( DAWordField, DADescriptionField, DADescriptionScrollBar );
                				handled = true;
	                			break;
	                		}
	                    }
					}
                    handled = true;
                }
                
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyPlaySoundChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyPlaySoundKeycode )
                {
                	ToolsPlayVoice();
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyExportChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyExportKeycode )
                {
                	ToolsSendMenuCmd( global->prefs.exportAppCreatorID == sysFileCMemo ? chrCapital_M : chrCapital_Z );
	                handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyOneKeyChgDicChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyOneKeyChgDicKeycode)
                {
                	ToolsNextDictionaryCommand();
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyClearFieldChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyClearFieldKeycode)
                {
                	handled = ToolsClearInput ( DAWordField );
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyShortcutChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyShortcutKeycode)
                {
					ShortcutPopEventHandler();
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyPopupChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyPopupKeycode)
                {
                	FormPopupDictList(DAForm);
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyGobackChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyGobackKeycode)
                {
                	FormHistorySeekBack (DAForm);
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keySearchAllChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keySearchAllKeycode)
                {
                	ToolsAllDictionaryCommand( DAWordField, DADescriptionField, DADescriptionScrollBar );
                	handled = true;
                }
            }

            else
            {
                handled = DAFormIncrementalSearch( eventP );
            }

            break;
        }

    case fldChangedEvent:
        {
            if ( eventP->data.fldChanged.fieldID == DAWordField )
                handled = DAFormIncrementalSearch( eventP );
            else
                ToolsUpdateScrollBar ( DADescriptionField, DADescriptionScrollBar );

            break;
        }
    case fldEnterEvent:
    	handled = DAFormJumpSearch(eventP);
    	break;

    case sclRepeatEvent:
        {
            ToolsScroll ( eventP->data.sclRepeat.newValue -
                          eventP->data.sclRepeat.value, false, DADescriptionField, DADescriptionScrollBar );
            break;
        }

    case ctlSelectEvent:
        {
            if ( eventP->data.ctlSelect.controlID == DAClipboardPushButton)
            {
                //global->prefs.getClipBoardAtStart = !global->prefs.getClipBoardAtStart;
                global->prefs.getClipBoardAtStart = CtlGetValue( GetObjectPtr(DAClipboardPushButton) );
	            
                if(global->prefs.getClipBoardAtStart)
                {
	                ToolsGetStartWord (global);

	                // Initial word for word field.
	                if ( global->initKeyWord[ 0 ] != chrNull )
	                {
	                    ToolsSetFieldPtr( DAWordField, &global->initKeyWord[ 0 ], StrLen( &global->initKeyWord[ 0 ] ), true );
	                    DAFormSearch( true, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
	                }
				}
                handled = true;
            }
            else if ( eventP->data.ctlSelect.controlID == DAJumpPushButton)
            {
            	//global->prefs.enableJumpSearch = !global->prefs.enableJumpSearch;
            	global->prefs.enableJumpSearch =  CtlGetValue( GetObjectPtr(DAJumpPushButton) );
            	handled = true;
            }
            else if ( eventP->data.ctlSelect.controlID == DANextDict )
            {
                ToolsNextDictionaryCommand();
                handled = true;
            }
            else if ( eventP->data.ctlSelect.controlID == DAPlayVoice )
            {
                ToolsPlayVoice ();
                handled = true;
            }

            else if ( eventP->data.ctlSelect.controlID == DAExportToMemo )
            {
                ToolsSendMenuCmd( global->prefs.exportAppCreatorID == sysFileCMemo ? chrCapital_M : chrCapital_Z );
                handled = true;
            }


            break;
        }

    case nilEvent:
        {
            handled = DAFormHandleNilEvent ();
            break;
        }

        // If there have not any event then we continue the broken search.
        if ( global->needSearch && !EvtSysEventAvail ( true ) )
        {
            DAFormSearch( global->putinHistory, global->highlightWordField, false, global->prefs.enableAutoSpeech );
        }
    }

    return handled;
}

///////////////////////////////////////////////////////////////////////////

#pragma mark -

/*
 * FUNCTION: MainFormAdjustObject
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */

Err MainFormAdjustObject ( const RectanglePtr toBoundsP )
{
    FormPtr frmP;
    ControlPtr ctlP;
    FieldPtr fldP;
    RectangleType	descFieldBound, descScrollBarBound, wordListBound, upBtonBound, downBtonBound;
    UInt16	descFieldIndex, descScrollBarIndex, wordListIndex, upBtonIndex, downBtonIndex;
    AppGlobalType	*global;

    global = AppGetGlobal();

    frmP = FrmGetActiveForm();
    descFieldIndex = FrmGetObjectIndex ( frmP, MainDescriptionField );
    FrmGetObjectBounds( frmP, descFieldIndex, &descFieldBound );
    descScrollBarIndex = FrmGetObjectIndex ( frmP, MainDescScrollBar );
    FrmGetObjectBounds( frmP, descScrollBarIndex, &descScrollBarBound );
    wordListIndex = FrmGetObjectIndex ( frmP, MainWordList );
    FrmGetObjectBounds( frmP, wordListIndex, &wordListBound );
    upBtonIndex = FrmGetObjectIndex ( frmP, MainWordListUpRepeatButton );
    FrmGetObjectBounds( frmP, upBtonIndex, &upBtonBound );
    downBtonIndex = FrmGetObjectIndex ( frmP, MainWordListDownRepeatButton );
    FrmGetObjectBounds( frmP, downBtonIndex, &downBtonBound );

    // Only display desc field.
    if ( ! global->wordListIsOn )
    {
        HideObject( frmP, MainWordListUpRepeatButton );
        HideObject( frmP, MainWordListDownRepeatButton );
        HideObject( frmP, MainWordList );
        HideObject( frmP, MainDescriptionField );
        HideObject( frmP, MainDescScrollBar );

        descFieldBound.topLeft.x = COORD_START_X + 1;
        descFieldBound.topLeft.y = COORD_START_Y;
        descFieldBound.extent.x = ( toBoundsP->extent.x - COORD_SCROLLBAR_WIDTH -1 );
        descFieldBound.extent.y = ( toBoundsP->extent.y + 2 - COORD_START_Y - COORD_TOOLBAR_HEIGHT );
        FrmSetObjectBounds( frmP, descFieldIndex, &descFieldBound );

        descScrollBarBound.topLeft.x = descFieldBound.topLeft.x + descFieldBound.extent.x;
        descScrollBarBound.topLeft.y = descFieldBound.topLeft.y;
        descScrollBarBound.extent.x = COORD_SCROLLBAR_WIDTH;
        descScrollBarBound.extent.y = descFieldBound.extent.y;
        FrmSetObjectBounds ( frmP, descScrollBarIndex, &descScrollBarBound );

        ShowObject( frmP, MainDescriptionField );
        ShowObject( frmP, MainDescScrollBar );
    }

    // word list is under of desc field.
    else if ( toBoundsP->extent.x < toBoundsP->extent.y )
    {
        HideObject( frmP, MainWordListUpRepeatButton );
        HideObject( frmP, MainWordListDownRepeatButton );
        HideObject( frmP, MainWordList );
        HideObject( frmP, MainDescriptionField );
        HideObject( frmP, MainDescScrollBar );

        wordListBound.topLeft.x = COORD_START_X + COORD_SPACE;
        wordListBound.topLeft.y = toBoundsP->extent.y - COORD_TOOLBAR_HEIGHT - wordListBound.extent.y;
        wordListBound.extent.x = toBoundsP->extent.x - COORD_SPACE * 4 - COORD_SCROLLBAR_WIDTH;
        FrmSetObjectBounds ( frmP, wordListIndex, &wordListBound );

        upBtonBound.topLeft.x = wordListBound.topLeft.x + wordListBound.extent.x + COORD_SPACE;
        upBtonBound.topLeft.y = wordListBound.topLeft.y - COORD_SPACE;
        upBtonBound.extent.x = COORD_SCROLLBAR_WIDTH + 2 * COORD_SPACE;
        upBtonBound.extent.y = wordListBound.extent.y / 2;
        FrmSetObjectBounds ( frmP, upBtonIndex, &upBtonBound );

        downBtonBound.extent.x = upBtonBound.extent.x;
        downBtonBound.extent.y = upBtonBound.extent.y;
        downBtonBound.topLeft.x = upBtonBound.topLeft.x;
        downBtonBound.topLeft.y = wordListBound.topLeft.y + wordListBound.extent.y + COORD_SPACE - downBtonBound.extent.y;
        FrmSetObjectBounds ( frmP, downBtonIndex, &downBtonBound );

        descFieldBound.topLeft.x = COORD_START_X;
        descFieldBound.topLeft.y = COORD_START_Y;
        descFieldBound.extent.x = ( toBoundsP->extent.x - COORD_SCROLLBAR_WIDTH );
        descFieldBound.extent.y = wordListBound.topLeft.y + 2 - descFieldBound.topLeft.y - COORD_SPACE;
        FrmSetObjectBounds ( frmP, descFieldIndex, &descFieldBound );

        descScrollBarBound.topLeft.x = descFieldBound.topLeft.x + descFieldBound.extent.x;
        descScrollBarBound.topLeft.y = descFieldBound.topLeft.y;
        descScrollBarBound.extent.x = COORD_SCROLLBAR_WIDTH;
        descScrollBarBound.extent.y = descFieldBound.extent.y;
        FrmSetObjectBounds ( frmP, descScrollBarIndex, &descScrollBarBound );

        // Set the label of the up and down button.
        ctlP = GetObjectPtr ( MainWordListUpRepeatButton );
        CtlGlueSetFont ( ctlP, symbolFont );
        FrmCopyLabel ( frmP, MainWordListUpRepeatButton, "\10" );

        ctlP = GetObjectPtr ( MainWordListDownRepeatButton );
        CtlGlueSetFont ( ctlP, symbolFont );
        FrmCopyLabel ( frmP, MainWordListDownRepeatButton, "\7" );

        ShowObject( frmP, MainWordListUpRepeatButton );
        ShowObject( frmP, MainWordListDownRepeatButton );
        ShowObject( frmP, MainWordList );
        ShowObject( frmP, MainDescriptionField );
        ShowObject( frmP, MainDescScrollBar );
    }
    // word list at left of desc field.
    else
    {
        HideObject( frmP, MainWordListUpRepeatButton );
        HideObject( frmP, MainWordListDownRepeatButton );
        HideObject( frmP, MainWordList );
        HideObject( frmP, MainDescriptionField );
        HideObject( frmP, MainDescScrollBar );

        if(global->prefs.font == global->font.smallTinyFontID ||global->prefs.font == global->font.largeTinyFontID)
	    {
			upBtonBound.topLeft.x = COORD_START_X;
	        upBtonBound.topLeft.y = COORD_START_Y + COORD_SPACE;
	        upBtonBound.extent.x = ( toBoundsP->extent.x - COORD_SCROLLBAR_WIDTH ) * 3 / 8;
	        upBtonBound.extent.y = COORD_SCROLLBAR_WIDTH + COORD_SPACE;
	    }
	    else if(global->prefs.font == global->font.smallFontID ||global->prefs.font == global->font.largeFontID)
	    {
	    	upBtonBound.topLeft.x = COORD_START_X;
	        upBtonBound.topLeft.y = COORD_START_Y + COORD_SPACE + 1;
	        upBtonBound.extent.x = ( toBoundsP->extent.x - COORD_SCROLLBAR_WIDTH ) * 3 / 8;
	        upBtonBound.extent.y = COORD_SCROLLBAR_WIDTH + COORD_SPACE;
	    }
        FrmSetObjectBounds ( frmP, upBtonIndex, &upBtonBound );

        wordListBound.topLeft.x = upBtonBound.topLeft.x + COORD_SPACE;
        wordListBound.topLeft.y = upBtonBound.topLeft.y + upBtonBound.extent.y + COORD_SPACE;
	    wordListBound.extent.x = upBtonBound.extent.x - COORD_SPACE * 2;
        FrmSetObjectBounds( frmP, wordListIndex, &wordListBound );

        downBtonBound.topLeft.x = upBtonBound.topLeft.x;
        downBtonBound.topLeft.y = wordListBound.topLeft.y + wordListBound.extent.y + COORD_SPACE + 1;
	    downBtonBound.extent.x = upBtonBound.extent.x;
        downBtonBound.extent.y = upBtonBound.extent.y;
        FrmSetObjectBounds ( frmP, downBtonIndex, &downBtonBound );

        descFieldBound.topLeft.x = wordListBound.topLeft.x + wordListBound.extent.x + 2 * COORD_SPACE + 1;
        descFieldBound.topLeft.y = COORD_START_Y;
        descFieldBound.extent.x = ( toBoundsP->extent.x - COORD_SCROLLBAR_WIDTH ) - descFieldBound.topLeft.x;
        descFieldBound.extent.y = ( toBoundsP->extent.y + 2 - COORD_START_Y - COORD_TOOLBAR_HEIGHT);
        FrmSetObjectBounds( frmP, descFieldIndex, &descFieldBound );

        descScrollBarBound.topLeft.x = descFieldBound.topLeft.x + descFieldBound.extent.x;
        descScrollBarBound.topLeft.y = descFieldBound.topLeft.y;
        descScrollBarBound.extent.x = COORD_SCROLLBAR_WIDTH;
        descScrollBarBound.extent.y = descFieldBound.extent.y;
        FrmSetObjectBounds ( frmP, descScrollBarIndex, &descScrollBarBound );

        // Set the label of the up and down button.
        ctlP = GetObjectPtr ( MainWordListUpRepeatButton );
        CtlGlueSetFont ( ctlP, symbol7Font );
        FrmCopyLabel ( frmP, MainWordListUpRepeatButton, "\1" );

        ctlP = GetObjectPtr ( MainWordListDownRepeatButton );
        CtlGlueSetFont ( ctlP, symbol7Font );
        FrmCopyLabel ( frmP, MainWordListDownRepeatButton, "\2" );

        ShowObject( frmP, MainWordListUpRepeatButton );
        ShowObject( frmP, MainWordListDownRepeatButton );
        ShowObject( frmP, MainWordList );
        ShowObject( frmP, MainDescriptionField );
        ShowObject( frmP, MainDescScrollBar );
    }

    fldP = ( FieldType* ) FrmGetObjectPtr ( frmP, descFieldIndex );
    FldRecalculateField ( fldP, false );

    return errNone;
}

/*
 * FUNCTION: MainFormPutWordFieldToHistory
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */

static Err FormHistorySeekBack( UInt16 formId )
{
    UInt16	oldIdx, i;
    AppGlobalType	*global;

    global = AppGetGlobal();

    // check history list to adjust current history index.
    i = 0;
    while ( i <= global->historySeekIdx
            && global->prefs.history[ i ][ 0 ] != chrNull )
    {
        i++;
    }

    if ( i < global->historySeekIdx
            || global->prefs.history[ global->historySeekIdx ][ 0 ] == chrNull )
    {
        global->historySeekIdx = 0;
    }

    // seek next.
    oldIdx = global->historySeekIdx;

    // loop back buffer.
    global->historySeekIdx++;
    if ( global->historySeekIdx >= MAX_HIS_FAR )
        global->historySeekIdx = 0;

    if ( global->prefs.history[ global->historySeekIdx ][ 0 ] == chrNull )
        global->historySeekIdx = 0;

    if ( global->prefs.history[ global->historySeekIdx ][ 0 ] == chrNull
            || oldIdx == global->historySeekIdx )
    {
        // if history is empty or only one item in history then we do nothing.
        SndPlaySystemSound( sndWarning );
        return ~errNone;
    }

    if(formId == MainForm)
	{
	    // put history word to search, but not put it to history list.
	    ToolsSetFieldPtr ( MainWordField, &global->prefs.history[ global->historySeekIdx ][ 0 ],
	                       StrLen( &global->prefs.history[ global->historySeekIdx ][ 0 ] ), true );
	    MainFormSearch( false, true, global->prefs.enableHighlightWord, true, false, global->prefs.enableAutoSpeech );
	}
	if(formId == DAForm)
	{
	    // put history word to search, but not put it to history list.
	    ToolsSetFieldPtr ( DAWordField, &global->prefs.history[ global->historySeekIdx ][ 0 ],
	                       StrLen( &global->prefs.history[ global->historySeekIdx ][ 0 ] ), true );
        DAFormSearch( false, global->prefs.enableHighlightWord, false, global->prefs.enableAutoSpeech );
	}
    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:	MainFormSearch
 *
 * DESCRIPTION: This routine search the word that in word field.
 *
 * PARAMETERS:
 *				->	putinHistory If be true then put the word to history list.
 *				->	updateWordList If be true then update word list.
 *				->	highlightWordField If be true then highlight the word field.
 *				->	updateDescField If be true then update descript field.
 *				->	bAppend If be true then append else replace.
 *
 * RETURN:
 *				errNone if success.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static Err MainFormSearch( Boolean putinHistory, Boolean updateWordList,
                           Boolean highlightWordField, Boolean updateDescField,
                           Boolean bEnableBreak, Boolean bEnableAutoSpeech )
{
    UInt16	matchs;
    UInt8	*explainPtr;
    UInt32	explainLen;
    AppGlobalType	*global;
    ZDicDBDictInfoType	*dictInfo;
    Err	err;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;

    if ( bEnableBreak )
    {
        Int32 ticksPreHalfSecond;
        Int16 delayTimes;

        ticksPreHalfSecond = SysTicksPerSecond() / TIMES_PRE_SECOND;
        delayTimes = global->prefs.incSearchDelay;
        while ( !EvtSysEventAvail ( true )
                && delayTimes > 0 )
        {
            Int16 x, y;
            Boolean penDown;

            EvtGetPen ( &x, &y, &penDown );
            if ( penDown )
                break;

            SysTaskDelay ( ticksPreHalfSecond );
            delayTimes--;
        }

        if ( delayTimes > 0 )
        {
            global->needSearch = true;
            global->putinHistory = putinHistory;
            global->updateWordList = updateWordList;
            global->highlightWordField = highlightWordField;
            global->updateDescField = updateDescField;
            return errNone;
        }
        else
            global->needSearch = false;
    }

    err = ToolsSearch( MainWordField, &matchs, &explainPtr, &explainLen );
    if ( err == errNone && explainPtr != NULL && explainLen != 0 )
    {
        FormType * frmP;
        MemHandle	bufH;
        Char *str;

        // Clear the menu status from the display
        MenuEraseStatus( 0 );

        if ( putinHistory )
        {
            ToolsPutWordFieldToHistory( MainWordField );
        }

        if ( updateDescField )
        {
            bufH = MemHandleNew( explainLen + 1 );
            if ( bufH == NULL )
            {
                FrmAlert ( NoEnoughMemoryAlert );
                return memErrNotEnoughSpace;
            }

            // format string.
            str = MemHandleLock( bufH );
            MemMove( str, explainPtr, explainLen );
            str[ explainLen ] = chrNull;
            ToolsFormatExplain( ( UInt8* ) str );
            ToolsGMXTranslate( str, dictInfo->phonetic[ dictInfo->curMainDictIndex ] );
            MemHandleUnlock( bufH );

            // update display.
            ToolsSetFieldHandle( MainDescriptionField, bufH, true );
            ToolsUpdateScrollBar ( MainDescriptionField, MainDescScrollBar );
        }

        if ( updateWordList )
            MainFormUpdateWordList();

        // select all the text in word field, then use can clear it easy.
        if ( highlightWordField )
            ToolsHighlightField( MainWordField );

        // Display or hide the player button
        frmP = FrmGetActiveForm ();
        if ( ZDicVoiceIsExist ( ( Char * ) explainPtr ) )
        {
            ShowObject ( frmP, MainPlayVoice );
            if ( bEnableAutoSpeech )
                ToolsPlayVoice ();
        }
        else
            HideObject ( frmP, MainPlayVoice );
    }

    return errNone;
}

/*
 * FUNCTION: MainFormChangeWordFieldCase
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */
static Err MainFormChangeWordFieldCase( void )
{
    FieldType	* fieldP;
    Char	*wordStr, newStr[ MAX_WORD_LEN + 1 ];
    Err	err;
    AppGlobalType *global;

    global = AppGetGlobal();

    fieldP = GetObjectPtr( MainWordField );
    wordStr = FldGetTextPtr ( fieldP );
    if ( wordStr == NULL || wordStr[ 0 ] == chrNull )
        return errNone;

    MemSet( newStr, sizeof( newStr ), 0 );
    StrNCopy( newStr, wordStr, MAX_WORD_LEN );

    err = ToolsChangeCase ( newStr );
    if ( err == errNone )
    {
        ToolsSetFieldPtr ( MainWordField, newStr, StrLen( newStr ), true );
        MainFormSearch( false, true, global->prefs.enableHighlightWord, true, false, global->prefs.enableAutoSpeech );
    }

    return errNone;
}

/*
 * FUNCTION: MainFormWordListPageScroll
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:   direction     winUp or winDown
 *
 * RETURNED:    nothing
 *
 */

static void MainFormWordListPageScroll ( WinDirectionType direction, Int16 selectItemNum )
{
    ListType	* lstP;
    AppGlobalType	*global;
    Err	err;

    global = AppGetGlobal();

    if ( direction == winUp )
    {
        err = ZDicLookupWordListInit( winUp, false );

    }
    else
    {
        err = ZDicLookupWordListInit( winDown, false );
    }

    if ( err )
        return ;

    lstP = ( ListType* ) GetObjectPtr( MainWordList );
    
    if(global->prefs.font == global->font.smallTinyFontID ||global->prefs.font == global->font.largeTinyFontID)
    {
		LstSetListChoices ( lstP, &global->wordlisttinyBuf.itemPtr[ 0 ], global->wordlisttinyBuf.itemUsed );
    }
    else if(global->prefs.font == global->font.smallFontID ||global->prefs.font == global->font.largeFontID)
    {
    	LstSetListChoices ( lstP, &global->wordlistBuf.itemPtr[ 0 ], global->wordlistBuf.itemUsed );
    }
    
    
    LstSetSelection ( lstP, selectItemNum );
    LstDrawList ( lstP );

}

/*
 * FUNCTION: MainFormSelectWordList
 *
 * DESCRIPTION: when user select a word from word list, we
 *				should display it in descript field.
 *
 * PARAMETERS: event.
 *
 * RETURNED: true if the field handled the event
 *
 */

static Boolean MainFormSelectWordList( EventType * event )
{
    UInt8	* explainPtr,maxworditem;
    UInt32	explainLen;
    AppGlobalType	*global;
    ZDicDBDictInfoType	*dictInfo;
    Err	err;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;

	if(global->prefs.font == global->font.smallTinyFontID ||global->prefs.font == global->font.largeTinyFontID)
    	maxworditem = MAX_WORD_ITEM_TINY;
    if(global->prefs.font == global->font.smallFontID ||global->prefs.font == global->font.largeFontID)
    	maxworditem = MAX_WORD_ITEM;

    if ( event->data.lstSelect.selection >= 0
            && event->data.lstSelect.selection < maxworditem )
    {
        err = ZDicLookupWordListSelect( event->data.lstSelect.selection, &explainPtr, &explainLen );
        if ( err == errNone )
        {
            FormType * frmP;
            MemHandle	bufH;
            Char *str;
            Int16	len;

            bufH = MemHandleNew( explainLen + 1 );
            if ( bufH == NULL )
                return memErrNotEnoughSpace;

            // format string.
            str = MemHandleLock( bufH );
            MemMove( str, explainPtr, explainLen );
            str[ explainLen ] = chrNull;
            ToolsFormatExplain( ( UInt8* ) str );
            ToolsGMXTranslate( str, dictInfo->phonetic[ dictInfo->curMainDictIndex ] );
            MemHandleUnlock( bufH );

            // update display.
            ToolsSetFieldHandle( MainDescriptionField, bufH, true );
            ToolsUpdateScrollBar ( MainDescriptionField, MainDescScrollBar );

            // update input field
            // get key word.
            len = 0;
            while ( explainPtr[ len ] != chrHorizontalTabulation && explainPtr[ len ] != chrNull && len < MAX_WORD_LEN )
            {
                global->data.readBuf[ len ] = explainPtr[ len ];
                len++;
            }
            global->data.readBuf[ len ] = chrNull;
            ToolsSetFieldPtr( MainWordField, ( char * ) global->data.readBuf, len, true );

            if ( global->prefs.enableHighlightWord )
                ToolsHighlightField( MainWordField );

            // Display or hide the player button
            frmP = FrmGetActiveForm ();
            if ( ZDicVoiceIsExist ( ( Char * ) explainPtr ) )
            {
                ShowObject ( frmP, MainPlayVoice );
                if ( global->prefs.enableAutoSpeech )
                    ToolsPlayVoice ();
            }
            else
                HideObject ( frmP, MainPlayVoice );
        }
    }

    return true;
}

/*
 * FUNCTION: MainFormUpdateWordList
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */

static Err MainFormUpdateWordList( void )
{
    AppGlobalType	* global;
    ListType	*lstP;
    Err	err;

    global = AppGetGlobal();

    if ( global->wordListIsOn )
    {
        err = ZDicLookupWordListInit( winDown, true );
        if ( err == errNone )
        {
            // if description is changed so word list must changed. else do nothing.
            lstP = ( ListType* ) GetObjectPtr( MainWordList );
            
            if(global->prefs.font == global->font.smallTinyFontID ||global->prefs.font == global->font.largeTinyFontID)
		    {
				LstSetListChoices ( lstP, &global->wordlisttinyBuf.itemPtr[ 0 ], global->wordlisttinyBuf.itemUsed );
		    }
		    else if(global->prefs.font == global->font.smallFontID ||global->prefs.font == global->font.largeFontID)
		    {
		    	LstSetListChoices ( lstP, &global->wordlistBuf.itemPtr[ 0 ], global->wordlistBuf.itemUsed );
		    }
            
            LstSetSelection ( lstP, 0 );
            LstDrawList ( lstP );
        }

    }
    return errNone;
}

static Err FormPopupDictList( UInt16 formId )
{
	AppGlobalType	* global;
	ListType	*lstP;
	UInt16 i,j,selection, count;
    MemHandle	itemsHandle;
    Char	**itemsPtr;
	ZDicDBDictPopupInfoType		*popupInfo;
    ZDicDBDictInfoType 			*dictInfo;
    
    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
    popupInfo = &global->prefs.popupInfo;
	
	ZDicDBInitPopupList();
	
	count = 0;
    while ( global->popuplistItem[ count ] != chrNull )
        count++;

    if ( count == 0 )
        return errNone;
    
    itemsHandle = MemHandleNew( sizeof( Char* ) * count );
    if ( itemsHandle == NULL )
        return memErrNotEnoughSpace;
    
    itemsPtr = MemHandleLock ( itemsHandle );
    for ( i = 0; i < count; i++ )
    {
        *( itemsPtr + i ) = &global->popuplistItem[ i ][ 0 ];
    }
	
	if(formId == MainForm)
		lstP = GetObjectPtr( lstMainPopup );
	else if(formId == DAForm)
		lstP = GetObjectPtr( lstDAPopup );

    LstSetListChoices ( lstP, itemsPtr, count );
    
    if(formId == MainForm)//����list����
	{
		if(count < 13)
	    {
	    	LstSetHeight ( lstP, count );
	    }
	    else
	    {
	    	LstSetHeight ( lstP, 13 );
	    }
	}
	else if(formId == DAForm)
	{
	    if(count > 5 && global->prefs.daSize == 0)
	    {
	    	LstSetHeight ( lstP, 5 );
	    }
	    else if(count >11 && global->prefs.daSize == 1)
	    {
	    	LstSetHeight ( lstP, 11 );
	    }
	    else
	    {
	    	LstSetHeight ( lstP, count );
	    }
    }
    
    if( popupInfo->totalNumber != 0 )
    {
	    for(i =0;i<popupInfo->totalNumber;i++)
	    {
	    	if(popupInfo->dictIndex[i] == dictInfo->curMainDictIndex)
	    	{
	    		j = i;
	    		break;
	    	}
	    	else
	    	{
	    		j = 0;
	    	}
	    }
	}
    LstSetSelection ( lstP, j );
    LstGlueSetFont ( lstP, global->font.smallFontID );
    selection = LstPopupList ( lstP );
    
    if ( selection != noListSelection )
    {
    	if(formId == MainForm)
			ToolsChangeListDictionaryCommand(MainForm,popupInfo->dictIndex[selection] );
		else if(formId == DAForm)
			ToolsChangeListDictionaryCommand(DAForm,popupInfo->dictIndex[selection] );
    	
    }
    return errNone;
}

/*
 * FUNCTION: MainFormPopupHistoryList
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */

static Err MainFormPopupHistoryList( void )
{
    AppGlobalType	* global;
    UInt16	i, count;
    MemHandle	itemsHandle;
    Char	**itemsPtr;
    ListType	*lstP;
    Int16	newSelection;
    
    global = AppGetGlobal();
    
    // get history list item number;
    count = 0;
    while ( global->prefs.history[ count ][ 0 ] != chrNull )
        count++;

    if ( count == 0 )
        return errNone;
    itemsHandle = MemHandleNew( sizeof( Char* ) * count );
    if ( itemsHandle == NULL )
        return memErrNotEnoughSpace;
    itemsPtr = MemHandleLock ( itemsHandle );
    for ( i = 0; i < count; i++ )
    {
        *( itemsPtr + i ) = &global->prefs.history[ i ][ 0 ];
    }

    lstP = GetObjectPtr( MainHistoryPopupList );
    LstSetListChoices ( lstP, itemsPtr, count );
    LstSetHeight ( lstP, count );
    LstSetSelection ( lstP, 0 );
    //LstSetSelection ( lstP, noListSelection );
    LstGlueSetFont ( lstP, global->font.smallFontID );
    newSelection = LstPopupList ( lstP );

    if ( newSelection != noListSelection )
    {
        ToolsSetFieldPtr ( MainWordField, &global->prefs.history[ newSelection ][ 0 ],
                           StrLen( &global->prefs.history[ newSelection ][ 0 ] ), true );
        MainFormSearch( false, true, global->prefs.enableHighlightWord, true, false, global->prefs.enableAutoSpeech );
    }
    MemHandleUnlock( itemsHandle );
    MemHandleFree( itemsHandle );

    return errNone;
}

/*
 * FUNCTION: MainFormChangeDictionary
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 */ 
/*
static Err MainFormChangeDictionary(void)
{
	ZDicDBDictInfoType	*dictInfo;
	AppGlobalType		*global;
	ListType			*lstP;
	Int16				newSelection, i;
 
	global = AppGetGlobal();
	dictInfo = &global->prefs.dictInfo;
	
	lstP = (ListType*)GetObjectPtr(MainDictionaryPopList);
	if (lstP)
	{
		// set the list item pointer.
		i = 0;
		while (i < dictInfo->totalNumber)
		{
			global->listItem[i] = &dictInfo->displayName[i][0];
			i++;
		}
 
		// popup dictionary list.
		LstSetListChoices (lstP, global->listItem, dictInfo->totalNumber);
		LstSetSelection (lstP, dictInfo->curMainDictIndex);
		LstSetHeight (lstP, dictInfo->totalNumber);
		newSelection = LstPopupList (lstP);
		
		if (newSelection != noListSelection)
		{
			ZDicCloseCurrentDict();
			dictInfo->curMainDictIndex = newSelection;
			FrmUpdateForm(MainForm, updateDictionaryChanged);
		}
	}
	
	return errNone;
}
*/ 
/*
 * FUNCTION: MainFormIncrementalSearch
 *
 * DESCRIPTION: Adds a character to word field and looks up the word.
 *
 * PARAMETERS: event - EventType* containing character to add to word field.
 *
 * RETURNED: true if the field handled the event
 *
 */

static Boolean MainFormIncrementalSearch( EventType * event )
{

    FormType	* frmP;
    UInt16	fldIndex;
    FieldPtr	fldP;
    AppGlobalType	*global;

    global = AppGetGlobal();

    frmP = FrmGetActiveForm();
    fldIndex = FrmGetObjectIndex( frmP, MainWordField );
    FrmSetFocus( frmP, fldIndex );
    fldP = FrmGetObjectPtr ( frmP, fldIndex );

    if ( FldHandleEvent ( fldP, event ) || event->eType == fldChangedEvent )
    {
        if ( global->prefs.enableIncSearch )
        {
            MainFormSearch( false, true, false, true, true, global->prefs.enableAutoSpeech );
        }

        return true;
    }

    return false;
}

/***********************************************************************
 *
 * FUNCTION:	MainFormJumpSearch
 *
 * DESCRIPTION: Jump search a word if user select a word.
 *
 * PARAMETERS:
 *				->	event The Event that we received.
 *
 * RETURN:
 *				true if handled else false.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static Boolean MainFormJumpSearch( EventType * event )
{
    FormType	* frmP;
    Char	*buf;
    FieldType	*field;
    AppGlobalType	*global;
    Boolean	handled = false;

    if ( event->data.fldEnter.fieldID != MainDescriptionField )
        return false;

    field = ( FieldType * ) GetObjectPtr( MainDescriptionField );
    if ( FldHandleEvent ( field, event ) )
    {
        global = AppGetGlobal();
        buf = ( Char * ) global->data.readBuf;

        if ( global->prefs.enableJumpSearch )
        {
            // put old word into history and search new word and put it into history.
            ToolsPutWordFieldToHistory( MainWordField );
            if ( ToolsGetFieldHighlightText( field, buf, MAX_WORD_LEN, global->prefs.enableSingleTap ) == errNone )
            {
                ToolsSetFieldPtr( MainWordField, buf, StrLen( buf ), true );
                MainFormSearch( true, true, global->prefs.enableHighlightWord, true, false, global->prefs.enableAutoSpeech );

                // Set force to input field.
                frmP = FrmGetActiveForm ();
                FrmSetFocus( frmP, FrmGetObjectIndex( frmP, MainWordField ) );
            }
        }
        handled = true;
    }

    return handled;
}

/***********************************************************************
 *
 * FUNCTION:	MainFormUpdateDisplay
 *
 * DESCRIPTION: This routine redraw the MainForm form.
 *
 * PARAMETERS:
 *				->	updateCode Update code.
 *
 * RETURN:
 *				nothing.
 *
 * REVISION HISTORY:
 *		Name			Date		Description
 *		----			----		-----------
 *		ZhongYuanHuan	12/Aug/04	Initial Revision
 *
 ***********************************************************************/

static void MainFormUpdateDisplay( UInt16 updateCode )
{
    FormType * frmP;
    ZDicDBDictInfoType	*dictInfo;
    AppGlobalType	*global;
    RectangleType formBounds;
    Err	err = errNone;

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;
	frmP = FrmGetActiveForm ();
	
    if ( updateCode & updateDictionaryChanged )
    {
        // Open new dictionary database.
        err = ZDicOpenCurrentDict();
        if ( err != errNone )
        {
            ToolsQuitApp();
            return ;
        }

        // Update dictionary name and research current word.
        StrCopy( global->dictTriggerName, &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
        ToolsFontTruncateName( global->dictTriggerName, stdFont, ResLoadConstant( DictionaryNameMaxLenInPixels ) );
        
        FrmCopyTitle ( frmP, global->dictTriggerName );
        MainFormSearch( false, true, global->prefs.enableHighlightWord, true, false, global->prefs.enableAutoSpeech );
    }

    if ( updateCode & updateFontChanged )
    {
        FldSetFont( GetObjectPtr( MainDescriptionField ), global->prefs.font );
        
       	if(	global->prefs.font == global->font.smallTinyFontID || global->prefs.font == global->font.largeTinyFontID)
	    {
	    	LstGlueSetFont (GetObjectPtr(MainWordList), global->font.largeTinyFontID);
	    	HideObject (frmP,MainWordList);
	    	LstSetHeight ( GetObjectPtr (MainWordList), 14);
	    	ShowObject (frmP,MainWordList);
	    }
	    else if(global->prefs.font == global->font.smallFontID || global->prefs.font == global->font.largeFontID)
	    {
	    	LstGlueSetFont (GetObjectPtr(MainWordList), global->font.smallFontID);
	    	HideObject (frmP,MainWordList);
	    	LstSetHeight  ( GetObjectPtr (MainWordList), 10);
	    	ShowObject (frmP,MainWordList);
        }
        
		// get the current bounds for the form
	    ZDicToolsWinGetBounds ( FrmGetWindowHandle ( FrmGetActiveForm() ), &formBounds );
	    MainFormAdjustObject ( &formBounds );
	    
        ToolsUpdateScrollBar ( MainDescriptionField, MainDescScrollBar );
    }

    if ( updateCode & frmRedrawUpdateCode )
    {
        FrmDrawForm( FrmGetActiveForm() );
    }

    return ;
}

/*
 * FUNCTION: MainFormWordListUseAble
 *
 * DESCRIPTION: This routine display wordlist or hide it.
 *
 * PARAMETERS:
 *
 * frm
 *     pointer to the MainForm form.
 */

static void MainFormWordListUseAble( Boolean turnOver, Boolean redraw )
{
    FormType	* frmP;
    RectangleType formBounds;
    AppGlobalType	*global;

    global = AppGetGlobal();

    frmP = FrmGetActiveForm();

    // get the current bounds for the form
    ZDicToolsWinGetBounds ( FrmGetWindowHandle ( frmP ), &formBounds );

    if ( turnOver )
    {
        global->wordListIsOn = !global->wordListIsOn;
    }

    MainFormAdjustObject ( &formBounds );

    if ( redraw )
    {
        MemHandle	textH;
        FieldType	*fldP;

        if ( global->wordListIsOn )
        {
            ShowObject ( frmP, MainWordList );
        }

        fldP = GetObjectPtr ( MainDescriptionField );
        textH = FldGetTextHandle( fldP );
        FldSetTextHandle ( fldP, NULL );
        FldSetTextHandle ( fldP, textH );
        FldDrawField( fldP );
        MainFormUpdateWordList();
        ToolsUpdateScrollBar ( MainDescriptionField, MainDescScrollBar );
    }

    return ;
}

/*
 * FUNCTION: MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:
 *
 * frm
 *     pointer to the MainForm form.
 */

static void MainFormInit( FormType *frmP )
{
    ZDicDBDictInfoType	* dictInfo;
    AppGlobalType	*global;
    FieldType	*field;
    //ListType	*lst;

    // Set the draw and acitve windows to fixe bugs of chinese os.
    WinSetDrawWindow ( FrmGetWindowHandle ( frmP ) );
    WinSetActiveWindow ( FrmGetWindowHandle ( frmP ) );

    global = AppGetGlobal();
    dictInfo = &global->prefs.dictInfo;

    global->historySeekIdx = 0;

    // initial word list draw function.
   	if(	global->prefs.font == global->font.smallTinyFontID || global->prefs.font == global->font.largeTinyFontID)
    {
    	LstGlueSetFont (GetObjectPtr(MainWordList), global->font.largeTinyFontID);
    	LstSetHeight ( GetObjectPtr (MainWordList), 14);
    }
    else if(global->prefs.font == global->font.smallFontID || global->prefs.font == global->font.largeFontID)
    {
    	LstGlueSetFont (GetObjectPtr(MainWordList), global->font.smallFontID);
    	LstSetHeight  ( GetObjectPtr (MainWordList), 10);
    }

    // initial dictionary name
    StrCopy( global->dictTriggerName, &dictInfo->displayName[ dictInfo->curMainDictIndex ][ 0 ] );
    ToolsFontTruncateName( global->dictTriggerName, stdFont, ResLoadConstant( DictionaryNameMaxLenInPixels ) );
    FrmCopyTitle ( frmP, global->dictTriggerName );

    // initial description and word field font.
    field = GetObjectPtr( MainDescriptionField );
    FldSetFont( field, global->prefs.font );
    field = GetObjectPtr( MainWordField );
    FldSetFont( field, global->font.smallFontID );

    // Initial word for word field.
    if ( global->initKeyWord[ 0 ] != chrNull )
    {
        ToolsSetFieldPtr( MainWordField, &global->initKeyWord[ 0 ], StrLen( &global->initKeyWord[ 0 ] ), false );
    }
    else
    {
        ToolsSetFieldPtr( MainWordField, &global->initKeyWord[ 0 ], 2, false );
    }
    FrmSetFocus( frmP, FrmGetObjectIndex( frmP, MainWordField ) );

    // Initial word list
    global->wordListIsOn = global->prefs.enableWordList;
    MainFormWordListUseAble( false, false );

    // Set enable jump search group selection.
    FrmSetControlGroupSelection ( frmP, 1,
                                  global->prefs.enableJumpSearch ?
                                  MainJumpPushButton : MainSelectPushButton );

    return ;
}

/*
 * FUNCTION: MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:
 *
 * command
 *     menu item id
 */

static Boolean MainFormDoCommand( UInt16 command )
{
    Boolean handled = false;
    AppGlobalType *	global;

    global = AppGetGlobal();

    if ( command >= ZDIC_DICT_MENUID )
    {
        handled = ToolsChangeDictionaryCommand( MainForm, command );
        return handled;
    }

    switch ( command )
    {
    case OptionsAboutZDic:
        {
            FormType * frmP;

            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            // Display the About Box.
            frmP = FrmInitForm ( AboutForm );
            FrmDoDialog ( frmP );
            FrmDeleteForm ( frmP );

            handled = true;
            break;
        }
    case OptionsDictOptions:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            // Display the preference from the display
            PrefFormPopupPrefsDialog();
            handled = true;
            break;
        }
    case OptionsPreferences:
        {
        	//FormType *frmP;
        	
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            // Display the details from the display
            DetailsFormPopupDetailsDialog();
			
			/*
	        frmP = FrmGetActiveForm();
		    // Set enable jump search group selection.
		    if(global->prefs.enableJumpSearch){
				HideObject( frmP, MainSelectPushButton);
				ShowObject( frmP, MainJumpPushButton);
		    }else{
		    	HideObject( frmP, MainJumpPushButton);
		    	ShowObject( frmP, MainSelectPushButton);
		    }
		    */
            handled = true;
            break;
        }
    case OptionsExportOptions:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            // Display exorpt options from.
            ExportPopupDialog();
            handled = true;
            break;
        }
    case OptionsGotoWord:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            MainFormSearch( true, true, global->prefs.enableHighlightWord, true, false, global->prefs.enableAutoSpeech );
            handled = true;
            break;
        }
    case OptionsClearWord:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            handled = ToolsClearInput ( MainWordField );
            break;
        }
    case OptionsClearHistory:
        {
            global->prefs.history[ 0 ][ 0 ] = chrNull;
            global->historySeekIdx = 0;
            handled = true;
            break;
        }
    case OptionsChangeCase:
        {
            // Clear the menu status from the display
            MenuEraseStatus( 0 );

            MainFormChangeWordFieldCase();
            handled = true;
            break;
        }
    case OptionsList:
        {
            AppGlobalType	*global;

            // Clear the menu status from the display
            MenuEraseStatus( 0 );


            global = AppGetGlobal();

            if ( !global->prefs.enableWordList && global->wordListIsOn )
                global->prefs.enableWordList = false;
            else
                global->prefs.enableWordList = !global->prefs.enableWordList;

            global->wordListIsOn = global->prefs.enableWordList;
            MainFormWordListUseAble( false, true );
            handled = true;
            break;
        }
    case OptionsExportMemo:
        {
            ExportToMemo ( MainDescriptionField );
            handled = true;
            break;
        }
    case OptionsExportSugarMemo:
        {
            ExportToSugarMemo( MainDescriptionField );
            handled = true;
            break;
        }
    case DictionarysDictAll:
        {
            ToolsAllDictionaryCommand( MainWordField, MainDescriptionField, MainDescScrollBar );
            handled = true;
            break;
        }
    }

    return handled;
}

/*
 * FUNCTION: MainFormHandleEvent
 *
 * DESCRIPTION:
 *
 * This routine is the event handler for the "MainForm" of this 
 * application.
 *
 * PARAMETERS:
 *
 * eventP
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed to
 *     FrmHandleEvent
 */

static Boolean MainFormHandleEvent( EventType * eventP )
{
    Boolean	handled = false;
    FormType	*frmP;
    AppGlobalType	*global;


    global = AppGetGlobal();


    switch ( eventP->eType )
    {
    case menuEvent:
        return MainFormDoCommand( eventP->data.menu.itemID );

    case menuOpenEvent:
        {
            ToolsCreatDictionarysMenu ();
            // don't set handled = true
            break;
        }

    case frmOpenEvent:

        // Do not draw anything before FrmDrawForm because
        // FrmDrawForm will save current screen.
        frmP = FrmGetActiveForm();
        ZDicDIAFormLoadInitial ( global, frmP );
        MainFormInit( frmP );
        ZDicDIADisplayChange ( global );
        FrmDrawForm ( frmP );

        MainFormSearch( true, true, global->prefs.enableHighlightWord, true, false, global->prefs.enableAutoSpeech );

        handled = true;
        break;

    case frmUpdateEvent:
        /*
         * To do any custom drawing here, first call
         * FrmDrawForm(), then do your drawing, and
         * then set handled to true. 
         */
        MainFormUpdateDisplay( eventP->data.frmUpdate.updateCode );
        handled = true;
        break;

    case frmCloseEvent:
        ToolsSetFieldHandle( MainDescriptionField, NULL, false );
        ToolsSetFieldHandle( MainWordField, NULL, false );
        ZDicDIAFormClose ( global );
        break;

    case winDisplayChangedEvent:
        handled = ZDicDIADisplayChange ( global );
        if ( handled )
        {
            frmP = FrmGetActiveForm ();
            FrmDrawForm ( frmP );
        }
        break;

    case winEnterEvent:
        ZDicDIAWinEnter ( global, eventP );
        handled = true;
        break;

    case ctlSelectEvent:
        {
            switch(eventP->data.ctlSelect.controlID)
            {
            	case MainNewButton:
            	{
            		handled = ToolsClearInput ( MainWordField );
            		break;
            	}
				case MainGotoButton:
          		{
	                ToolsAllDictionaryCommand( MainWordField, MainDescriptionField, MainDescScrollBar );
	                handled = true;
	                break;
            	}
				case MainExitButton:
	            {
	                // get history list for seek back.
	                ToolsQuitApp();
	                handled = true;
	                break;
	            }
				case MainHistoryTrigger:
            	{
	                MainFormPopupHistoryList();
	                handled = true;
	                break;
	            }
				case MainWordListDownRepeatButton:
	            {
	                MainFormWordListPageScroll( winDown, 0 );
	                handled = true;
		            break;
	            }
				case MainWordListUpRepeatButton:
	            {
	                MainFormWordListPageScroll( winUp, 0 );
	                handled = true;
		            break;
	            }
				case MainWordButton:
	            {
	                if ( !global->prefs.enableWordList && global->wordListIsOn )
	                    global->prefs.enableWordList = false;
	                else
	                    global->prefs.enableWordList = !global->prefs.enableWordList;

	                global->wordListIsOn = global->prefs.enableWordList;
	                MainFormWordListUseAble( false, true );
	                handled = true;
	                break;
            	}
				case MainSelectPushButton:
				case MainJumpPushButton:
	            {
	                global->prefs.enableJumpSearch =
	                    eventP->data.ctlSelect.controlID == MainSelectPushButton ? false : true;
	                handled = true;
	                break;
	            }
				case MainPlayVoice:
		        {
		            ToolsPlayVoice ();
		            handled = true;
		            break;
		        }
				case MainExportToMemo:
	            {
	                ToolsSendMenuCmd( global->prefs.exportAppCreatorID == sysFileCMemo ? chrCapital_M : chrCapital_Z );
	                handled = true;
	                break;
	            }
	        break;
	        }
        }

    case ctlRepeatEvent:
        {
            switch ( eventP->data.ctlRepeat.controlID )
            {
	            case MainWordListUpRepeatButton:
	                MainFormWordListPageScroll( winUp, 0 );
	                break;

	            case MainWordListDownRepeatButton:
	                MainFormWordListPageScroll( winDown, 0 );
	                break;

	            case MainBackHistoryRepeatButton:
	                FormHistorySeekBack (MainForm);
	                break;
            }
        }

    case lstSelectEvent:
        if ( eventP->data.ctlSelect.controlID == MainWordList )
        {
            handled = MainFormSelectWordList( eventP );
        }

        break;

    case keyDownEvent:
        {
            if ( eventP->data.keyDown.chr == chrLineFeed )
            {
                MainFormSearch( true, true, global->prefs.enableHighlightWord, true, false, global->prefs.enableAutoSpeech );
                handled = true;
            }
            else if ( EvtKeydownIsVirtual( eventP ) )
            {
                if ( NavDirectionPressed( eventP, Up ) || ( eventP->data.keyDown.chr ) == vchrRockerUp || ( eventP->data.keyDown.chr ) == vchrPageUp  || ( eventP->data.keyDown.chr ) == vchrThumbWheelUp/* || (eventP->data.keyDown.chr) == vchrJogUp*/)
                {
                	Char s = ToolsGetOptKeyStatus();
                	if(	(global->prefs.OptUD && hasOptionPressed(eventP->data.keyDown.modifiers)) ||
                		(global->prefs.OptUD && s > 0))
					{
						if(global->prefs.UD)
						{
							if(global->prefs.SwitchUDLR)
							{
								ToolsScrollWord ( winUp, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
								MainFormUpdateWordList( );
							}
							else
							{
								ToolsPrevDictionaryCommand();
							}
						}
						else
						{
							ToolsPageScroll ( winUp, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
						}
						if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else if((!global->prefs.OptUD) && (!hasOptionPressed(eventP->data.keyDown.modifiers)))
					{
                    	if(global->prefs.UD)
						{
							if(global->prefs.SwitchUDLR)
							{
								ToolsScrollWord ( winUp, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
								MainFormUpdateWordList( );
							}
							else
							{
								ToolsPrevDictionaryCommand();
							}
						}
						else
						{
							ToolsPageScroll ( winUp, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
						}
					}
					else
					{
						ToolsPageScroll ( winUp, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
					}
                    handled = true;
                }
                else if ( NavDirectionPressed( eventP, Down )  || ( eventP->data.keyDown.chr ) == vchrRockerDown || ( eventP->data.keyDown.chr ) == vchrPageDown || ( eventP->data.keyDown.chr ) == vchrThumbWheelDown/* || (eventP->data.keyDown.chr) == vchrJogDown*/)
                {
                	Char s = ToolsGetOptKeyStatus();
                	if(	(global->prefs.OptUD && hasOptionPressed(eventP->data.keyDown.modifiers)) ||
                		(global->prefs.OptUD && s > 0))
					{
						if(global->prefs.UD)
						{
							if(global->prefs.SwitchUDLR)
							{
								ToolsScrollWord ( winDown, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
								MainFormUpdateWordList( );
		                	}
							else
							{
								ToolsNextDictionaryCommand();
							}
						}
						else
						{
							ToolsPageScroll ( winDown, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
						}
						if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else if((!global->prefs.OptUD) && (!hasOptionPressed(eventP->data.keyDown.modifiers)))
					{
                    	if(global->prefs.UD)
						{
							if(global->prefs.SwitchUDLR)
							{
								ToolsScrollWord ( winDown, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
		                		MainFormUpdateWordList( );
							}
							else
							{
								ToolsNextDictionaryCommand();
							}
						}
						else
						{
							ToolsPageScroll ( winDown, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
						}
					}
					else
					{
						ToolsPageScroll ( winDown, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
					}
                    handled = true;
                }
                else if ( NavDirectionPressed( eventP, Left ) && global->prefs.LR )
                {
                	Char s = ToolsGetOptKeyStatus();
                	if(	(global->prefs.OptLR && hasOptionPressed(eventP->data.keyDown.modifiers)) ||
                		(global->prefs.OptLR && s > 0))
					{
						if(global->prefs.SwitchUDLR)
						{
							ToolsPrevDictionaryCommand();
						}
						else
						{
							ToolsScrollWord ( winUp, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
							MainFormUpdateWordList( );
						}
						if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else if((!global->prefs.OptLR) && (!hasOptionPressed(eventP->data.keyDown.modifiers)))
					{
						if(global->prefs.SwitchUDLR)
						{
							ToolsPrevDictionaryCommand();
						}
						else
						{
							ToolsScrollWord ( winUp, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
							MainFormUpdateWordList( );
						}
					}
                }
                else if ( NavDirectionPressed( eventP, Right ) && global->prefs.LR )
                {
                	Char s = ToolsGetOptKeyStatus();
                	if(	(global->prefs.OptLR && hasOptionPressed(eventP->data.keyDown.modifiers)) ||
                		(global->prefs.OptLR && s > 0))
					{
						if(global->prefs.SwitchUDLR)
						{
							ToolsNextDictionaryCommand();
						}
						else
						{
							ToolsScrollWord ( winDown, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
							MainFormUpdateWordList( );
						}
						if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else if((!global->prefs.OptLR) && (!hasOptionPressed(eventP->data.keyDown.modifiers)))
					{
						if(global->prefs.SwitchUDLR)
						{
							ToolsNextDictionaryCommand();
						}
						else
						{
							ToolsScrollWord ( winDown, MainDescriptionField, MainDescScrollBar, MainWordField, MainPlayVoice, global->prefs.enableHighlightWord );
							MainFormUpdateWordList( );
						}
					}
                }
                else if ( (NavSelectPressed( eventP )
                          || ( eventP->data.keyDown.chr ) == vchrRockerCenter
                          || ( eventP->data.keyDown.chr ) == vchrThumbWheelPush
                          || ( eventP->data.keyDown.chr ) == vchrJogRelease)
                          && (global->prefs.SelectKeyUsed == 0)
                        )
                {
                	Char s = ToolsGetOptKeyStatus();
                	if(hasOptionPressed(eventP->data.keyDown.modifiers) || s > 0)
					{
						switch(global->prefs.OptSelectKeyFunc)
	                	{
	                		case 0:
	                		{
	                			ToolsPlayVoice();
	                			break;
	                		}
	                		case 1:
	                		{
	                			if ( !global->prefs.enableWordList && global->wordListIsOn )
				                {
				                	global->prefs.enableWordList = false;
				                }
				            	else
				            	{
				                	global->prefs.enableWordList = !global->prefs.enableWordList;
								}
				            	global->wordListIsOn = global->prefs.enableWordList;
				            	MainFormWordListUseAble( false, true );
	                			break;
	                		}
	                		case 2:
	                		{
	                			MainFormPopupHistoryList();
	                			break;
	                		}
	                		case 3:
	                		{
	                			ToolsNextDictionaryCommand();
	                			break;
	                		}
	                		case 4:
	                		{
	                			ToolsClearInput ( MainWordField );
	                			break;
	                		}
	                		case 5:
	                		{
	                			FormHistorySeekBack (MainForm);
	                			break;
	                		}
	                		case 6:
	                		{
	                			ToolsSendMenuCmd( global->prefs.exportAppCreatorID == sysFileCMemo ? chrCapital_M : chrCapital_Z );
	                			break;
	                		}
	                		case 7:
	                		{
	                			ShortcutPopEventHandler();
	                			break;
	                		}
	                		case 8:
	                		{
	                			FormPopupDictList(MainForm);
	                			break;
	                		}
	                		case 9:
	                		{
	                			ToolsAllDictionaryCommand( MainWordField, MainDescriptionField, MainDescScrollBar );
                				handled = true;
	                			break;
	                		}
	                    }
	                    if(s < 2) ToolsSetOptKeyStatus(0);
					}
					else
					{
						switch(global->prefs.SelectKeyFunc)
	                	{
	                		case 0:
	                		{
	                			ToolsPlayVoice();
	                			break;
	                		}
	                		case 1:
	                		{
	                			if ( !global->prefs.enableWordList && global->wordListIsOn )
				                {
				                	global->prefs.enableWordList = false;
				                }
				            	else
				            	{
				                	global->prefs.enableWordList = !global->prefs.enableWordList;
								}
				            	global->wordListIsOn = global->prefs.enableWordList;
				            	MainFormWordListUseAble( false, true );
	                			break;
	                		}
	                		case 2:
	                		{
	                			MainFormPopupHistoryList();
	                			break;
	                		}
	                		case 3:
	                		{
	                			ToolsNextDictionaryCommand();
	                			break;
	                		}
	                		case 4:
	                		{
	                			ToolsClearInput ( MainWordField );
	                			break;
	                		}
	                		case 5:
	                		{
	                			FormHistorySeekBack (MainForm);
	                			break;
	                		}
	                		case 6:
	                		{
	                			ToolsSendMenuCmd( global->prefs.exportAppCreatorID == sysFileCMemo ? chrCapital_M : chrCapital_Z );
	                			break;
	                		}
	                		case 7:
	                		{
	                			ShortcutPopEventHandler();
	                			break;
	                		}
	                		case 8:
	                		{
	                			FormPopupDictList(MainForm);
	                			break;
	                		}
	                		case 9:
	                		{
	                			ToolsAllDictionaryCommand( MainWordField, MainDescriptionField, MainDescScrollBar );
                				handled = true;
	                			break;
	                		}
	                    }
					}
                    handled = true;
                }
                
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyPlaySoundChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyPlaySoundKeycode)
                {
                	ToolsPlayVoice();
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyExportChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyExportKeycode)
                {
                	ToolsSendMenuCmd( global->prefs.exportAppCreatorID == sysFileCMemo ? chrCapital_M : chrCapital_Z );
	                handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyWordListChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyWordListKeycode)
                {
                	if ( !global->prefs.enableWordList && global->wordListIsOn )
                    {
                    	global->prefs.enableWordList = false;
                    }
                	else
                	{
                    	global->prefs.enableWordList = !global->prefs.enableWordList;
					}
                	global->wordListIsOn = global->prefs.enableWordList;
                	MainFormWordListUseAble( false, true );
                	
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyHistoryChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyHistoryKeycode)
                {
					MainFormPopupHistoryList();
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyOneKeyChgDicChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyOneKeyChgDicKeycode)
                {
                	ToolsNextDictionaryCommand();
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyClearFieldChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyClearFieldKeycode)
                {
                	handled = ToolsClearInput ( MainWordField );
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyShortcutChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyShortcutKeycode)
                {
					ShortcutPopEventHandler();
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyPopupChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyPopupKeycode)
                {
                	FormPopupDictList(MainForm);
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keyGobackChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keyGobackKeycode)
                {
                	FormHistorySeekBack (MainForm);
                	handled = true;
                }
                else if( eventP->data.keyDown.chr == (WChar)global->prefs.keySearchAllChr &&
                		 eventP->data.keyDown.keyCode == (WChar)global->prefs.keySearchAllKeycode)
                {
                	ToolsAllDictionaryCommand( MainWordField, MainDescriptionField, MainDescScrollBar );
                	handled = true;
                }
            }
            else
            {
                handled = MainFormIncrementalSearch( eventP );
            }

            break;
        }

    case fldChangedEvent:
        {
            if ( eventP->data.fldChanged.fieldID == MainWordField )
                handled = MainFormIncrementalSearch( eventP );
            else
            	ToolsUpdateScrollBar ( MainDescriptionField, MainDescScrollBar );
            break;
        }

	case fldEnterEvent:
		{
			handled = MainFormJumpSearch( eventP );
 			break;
  		}

    case sclRepeatEvent:
        {
            ToolsScroll ( eventP->data.sclRepeat.newValue - eventP->data.sclRepeat.value, false, MainDescriptionField, MainDescScrollBar );
            break;
        }

        // If there have not any event then we continue the broken search.
        if ( global->needSearch && !EvtSysEventAvail ( true ) )
        {
            MainFormSearch( global->putinHistory, global->updateWordList,
                            global->highlightWordField, global->updateDescField, false, global->prefs.enableAutoSpeech );
        }

    }

    return handled;
}

///////////////////////////////////////////////////////////////////////////
