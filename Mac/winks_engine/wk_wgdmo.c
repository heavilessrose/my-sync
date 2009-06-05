
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_uifm.h"
#include "wk_wdt.h"

int Winks_UI_InitCCShow()
{
    return 0;
}

int Winks_UI_DeInitCCShow()
{
    return 0;
}

static int wk_strlen_ucs2_type(const char * text)
{
    int nCount = 0;
    //int nLength = 0;

    if (text == NULL)
    {
        return (0);
    }
    else
    {


        /*----------------------------------------------------------------*/
        /* Code Body                                                      */
        /*----------------------------------------------------------------*/
        /* Check for NULL character only at the odd no. of bytes 
           assuming forst byte start from zero */
        if (text)
        {
            while (text[nCount] != 0 || text[nCount + 1] != 0)
            {
                //++nLength;
                nCount += 2;
            }
        }
        return nCount;
    }
}

static int wk_drawrect( int x, int y, int width, int high, int color )
{
    
    winks_ui_fillRect( x, y, x + width, y + high, color );
    

    return 0;
}


char pret[64];

static const char* wk_getchar( int x, int y )
{
#define DMOMAXXNUM   4
#define DMOMAXYNUM   5
    char pascii[32];

    Winks_mem_set( pret, 0, 64 );

    if( x < 0 )
        sprintf( pascii, "Yes" );
    else if( y < 0 )
        sprintf( pascii, "No" );
    else
        sprintf( pascii, "%d %d", x, y );

    Winks_Ascii2UnicodeString( pascii, pret, 64 );

    return (const char* )pret;
}
/*WK_WG_EDITBOX	        1   //编辑框	*/       
int wk_wg_Editbox_Draw(Winks_Widget_s* pEditbox)
{
    Winks_Widget_s *pWidget = pEditbox;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKBLUE );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_Editbox_PenHandler( Winks_Widget_s* pEditbox, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_Editbox_KeyHandler( Winks_Widget_s* pEditbox, int KeyCode, int KeyType )
{
    return WK_UIFM_FOCUSCHANGE;
}

int wk_wg_Editbox_StateModify( Winks_Widget_s* pEditbox, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pEditbox;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKBLUE );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}


/*WK_WG_BUTTON	        2   //按钮	*/   

