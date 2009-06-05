
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_uifm.h"
#include "wk_wdt.h"

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_WDGETGLOBALVAR (get_module_glob(WK_MODULE_WIDGET))
#else
Winks_Widget_Global_s Winks_Wd_Global_GB;
#define WINKS_WDGETGLOBALVAR (&Winks_Wd_Global_GB)
#endif

/************************************************************************************\
函数原型：int Winks_Widget_Init( void )
函数介绍：本函数用来初始化控件模块。

参数说明：

函数返回值：
    成功返回零，失败返回-1。

实现描述：
    初始化控件模块。
\************************************************************************************/
int Winks_Widget_Init( void )
{
    Winks_Widget_Global_s* pGlobal = NULL;

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    if( ((pGlobal = get_module_glob( WK_MODULE_WIDGET )) != NULL)
        && pGlobal->ifinit )
    {
        Winks_printf( "Winks Widget init have inited\r\n" );
        return -1;
    }
    if( pGlobal == NULL )
    {
        pGlobal = Winks_salAlloc( sizeof(Winks_Widget_Global_s) );
        if( pGlobal == NULL )
        {
            Winks_printf( "Winks Widget init alloc memory failure\r\n" );
            return -1;
        }
    }
    Winks_mem_set( pGlobal, 0, sizeof(Winks_Widget_Global_s) );
    set_module_glob( WK_MODULE_WIDGET, pGlobal);
#else
    pGlobal = &Winks_Wd_Global_GB;
    if( pGlobal->ifinit )
    {
        Winks_printf( "Winks Widget init have inited\r\n" );
        return -1;
    }
    Winks_mem_set( pGlobal, 0, sizeof(Winks_Widget_Global_s) );
#endif

    pGlobal->FuncItem[WK_WG_EDITBOX].InitFunc = wk_wg_Editbox_Init;
    pGlobal->FuncItem[WK_WG_EDITBOX].DrawFunc = wk_wg_Editbox_Draw;
    pGlobal->FuncItem[WK_WG_EDITBOX].StateModifyFunc = wk_wg_Editbox_StateModify;
    pGlobal->FuncItem[WK_WG_EDITBOX].KeyHandlerFunc = wk_wg_Editbox_KeyHandler;
    pGlobal->FuncItem[WK_WG_EDITBOX].PenHandlerFunc = wk_wg_Editbox_PenHandler;
    pGlobal->FuncItem[WK_WG_EDITBOX].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_BUTTON].InitFunc = wk_wg_Button_Init;
    pGlobal->FuncItem[WK_WG_BUTTON].DrawFunc = wk_wg_Button_Draw;
    pGlobal->FuncItem[WK_WG_BUTTON].StateModifyFunc = wk_wg_Button_StateModify;
    pGlobal->FuncItem[WK_WG_BUTTON].KeyHandlerFunc = wk_wg_Button_KeyHandler;
    pGlobal->FuncItem[WK_WG_BUTTON].PenHandlerFunc = wk_wg_Button_PenHandler;
    pGlobal->FuncItem[WK_WG_BUTTON].ChangeFunc = NULL;
    
    pGlobal->FuncItem[WK_WG_LISTITEM].InitFunc = NULL;
    pGlobal->FuncItem[WK_WG_LISTITEM].DrawFunc = NULL;
    pGlobal->FuncItem[WK_WG_LISTITEM].StateModifyFunc = NULL;
    pGlobal->FuncItem[WK_WG_LISTITEM].KeyHandlerFunc = NULL;
    pGlobal->FuncItem[WK_WG_LISTITEM].PenHandlerFunc = NULL;
    pGlobal->FuncItem[WK_WG_LISTITEM].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_LISTGROUP].InitFunc = wk_wg_ListGroup_Init;
    pGlobal->FuncItem[WK_WG_LISTGROUP].DrawFunc = wk_wg_ListGroup_Draw;
    pGlobal->FuncItem[WK_WG_LISTGROUP].StateModifyFunc = wk_wg_ListGroup_StateModify;
    pGlobal->FuncItem[WK_WG_LISTGROUP].KeyHandlerFunc = wk_wg_ListGroup_KeyHandler;
    pGlobal->FuncItem[WK_WG_LISTGROUP].PenHandlerFunc = wk_wg_ListGroup_PenHandler;
    pGlobal->FuncItem[WK_WG_LISTGROUP].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_TAPTITLEITEM].InitFunc = NULL;
    pGlobal->FuncItem[WK_WG_TAPTITLEITEM].DrawFunc = NULL;
    pGlobal->FuncItem[WK_WG_TAPTITLEITEM].StateModifyFunc = NULL;
    pGlobal->FuncItem[WK_WG_TAPTITLEITEM].KeyHandlerFunc = NULL;
    pGlobal->FuncItem[WK_WG_TAPTITLEITEM].PenHandlerFunc = NULL;
    pGlobal->FuncItem[WK_WG_TAPTITLEITEM].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_TAPTITLE].InitFunc = wk_wg_TapTitle_Init;
    pGlobal->FuncItem[WK_WG_TAPTITLE].DrawFunc = wk_wg_TapTitle_Draw;
    pGlobal->FuncItem[WK_WG_TAPTITLE].StateModifyFunc = wk_wg_TapTitle_StateModify;
    pGlobal->FuncItem[WK_WG_TAPTITLE].KeyHandlerFunc = wk_wg_TapTitle_KeyHandler;
    pGlobal->FuncItem[WK_WG_TAPTITLE].PenHandlerFunc = wk_wg_TapTitle_PenHandler;
    pGlobal->FuncItem[WK_WG_TAPTITLE].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_NORMALTITLE].InitFunc = wk_wg_NormalTitle_Init;
    pGlobal->FuncItem[WK_WG_NORMALTITLE].DrawFunc = wk_wg_NormalTitle_Draw;
    pGlobal->FuncItem[WK_WG_NORMALTITLE].StateModifyFunc = wk_wg_NormalTitle_StateModify;
    pGlobal->FuncItem[WK_WG_NORMALTITLE].KeyHandlerFunc = wk_wg_NormalTitle_KeyHandler;
    pGlobal->FuncItem[WK_WG_NORMALTITLE].PenHandlerFunc = wk_wg_NormalTitle_PenHandler;
    pGlobal->FuncItem[WK_WG_NORMALTITLE].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_POPUPMENUITEM].InitFunc = NULL;
    pGlobal->FuncItem[WK_WG_POPUPMENUITEM].DrawFunc = NULL;
    pGlobal->FuncItem[WK_WG_POPUPMENUITEM].StateModifyFunc = NULL;
    pGlobal->FuncItem[WK_WG_POPUPMENUITEM].KeyHandlerFunc = NULL;
    pGlobal->FuncItem[WK_WG_POPUPMENUITEM].PenHandlerFunc = NULL;
    pGlobal->FuncItem[WK_WG_POPUPMENUITEM].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_POPMENU].InitFunc = wk_wg_PopMenu_Init;
    pGlobal->FuncItem[WK_WG_POPMENU].DrawFunc = wk_wg_PopMenu_Draw;
    pGlobal->FuncItem[WK_WG_POPMENU].StateModifyFunc = wk_wg_PopMenu_StateModify;
    pGlobal->FuncItem[WK_WG_POPMENU].KeyHandlerFunc = wk_wg_PopMenu_KeyHandler;
    pGlobal->FuncItem[WK_WG_POPMENU].PenHandlerFunc = wk_wg_PopMenu_PenHandler;
    pGlobal->FuncItem[WK_WG_POPMENU].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_LEFTSOFTKEY].InitFunc = wk_wg_LeftSoftKey_Init;
    pGlobal->FuncItem[WK_WG_LEFTSOFTKEY].DrawFunc = wk_wg_LeftSoftKey_Draw;
    pGlobal->FuncItem[WK_WG_LEFTSOFTKEY].StateModifyFunc = wk_wg_LeftSoftKey_StateModify;
    pGlobal->FuncItem[WK_WG_LEFTSOFTKEY].KeyHandlerFunc = wk_wg_LeftSoftKey_KeyHandler;
    pGlobal->FuncItem[WK_WG_LEFTSOFTKEY].PenHandlerFunc = wk_wg_LeftSoftKey_PenHandler;
    pGlobal->FuncItem[WK_WG_LEFTSOFTKEY].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_RIGHTSOFTKEY].InitFunc = wk_wg_RightSoftKey_Init;
    pGlobal->FuncItem[WK_WG_RIGHTSOFTKEY].DrawFunc = wk_wg_RightSoftKey_Draw;
    pGlobal->FuncItem[WK_WG_RIGHTSOFTKEY].StateModifyFunc = wk_wg_RightSoftKey_StateModify;
    pGlobal->FuncItem[WK_WG_RIGHTSOFTKEY].KeyHandlerFunc = wk_wg_RightSoftKey_KeyHandler;
    pGlobal->FuncItem[WK_WG_RIGHTSOFTKEY].PenHandlerFunc = wk_wg_RightSoftKey_PenHandler;
    pGlobal->FuncItem[WK_WG_RIGHTSOFTKEY].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_CENTERSOFTKEY].InitFunc = wk_wg_CenterSoftKey_Init;
    pGlobal->FuncItem[WK_WG_CENTERSOFTKEY].DrawFunc = wk_wg_CenterSoftKey_Draw;
    pGlobal->FuncItem[WK_WG_CENTERSOFTKEY].StateModifyFunc = wk_wg_CenterSoftKey_StateModify;
    pGlobal->FuncItem[WK_WG_CENTERSOFTKEY].KeyHandlerFunc = wk_wg_CenterSoftKey_KeyHandler;
    pGlobal->FuncItem[WK_WG_CENTERSOFTKEY].PenHandlerFunc = wk_wg_CenterSoftKey_PenHandler;
    pGlobal->FuncItem[WK_WG_CENTERSOFTKEY].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_POPUP].InitFunc = wk_wg_PopUp_Init;
    pGlobal->FuncItem[WK_WG_POPUP].DrawFunc = wk_wg_PopUp_Draw;
    pGlobal->FuncItem[WK_WG_POPUP].StateModifyFunc = wk_wg_PopUp_StateModify;
    pGlobal->FuncItem[WK_WG_POPUP].KeyHandlerFunc = wk_wg_PopUp_KeyHandler;
    pGlobal->FuncItem[WK_WG_POPUP].PenHandlerFunc = wk_wg_PopUp_PenHandler;
    pGlobal->FuncItem[WK_WG_POPUP].ChangeFunc = wk_wg_PopUp_Change;

    pGlobal->FuncItem[WK_WG_STATIC].InitFunc = wk_wg_Static_Init;
    pGlobal->FuncItem[WK_WG_STATIC].DrawFunc = wk_wg_Static_Draw;
    pGlobal->FuncItem[WK_WG_STATIC].StateModifyFunc = wk_wg_Static_StateModify;
    pGlobal->FuncItem[WK_WG_STATIC].KeyHandlerFunc = wk_wg_Static_KeyHandler;
    pGlobal->FuncItem[WK_WG_STATIC].PenHandlerFunc = wk_wg_Static_PenHandler;
    pGlobal->FuncItem[WK_WG_STATIC].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_MATRIXMENUITEM].InitFunc = NULL;
    pGlobal->FuncItem[WK_WG_MATRIXMENUITEM].DrawFunc = wk_wg_MatrixMenuItem_Draw;
    pGlobal->FuncItem[WK_WG_MATRIXMENUITEM].StateModifyFunc = wk_wg_MatrixMenuItem_StateModify;
    pGlobal->FuncItem[WK_WG_MATRIXMENUITEM].KeyHandlerFunc = NULL;
    pGlobal->FuncItem[WK_WG_MATRIXMENUITEM].PenHandlerFunc = NULL;
    pGlobal->FuncItem[WK_WG_MATRIXMENUITEM].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_MATRIXMENU].InitFunc = wk_wg_MatrixMenu_Init;
    pGlobal->FuncItem[WK_WG_MATRIXMENU].DrawFunc = wk_wg_MatrixMenu_Draw;
    pGlobal->FuncItem[WK_WG_MATRIXMENU].StateModifyFunc = wk_wg_MatrixMenu_StateModify;
    pGlobal->FuncItem[WK_WG_MATRIXMENU].KeyHandlerFunc = wk_wg_MatrixMenu_KeyHandler;
    pGlobal->FuncItem[WK_WG_MATRIXMENU].PenHandlerFunc = wk_wg_MatrixMenu_PenHandler;
    pGlobal->FuncItem[WK_WG_MATRIXMENU].ChangeFunc = NULL;

    pGlobal->FuncItem[WK_WG_SPINBOX].InitFunc = wk_wg_SpinBox_Init;
    pGlobal->FuncItem[WK_WG_SPINBOX].DrawFunc = wk_wg_SpinBox_Draw;
    pGlobal->FuncItem[WK_WG_SPINBOX].StateModifyFunc = wk_wg_SpinBox_StateModify;
    pGlobal->FuncItem[WK_WG_SPINBOX].KeyHandlerFunc = wk_wg_SpinBox_KeyHandler;
    pGlobal->FuncItem[WK_WG_SPINBOX].PenHandlerFunc = wk_wg_SpinBox_PenHandler;
    pGlobal->FuncItem[WK_WG_SPINBOX].ChangeFunc = NULL;

	pGlobal->FuncItem[WK_WG_GLIDEPOLE].InitFunc = wk_wg_GlidePole_Init;
	pGlobal->FuncItem[WK_WG_GLIDEPOLE].DrawFunc = wk_wg_GlidePole_Draw;
	pGlobal->FuncItem[WK_WG_GLIDEPOLE].StateModifyFunc = wk_wg_GlidePole_StateModify;
	pGlobal->FuncItem[WK_WG_GLIDEPOLE].KeyHandlerFunc = wk_wg_GlidePole_KeyHandler;
	pGlobal->FuncItem[WK_WG_GLIDEPOLE].PenHandlerFunc = wk_wg_GlidePole_PenHandler;
	pGlobal->FuncItem[WK_WG_GLIDEPOLE].ChangeFunc = NULL;

	Winks_mem_set(&pGlobal->TapTitle, 0, sizeof(Winks_Widget_TapTitle_s) + (WK_WG_TAPNUM - 1) * sizeof(Winks_Widget_TapListItem_s));

    pGlobal->TapTitle.bwidget.type = WK_WG_TAPTITLE;
    pGlobal->TapTitle.bwidget.flag |= WK_WG_FGGETFOCUS;
    pGlobal->TapTitle.bwidget.lux = 2;
    pGlobal->TapTitle.bwidget.luy = 2;
    pGlobal->TapTitle.bwidget.width = WINKS_SCREEN_WIDTH - 4;
    pGlobal->TapTitle.bwidget.high = (WINKS_SCREEN_HEIGHT * 8 - 160) / 100;
    pGlobal->TapTitle.bwidget.xindex = 0;
    pGlobal->TapTitle.bwidget.yindex = 0;
    pGlobal->TapTitle.v_HighlightImageId = WK_UIFM_IMGID_TAPHL;
    pGlobal->TapTitle.v_TotalItemsNum = WK_WG_TAPNUM;
    pGlobal->TapTitle.v_FocusItemIndex = 0;

	pGlobal->FuncItem[WK_WG_TAPTITLE].InitFunc( (Winks_Widget_s *)&pGlobal->TapTitle );

	pGlobal->TapTitle.bwidget.WidgetID = WK_TAPTITLE_WIGDETID;

    pGlobal->TapTitle.a_ItemListData[0].v_IconID = WK_UIFM_IMGID_TAP00;
    pGlobal->TapTitle.a_ItemListData[0].v_string = WK_UIFM_STRID_TAP00;
    pGlobal->TapTitle.a_ItemListData[0].v_ScreenID = WK_UIFM_SCNID_TAP00;

    pGlobal->TapTitle.a_ItemListData[1].v_IconID = WK_UIFM_IMGID_TAP01;
    pGlobal->TapTitle.a_ItemListData[1].v_string = WK_UIFM_STRID_TAP01;
    pGlobal->TapTitle.a_ItemListData[1].v_ScreenID = WK_UIFM_SCNID_TAP01;

    pGlobal->TapTitle.a_ItemListData[2].v_IconID = WK_UIFM_IMGID_TAP02;
    pGlobal->TapTitle.a_ItemListData[2].v_string = WK_UIFM_STRID_TAP02;
    pGlobal->TapTitle.a_ItemListData[2].v_ScreenID = WK_UIFM_SCNID_TAP02;
    
    pGlobal->TapTitle.a_ItemListData[3].v_IconID = WK_UIFM_IMGID_TAP03;
    pGlobal->TapTitle.a_ItemListData[3].v_string = WK_UIFM_STRID_TAP03;
    pGlobal->TapTitle.a_ItemListData[3].v_ScreenID = WK_UIFM_SCNID_TAP03;

    pGlobal->TapTitle.a_ItemListData[4].v_IconID = WK_UIFM_IMGID_TAP04;
    pGlobal->TapTitle.a_ItemListData[4].v_string = WK_UIFM_STRID_TAP04;
    pGlobal->TapTitle.a_ItemListData[4].v_ScreenID = WK_UIFM_SCNID_TAP04;

    pGlobal->TapTitle.a_ItemListData[5].v_IconID = WK_UIFM_IMGID_TAP05;
    pGlobal->TapTitle.a_ItemListData[5].v_string = WK_UIFM_STRID_TAP05;
    pGlobal->TapTitle.a_ItemListData[5].v_ScreenID = WK_UIFM_SCNID_TAP05;
