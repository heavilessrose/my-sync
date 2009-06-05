
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
����ԭ�ͣ�int Winks_Widget_Init( void )
�������ܣ�������������ʼ���ؼ�ģ�顣

����˵����

��������ֵ��
    �ɹ������㣬ʧ�ܷ���-1��

ʵ��������
    ��ʼ���ؼ�ģ�顣
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
����ԭ�ͣ�int Winks_Widget_DeInit( void )
�������ܣ�����������ȥ��ʼ���ؼ�ģ�顣

����˵����

��������ֵ��
    �ɹ������㣬ʧ�ܷ���-1��

ʵ��������
    ȥ��ʼ���ؼ�ģ�顣
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
����ԭ�ͣ�int Winks_Widget_Draw( Winks_Widget_s* pWidget )
�������ܣ���������������һ���ؼ���

����˵����
    ��������	��������	    ��������	    ��ע
    pWidget	    Winks_Widget_s*	��Ҫ���ƵĿؼ�	
��������ֵ��
    ����ʧ�ܷ���-1�����ɹ�������ز�ͬ�ɹ���Ϣ���μ���������ֵԤ���塣

ʵ��������
    �����ڲ�����ͨ��pWidget->type��Ա�жϸÿؼ����ͣ����������Ӧ�ؼ�ʵ����ƺ������л��ơ�
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
����ԭ�ͣ�int Winks_Widget_StateModify( Winks_Widget_s* pWidget, int Option, int ifvar )
�������ܣ���������������һ���ؼ���״̬�仯�Ļ��ơ����ܵ�״̬�仯���±�
		
    ״ֵ̬(Option)	���壨ifvar��
    WK_WG_FGGRAY	���ʾ���ؼ����ûң�
                    �ٱ�ʾ���ؼ�δ���ûҡ�
    WK_WG_FGFOCUS	���ʾ���ؼ���ý��㣬
                    �ٱ�ʾ���ؼ�ʧȥ���㡣

����˵����
    ��������	��������	    ��������	    ��ע
    pWidget	    Winks_Widget_s*	��Ҫ���ƵĿؼ�	
    Option	    int	            ״ֵ̬	        �μ��ϱ�
    ifvar	    int	            ����߼٣���״ֵ̬һ���ʾ���ؼ�״̬��������Щ�仯��	�μ��ϱ�
��������ֵ��
	����ʧ�ܷ���-1�����ɹ�������ز�ͬ�ɹ���Ϣ���μ���������ֵԤ���塣

ʵ��������
    �����ڲ�����ͨ��pWidget->type��Ա�жϸÿؼ����ͣ����������Ӧ�ؼ�ʵ�役����ƺ������л��ơ�
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
����ԭ�ͣ�int Winks_Widget_KeyHandler( Winks_Widget_s* pWidget, int KeyCode, int KeyType )
�������ܣ���������������һ���ؼ��и��������¼�����ʱ�Ļ��ơ�

����˵����
        ��������	��������	    ��������	    ��ע
        pWidget	    Winks_Widget_s*	��Ҫ���ƵĿؼ�	
        KeyCode	    int	            ������	
        KeyType	    int	            ��������	    ����|̧��|����|�ظ�
��������ֵ��
		����ʧ�ܷ���-1�����ɹ�������ز�ͬ�ɹ���Ϣ���μ���������ֵԤ���塣

ʵ��������
        �����ڲ�����ͨ��pWidget->type��Ա�жϸÿؼ����ͣ����������Ӧ�ؼ�ʵ����̴��������л��ơ�
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
����ԭ�ͣ�int Winks_Widget_PenHandler( Winks_Widget_s* pWidget, int PenType, int x, int y )
�������ܣ���������������һ���ؼ��д������¼�����ʱ�Ļ��ơ�

����˵����
        ��������	��������	        ��������	            ��ע
        pWidget	    Winks_Widget_s*	    ��Ҫ���ƵĿؼ�	
        PenType	    int	����������	    ����|̧��|�ƶ�|����|�ظ�
        x	        int	��λ�ú�����	
        y	        int	��λ��������	
��������ֵ��
		����ʧ�ܷ���-1�����ɹ�������ز�ͬ�ɹ���Ϣ���μ���������ֵԤ���塣