int wk_wg_Button_Draw(Winks_Widget_s* pButton)
{
    Winks_Widget_s *pWidget = pButton;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKGREEN );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_Button_PenHandler( Winks_Widget_s* pButton, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_Button_KeyHandler( Winks_Widget_s* pButton, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_Button_StateModify( Winks_Widget_s* pButton, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pButton;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKGREEN );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

/*WK_WG_LISTITEM	        3   //列表项	*/     

//int wk_wg_Draw_ListItem(Winks_Widget_Listitem_s * pListItem);

/*WK_WG_LISTGROUP	        4   //列表组	*/     

int wk_wg_ListGroup_Draw(Winks_Widget_s* pListGroup)
{
    Winks_Widget_s *pWidget = pListGroup;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKCYAN );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_ListGroup_PenHandler( Winks_Widget_s* pListGroup, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_ListGroup_KeyHandler( Winks_Widget_s* pListGroup, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_ListGroup_StateModify( Winks_Widget_s* pListGroup, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pListGroup;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKCYAN );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

/*WK_WG_TAPTITLEITEM	    5   //分页标题栏单项	*/

//int wk_wg_Draw_TapTitleItem(Winks_Widget_TapListItem_s * pTapTitleItem);

/*WK_WG_TAPTITLE	        6   //分页标题栏	*/ 

int wk_wg_TapTitle_Draw(Winks_Widget_s* pTapTitle)
{
    Winks_Widget_s *pWidget = pTapTitle;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKRED );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_TapTitle_PenHandler( Winks_Widget_s* pTapTitle, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_TapTitle_KeyHandler( Winks_Widget_s* pTapTitle, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_TapTitle_StateModify( Winks_Widget_s* pTapTitle, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pTapTitle;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKRED );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

/*WK_WG_NORMALTITLE	    7   //普通标题栏	*/   

int wk_wg_NormalTitle_Draw(Winks_Widget_s* pNormalTitle)
{
    Winks_Widget_s *pWidget = pNormalTitle;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKMAGENTA );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_NormalTitle_PenHandler( Winks_Widget_s* pNormalTitle, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_NormalTitle_KeyHandler( Winks_Widget_s* pNormalTitle, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_NormalTitle_StateModify( Winks_Widget_s* pNormalTitle, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pNormalTitle;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKMAGENTA );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

/*WK_WG_POPUPMENUITEM	    8   //悬浮菜单单项	*/

//int wk_wg_Draw_PopUpItem(Winks_Widget_PopMenuItem_s * pPopUpItem);

/*WK_WG_LEFTSOFTKEY	    9   //左软键	*/       

int wk_wg_LeftSoftKey_Draw(Winks_Widget_s* pLeftSoftKey)
{
    Winks_Widget_s *pWidget = pLeftSoftKey;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKYELLOW );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_LeftSoftKey_PenHandler( Winks_Widget_s* pLeftSoftKey, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_LeftSoftKey_KeyHandler( Winks_Widget_s* pLeftSoftKey, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_LeftSoftKey_StateModify( Winks_Widget_s* pLeftSoftKey, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pLeftSoftKey;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKYELLOW );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

/*WK_WG_RIGHTSOFTKEY	    10  //右软键	*/     

int wk_wg_RightSoftKey_Draw(Winks_Widget_s* p_RightSoftKey)
{
    Winks_Widget_s *pWidget = p_RightSoftKey;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKGRAY );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_RightSoftKey_PenHandler( Winks_Widget_s* p_RightSoftKey, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_RightSoftKey_KeyHandler( Winks_Widget_s* p_RightSoftKey, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_RightSoftKey_StateModify( Winks_Widget_s* p_RightSoftKey, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = p_RightSoftKey;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_DARKGRAY );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

/*WK_WG_CENTERSOFTKEY	    11  //中间软键	*/   
int wk_wg_CenterSoftKey_Draw(Winks_Widget_s* pCenterSoftKey)
{
    Winks_Widget_s *pWidget = pCenterSoftKey;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_LIGHTGRAY );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_CenterSoftKey_PenHandler( Winks_Widget_s* pCenterSoftKey, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_CenterSoftKey_KeyHandler( Winks_Widget_s* pCenterSoftKey, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_CenterSoftKey_StateModify( Winks_Widget_s* pCenterSoftKey, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pCenterSoftKey;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_LIGHTGRAY );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

/*WK_WG_POPUP	            12  //弹出式显示框	*/

int wk_wg_PopUp_Draw(Winks_Widget_s* pPopUp)
{
    Winks_Widget_s *pWidget = pPopUp;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_BLUE );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_PopUp_PenHandler( Winks_Widget_s* pPopUp, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_PopUp_KeyHandler( Winks_Widget_s* pPopUp, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_PopUp_StateModify( Winks_Widget_s* pPopUp, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pPopUp;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_BLUE );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

int wk_wg_PopUp_Change( Winks_Widget_s* pPopUp, int conflag )
{
    return 0;
}

/*WK_WG_POPUP	            12  //弹出式显示框	*/

int wk_wg_PopMenu_Draw(Winks_Widget_s* pPopUp)
{
    Winks_Widget_s *pWidget = pPopUp;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_GREEN );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_PopMenu_PenHandler( Winks_Widget_s* pPopUp, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_PopMenu_KeyHandler( Winks_Widget_s* pPopUp, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_PopMenu_StateModify( Winks_Widget_s* pPopUp, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pPopUp;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_GREEN );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

//int wk_wg_PopUp_Change( Winks_Widget_s* pPopUp, int conflag );

/*WK_WG_STATIC	        14  //静态框*/         
int wk_wg_Static_Draw(Winks_Widget_s* pStatic)
{
    Winks_Widget_s *pWidget = pStatic;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_CYAN );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_Static_PenHandler( Winks_Widget_s* pStatic, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_Static_KeyHandler( Winks_Widget_s* pStatic, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_Static_StateModify( Winks_Widget_s* pStatic, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = pStatic;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_CYAN );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}

/*WK_WG_MATRIXMENUITEM	15  //矩阵菜单单项	*/ 

//int wk_wg_Draw_MatrixMenuItem(Winks_Widget_MatrixMenuItem_s * pMatrixMenuItem);

/*WK_WG_MATRIXMENU	    16  //矩阵菜单列表  */ 

int wk_wg_MatrixMenu_Draw(Winks_Widget_s* MatrixMenu)
{
    Winks_Widget_s *pWidget = MatrixMenu;
    const char* pchar = NULL;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );
    if( pWidget->flag & WK_WG_FGFOCUS )
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
    else
        wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_RED );
    winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
    return 0;
}

int wk_wg_MatrixMenu_PenHandler( Winks_Widget_s* MatrixMenu, int PenType, int x, int y )
{
    return 0;
}

int wk_wg_MatrixMenu_KeyHandler( Winks_Widget_s* MatrixMenu, int KeyCode, int KeyType )
{
    return 0;
}

int wk_wg_MatrixMenu_StateModify( Winks_Widget_s* MatrixMenu, int Option, int ifvar )
{
    const char* pchar = NULL;
    Winks_Widget_s *pWidget = MatrixMenu;

    pchar = wk_getchar( pWidget->xindex, pWidget->yindex );

    if( (Option == WK_WG_FGFOCUS) )
    {
        if( (pWidget->flag & WK_WG_FGFOCUS) && !ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_PIXEL_RED );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag &= ~WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
        else if( (!(pWidget->flag & WK_WG_FGFOCUS)) && ifvar )
        {
            wk_drawrect( pWidget->lux, pWidget->luy, pWidget->width, pWidget->high, WINKS_HILIGHT_COLOR );
            winks_ui_textOutlen( pWidget->lux + 1, pWidget->luy + 1, pchar, wk_strlen_ucs2_type( pchar ) );
            pWidget->flag |= WK_WG_FGFOCUS;

            return WK_UIFM_CONTENTCHANGE;
        }
    }

    return WK_UIFM_SUCCESS;
}