// 
//     pGlobal->TapTitle.a_ItemListData[6].v_IconID = WK_UIFM_IMGID_TAP06;
//     pGlobal->TapTitle.a_ItemListData[6].v_string = WK_UIFM_STRID_TAP06;
//     pGlobal->TapTitle.a_ItemListData[6].v_ScreenID = WK_UIFM_SCNID_TAP06;
// 
//     pGlobal->TapTitle.a_ItemListData[7].v_IconID = WK_UIFM_IMGID_TAP07;
//     pGlobal->TapTitle.a_ItemListData[7].v_string = WK_UIFM_STRID_TAP07;
//     pGlobal->TapTitle.a_ItemListData[7].v_ScreenID = WK_UIFM_SCNID_TAP07;

    pGlobal->ifinit = 1;
    return 0;
    
}
/************************************************************************************\
函数原型：int Winks_Widget_DeInit( void )
函数介绍：本函数用来去初始化控件模块。

参数说明：

函数返回值：
    成功返回零，失败返回-1。

实现描述：
    去初始化控件模块。
\************************************************************************************/
int Winks_Widget_DeInit( void )
{
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    Winks_salFree( pGlobal );
#else
    Winks_mem_set( pGlobal, 0, sizeof(Winks_Widget_Global_s) );
#endif

    return 0;
}