ʵ��������
        �����ڲ�����ͨ��pWidget->type��Ա�жϸÿؼ����ͣ����������Ӧ�ؼ�ʵ�崥���ʴ��������л��ơ�
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
����ԭ�ͣ�int Winks_Widget_Change( Winks_Widget_s* pWidget, int conflag )
�������ܣ�
        ��������������һ���ؼ������ݷ����仯ʱ�Ļ��ơ���ǰֻ֧��PopUp�ؼ�����ģ��ͨ��pWidget->type
        �жϿؼ����ͣ����տռ�ṹ����д�������޸���ʾ���ơ�Ŀǰ����֧�ֵ���ʽ��ʾ�������ؼ��޸�
        �����������ӡ�֧��������±�
		
        �ؼ�����(pWidget->type)	    conflag	            �Ķ�������	��ע
        WK_WG_POPUP	                WK_WG_PPCF_TTSTR	��������	
	                                WK_WG_PPCF_UPSTR	�ϲ�����	
	                                WK_WG_PPCF_ICON	    �в�ͼ��	
	                                WK_WG_PPCF_DNSTR	�²�����	
        ����	��֧��

����˵����
        ��������	��������	        ��������	��ע
        pWidget	    Winks_Widget_s*	    ��Ҫ���ƵĿؼ�	
        conflag	    int	                �޸ĵ�����	���ϱ�����
��������ֵ��
	    ����ʧ�ܷ���-1�����ɹ�������ز�ͬ�ɹ���Ϣ���μ���������ֵԤ���塣

ʵ��������
        �����ڲ�����ͨ��pWidget->type��Ա�жϸÿؼ����ͣ����������Ӧ�ؼ�ʵ�崥���ʴ��������л��ơ�
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