/************************************************************************************\
函数原型：int Winks_Widget_Draw( Winks_Widget_s* pWidget )
函数介绍：本函数用来绘制一个控件。

参数说明：
    参数名称	参数类型	    参数介绍	    备注
    pWidget	    Winks_Widget_s*	需要绘制的控件	
函数返回值：
    绘制失败返回-1，按成功情况返回不同成功消息，参见函数返回值预定义。

实现描述：
    函数内部首先通过pWidget->type成员判断该控件类型，而后调用相应控件实体绘制函数进行绘制。
\************************************************************************************/
int Winks_Widget_Draw( Winks_Widget_s* pWidget )
{
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;
    int ret = 0;

    if( (pWidget == NULL) || (pWidget->type < 0) || (pWidget->type >= WK_WG_MAXWIDGETTYPENUM) )
    {
        Winks_printf( "Winks Widget Draw param error\r\n" );
        return -1;
    }

    if( pWidget->flag & WK_WG_FGNOTSHOW )
        return 0;
    
    if( pGlobal->FuncItem[pWidget->type].DrawFunc != NULL )
    {
        WINKS_BUFFER_ON;
        ret = pGlobal->FuncItem[pWidget->type].DrawFunc( pWidget );
        WINKS_BUFFER_OFF;
        return ret;
    }

    Winks_printf( "Winks Widget Draw no draw func\r\n" );
    
    return -1;
}
/************************************************************************************\
函数原型：int Winks_Widget_StateModify( Winks_Widget_s* pWidget, int Option, int ifvar )
函数介绍：本函数用来控制一个控件的状态变化的绘制。可能的状态变化如下表：
		
    状态值(Option)	含义（ifvar）
    WK_WG_FGGRAY	真表示本控件被置灰，
                    假表示本控件未被置灰。
    WK_WG_FGFOCUS	真表示本控件获得焦点，
                    假表示本控件失去焦点。

参数说明：
    参数名称	参数类型	    参数介绍	    备注
    pWidget	    Winks_Widget_s*	需要绘制的控件	
    Option	    int	            状态值	        参见上表。
    ifvar	    int	            真或者假，和状态值一起表示本控件状态发生了哪些变化。	参见上表。
函数返回值：
	绘制失败返回-1，按成功情况返回不同成功消息，参见函数返回值预定义。

实现描述：
    函数内部首先通过pWidget->type成员判断该控件类型，而后调用相应控件实体焦点控制函数进行绘制。
\************************************************************************************/
int Winks_Widget_StateModify( Winks_Widget_s* pWidget, int Option, int ifvar )
{
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;
    int ret = 0;

    if( (pWidget == NULL) || (pWidget->type < 0) || (pWidget->type >= WK_WG_MAXWIDGETTYPENUM) )
    {
        Winks_printf( "Winks Widget StateModify param error\r\n" );
        return -1;
    }

    if( pGlobal->FuncItem[pWidget->type].StateModifyFunc != NULL )
    {
        WINKS_BUFFER_ON;
        ret = pGlobal->FuncItem[pWidget->type].StateModifyFunc( pWidget, Option, ifvar );
        WINKS_BUFFER_OFF;
        return ret;
    }

    Winks_printf( "Winks Widget StateModify no func\r\n" );

    return -1;
}

/************************************************************************************\
函数原型：int Winks_Widget_KeyHandler( Winks_Widget_s* pWidget, int KeyCode, int KeyType )
函数介绍：本函数用来处理一个控件中各个键盘事件发生时的绘制。

参数说明：
        参数名称	参数类型	    参数介绍	    备注
        pWidget	    Winks_Widget_s*	需要绘制的控件	
        KeyCode	    int	            键盘码	
        KeyType	    int	            键盘类型	    按下|抬起|长按|重复
函数返回值：
		绘制失败返回-1，按成功情况返回不同成功消息，参见函数返回值预定义。

实现描述：
        函数内部首先通过pWidget->type成员判断该控件类型，而后调用相应控件实体键盘处理函数进行绘制。
\************************************************************************************/
int Winks_Widget_KeyHandler( Winks_Widget_s* pWidget, int KeyCode, int KeyType )
{
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;
    int ret = 0;

    if( (pWidget == NULL) || (pWidget->type < 0) || (pWidget->type >= WK_WG_MAXWIDGETTYPENUM) )
    {
        Winks_printf( "Winks Widget KeyHandler param error\r\n" );
        return -1;
    }

    if( pGlobal->FuncItem[pWidget->type].KeyHandlerFunc != NULL )
    {
        WINKS_BUFFER_ON;
        ret = pGlobal->FuncItem[pWidget->type].KeyHandlerFunc( pWidget, KeyCode, KeyType );
        WINKS_BUFFER_OFF;
        return ret;
    }

    Winks_printf( "Winks Widget KeyHandler no func\r\n" );

    return -1;
    
}

/************************************************************************************\
函数原型：int Winks_Widget_PenHandler( Winks_Widget_s* pWidget, int PenType, int x, int y )
函数介绍：本函数用来处理一个控件中触摸笔事件发生时的绘制。

参数说明：
        参数名称	参数类型	        参数介绍	            备注
        pWidget	    Winks_Widget_s*	    需要绘制的控件	
        PenType	    int	触摸笔类型	    按下|抬起|移动|长按|重复
        x	        int	笔位置横坐标	
        y	        int	笔位置纵坐标	
函数返回值：
		绘制失败返回-1，按成功情况返回不同成功消息，参见函数返回值预定义。

实现描述：
        函数内部首先通过pWidget->type成员判断该控件类型，而后调用相应控件实体触摸笔处理函数进行绘制。
\************************************************************************************/
#ifdef WINKS_TOUCH_PANNEL
int Winks_Widget_PenHandler( Winks_Widget_s* pWidget, int PenType, int x, int y )
{
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;
    int ret = 0;

    if( (pWidget == NULL) || (pWidget->type < 0) || (pWidget->type >= WK_WG_MAXWIDGETTYPENUM) )
    {
        Winks_printf( "Winks Widget PenHandler param error\r\n" );
        return -1;
    }

    if( pGlobal->FuncItem[pWidget->type].PenHandlerFunc != NULL )
    {
        WINKS_BUFFER_ON;
        ret = pGlobal->FuncItem[pWidget->type].PenHandlerFunc( pWidget, PenType, x, y );
        WINKS_BUFFER_OFF;
        return ret;
    }

    Winks_printf( "Winks Widget PenHandler no func\r\n" );

    return -1;
    
}
#endif
/************************************************************************************\
函数原型：int Winks_Widget_Change( Winks_Widget_s* pWidget, int conflag )
函数介绍：
        本函数用来处理一个控件中内容发生变化时的绘制。当前只支持PopUp控件。本模块通过pWidget->type
        判断控件类型，按照空间结构中填写的内容修改显示绘制。目前仅仅支持弹出式显示框，其他控件修改
        功能随后按需添加。支持情况如下表：
		
        控件类型(pWidget->type)	    conflag	            改动的内容	备注
        WK_WG_POPUP	                WK_WG_PPCF_TTSTR	标题文字	
	                                WK_WG_PPCF_UPSTR	上部文字	
	                                WK_WG_PPCF_ICON	    中部图标	
	                                WK_WG_PPCF_DNSTR	下部文字	
        其他	不支持

参数说明：
        参数名称	参数类型	        参数介绍	备注
        pWidget	    Winks_Widget_s*	    需要绘制的控件	
        conflag	    int	                修改的内容	见上表描述
函数返回值：
	    绘制失败返回-1，按成功情况返回不同成功消息，参见函数返回值预定义。

实现描述：
        函数内部首先通过pWidget->type成员判断该控件类型，而后调用相应控件实体触摸笔处理函数进行绘制。
\************************************************************************************/
int Winks_Widget_Change( Winks_Widget_s* pWidget, int conflag )
{
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;
    int ret = 0;

    if( (pWidget == NULL) || (pWidget->type < 0) || (pWidget->type >= WK_WG_MAXWIDGETTYPENUM) )
    {
        Winks_printf( "Winks Widget Change param error\r\n" );
        return -1;
    }

    if( pGlobal->FuncItem[pWidget->type].ChangeFunc != NULL )
    {
        WINKS_BUFFER_ON;
        ret = pGlobal->FuncItem[pWidget->type].ChangeFunc( pWidget, conflag );
        WINKS_BUFFER_OFF;
        return ret;
    }

    Winks_printf( "Winks Widget Change no func\r\n" );

    return -1;
    
}

/* 矩阵比较函数
   pf完全被pn覆盖返回 < 0
   pf没有完全被pn覆盖返回 > 0 
*/
static int winks_widget_RectCompare( Winks_Rect_s* pf, Winks_Rect_s* pn )
{
    if( (pf->top >= pn->top)
        && (pf->left >= pn->left)
        && (pf->right <= pn->right)
        && (pf->bottom <= pn->bottom) )
    {
        return -1;
    }

    return 1;
    
}

/* 将一个控件的矩阵位置转化成一个矩阵结构 */
static int winks_widget_GetWidgetRect( Winks_Widget_s* pWidget, Winks_Rect_s* pRect )
{
    pRect->left = pWidget->lux;
    pRect->top = pWidget->luy;
    pRect->right = pWidget->lux + pWidget->width;
    pRect->bottom = pWidget->luy + pWidget->high;

    return 0;
}

static int winks_widget_GetLayerRect( Winks_Widget_ScnLayer_s* pLayer, Winks_Rect_s* pRect )
{
    Winks_Widget_s* pWidget = NULL;

    if( pLayer->flag & (WK_WG_SLPOPUP | WK_WG_SLPOPMENU) )
    {
        /* 弹出式界面层矩阵大小就是弹出式显示框大小 */
        if( pLayer->flag & WK_WG_SLPOPUP )
        {
            if( (pWidget = Winks_Widget_GetTypeWidget( pLayer->pWidget, WK_WG_POPUP )) == NULL )
            {
                Winks_fail( "Winks Widget Find popup in popup layer failure.\r\n" );
                return -1;
            }
        }
        if( pLayer->flag & WK_WG_SLPOPMENU )
        {
            if( (pWidget = Winks_Widget_GetTypeWidget( pLayer->pWidget, WK_WG_POPMENU )) == NULL )
            {
                Winks_fail( "Winks Widget Find popmenu in popmenu layer failure.\r\n" );
                return -1;
            }
        }
        winks_widget_GetWidgetRect( pWidget, pRect );
    }
    else
    {
        /* 基本界面层矩阵大小就是屏幕大小 */
        pRect->left = 0;
        pRect->top = 0;
        pRect->right = WINKS_SCREEN_WIDTH;
        pRect->bottom = WINKS_SCREEN_HEIGHT;
    }

    return 0;
}
/************************************************************************************\
函数原型：int Winks_Widget_ScreenDraw( Winks_Widget_ScnLayer_s* pScnLayer )
函数介绍：
        本函数用来绘制一个屏幕的所有控件。界面程序将本屏幕所有界面层的控件传入本函数，本函数会按照传入的信息绘制整个屏幕。
        界面程序也可临时将该界面层的链表指针置空的方式使用本函数提请绘制某一界面层。

参数说明：
        参数名称	参数类型	                参数介绍                	备注
        pScnLayer	Winks_Widget_ScnLayer_s*	需要绘制的界面层链表头指针。	
函数返回值：
		绘制成功返回零，失败返回-1。

实现描述：
        函数首先寻找所有需要显示的界面层（WK_WG_SLSHOW），然后按照各个显示界面层中层数信息（layernum）
        从下至上依次绘制每一界面层中需要绘制的控件，直至完成整个屏幕的绘制工作。
\************************************************************************************/
int Winks_Widget_ScreenDraw( Winks_Widget_ScnLayer_s* pScnLayer )
{
    Winks_Widget_Layer_s Layers[WK_WGAPI_MAXLAYER];
    Winks_Widget_ScnLayer_s* pNextLayer = NULL;
    Winks_Widget_s* pWidget = NULL;
    Winks_Rect_s Rect;
    int i = 0, j = 0, ifgray = 0, ifskbar = 0;

    Winks_mem_set( Layers, 0, sizeof(Winks_Widget_Layer_s) * WK_WGAPI_MAXLAYER );

    /* 按顺序分层排列显示的界面层 */
    while( pScnLayer )
    {
        if( !(pScnLayer->flag & (WK_WG_SLPOPUP | WK_WG_SLPOPMENU)) )
        {
            /* 非弹出层永远需要显示 */
            pScnLayer->flag |= WK_WG_SLSHOW; 
        }
        if( pScnLayer->flag & WK_WG_SLSHOW )
        {
            Winks_assert( pScnLayer->layernum < WK_WGAPI_MAXLAYER );
            Layers[pScnLayer->layernum].pLayer = pScnLayer;
            /* 寻找本层最大矩阵位置 */
            if( winks_widget_GetLayerRect( pScnLayer, &(Layers[pScnLayer->layernum].Rect) ) < 0 )
            {
                Winks_printf( "Winks Widget ScreenDraw find rect error\r\n" );
                return -1;
            }
        }

        pScnLayer = pScnLayer->pnext;
    }

    WINKS_BUFFER_ON;

	// 绘制黑色背景和灰色矩形框
	winks_ui_fillRect( 0, 0, WINKS_SCREEN_WIDTH, WINKS_SCREEN_HEIGHT, WINKS_PIXEL_BLACK );
	winks_ui_fillRect( 2, 2, WINKS_SCREEN_WIDTH - 2, WINKS_SCREEN_HEIGHT - 21, WINKS_PIXEL_GRAY );

    for( i = 0; i < WK_WGAPI_MAXLAYER; i ++ )
    {
        /* 从下至上，逐层绘制 */
        if( Layers[i].pLayer )
        {
            if( i < WK_WGAPI_MAXLAYER - 1 )
            {
                for( j = i + 1; j < WK_WGAPI_MAXLAYER; j ++ )
                {
                    if( Layers[j].pLayer )
                    {
                        pNextLayer = Layers[j].pLayer;
                        break;
                    }
                }
            }
            pWidget = Layers[i].pLayer->pWidget;
            while( pWidget )
            {
                if( pNextLayer )
                {
                    /* 如果上面还有其它层，我们只绘制未被上面一层覆盖的控件,同时，本层左右软键不绘制 */
                    if( (pWidget->type != WK_WG_LEFTSOFTKEY) && (pWidget->type != WK_WG_RIGHTSOFTKEY)
                        && (pWidget->type != WK_WG_CENTERSOFTKEY) )
                    {
                        winks_widget_GetWidgetRect( pWidget, &Rect );
                        if( winks_widget_RectCompare( &Rect, &(Layers[j].Rect) ) > 0 )
                        {
                            if( !ifgray )
                            {
                                WINKS_GRAY_BEGIN( 1, 0, 0, WINKS_SCREEN_WIDTH, WINKS_SCREEN_HEIGHT );
                                ifgray = 1;
                            }
                            Winks_Widget_Draw( pWidget );
                        }
                    }
                }
                else
                {
                    if( ifgray )
                    {
                        WINKS_GRAY_END( 1, 0, 0, WINKS_SCREEN_WIDTH, WINKS_SCREEN_HEIGHT );
                        ifgray = 0;
                    }
                    if( !ifskbar )
                    {
                        winks_ui_fillRect( WINKS_SCRPOS_SOFTKEYBAR_XPOS, WINKS_SCRPOS_SOFTKEYBAR_YPOS, 
                            WINKS_SCRPOS_SOFTKEYBAR_XPOS + WINKS_SCRPOS_SOFTKEYBAR_WIDTH, 
                            WINKS_SCRPOS_SOFTKEYBAR_YPOS + WINKS_SCRPOS_SOFTKEYBAR_HEIGHT, 
                            WINKS_PIXEL_BLACK );
                        ifskbar = 1;
                    }
                    Winks_Widget_Draw( pWidget );
                }

                pWidget = pWidget->pnext;
            }
        }
        if( pNextLayer )
        {
            i = j - 1;
            pNextLayer = NULL;
        }
    }
    WINKS_BUFFER_OFF;

    return 0;
}