/* ����ȽϺ���
   pf��ȫ��pn���Ƿ��� < 0
   pfû����ȫ��pn���Ƿ��� > 0 
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

/* ��һ���ؼ��ľ���λ��ת����һ������ṹ */
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
        /* ����ʽ���������С���ǵ���ʽ��ʾ���С */
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
        /* �������������С������Ļ��С */
        pRect->left = 0;
        pRect->top = 0;
        pRect->right = WINKS_SCREEN_WIDTH;
        pRect->bottom = WINKS_SCREEN_HEIGHT;
    }

    return 0;
}
/************************************************************************************\
����ԭ�ͣ�int Winks_Widget_ScreenDraw( Winks_Widget_ScnLayer_s* pScnLayer )
�������ܣ�
        ��������������һ����Ļ�����пؼ���������򽫱���Ļ���н����Ŀؼ����뱾�������������ᰴ�մ������Ϣ����������Ļ��
        �������Ҳ����ʱ���ý���������ָ���ÿյķ�ʽʹ�ñ������������ĳһ����㡣

����˵����
        ��������	��������	                ��������                	��ע
        pScnLayer	Winks_Widget_ScnLayer_s*	��Ҫ���ƵĽ��������ͷָ�롣	
��������ֵ��
		���Ƴɹ������㣬ʧ�ܷ���-1��

ʵ��������
        ��������Ѱ��������Ҫ��ʾ�Ľ���㣨WK_WG_SLSHOW����Ȼ���ո�����ʾ������в�����Ϣ��layernum��
        �����������λ���ÿһ���������Ҫ���ƵĿؼ���ֱ�����������Ļ�Ļ��ƹ�����
\************************************************************************************/
int Winks_Widget_ScreenDraw( Winks_Widget_ScnLayer_s* pScnLayer )
{
    Winks_Widget_Layer_s Layers[WK_WGAPI_MAXLAYER];
    Winks_Widget_ScnLayer_s* pNextLayer = NULL;
    Winks_Widget_s* pWidget = NULL;
    Winks_Rect_s Rect;
    int i = 0, j = 0, ifgray = 0, ifskbar = 0;

    Winks_mem_set( Layers, 0, sizeof(Winks_Widget_Layer_s) * WK_WGAPI_MAXLAYER );

    /* ��˳��ֲ�������ʾ�Ľ���� */
    while( pScnLayer )
    {
        if( !(pScnLayer->flag & (WK_WG_SLPOPUP | WK_WG_SLPOPMENU)) )
        {
            /* �ǵ�������Զ��Ҫ��ʾ */
            pScnLayer->flag |= WK_WG_SLSHOW; 
        }
        if( pScnLayer->flag & WK_WG_SLSHOW )
        {
            Winks_assert( pScnLayer->layernum < WK_WGAPI_MAXLAYER );
            Layers[pScnLayer->layernum].pLayer = pScnLayer;
            /* Ѱ�ұ���������λ�� */
            if( winks_widget_GetLayerRect( pScnLayer, &(Layers[pScnLayer->layernum].Rect) ) < 0 )
            {
                Winks_printf( "Winks Widget ScreenDraw find rect error\r\n" );
                return -1;
            }
        }

        pScnLayer = pScnLayer->pnext;
    }

    WINKS_BUFFER_ON;

	// ���ƺ�ɫ�����ͻ�ɫ���ο�
	winks_ui_fillRect( 0, 0, WINKS_SCREEN_WIDTH, WINKS_SCREEN_HEIGHT, WINKS_PIXEL_BLACK );
	winks_ui_fillRect( 2, 2, WINKS_SCREEN_WIDTH - 2, WINKS_SCREEN_HEIGHT - 21, WINKS_PIXEL_GRAY );

    for( i = 0; i < WK_WGAPI_MAXLAYER; i ++ )
    {
        /* �������ϣ������� */
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
                    /* ������滹�������㣬����ֻ����δ������һ�㸲�ǵĿؼ�,ͬʱ������������������� */
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
����ԭ��: Winks_Widget_ScnLayer_s* Winks_Widget_ScreenFindFirstLayer( Winks_Widget_ScnLayer_s* pScnLayer )
�������ܣ�
        ��������������һ����������������ϲ�Ľ���㡣
����˵����
        ��������	��������	                ��������	                ��ע
        pScnLayer	Winks_Widget_ScnLayer_s*	��Ҫ���ƵĽ��������ͷָ�롣	
��������ֵ��
		���Ƴɹ������ҵ��Ľ���㣬ʧ�ܷ���NULL��

ʵ��������
        ��������Ѱ��������Ҫ��ʾ�Ľ���㣨WK_WG_SLSHOW����Ȼ���ո�����ʾ������в�����Ϣ��layernum��
        �ҵ����ϲ�Ľ���㣬���ظ������ߡ�
\************************************************************************************/
Winks_Widget_ScnLayer_s* Winks_Widget_ScreenFindFirstLayer( Winks_Widget_ScnLayer_s* pScnLayer )
{
    Winks_Widget_ScnLayer_s* pret = NULL;

    pret = pScnLayer;
    while( pScnLayer )
    {
        if( !(pScnLayer->flag & (WK_WG_SLPOPUP | WK_WG_SLPOPMENU)) )
        {
            /* �ǵ�������Զ��Ҫ��ʾ */
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
����ԭ��: int Winks_Widget_ActiveLayer( Winks_Widget_ScnLayer_s* pScnLayer, int LayerID, int ifac )
�������ܣ�
        ���������������ȥ����һ����Ļ��ĳһ������㡣
����˵����
        ��������	��������	                ��������	                ��ע
        pScnLayer	Winks_Widget_ScnLayer_s*	��Ҫ���ƵĽ��������ͷָ�롣
        LayerID     int                         �����ID                    
        ifac        int                         �������ٴ���ȥ���
��������ֵ��
		���Ƴɹ�����0��ʧ�ܷ���-1��

ʵ��������
        �����������ȥ����ĳһ������㣬�ֲ�����ͬ��Ϊ���£�
        ����
            �ý����δ��ʾ���ý������ʾ���������ϲ㡣
            �ý������ʾ�У��رոý���������н���㣬��ǰ�����Ϊ���ϲ㡣
        ȥ����
            �ý����δ��ʾ���޶�����
            �ý������ʵ�У��رոý�����Լ��ý���������н���㣬�ý������һ��Ϊ���ϲ㡣
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
        /* ��������㣬�������޶��� */
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
����ԭ��: int Winks_Widget_ActiveWidget_Ex( Winks_Widget_s* pRootWidget, int WidgetID )
�������ܣ�
        ��������������һ���ؼ�������ĳһ���ؼ������ý��㣩��
����˵����
        ��������	��������	                ��������	                ��ע
        pRootWidget	    Winks_Widget_s*	�ؼ�����ͷָ�롣
        WidgetID    int                 �ؼ�ID                    
��������ֵ��
		���Ƴɹ�����0��ʧ�ܷ���-1��

ʵ��������
        ����������Ϊ�ؼ�����ĳһ���ؼ����ý��㣬�������Ŀؼ��Ѿ���ý��㣬�򱾺����޶���������
        ��ȥ��ԭ�н���Ŀؼ��Ľ���״̬���������Ƶ����ؼ���
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
����ԭ��: int Winks_Widget_ActiveWidget( Winks_Widget_ScnLayer_s* pLayer, int WidgetID )
�������ܣ�
        ��������������һ���������ĳһ���ؼ������ý��㣩��
����˵����
        ��������    ��������                    ��������                    ��ע
        pLayer      Winks_Widget_ScnLayer_s*    �ؼ����ڵĽ���㡣
        WidgetID    int                         �ؼ�ID                    
��������ֵ��
        ���Ƴɹ�����0��ʧ�ܷ���-1��

ʵ��������
        ����������Ϊ�������ĳһ���ؼ����ý��㣬�������Ŀؼ��Ѿ���ý��㣬�򱾺����޶���������
        ��ȥ��ԭ�н���Ŀؼ��Ľ���״̬���������Ƶ����ؼ���
\************************************************************************************/
int Winks_Widget_ActiveWidget( Winks_Widget_ScnLayer_s* pLayer, int WidgetID )
{
    if( pLayer == NULL )
        return -1;
    
    return Winks_Widget_ActiveWidget_Ex(pLayer->pWidget, WidgetID);
}

/* ���ҿؼ�������ָ��������������пؼ����� */
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

/* ���ҿؼ�������ָ��������������пؼ����� */
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

/* ���ҿؼ�������ָ����ָ���пؼ���ǰ��Ŀؼ�
   thiscolumn == WK_WGAPI_MAXMATRIXX - 1 ����������һ���ؼ�
   ����ʧ�ܷ���-1.
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

/* ���ҿؼ�������ָ����ָ���пؼ������Ŀؼ�
   thiscolumn == 0 ������ҵ�һ��һ���ؼ�
   ����ʧ�ܷ���-1.
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
����ԭ�ͣ�int Winks_Widget_ScreenNavKeyDeal( Winks_Widget_ScnLayer_s* pScnLayer, int KeyCode, int KeyType, 
                    Winks_Widget_s** ppLostFocus, Winks_Widget_s** pGetFocus )
�������ܣ�
            ������ͳһ����һ������ĵ�������Ϊ���������������ܵ������������������ᴦ�����ڵ�����
            ��ɵĿؼ������Ǩ�����ú���ʾ������ͬʱ���ᰴ�轫�ü�����Ϣ����ÿؼ�������Winks_Widget_KeyHandler����
            ����������ԭ��Ϊ����ʾ�Ľ��������ǰ�����һ���У�������ڸò�Ŀؼ�����������Ǩ�㣺
            ��ֵ	����	                                        ̧��	            ����
            ��	    �����ƶ�����һ��������<=��ǰ������Ŀؼ���      �޶�����            �ظ�������Ϊ��	
                    �統ǰΪ��һ�����޶�����	
            ��	    �����ƶ�����һ��������<=��ǰ������Ŀؼ���      �޶�����	        �ظ�������Ϊ��
                    �統ǰΪ���һ���޶�����	
            ��	    �����ƶ�������������+1�Ŀؼ������Ѿ��Ǳ���      �޶�����	        �ظ�������Ϊ��
                    ���һ���ؼ����ƶ�����һ�е�һ���ؼ���	
            ��	    �����ƶ�������������-1�Ŀؼ������Ѿ��Ǳ���      �޶�����            �ظ�������Ϊ��	
                    ��һ���ؼ����ƶ�����һ�����һ���ؼ���	
            �ر�ģ����ĳһ�������Ͽؼ�����Ǩ�㲻��������ԭ�򣬽���������Լ����������������ɵ��ñ�������
����˵����
            ��������	��������	                ��������	                ��ע
            pScnLayer	Winks_Widget_ScnLayer_s*	��������������ͷָ�롣	
            KeyCode	    int	                        ��ֵ�����������¼�ֵ��
                                                        WINKS_KEY_UP   
                                                        WINKS_KEY_DOWN 
                                                        WINKS_KEY_LEFT 
                                                        WINKS_KEY_RIGHT	    
            KeyType	    int	                        �������͡�	
            ppLostFocus	Winks_Widget_s**	        ����ʧȥ����Ŀؼ���	
            ppGetFocus	Winsk_Widget_s**	        ���ػ�ý���Ŀؼ���	
��������ֵ��
		    ���Ƴɹ������㣬ʧ�ܷ���-1��

ʵ��������
            ��������Ѱ����ǰ��Ľ���㣬Ȼ���ոý�����пؼ�������Ϣȷ��ʧȥ����Ŀؼ��ͻ�ý���Ŀؼ���
            �����н����Ǩ���ƹ�����ͬʱ�������Ὣʧȥ����Ŀؼ��ͻ�ý���Ŀؼ���Ϊ�������ظ������ߡ�
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
        /* KeyUp�޶��� */
        Winks_printf( "Winks Widget NavKeyDeal get keyup, return\r\n" );
        return 0;
    }

    if( (ret == WK_UIFM_FOCUSCHANGE) || (ret == WK_UIFM_FCCHANGE) )
    {
        /* ������Ҫ�����仯 */
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
            /* ֻ��һ���ɻ�ý���Ŀؼ���ֱ�ӷ��� */
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
                /* ֻ��һ�л�������һ�� */
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
                /* ֻ��һ�л�������һ�� */
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
        /* ������¿ؼ�����˽��㣬��������� */
        Winks_Widget_StateModify( pWidget, WK_WG_FGFOCUS, 0 );
        Winks_Widget_StateModify( pTmpWidget, WK_WG_FGFOCUS, 1 );
    }

    if( ppLostFocus )
        *ppLostFocus = pWidget;
    if( ppGetFocus )
        *ppGetFocus = pTmpWidget;

    return ret;
}

/* �ж�һ�����ǲ�����һ�������� */
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
����ԭ�ͣ�int Winks_Widget_ScreenPenDeal( Winks_Widget_ScnLayer_s* pScnLayer, int PenType, int x, int y, Winks_Widget_s** ppLostFocus, Winks_Widget_s** pGetFocus )
�������ܣ�
        ������ͳһ����һ������Ĵ�������Ϊ�������������ᴦ�����ڴ�������ɵĿؼ������Ǩ�����ú���ʾ������
        ͬʱ���ᰴ�轫����Ϣ����ÿؼ�������Winks_Widget_PenHandler����ͨ�������ʲ���������ɽ���Ǩ�㣬
        �����ܻ�����������������Щ����Ӧ�ɽ�������Լ�����
        ����������ԭ��Ϊ����ʾ�Ľ��������ǰ�����һ���У�ͨ���жϴ��������ͺ͵㴥λ���жϿؼ������Ǩ�㡣
        ������	        ���㲻�����ϲ�������	���㲻�����ϲ��κοؼ�������	���������ϲ�ĳһ�ؼ�������
        PENDOWN	        �޶�����	            �޶�����	                    �����������ؼ���
        PENUP	        �޶�����	            �޶�����	                    �޶�����
        PENMOVE	        �޶�����	            �޶�����	                    �����������ؼ���
        PENREPEAT	    �޶�����	            �޶�����	                    �ظ�PENDOWN��Ϊ��
        PENLONGPRESS	�޶�����	            �޶�����	                    �ظ�PENDOWN��Ϊ��
        �ر�ģ����ĳһ�������Ͽؼ�����Ǩ�㲻��������ԭ�򣬽���������Լ�����������Ϣ�������ɵ��ñ�������
����˵����
        ��������	��������	                ��������	                ��ע
        pScnLayer	Winks_Widget_ScnLayer_s*	��������������ͷָ�롣	
        PenType	    int	                        �������͡�	
        x	        int	                        ��������ꡣ	
        y	        int	                        ���������ꡣ	
        ppLostFocus	Winks_Widget_s**	        ����ʧȥ����Ŀؼ���	
        ppGetFocus	Winsk_Widget_s**	        ���ػ�ý���Ŀؼ���	
��������ֵ��
		���Ƴɹ������㣬ʧ�ܷ���-1��

ʵ��������
        ��������Ѱ����ǰ��Ľ���㣬Ȼ���ոý�����пؼ�������Ϣȷ��ʧȥ����Ŀؼ��ͻ�ý���Ŀؼ��������н���
        ��Ǩ���ƹ�����ͬʱ�������Ὣʧȥ����Ŀؼ��ͻ�ý���Ŀؼ���Ϊ�������ظ������ߡ�
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
            /* ����δ�����κοؼ��������ǵ���ʽ��ʾ��ı�Ե */
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
                /* ����¿ؼ��ǿ��Ի�ý���ģ���Ҫ�������� */
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
����ԭ�ͣ�Winks_Widget_s* Winks_Widget_GetFocusWidget( Winks_Widget_s* pWidget )
�������ܣ���������������ĳ�ؼ������л�ý���Ŀؼ���

����˵����
        ��������	��������	    ��������	    ��ע
        pWidget	    Winks_Widget_s*	�ؼ�����ͷָ��	

��������ֵ��
		�ɹ����ػ�ý���Ŀؼ�ָ�룬ʧ�ܷ���NULL��

ʵ��������
        �������ڿؼ��������ҵ���ý���Ŀؼ�������ָ�롣�����ƻ���һ���ؼ�������ͬһʱ��ֻ����һ���ؼ�
        ��ý��㡣���������棨�����ж����ý���Ŀؼ��Ľ��棩�Ļ����������Ӧ�Լ����ң������򷵻�ֵ
        ����Ԥ�ڡ�
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
����ԭ�ͣ�Winks_Widget_s* Winks_Widget_GetTypeWidget( Winks_Widget_s* pWidget, int WidgetType )
�������ܣ�
        ��������������ĳ�ؼ��������ض����͵Ŀؼ������ǽ������ȷ�ϱ��ؼ�������ֻ��һ�������͵Ŀؼ���
        ���򲻿�ʹ�ñ��������ҡ����һ���ؼ������л��ж��ͬһ���͵Ŀؼ�������������ֵ����Ԥ�ڡ�
        һ����˵�����������������������������֮��Ľ����Ψһ�Ŀؼ���

����˵����
        ��������	��������	    ��������	        ��ע
        pWidget	    Winks_Widget_s*	�ؼ�����ͷָ��	
        WidgetType	int	�ؼ�����	WK_WG_LEFTSOFTKEY��

��������ֵ��
		�ɹ����ػ�ý���Ŀؼ�ָ�룬ʧ�ܷ���NULL��

ʵ��������
        �������ڿؼ��������ҵ�ָ�����Ϳؼ�������ָ�롣�����ƻ���һ���ؼ�������ͬһʱ��ֻ����һ��
        ָ�����Ϳؼ��������������ж��ָ�����͵Ŀؼ��Ļ����������Ӧ�Լ����ң������򷵻�ֵ����Ԥ�ڡ�
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
����ԭ�ͣ�Winks_Widget_s* Winks_Widget_WIdgetAlloc( int type, int lux, int luy, int width, int high, 
            int xindex, int yindex, int allocsize )
�������ܣ�
        ���������������������Ϊÿһ�ֿؼ������ڴ沢����һЩ������Ա�ĸ�ֵ������
        ������Ŀ����Ϊ�˼򻯱�̣����ʹ�������
        ���ںܶ�ʱ��һ���ؼ���С���ܻᳬ���ÿؼ��ṹ�Ĵ�С��������пɱ��ַ���
        ���ԵĿؼ����ڴ����ÿؼ��ڴ�ռ�ʱӦԤ�Ƚ���δ�СҲ������ؼ��ṹ����
        �˱��������뵽�Ŀؼ��ڴ��С�ɵ�����ָ��(allocsize)��

����˵����
        ��������	��������	��������	            ��ע
        type	    int	        �ؼ�����	
        lux	        int	        �ؼ����Ͻ�����x	
        luy	        int	        �ؼ����Ͻ�����y	
        width	    int	        �ؼ����	
        high	    int	        �ؼ��߶�	
        xindex	    int	        �ڸò�ؼ������еĺ����ꡣ	
        yindex	    int	        �ڸò�ؼ������е������ꡣ	
        allocsize	int	        �ṹ����	

��������ֵ��
		�ɹ�������������Ŀؼ�ָ�룬ʧ�ܷ���NULL��

ʵ��������
        ���������û�ָ�����ڴ��С�����ڴ棬�����û�����Ĳ�����д������Ա���ݡ�
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
����ԭ�ͣ�Winks_Widget_TapTitle_s* Winks_Widget_GetTapTitle( void )
�������ܣ�
        ���ڵ�ǰ���������ÿһ����������ķ�ҳ������Ӧ����ͬһ����������ȫ�ִ��ڣ���˷�ҳ��������
        �ؼ���������ڴ沢��ʼ����ÿ��������Ҫ���Ʒ�ҳ������ʱֻ��ͨ����������ȡ�÷�ҳ������ʹ�ü��ɡ�
        �ؼ���ܽ��������ҳ�������ڴ�����ͳ�ʼ��������������Ǩ����ҳ�����������ı仯��ÿһҳ��Ӧ
        ����ĻID�͵�ǰ������ҳ�����ȣ������ɽ��渺�������
        ���⣬��������Ҫ�ͷű�����ؼ�����ʱ��ǵò����ͷű��ṹ��

����˵����
        �ޡ�
��������ֵ��
		�ɹ����ط�ҳ�������ؼ�ָ�룬ʧ�ܷ���NULL��

ʵ��������
        ���������ؿ�ܱ�����ȫ�ַ�ҳ���������ݡ�
\************************************************************************************/
Winks_Widget_TapTitle_s* Winks_Widget_GetTapTitle( void )
{
    Winks_Widget_Global_s* pGlobal = WINKS_WDGETGLOBALVAR;    

    return ( Winks_Widget_TapTitle_s * )&(pGlobal->TapTitle);
}

/************************************************************************************\
����ԭ�ͣ�Winks_Widget_ScnLayer_s* Winks_Widget_FindLayerByID( Winks_Widget_ScnLayer_s* pHead, int ID )
�������ܣ�
        ͨ�������ID���ҽ���㡣

����˵����
        ��������	��������	                ��������	            ��ע
        pHead       Winks_Widget_ScnLayer_s*    �����ͷָ��
        ID	        int	                        �����ID(LayerID)	

��������ֵ��
		�ɹ������ҵ��Ľ����ָ�룬ʧ�ܷ���NULL��

ʵ��������
        ����������ָ��ID�Ľ���㡣
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
����ԭ�ͣ�Winks_Widget_s* Winks_Widget_FindWidgetByID( Winks_Widget_ScnLayer_s* pHead, int ID )
�������ܣ�
        ͨ���ؼ�ID���ҿؼ��������һ���������Ļ���н�����в��ң����������ȷ����ID��ĻΨһ��

����˵����
        ��������	��������	                ��������	            ��ע
        pHead       Winks_Widget_ScnLayer_s*    �����ͷָ��
        ID	        int	                        �ؼ�ID(WidgetID)	

��������ֵ��
		�ɹ������ҵ��Ľ����ָ�룬ʧ�ܷ���NULL��

ʵ��������
        ����������ָ��ID�Ŀؼ���
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
����ԭ�ͣ�Winks_Widget_ScnLayer_s* Winks_Widget_FindLayerByWidget( Winks_Widget_ScnLayer_s* pHead, int ID )
�������ܣ�
        ͨ���ؼ�ID�����������Ľ���㡣

����˵����
        ��������	��������	                ��������	            ��ע
        pHead       Winks_Widget_ScnLayer_s*    �����ͷָ��
        ID	        int	                        �ؼ�ID(WidgetID)	

��������ֵ��
		�ɹ������ҵ��Ľ����ָ�룬ʧ�ܷ���NULL��

ʵ��������
        ����������ָ���Ľ���㡣
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