/************************************************************************************\
函数原型: Winks_Widget_ScnLayer_s* Winks_Widget_ScreenFindFirstLayer( Winks_Widget_ScnLayer_s* pScnLayer )
函数介绍：
        本函数用来查找一个界面层链表中最上层的界面层。
参数说明：
        参数名称	参数类型	                参数介绍	                备注
        pScnLayer	Winks_Widget_ScnLayer_s*	需要绘制的界面层链表头指针。	
函数返回值：
		绘制成功返回找到的界面层，失败返回NULL。

实现描述：
        函数首先寻找所有需要显示的界面层（WK_WG_SLSHOW），然后按照各个显示界面层中层数信息（layernum）
        找到最上层的界面层，返回给调用者。
\************************************************************************************/
Winks_Widget_ScnLayer_s* Winks_Widget_ScreenFindFirstLayer( Winks_Widget_ScnLayer_s* pScnLayer )
{
    Winks_Widget_ScnLayer_s* pret = NULL;

    pret = pScnLayer;
    while( pScnLayer )
    {
        if( !(pScnLayer->flag & (WK_WG_SLPOPUP | WK_WG_SLPOPMENU)) )
        {
            /* 非弹出层永远需要显示 */
            pScnLayer->flag |= WK_WG_SLSHOW; 
        }
        if( pScnLayer->flag & WK_WG_SLSHOW )
        {
            if( pScnLayer->layernum > pret->layernum )
                pret = pScnLayer;
        }
        pScnLayer = pScnLayer->pnext;
    }

    return pret;
}

/************************************************************************************\
函数原型: int Winks_Widget_ActiveLayer( Winks_Widget_ScnLayer_s* pScnLayer, int LayerID, int ifac )
函数介绍：
        本函数用来激活或去激活一个屏幕中某一个界面层。
参数说明：
        参数名称	参数类型	                参数介绍	                备注
        pScnLayer	Winks_Widget_ScnLayer_s*	需要绘制的界面层链表头指针。
        LayerID     int                         界面层ID                    
        ifac        int                         真代表激活，假代表去激活。
函数返回值：
		绘制成功返回0，失败返回-1。

实现描述：
        本函数激活或去激活某一个界面层，分参数不同行为如下：
        激活
            该界面层未显示，该界面层显示并弹到最上层。
            该界面层显示中，关闭该界面层上所有界面层，当前界面层为最上层。
        去激活
            该界面层未显示，无动作。
            该界面层现实中，关闭该界面层以及该界面层上所有界面层，该界面层下一层为最上层。
\************************************************************************************/
int Winks_Widget_ActiveLayer( Winks_Widget_ScnLayer_s* pScnLayer, int LayerID, int ifac )
{
    Winks_Widget_ScnLayer_s* pLayer = NULL;
    Winks_Widget_ScnLayer_s* pFirst = NULL;

    if( (pLayer = Winks_Widget_FindLayerByID( pScnLayer, LayerID )) == NULL )
    {
        Winks_fail( "Winks Widget ActiveLayer param error\r\n" );
        return -1;
    }

    if( !(pLayer->flag & (WK_WG_SLPOPUP | WK_WG_SLPOPMENU)) )
    {
        /* 基础界面层，本函数无动作 */
        Winks_printf( "Winks Widget Active an basic layer\r\n" );
        return 0;
    }

    if( ifac )
    {
        if( pLayer->flag & WK_WG_SLSHOW )
        {
            while( (pFirst = Winks_Widget_ScreenFindFirstLayer( pScnLayer )) != NULL )
            {
                if( pLayer == pFirst )
                    break;
                pFirst->flag &= ~WK_WG_SLSHOW;
            }
        }
        else
        {
            pFirst = Winks_Widget_ScreenFindFirstLayer( pScnLayer );
            pLayer->flag |= WK_WG_SLSHOW;
            pLayer->layernum = pFirst->layernum + 1;
        }
    }
    else
    {
        if( pLayer->flag & WK_WG_SLSHOW )
        {
            while( (pFirst = Winks_Widget_ScreenFindFirstLayer( pScnLayer )) != NULL )
            {
                pFirst->flag &= ~WK_WG_SLSHOW;
                if( pLayer == pFirst )
                    break;
            }
        }
    }

    //Winks_Widget_ScreenDraw( pScnLayer );

    return 0;
}

/************************************************************************************\
函数原型: int Winks_Widget_ActiveWidget_Ex( Winks_Widget_s* pRootWidget, int WidgetID )
函数介绍：
        本函数用来激活一个控件链表中某一个控件（设置焦点）。
参数说明：
        参数名称	参数类型	                参数介绍	                备注
        pRootWidget	    Winks_Widget_s*	控件链表头指针。
        WidgetID    int                 控件ID                    
函数返回值：
		绘制成功返回0，失败返回-1。

实现描述：
        本函数用来为控件链表某一个控件设置焦点，如果传入的控件已经获得焦点，则本函数无动作，否则
        将去除原有焦点的控件的焦点状态并将焦点移到本控件。
\************************************************************************************/
int Winks_Widget_ActiveWidget_Ex( Winks_Widget_s* pRootWidget, int WidgetID )
{
    Winks_Widget_s* pTmp = NULL;
    Winks_Widget_s* pWidget = NULL;
    Winks_Widget_s* pFocus = NULL;

    pTmp = pRootWidget;
    while( pTmp )
    {
        if( pTmp->flag & WK_WG_FGFOCUS )
            pFocus = pTmp;
        if( pTmp->WidgetID == WidgetID )
            pWidget = pTmp;
        pTmp = pTmp->pnext;
    }
    
    if( pWidget == NULL || !(pWidget->flag & WK_WG_FGGETFOCUS) )
    {
        Winks_fail( "WK widget activewidget not find this widget\r\n" );
        return -1;
    }

    if( pWidget == pFocus )
    {
        Winks_printf( "WK widget activewidget has avtived\r\n" );
        return 0;
    }
    
    if( pFocus != NULL )
        Winks_Widget_StateModify( pFocus, WK_WG_FGFOCUS, 0 );
    
    Winks_Widget_StateModify( pWidget, WK_WG_FGFOCUS, 1 );

    return 0;
}

/************************************************************************************\
函数原型: int Winks_Widget_ActiveWidget( Winks_Widget_ScnLayer_s* pLayer, int WidgetID )
函数介绍：
        本函数用来激活一个界面层中某一个控件（设置焦点）。
参数说明：
        参数名称    参数类型                    参数介绍                    备注
        pLayer      Winks_Widget_ScnLayer_s*    控件所在的界面层。
        WidgetID    int                         控件ID                    
函数返回值：
        绘制成功返回0，失败返回-1。

实现描述：
        本函数用来为界面层中某一个控件设置焦点，如果传入的控件已经获得焦点，则本函数无动作，否则
        将去除原有焦点的控件的焦点状态并将焦点移到本控件。
\************************************************************************************/
int Winks_Widget_ActiveWidget( Winks_Widget_ScnLayer_s* pLayer, int WidgetID )
{
    if( pLayer == NULL )
        return -1;
    
    return Winks_Widget_ActiveWidget_Ex(pLayer->pWidget, WidgetID);
}

/* 查找控件矩阵中指定行上面最近的有控件的行 */
int winks_widget_findupline( Winks_Widget_s** ppWidget, int thisline )
{
    int retline = 0;
    int i = 0, j = 0;

    if( thisline == 0 )
        retline = WK_WGAPI_MAXMATRIXY - 1;
    else
        retline = thisline - 1;

    while( j < WK_WGAPI_MAXMATRIXY )
    {
        for( i = 0; i < WK_WGAPI_MAXMATRIXX; i++ )
        {
            if( *(ppWidget + (retline * WK_WGAPI_MAXMATRIXX + i)) != NULL )
                return retline;
        }

        retline --;
        if( retline < 0 )
            retline = WK_WGAPI_MAXMATRIXY - 1;
        j++;
    }
        
    if( j == WK_WGAPI_MAXMATRIXY )
    {
        Winks_fail( "Winks Widget find upline failure\r\n" );
        return -1;
    }
    return retline;
}

/* 查找控件矩阵中指定行下面最近的有控件的行 */
int winks_widget_finddownline( Winks_Widget_s** ppWidget, int thisline )
{
    int retline = 0;
    int i = 0, j = 0;

    if( thisline == WK_WGAPI_MAXMATRIXY - 1 )
        retline = 0;
    else
        retline = thisline + 1;

    while( j < WK_WGAPI_MAXMATRIXY )
    {
        for( i = 0; i < WK_WGAPI_MAXMATRIXX; i++ )
        {
            if( *(ppWidget + (retline * WK_WGAPI_MAXMATRIXX + i)) != NULL )
            {
                return retline;
            }
        }

        retline ++;
        if( retline > WK_WGAPI_MAXMATRIXY - 1 )
            retline = 0;
        j++;
    }
    if( j == WK_WGAPI_MAXMATRIXY )
    {
        Winks_fail( "Winks Widget find upline failure\r\n" );
        return -1;
    }
        
    return retline;
}

/* 查找控件矩阵中指定行指定列控件或前面的控件
   thiscolumn == WK_WGAPI_MAXMATRIXX - 1 代表查找最后一个控件
   查找失败返回-1.
 */
int winks_widget_findbeforecolumn( Winks_Widget_s** ppWidget, int thisline, int thiscolumn )
{
    int i = 0;

    if( (thiscolumn < 0) || (thiscolumn >= WK_WGAPI_MAXMATRIXX) )
        return -1;

    for( i = thiscolumn; i >= 0; i-- )
    {
        if( *(ppWidget + (thisline * WK_WGAPI_MAXMATRIXX + i)) )
            return i;
    }

    return -1;
}

/* 查找控件矩阵中指定行指定列控件或后面的控件
   thiscolumn == 0 代表查找第一个一个控件
   查找失败返回-1.
 */
int winks_widget_findaftercolumn( Winks_Widget_s** ppWidget, int thisline, int thiscolumn )
{
    int i = 0;
    
    if( (thiscolumn < 0) || (thiscolumn >= WK_WGAPI_MAXMATRIXX) )
        return -1;

    for( i = thiscolumn; i < WK_WGAPI_MAXMATRIXX; i++ )
    {
        if( *(ppWidget + (thisline * WK_WGAPI_MAXMATRIXX + i)) )
            return i;
    }

    return -1;
}

/************************************************************************************\
函数原型：int Winks_Widget_ScreenNavKeyDeal( Winks_Widget_ScnLayer_s* pScnLayer, int KeyCode, int KeyType, 
                    Winks_Widget_s** ppLostFocus, Winks_Widget_s** pGetFocus )
函数介绍：
            本函数统一处理一个界面的导航键行为。本函数仅仅接受导航键参数。本函数会处理由于导航键
            造成的控件焦点的迁徙设置和显示工作。同时还会按需将该键盘消息传入该控件请求处理（Winks_Widget_KeyHandler）。
            本函数处理原则为在显示的界面层中最前面的那一层中，焦点会在该层的控件矩阵中如下迁徙：
            键值	按下	                                        抬起	            长按
            ↑	    焦点移动到上一行纵坐标<=当前纵坐标的控件。      无动作。            重复按下行为。	
                    如当前为第一行则无动作。	
            ↓	    焦点移动到下一行纵坐标<=当前纵坐标的控件。      无动作。	        重复按下行为。
                    如当前为最后一行无动作。	
            →	    焦点移动到本行纵坐标+1的控件，如已经是本行      无动作。	        重复按下行为。
                    最后一个控件则移动到下一行第一个控件。	
            ←	    焦点移动到本行纵坐标-1的控件，如已经是本行      无动作。            重复按下行为。	
                    第一个控件则移动到上一行最后一个控件。	
            特别的，如果某一个界面上控件焦点迁徙不符合上述原则，界面程序需自己处理导航键，而不可调用本函数。
参数说明：
            参数名称	参数类型	                参数介绍	                备注
            pScnLayer	Winks_Widget_ScnLayer_s*	本界面界面层链表头指针。	
            KeyCode	    int	                        键值，仅接受如下键值：
                                                        WINKS_KEY_UP   
                                                        WINKS_KEY_DOWN 
                                                        WINKS_KEY_LEFT 
                                                        WINKS_KEY_RIGHT	    
            KeyType	    int	                        按键类型。	
            ppLostFocus	Winks_Widget_s**	        返回失去焦点的控件。	
            ppGetFocus	Winsk_Widget_s**	        返回获得焦点的控件。	
函数返回值：
		    绘制成功返回零，失败返回-1。

实现描述：
            函数首先寻找最前面的界面层，然后按照该界面层中控件矩阵信息确定失去焦点的控件和获得焦点的控件，
            并进行焦点变迁绘制工作。同时本函数会将失去焦点的控件和获得焦点的控件作为参数返回给调用者。
\************************************************************************************/
int Winks_Widget_ScreenNavKeyDeal( Winks_Widget_ScnLayer_s* pScnLayer, int KeyCode, int KeyType, 
                                  Winks_Widget_s** ppLostFocus, Winks_Widget_s** ppGetFocus )
{
    Winks_Widget_ScnLayer_s* pLayer = NULL;
    Winks_Widget_s* pWidget = NULL;
    Winks_Widget_s* pTmpWidget = NULL;
    int ret = 0, wdnum = 0, fline = 0, fcolumn = 0;
    Winks_Widget_s* WidgetMatrix[WK_WGAPI_MAXMATRIXX][WK_WGAPI_MAXMATRIXY];

    if( (pLayer = Winks_Widget_ScreenFindFirstLayer( pScnLayer )) == NULL )
    {
        Winks_fail( "Winks Widget can not find first layer\r\n" );
        return -1;
    }
    if( (pWidget = Winks_Widget_GetFocusWidget( pLayer->pWidget )) == NULL )
    {
        //Winks_fail( "Winks Widget can not find focus widget\r\n" );
        Winks_printf( "Winks Widget can not find focus widget\r\n" );
        return -1;
    }

    if( (KeyCode != WINKS_KEY_UP) && (KeyCode != WINKS_KEY_DOWN)
        && (KeyCode != WINKS_KEY_LEFT) && (KeyCode != WINKS_KEY_RIGHT) )
    {
        Winks_fail( "Winks Widget Navkey handler recv key not navkey.\r\n" );
        return -1;
    }

    ret = Winks_Widget_KeyHandler( pWidget, KeyCode, KeyType );

    if( KeyType == WINKS_KEY_FLAG_KEYUP )
    {
        /* KeyUp无动作 */
        Winks_printf( "Winks Widget NavKeyDeal get keyup, return\r\n" );
        return 0;
    }

    if( (ret == WK_UIFM_FOCUSCHANGE) || (ret == WK_UIFM_FCCHANGE) )
    {
        /* 焦点需要发生变化 */
        Winks_mem_set( WidgetMatrix, 0, sizeof(Winks_Widget_s*) * WK_WGAPI_MAXMATRIXX * WK_WGAPI_MAXMATRIXY );
        pTmpWidget = pLayer->pWidget;
        while( pTmpWidget )
        {
            if( pTmpWidget->flag & WK_WG_FGGETFOCUS )
            {
                Winks_assert( pTmpWidget->xindex < WK_WGAPI_MAXMATRIXX );
                Winks_assert( pTmpWidget->yindex < WK_WGAPI_MAXMATRIXY );
                WidgetMatrix[pTmpWidget->yindex][pTmpWidget->xindex] = pTmpWidget;
                wdnum ++;
            }

            pTmpWidget = pTmpWidget->pnext;
        }
        if( wdnum <= 1 )
        {
            /* 只有一个可获得焦点的控件，直接返回 */
            if( ppLostFocus )
                *ppLostFocus = NULL;
            if( ppGetFocus )
                *ppGetFocus = pWidget;
            return ret;
        }
        
        switch( KeyCode )
        {
        case WINKS_KEY_UP:
            if( (fline = winks_widget_findupline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex )) < pWidget->yindex )
            {
                if( (fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex )) < 0 )
                    fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex );
                pTmpWidget = WidgetMatrix[fline][fcolumn];
            }
            else
            {
                /* 只有一行或最上面一行 */
                pTmpWidget = pWidget;
                pWidget = NULL;
            }
            break;

        case WINKS_KEY_DOWN:
            if( (fline = winks_widget_finddownline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex )) > pWidget->yindex )
            {
                if( (fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex )) < 0 )
                    fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, pWidget->xindex );
                pTmpWidget = WidgetMatrix[fline][fcolumn];
            }
            else
            {
                /* 只有一行或最下面一行 */
                pTmpWidget = pWidget;
                pWidget = NULL;
            }
            break;

        case WINKS_KEY_LEFT:
            fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex, pWidget->xindex - 1 );
            if(  fcolumn < 0 )
            {
                fline = winks_widget_findupline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex );
                fcolumn = winks_widget_findbeforecolumn( (Winks_Widget_s** )WidgetMatrix, fline, WK_WGAPI_MAXMATRIXX - 1 );
                pTmpWidget = WidgetMatrix[fline][fcolumn];
            }
            else
            {
                pTmpWidget = WidgetMatrix[pWidget->yindex][fcolumn];
            }
            break;

        case WINKS_KEY_RIGHT:
            fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex, pWidget->xindex + 1 );
            if( fcolumn < 0 )
            {
                fline = winks_widget_finddownline( (Winks_Widget_s** )WidgetMatrix, pWidget->yindex );
                fcolumn = winks_widget_findaftercolumn( (Winks_Widget_s** )WidgetMatrix, fline, 0 );
                pTmpWidget = WidgetMatrix[fline][fcolumn];
            }
            else
            {
                pTmpWidget = WidgetMatrix[pWidget->yindex][fcolumn];
            }
            break;
        }
    }
    else
    {
        pTmpWidget = pWidget;
        pWidget = NULL;
    }

    if( pWidget )
    {
        /* 如果有新控件获得了焦点，则请求绘制 */
        Winks_Widget_StateModify( pWidget, WK_WG_FGFOCUS, 0 );
        Winks_Widget_StateModify( pTmpWidget, WK_WG_FGFOCUS, 1 );
    }

    if( ppLostFocus )
        *ppLostFocus = pWidget;
    if( ppGetFocus )
        *ppGetFocus = pTmpWidget;

    return ret;
}

/* 判断一个点是不是在一个矩形中 */
static int winks_widget_ifpinrect( int x, int y, Winks_Rect_s* prect )
{
    if( (x >= prect->left) && (x <= prect->right) && (y >= prect->top) && (y <= prect->bottom) )
        return 1;

    return 0;
}

static int winks_widget_ifpinlayer( int x, int y, Winks_Widget_ScnLayer_s* pLayer )
{
    Winks_Rect_s rect;
    Winks_Widget_s* pWidget = NULL;
    
    winks_widget_GetLayerRect( pLayer, &rect );

    if( winks_widget_ifpinrect(x, y, &rect) )
        return 1;

    if( pLayer->flag & (WK_WG_SLPOPUP | WK_WG_SLPOPMENU) )
    {
        if( (pWidget = Winks_Widget_GetTypeWidget( pLayer->pWidget, WK_WG_LEFTSOFTKEY )) != NULL )
        {
            winks_widget_GetWidgetRect( pWidget, &rect );
            if( winks_widget_ifpinrect(x, y, &rect) )
                return 1;
        }
        if( (pWidget = Winks_Widget_GetTypeWidget( pLayer->pWidget, WK_WG_RIGHTSOFTKEY )) != NULL )
        {
            winks_widget_GetWidgetRect( pWidget, &rect );
            if( winks_widget_ifpinrect(x, y, &rect) )
                return 1;
        }
        if( (pWidget = Winks_Widget_GetTypeWidget( pLayer->pWidget, WK_WG_CENTERSOFTKEY )) != NULL )
        {
            winks_widget_GetWidgetRect( pWidget, &rect );
            if( winks_widget_ifpinrect(x, y, &rect) )
                return 1;
        }
    }

    return 0;
    
}
/************************************************************************************\
函数原型：int Winks_Widget_ScreenPenDeal( Winks_Widget_ScnLayer_s* pScnLayer, int PenType, int x, int y, Winks_Widget_s** ppLostFocus, Winks_Widget_s** pGetFocus )
函数介绍：
        本函数统一处理一个界面的触摸笔行为。本函数仅仅会处理由于触摸笔造成的控件焦点的迁徙设置和显示工作。
        同时还会按需将笔消息传入该控件请求处理（Winks_Widget_PenHandler）。通常触摸笔不仅仅会造成焦点迁徙，
        还可能会引发其他动作，这些动作应由界面程序自己处理。
        本函数处理原则为在显示的界面层中最前面的那一层中，通过判断触摸笔类型和点触位置判断控件焦点的迁徙。
        笔类型	        触点不再最上层区域内	触点不在最上层任何控件区域内	触点在最上层某一控件区域内
        PENDOWN	        无动作。	            无动作。	                    焦点移至本控件。
        PENUP	        无动作。	            无动作。	                    无动作。
        PENMOVE	        无动作。	            无动作。	                    焦点移至本控件。
        PENREPEAT	    无动作。	            无动作。	                    重复PENDOWN行为。
        PENLONGPRESS	无动作。	            无动作。	                    重复PENDOWN行为。
        特别的，如果某一个界面上控件焦点迁徙不符合上述原则，界面程序需自己处理触摸笔消息，而不可调用本函数。
参数说明：
        参数名称	参数类型	                参数介绍	                备注
        pScnLayer	Winks_Widget_ScnLayer_s*	本界面界面层链表头指针。	
        PenType	    int	                        按键类型。	
        x	        int	                        触点横坐标。	
        y	        int	                        触点纵坐标。	
        ppLostFocus	Winks_Widget_s**	        返回失去焦点的控件。	
        ppGetFocus	Winsk_Widget_s**	        返回获得焦点的控件。	
函数返回值：
		绘制成功返回零，失败返回-1。

实现描述：
        函数首先寻找最前面的界面层，然后按照该界面层中控件矩阵信息确定失去焦点的控件和获得焦点的控件，并进行焦点
        变迁绘制工作。同时本函数会将失去焦点的控件和获得焦点的控件作为参数返回给调用者。
\************************************************************************************/
int Winks_Widget_ScreenPenDeal( Winks_Widget_ScnLayer_s* pScnLayer, int PenType, int x, int y, 
                               Winks_Widget_s** ppLostFocus, Winks_Widget_s** ppGetFocus, Winks_Widget_s** ppPoint )
{
    Winks_Widget_ScnLayer_s* pLayer = NULL;
    Winks_Widget_s* pWidget = NULL;
    Winks_Widget_s* pTmpWidget = NULL;
    Winks_Widget_s* pPoint = NULL;
    int ret = 0, ifredraw = 0;
    Winks_Rect_s rect;

    /*
    if( (pLayer = Winks_Widget_ScreenFindFirstLayer( pScnLayer )) == NULL )
    {
        Winks_fail( "Winks Widget can not find first layer\r\n" );
        return -1;
    }
    */

    while( (pLayer = Winks_Widget_ScreenFindFirstLayer( pScnLayer )) != NULL )
    {
        /*
        if( (pWidget = Winks_Widget_GetFocusWidget( pLayer->pWidget )) == NULL )
        {
            //Winks_fail( "Winks Widget can not find focus widget\r\n" );
            Winks_printf( "Winks Widget can not find focus widget\r\n" );
            return -1;
        }
        */
        pWidget = Winks_Widget_GetFocusWidget( pLayer->pWidget );

        //winks_widget_GetLayerRect( pLayer, &rect );
        
        if( !winks_widget_ifpinlayer( x, y, pLayer ) )
        {
            if( pLayer->flag & WK_WG_SLPOPMENU )
            {
                pLayer->flag &= ~WK_WG_SLSHOW;
                ifredraw = 1;
                continue;
            }
            else
            {
                Winks_printf( "Winks Widget ScreenPenDeal point not in up layer\r\n" );
                if( ppLostFocus )
                    *ppLostFocus = NULL;
                if( ppGetFocus )
                    *ppGetFocus = pWidget;
                if( ppPoint )
                    *ppPoint = NULL;

                return -1;
            }
        }
        
        pTmpWidget = pLayer->pWidget;
        while( pTmpWidget )
        {
            if( pTmpWidget->type != WK_WG_POPUP )
            {
                winks_widget_GetWidgetRect( pTmpWidget, &rect );
                if( winks_widget_ifpinrect( x, y, &rect ) )
                    break;
            }
            pTmpWidget = pTmpWidget->pnext;
        }
        
        pPoint = pTmpWidget;

        if( pTmpWidget == NULL )
        {
            /* 触点未点中任何控件，可能是弹出式显示框的边缘 */
            pWidget = NULL;
            pTmpWidget = pWidget;
            Winks_printf( "Winks Widget ScreenPenDeal point not in any widget\r\n" );
            ret = -1;
        }
        else
        {
            WINKS_BUFFER_ON;
            ret = Winks_Widget_PenHandler( pTmpWidget, PenType, x, y );
            if( (pTmpWidget != pWidget) &&
                (pTmpWidget->flag & WK_WG_FGGETFOCUS) )
            {
                /* 如果新控件是可以获得焦点的，需要更换焦点 */
                if( pWidget )
                    Winks_Widget_StateModify( pWidget, WK_WG_FGFOCUS, 0 );
                Winks_Widget_StateModify( pTmpWidget, WK_WG_FGFOCUS, 1 );
            }
            else
            {
                pTmpWidget = NULL;
            }
            WINKS_BUFFER_OFF;
        }

        break;
    }
    
    if( pLayer == NULL )
    {
        Winks_fail( "Winks Widget can not find first layer\r\n" );
        return -1;
    }

    if( ifredraw )
    {
        Winks_Widget_ScreenDraw( pScnLayer );
    }

    if( ppLostFocus )
        *ppLostFocus = pWidget;
    if( ppGetFocus )
        *ppGetFocus = pTmpWidget;
    if( ppPoint )
        *ppPoint = pPoint;

    return ret;

}

/************************************************************************************\
函数原型：Winks_Widget_s* Winks_Widget_GetFocusWidget( Winks_Widget_s* pWidget )
函数介绍：本函数用来查找某控件链表中获得焦点的控件。

参数说明：
        参数名称	参数类型	    参数介绍	    备注
        pWidget	    Winks_Widget_s*	控件链表头指针	

函数返回值：
		成功返回获得焦点的控件指针，失败返回NULL。

实现描述：
        本函数在控件链表中找到获得焦点的控件，返回指针。本机制基于一个控件链表中同一时间只会有一个控件
        获得焦点。如果特殊界面（可能有多个获得焦点的控件的界面）的话，界面程序应自己查找，本程序返回值
        不可预期。
\************************************************************************************/
Winks_Widget_s* Winks_Widget_GetFocusWidget( Winks_Widget_s* pWidget )
{
    
    while( pWidget )
    {
        if( pWidget->flag & WK_WG_FGFOCUS )
            return pWidget;

        pWidget = pWidget->pnext;
    }

    return NULL;

}

/************************************************************************************\
函数原型：Winks_Widget_s* Winks_Widget_GetTypeWidget( Winks_Widget_s* pWidget, int WidgetType )
函数介绍：
        本函数用来查找某控件链表中特定类型的控件。除非界面程序确认本控件链表中只有一个该类型的控件，
        否则不可使用本函数查找。如果一个控件链表中会有多个同一类型的控件，本函数返回值不可预期。
        一般来说，本函数可用来查找诸如左右软键之类的界面层唯一的控件。

参数说明：
        参数名称	参数类型	    参数介绍	        备注
        pWidget	    Winks_Widget_s*	控件链表头指针	
        WidgetType	int	控件类型	WK_WG_LEFTSOFTKEY等

函数返回值：
		成功返回获得焦点的控件指针，失败返回NULL。

实现描述：
        本函数在控件链表中找到指定类型控件，返回指针。本机制基于一个控件链表中同一时间只会有一个
        指定类型控件。如果界面可能有多个指定类型的控件的话，界面程序应自己查找，本程序返回值不可预期。
\************************************************************************************/
Winks_Widget_s* Winks_Widget_GetTypeWidget( Winks_Widget_s* pWidget, int WidgetType )
{
    
    while( pWidget )
    {
        if( pWidget->type == WidgetType )
            return pWidget;

        pWidget = pWidget->pnext;
    }

    return NULL;

}

/************************************************************************************\
函数原型：Winks_Widget_s* Winks_Widget_WIdgetAlloc( int type, int lux, int luy, int width, int high, 
            int xindex, int yindex, int allocsize )
函数介绍：
        本函数用来帮助界面程序为每一种控件申请内存并进行一些基本成员的赋值工作。
        本函数目的是为了简化编程，降低代码量。
        由于很多时候一个控件大小可能会超过该控件结构的大小，例如具有可变字符串
        属性的控件，在创建该控件内存空间时应预先将这段大小也申请入控件结构。因
        此本函数申请到的控件内存大小由调用者指定(allocsize)。

参数说明：
        参数名称	参数类型	参数介绍	            备注
        type	    int	        控件类型	
        lux	        int	        控件左上角坐标x	
        luy	        int	        控件左上角坐标y	
        width	    int	        控件宽度	
        high	    int	        控件高度	
        xindex	    int	        在该层控件矩阵中的横坐标。	
        yindex	    int	        在该层控件矩阵中的纵坐标。	
        allocsize	int	        结构长度	

函数返回值：
		成功返回申请出来的控件指针，失败返回NULL。

实现描述：
        本函数按用户指定的内存大小申请内存，并按用户传入的参数填写公共成员数据。
\************************************************************************************/
Winks_Widget_s* Winks_Widget_WidgetAlloc( int type, int lux, int luy, int width, int high, 
            int xindex, int yindex, int allocsize )
{
    Winks_Widget_s* pWidget = NULL;
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;

    if( allocsize < sizeof(Winks_Widget_s) )
    {
        Winks_printf( "Winks Widget widgetalloc size error\r\n" );
        return NULL;
    }

    if( (type < 0) || (type >= WK_WG_MAXWIDGETTYPENUM) )
    {
        Winks_printf( "Winks Widget alloc param error\r\n" );
        return NULL;
    }

    if( (pWidget = Winks_salAlloc(allocsize)) == NULL )
    {
        Winks_printf( "Winks Widget widgetalloc alloc memory error\r\n" );
        return NULL;
    }

    Winks_mem_set( pWidget, 0, allocsize );

    pWidget->type = (unsigned short)type;
    pWidget->xindex = (unsigned char)xindex;
    pWidget->yindex = (unsigned char)yindex;
    pWidget->lux = lux;
    pWidget->luy = luy;
    pWidget->width = width;
    pWidget->high = high;

    if( pGlobal->FuncItem[pWidget->type].InitFunc != NULL )
    {
        pGlobal->FuncItem[pWidget->type].InitFunc( pWidget );
    }

    return pWidget;
}

/************************************************************************************\
函数原型：Winks_Widget_TapTitle_s* Winks_Widget_GetTapTitle( void )
函数介绍：
        基于当前界面情况，每一个界面上面的分页标题栏应该是同一个，并可以全局存在，因此分页标题栏由
        控件框架申请内存并初始化，每个界面需要绘制分页标题栏时只需通过本函数获取该分页标题栏使用即可。
        控件框架仅仅负责分页标题栏内存申请和初始化工作，当界面迁徙后分页标题栏发生的变化（每一页对应
        的屏幕ID和当前高亮的页索引等）数据由界面负责调整。
        另外，当界面需要释放本界面控件链表时需记得不可释放本结构。

参数说明：
        无。
函数返回值：
		成功返回分页标题栏控件指针，失败返回NULL。

实现描述：
        本函数返回框架保留的全局分页标题栏数据。
\************************************************************************************/
Winks_Widget_TapTitle_s* Winks_Widget_GetTapTitle( void )
{
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;    

    return ( Winks_Widget_TapTitle_s * )&(pGlobal->TapTitle);
}

/************************************************************************************\
函数原型：Winks_Widget_ScnLayer_s* Winks_Widget_FindLayerByID( Winks_Widget_ScnLayer_s* pHead, int ID )
函数介绍：
        通过界面层ID查找界面层。

参数说明：
        参数名称	参数类型	                参数介绍	            备注
        pHead       Winks_Widget_ScnLayer_s*    界面层头指针
        ID	        int	                        界面层ID(LayerID)	

函数返回值：
		成功返回找到的界面层指针，失败返回NULL。

实现描述：
        本函数返回指定ID的界面层。
\************************************************************************************/
Winks_Widget_ScnLayer_s* Winks_Widget_FindLayerByID( Winks_Widget_ScnLayer_s* pHead, int ID )
{
    while( pHead )
    {
        if( pHead->LayerID == ID )
            return pHead;
        pHead = pHead->pnext;
    }

    return NULL;
}

/************************************************************************************\
函数原型：Winks_Widget_s* Winks_Widget_FindWidgetByID( Winks_Widget_ScnLayer_s* pHead, int ID )
函数介绍：
        通过控件ID查找控件。本查找会在整个屏幕所有界面层中查找，界面程序须确保该ID屏幕唯一。

参数说明：
        参数名称	参数类型	                参数介绍	            备注
        pHead       Winks_Widget_ScnLayer_s*    界面层头指针
        ID	        int	                        控件ID(WidgetID)	

函数返回值：
		成功返回找到的界面层指针，失败返回NULL。

实现描述：
        本函数返回指定ID的控件。
\************************************************************************************/
Winks_Widget_s* Winks_Widget_FindWidgetByID( Winks_Widget_ScnLayer_s* pHead, int ID )
{
    Winks_Widget_s* pret = NULL;

    while( pHead )
    {
        pret = pHead->pWidget;
        while( pret )
        {
            if( pret->WidgetID == ID )
                return pret;
            pret = pret->pnext;
        }
        pHead = pHead->pnext;
    }

    return NULL;
}

/************************************************************************************\
函数原型：Winks_Widget_ScnLayer_s* Winks_Widget_FindLayerByWidget( Winks_Widget_ScnLayer_s* pHead, int ID )
函数介绍：
        通过控件ID查找他所属的界面层。

参数说明：
        参数名称	参数类型	                参数介绍	            备注
        pHead       Winks_Widget_ScnLayer_s*    界面层头指针
        ID	        int	                        控件ID(WidgetID)	

函数返回值：
		成功返回找到的界面层指针，失败返回NULL。

实现描述：
        本函数返回指定的界面层。
\************************************************************************************/
Winks_Widget_ScnLayer_s* Winks_Widget_FindLayerByWidget( Winks_Widget_ScnLayer_s* pHead, int ID )
{
    Winks_Widget_s* pret = NULL;

    while( pHead )
    {
        pret = pHead->pWidget;
        while( pret )
        {
            if( pret->WidgetID == ID )
                return pHead;
            pret = pret->pnext;
        }
        pHead = pHead->pnext;
    }

    return NULL;
}
