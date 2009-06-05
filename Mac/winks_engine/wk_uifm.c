
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_engin.h"
#include "wk_uifm.h"
#include "wk_uifml.h"

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_UIFMGETGLOBALVAR (get_module_glob(WK_MODULE_UI))
#else
Winks_UIFM_Global_s Winks_UIFM_Global_GB;
#define WINKS_UIFMGETGLOBALVAR (&Winks_UIFM_Global_GB)
#endif

#ifdef WINKS_DEBUG
static int winks_uifm_GetTotalcachesize()
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0, ret = 0;
    
    for( i = 0; i < WK_UIFM_MAXWDCACHENUM; i++ )
    {
        if( pGlobal->WdCache.Item[i].ScreenID )
        {
            ret += pGlobal->WdCache.Item[i].size;
        }
    }

    Winks_assert( ret == pGlobal->WdCache.CurrSize );
    return ret;
}
#endif
static int winks_uifm_Handle( unsigned long msg, void *data, unsigned long size );

static Winks_UIFM_ScreenList_s* winks_uifm_GetScreenList( int ScreenID )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0;
    
    if( ScreenID <= 0 || ScreenID > WK_UIFM_MAXSCREENNUM )
        return NULL;

    for( i = 0; i < WK_UIFM_MAXSCREENNUM; i++ )
    {
        if( pGlobal->ScreenList[i].ScreenID == ScreenID )
        {
            return &(pGlobal->ScreenList[i]);
        }
    }

    return NULL;
}

static Winks_UIFM_WdCacheItem_s* winks_uifm_GetWidgetCache( int ScreenID )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0;
    
    if( ScreenID <= 0 || ScreenID > WK_UIFM_MAXSCREENNUM )
        return NULL;

    for( i = 0; i < WK_UIFM_MAXWDCACHENUM; i++ )
    {
        if( pGlobal->WdCache.Item[i].ScreenID == ScreenID )
        {
            return &(pGlobal->WdCache.Item[i]);
        }
    }

    return NULL;
}

static int winks_uifm_FreeWidgetCache( int i )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    Winks_UIFM_ScreenList_s* pSList = NULL;
#ifdef WINKS_DEBUG
    int size = 0;
#endif

    if( pGlobal->WdCache.Item[i].ScreenID == 0 )
        return -1;

    pSList = winks_uifm_GetScreenList( pGlobal->WdCache.Item[i].ScreenID );
    pSList->pfExit( &(pGlobal->WdCache.Item[i].pLayer), WK_UIFM_OPFREE, 0, 0 );
    pGlobal->WdCache.CurrSize -= pGlobal->WdCache.Item[i].size;
#ifdef WINKS_DEBUG
    size = pGlobal->WdCache.Item[i].size;
#endif
    Winks_assert( pGlobal->WdCache.CurrSize >= 0 );
    Winks_mem_set( &(pGlobal->WdCache.Item[i]), 0, sizeof(Winks_UIFM_WdCacheItem_s) );
    Winks_printf( "Winks UIFM cache %d free widget cache, miner %d, real size %d", pGlobal->WdCache.CurrSize, 
        size, winks_uifm_GetTotalcachesize() );

    return 0;
}

static int winks_uifm_DefaultValue()
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;

    pGlobal->ScreenList[0].ScreenID = WINKS_UI_UNACTIVATED_ID;
    pGlobal->ScreenList[0].pfEnter = Winks_UI_Enter_UnActivated;
    pGlobal->ScreenList[0].pfExit = Winks_UI_Exit_UnActivated;

    pGlobal->ScreenList[1].ScreenID = WINKS_UI_MAINMENU_ID;
    pGlobal->ScreenList[1].pfEnter = Winks_UI_Enter_MainMenu;
    pGlobal->ScreenList[1].pfExit = Winks_UI_Exit_MainMenu;

    pGlobal->ScreenList[2].ScreenID = WINKS_UI_ABOUT_ID;
    pGlobal->ScreenList[2].pfEnter = Winks_UI_Enter_About;
    pGlobal->ScreenList[2].pfExit = Winks_UI_Exit_About;

    pGlobal->ScreenList[3].ScreenID = WINKS_UI_HELP_ID;
    pGlobal->ScreenList[3].pfEnter = Winks_UI_Enter_Help;
    pGlobal->ScreenList[3].pfExit = Winks_UI_Exit_Help;

    pGlobal->ScreenList[4].ScreenID = WINKS_UI_DIY_ID;
    pGlobal->ScreenList[4].pfEnter = Winks_UI_Enter_Diy;
    pGlobal->ScreenList[4].pfExit = Winks_UI_Exit_Diy;

    pGlobal->ScreenList[5].ScreenID = WINKS_UI_NORMALSETTING_ID;
    pGlobal->ScreenList[5].pfEnter = Winks_UI_Enter_NormalSetting;
    pGlobal->ScreenList[5].pfExit = Winks_UI_Exit_NormalSetting;

    pGlobal->ScreenList[6].ScreenID = WINKS_UI_SHOWSETTING_ID;
    pGlobal->ScreenList[6].pfEnter = Winks_UI_Enter_ShowSetting;
    pGlobal->ScreenList[6].pfExit = Winks_UI_Exit_ShowSetting;

    pGlobal->ScreenList[7].ScreenID = WINKS_UI_ADVANCESETTING_ID;
    pGlobal->ScreenList[7].pfEnter = Winks_UI_Enter_AdvanceSetting;
    pGlobal->ScreenList[7].pfExit = Winks_UI_Exit_AdvanceSetting;

    pGlobal->ScreenList[8].ScreenID = WINKS_UI_UPDATESETTING_ID;
    pGlobal->ScreenList[8].pfEnter = Winks_UI_Enter_UpdateSetting;
    pGlobal->ScreenList[8].pfExit = Winks_UI_Exit_UpdateSetting;

    pGlobal->ScreenList[9].ScreenID = WINKS_UI_ADDFRIENDS_ID;
    pGlobal->ScreenList[9].pfEnter = Winks_UI_Enter_AddFriends;
    pGlobal->ScreenList[9].pfExit = Winks_UI_Exit_AddFriends;

    pGlobal->ScreenList[10].ScreenID = WINKS_UI_FRIENDSPREVIEW_ID;
    pGlobal->ScreenList[10].pfEnter = Winks_UI_Enter_FriendsPreview;
    pGlobal->ScreenList[10].pfExit = Winks_UI_Exit_FriendsPreview;

    pGlobal->ScreenList[11].ScreenID = WINKS_UI_APN_ID;
    pGlobal->ScreenList[11].pfEnter = Winks_UI_Enter_Apn;
    pGlobal->ScreenList[11].pfExit = Winks_UI_Exit_Apn;

	pGlobal->ScreenList[12].ScreenID = WINKS_UI_MYLIBRARY_ID;
	pGlobal->ScreenList[12].pfEnter = Winks_UI_Enter_MyLibrary;
	pGlobal->ScreenList[12].pfExit = Winks_UI_Exit_MyLibrary;

	pGlobal->ScreenList[13].ScreenID = WINKS_UI_WINKSSTORE_ID;
	pGlobal->ScreenList[13].pfEnter = Winks_UI_Enter_WinksStore;
	pGlobal->ScreenList[13].pfExit = Winks_UI_Exit_WinksStore;

	pGlobal->ScreenList[14].ScreenID = WINKS_UI_SETWINKS_ID;
	pGlobal->ScreenList[14].pfEnter = Winks_UI_Enter_SetWinks;
	pGlobal->ScreenList[14].pfExit = Winks_UI_Exit_SetWinks;

	pGlobal->ScreenList[15].ScreenID = WINKS_UI_SHARE_ID;
	pGlobal->ScreenList[15].pfEnter = Winks_UI_Enter_Share;
	pGlobal->ScreenList[15].pfExit = Winks_UI_Exit_Share;

	pGlobal->ScreenList[16].ScreenID = WINKS_UI_LOADING_ID;
	pGlobal->ScreenList[16].pfEnter = Winks_UI_Enter_Loading;
	pGlobal->ScreenList[16].pfExit = Winks_UI_Exit_Loading;
    /*
    pGlobal->ScreenList[12].ScreenID = WINKS_UI_CCSHOW_ID;
    pGlobal->ScreenList[12].pfEnter = NULL;
    pGlobal->ScreenList[12].pfExit = NULL;
    */

    return 0;

}
/************************************************************************************\
函数原型：int Winks_UIFM_Init( void )
函数介绍：
        本函数初始化UI框架。
        首先初始化全局数据结构，特别的，全局全部界面及对应函数列表会在这里初始化。同时也许初始化控件框架。
        然后创建UI消息通道，等待消息到来。

参数说明：
			
函数返回值：
		成功返回0，失败返回-1。
\************************************************************************************/
int Winks_UIFM_Init( void )
{
	Winks_UIFM_Global_s* pGlobal = NULL;

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    if( ((pGlobal = get_module_glob(WK_MODULE_UI)) != NULL)
        && pGlobal->ifinit )
    {
        Winks_printf( "Winks UIFM init have inited\r\n" );
        return -1;
    }
    if( pGlobal == NULL )
    {
        if( (pGlobal = Winks_salAlloc( sizeof(Winks_UIFM_Global_s) )) == NULL )
        {
            Winks_printf( "Winks UIFM init alloc memory failure\r\n" );
            return -1;
        }
    }
	set_module_glob( WK_MODULE_UI, pGlobal );
#else
	pGlobal = &Winks_UIFM_Global_GB;
    if( pGlobal->ifinit )
    {
        Winks_printf( "Winks UIFM init have inited\r\n" );
        return -1;
    }
#endif

	Winks_mem_set( pGlobal, 0, sizeof(Winks_UIFM_Global_s) );

	winks_uifm_DefaultValue();

	pGlobal->channel = Winks_CreateChn( winks_uifm_Handle );

	Winks_assert( pGlobal->channel != WINKS_CHN_NULL );

	Winks_RegisterUiChn( pGlobal->channel );
	
	//Winks_SetWaitingEvent( winks_ui_chn, 0x3f );/*WINKS_EVENT_CALL_COMMING*/
	//Winks_SetWaitingEvent( pGlobal->channel, WINKS_EVENT_OF_ALL_CALL );/*WINKS_EVENT_CALL_COMMING*/
    //Winks_AddWaitingEvent( pGlobal->channel, WINKS_EVENT_USB_DETECT);
    Winks_SetWaitingEvent( pGlobal->channel, WINKS_EVENT_USB_DETECT);

	//pGlobal->flag |= WK_UIFM_FGNORMAL;

    pGlobal->WdCache.TotalSize = WK_UIFM_MAXCACHESIZE;

    Winks_Widget_Init();
    
    //Winks_UI_InitCCShow();

    return 0;

}
/************************************************************************************\
函数原型：int Winks_UIFM_DeInit( void )
函数介绍：
        本函数去初始化UI框架。
        首先去初始化控件框架，删除所有相关信息，关闭消息通道，关闭定时器，删除控件缓存中内存。
        然后初始化全局结构。

参数说明：
			
函数返回值：
		成功返回0，失败返回-1。
\************************************************************************************/
int Winks_UIFM_DeInit( void )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0;

    for( i = 0; i < WK_UIFM_MAXTIMERNUM; i++ )
    {
        Winks_DestroyTimer( pGlobal->Timer[i].ID );
    }

    for( i = 0; i < WK_UIFM_MAXWDCACHENUM; i++ )
    {
        winks_uifm_FreeWidgetCache( i );
    }

    Winks_DestroyChn( pGlobal->channel );

    Winks_Widget_DeInit( );

    //Winks_UI_DeInitCCShow(  );

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    Winks_salFree( pGlobal );
#else
    Winks_mem_set( pGlobal, 0, sizeof(Winks_UIFM_Global_s) );
#endif

    return 0;

}

static Winks_UIFM_CurrentScreen_s* winks_uifm_GetCurrScreen()
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;

    if( pGlobal->flag & WK_UIFM_FGCCSHOW )
        return &(pGlobal->CCurrScreen);

    return &(pGlobal->NCurrScreen);
}

/************************************************************************************\
函数原型：int Winks_UIFM_EnterNewScreen( int ScreenID, void* Param )
函数介绍：
		本函数请求UI框架绘制ID为ScreenID的界面。

参数说明：
        参数名称	参数类型	参数介绍	备注
        ScreenID	int	        屏幕ID	
函数返回值：
		成功返回0，失败返回-1。

使用说明：
		本函数首先会调用当前界面的退出函数，然后清空当前屏幕全局信息，包括：
        1.	当前屏幕ID；
        2.	当前屏幕参数；
        3.	全部键盘回调函数；
        4.	触摸笔回调函数。
        5.	挂起回调函数；
        6.	恢复回调函数；
        7.	Paint回调函数。

        而后调用ScreenID的屏幕的进入函数，将当前屏幕ID置位为ScreenID。
\************************************************************************************/
int Winks_UIFM_EnterNewScreen( int ScreenID, void* Param )
{
    Winks_UIFM_ScreenList_s* pSList = NULL;
    Winks_UIFM_WdCacheItem_s* pCache = NULL;
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    
    pScreen = winks_uifm_GetCurrScreen();

    if( pScreen->ScreenID )
    {
        pSList = winks_uifm_GetScreenList( pScreen->ScreenID );
        if( (pCache = winks_uifm_GetWidgetCache( pScreen->ScreenID )) == NULL )
        {
            Winks_fail( "Winks UIFM get current widget cache failure" );
            return -1;
        }
        pSList->pfExit( &(pCache->pLayer), WK_UIFM_OPEXIT, pScreen->wParam, pScreen->lParam );
        if( pCache->pLayer == NULL )
        {
            /* 界面删除了控件链表，我们清空这个项，因为是清除的当前界面，肯定在最上面，不需移动位置 */
            pGlobal->WdCache.CurrSize -= pCache->size;
            pCache->ScreenID = 0;
            pCache->size = 0;
            Winks_printf( "Winks UIFM cache %d enternewscreen delete layer, miner %d, real %d", pGlobal->WdCache.CurrSize,
                pCache->size, winks_uifm_GetTotalcachesize() );
        }
    }

    Winks_mem_set( pScreen, 0, sizeof(Winks_UIFM_CurrentScreen_s) );
//#ifdef WINKS_DEBUG
//     WINKS_BUFFER_ON;
//     winks_ui_fillRect( 0, 0, WINKS_SCREEN_WIDTH, WINKS_SCREEN_HEIGHT, WINKS_PIXEL_BLACK );
//     WINKS_BUFFER_OFF;
//#endif
    pScreen->ScreenID = ScreenID;
    pSList = winks_uifm_GetScreenList( ScreenID );
    pSList->pfEnter( (int)Param );

    return 0;

}
/************************************************************************************\
函数原型：int Winks_UIFM_SetCurrentScreenParam( unsigned long* pwParam, unsigned long* plParam )
函数介绍：
		本函数设置当前界面屏幕参数。
	    为了便于适应各种不同平台，每一个界面不允许有自己的全局变量。UI框架可以帮助当前界面保留两个32位
        整形的参数。当前界面可将自己的一些全局信息（例如本界面的状态值等）保留在这里。

参数说明：
        参数名称	参数类型	    参数介绍	        备注
        pwParam	    unsigned long*	当前界面第一参数	为空代表不改变。
        plParam	    unsigned long*	当前界面第二参数	为空代表不改变。
函数返回值：
		成功返回0，失败返回-1。

使用说明：
        本函数用来设置当前屏幕两个参数，调用者可以用空指针表示不修改某个参数从而实现仅仅设置某一个参数的功能。
\************************************************************************************/
int Winks_UIFM_SetCurrentScreenParam( unsigned long* pwParam, unsigned long* plParam )
{
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;
    
    pScreen = winks_uifm_GetCurrScreen();

    if( pwParam )
        pScreen->wParam = *pwParam;
    if( plParam )
        pScreen->lParam = *plParam;

    return 0;
}

/************************************************************************************\
函数原型：int Winks_UIFM_GetCurrentScreenParam( int* pScreenID, unsigned long* pwParam, unsigned long* plParam )
函数介绍：
		本函数获取当前界面屏幕参数。
	    为了便于适应各种不同平台，每一个界面不允许有自己的全局变量。UI框架可以帮助当前界面保留两个32位整形的参数。
        当前界面可将自己的一些全局信息（例如本界面的状态值等）保留在这里。

参数说明：
        参数名称	参数类型	    参数介绍	        备注
        pScreenID	int*	        当前屏幕ID	        为空代表不关心。
        pwParam	    unsigned long*	当前界面第一参数	为空代表不关心。
        plParam	    unsigned long*	当前界面第二参数	为空代表不关心。
函数返回值：
		成功返回0，失败返回-1。

使用说明：
        本函数用来获取当前屏幕ID和两个参数，调用者可以用空指针表示不关心某个参数从而实现仅仅获取某几个参数的功能。
\************************************************************************************/
int Winks_UIFM_GetCurrentScreenParam( int* pScreenID, unsigned long* pwParam, unsigned long* plParam )
{
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;
    
    pScreen = winks_uifm_GetCurrScreen();

    if( pScreenID )
        *pScreenID = pScreen->ScreenID;
    if( pwParam )
        *pwParam = pScreen->wParam;
    if( plParam )
        *plParam = pScreen->lParam;

    return 0;
}

/************************************************************************************\
函数原型：Winks_Widget_ScnLayer_s* Winks_UIFM_GetScreenLayer( int ScreenID )
函数介绍：
		本函数获取某一界面界面层参数。即获得该界面所有控件链表信息。
	    为了控制内存消耗并增加程序运行效率，UI框架提供一套界面控件链表缓存机制。
	    每当某一个界面进入函数被调用时，界面程序都需首先试图在控件链表缓存中寻找本界面的控件链表，
        如果找不到再进行本界面控件链表的创建，并将创建出来的链表设置入控件链表缓存中。设置函数见下。

参数说明：
        参数名称	参数类型	参数介绍	备注
        ScreenID	int 	    屏幕ID	    为0代表当前屏幕。
函数返回值：
		成功返回指定界面界面层信息头指针，失败返回NULL。

使用说明：
        无。
\************************************************************************************/
Winks_Widget_ScnLayer_s* Winks_UIFM_GetScreenLayer( int ScreenID )
{
    Winks_UIFM_WdCacheItem_s* pCache = NULL;
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;

    if( ScreenID == 0 )
    {
        pScreen = winks_uifm_GetCurrScreen();
        ScreenID = pScreen->ScreenID;
    }

    pCache = winks_uifm_GetWidgetCache( ScreenID );
    if( pCache != NULL )
        return pCache->pLayer;

    return NULL;

}
/************************************************************************************\
函数原型：int Winks_UIFM_SetScreenLayer( int ScreenID, Winks_Widget_ScnLayer_s* pScnLayer, int memlen )
函数介绍：
		本函数设置某一界面界面层参数。即设置该界面所有控件链表信息。
	    为了控制内存消耗并增加程序运行效率，UI框架提供一套界面控件链表缓存机制。
	    每当某一个界面进入函数被调用时，界面程序都需首先试图在控件链表缓存中寻找本界面的控件链表，
        如果找不到再进行本界面控件链表的创建，并将创建出来的链表使用本函数设置入控件链表缓存中。
        以便下次进入本屏幕时直接获取使用。

参数说明：
        参数名称	参数类型	                参数介绍	                备注
        ScreenID	int	                        屏幕ID	                    为0代表当前屏幕。
        pScnLayer	Winks_Widget_ScnLayer_s*	本界面的界面层头指针。	
        memlen	    int	                        整个控件链表占用的内存空间。	
函数返回值：
        成功返回0，失败返回-1。如果该界面的控件链表在缓存中已经存在，则本次设置会覆盖原来的设置。
        特别的，由于此时需要请求删除原来设置的控件链表占用的内存空间，该界面的退出函数会被以
        WK_UIFM_OPFREE为参数调用。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_SetScreenLayer( int ScreenID, Winks_Widget_ScnLayer_s* pScnLayer, int memlen )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0, j = 0, iffull = 0;
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;

    if( ScreenID == 0 )
    {
        pScreen = winks_uifm_GetCurrScreen();
        ScreenID = pScreen->ScreenID;
    }

    for( i = 0; i < WK_UIFM_MAXWDCACHENUM; i++ )
    {
        if( pGlobal->WdCache.Item[i].ScreenID == 0 )
        {
            /* 找到空，后面一定都是空 */
            break;
        }
        if( pGlobal->WdCache.Item[i].ScreenID == ScreenID )
        {
            /* 有本屏幕信息，删除旧的 */
            winks_uifm_FreeWidgetCache( i );
        }
    }
    if( i == WK_UIFM_MAXWDCACHENUM )
    {
        iffull = 1;
    }

    i = 0;

    while( ((pGlobal->WdCache.CurrSize + memlen - pGlobal->WdCache.TotalSize) > 0) || iffull )
    {
        winks_uifm_FreeWidgetCache( i );
        i++;
        if( pGlobal->WdCache.CurrSize == 0 )
        {
            /* 全部清空还是不够，我们增加总大小，确保本次添加成功 */
            pGlobal->WdCache.TotalSize = memlen;
            break;
        }
        iffull = 0;
    }

    /* 删除空洞 */
    for( i = 0; i < WK_UIFM_MAXWDCACHENUM - 1; i++ )
    {
        if( pGlobal->WdCache.Item[i].ScreenID == 0 )
        {
            for( j = i + 1; j < WK_UIFM_MAXWDCACHENUM; j++ )
            {
                if( pGlobal->WdCache.Item[j].ScreenID != 0 )
                {
                    pGlobal->WdCache.Item[i].ScreenID = pGlobal->WdCache.Item[j].ScreenID;
                    pGlobal->WdCache.Item[i].size = pGlobal->WdCache.Item[j].size;
                    pGlobal->WdCache.Item[i].pLayer = pGlobal->WdCache.Item[j].pLayer;
                    Winks_mem_set( &(pGlobal->WdCache.Item[j]), 0, sizeof(Winks_UIFM_WdCacheItem_s) );
                    break;
                }
            }
            if( j == WK_UIFM_MAXWDCACHENUM )
                break;
        }
    }

    pGlobal->WdCache.CurrSize += memlen;
    pGlobal->WdCache.Item[i].ScreenID = ScreenID;
    pGlobal->WdCache.Item[i].size = memlen;
    pGlobal->WdCache.Item[i].pLayer = pScnLayer;
    Winks_printf( "Winks UIFM cache %d set screen layer, add %d, real %d", pGlobal->WdCache.CurrSize,
        memlen, winks_uifm_GetTotalcachesize() );

    Winks_assert( pGlobal->WdCache.CurrSize <= pGlobal->WdCache.TotalSize );
    return 0;

}

/************************************************************************************\
函数原型：const char* Winks_UIFM_GetString( int StringID )
函数介绍：
		本函数按照当前语言获取字符串内容数据。
	

参数说明：
        参数名称	参数类型	参数介绍	备注
        StringID	int	        字符串ID	
函数返回值：
		成功返回对应的字符串数据，失败返回NULL。

使用说明：
无。
\************************************************************************************/
#include "wk_ui_str_English.h"
#include "wk_ui_str_SChinese.h"
const char* Winks_UIFM_GetString( unsigned long StringID )
{
    switch (Winks_GetCurrentLanguage())
    {
        case 0:
        return (char *)&str_en[str_en_offset[StringID - 1]];  
            break;                //english
        case 1:
            return (char *)&str_sc[str_sc_offset[StringID - 1]];  //chinese
            break;
        default:
            return (char *)&str_en[str_en_offset[StringID - 1]];  
            break;
    }
}

#define WINKS_IMAGE_TYPE                "png"

const static char *Winks_Image_a[100] = 
{
    // 未激活页面
    "UNACTIVATED_BTN."WINKS_IMAGE_TYPE,                                    // 未激活页面按钮背景
    "UNACTIVATED_BTNHL."WINKS_IMAGE_TYPE,                                  // 未激活页面按钮高亮
    "UNACTIVATED_PICFRM."WINKS_IMAGE_TYPE,                                 // 未激活页面展示图片边框
    // 主页面
    "MAINMENU_ICON1."WINKS_IMAGE_TYPE,                                     // 主界面图标1
    "MAINMENU_ICON2."WINKS_IMAGE_TYPE,                                     // 主界面图标2
    "MAINMENU_ICON3."WINKS_IMAGE_TYPE,                                     // 主界面图标3
    "MAINMENU_ICON4."WINKS_IMAGE_TYPE,                                     // 主界面图标4
    "MAINMENU_ICON5."WINKS_IMAGE_TYPE,                                     // 主界面图标5
    "MAINMENU_ICON6."WINKS_IMAGE_TYPE,                                     // 主界面图标6
    "MAINMENU_HLBKG."WINKS_IMAGE_TYPE,                                     // 主界面高亮图标背景
    // 普通设置页面
    "NORMALSETTING_ITEMBKG."WINKS_IMAGE_TYPE,                              // 普通设置列表项背景
    "NORMALSETTING_ITEMHL."WINKS_IMAGE_TYPE,                               // 普通设置列表项高亮背景
    "NORMALSETTING_ICON1."WINKS_IMAGE_TYPE,                                // 普通设置图标1
    "NORMALSETTING_ICON2."WINKS_IMAGE_TYPE,                                // 普通设置图标2
    "NORMALSETTING_ICON3."WINKS_IMAGE_TYPE,                                // 普通设置图标3
    "NORMALSETTING_ICON4."WINKS_IMAGE_TYPE,                                // 普通设置图标4
    // 高级设置页面
    "ADVANCESETTING_GPITEMBKG."WINKS_IMAGE_TYPE,                           // 高级设置组列表项背景
	"ADVANCESETTING_GPITEMBKG2."WINKS_IMAGE_TYPE,                          // 高级设置组列表项背景2
	"ADVANCESETTING_GPITEMBKG3."WINKS_IMAGE_TYPE,                          // 高级设置组列表项背景3
    "ADVANCESETTING_ITEMBKG."WINKS_IMAGE_TYPE,                             // 高级设置列表项背景
    "ADVANCESETTING_ITEMHL."WINKS_IMAGE_TYPE,                              // 高级设置列表项高亮背景
    // 更新设置页面
    "UPDATESETTING_ITEMHL."WINKS_IMAGE_TYPE,                               // 更新设置列表项高亮背景
    "UPDATESETTING_SELBLOCK."WINKS_IMAGE_TYPE,                             // 更新设置滑块
	"UPDATESETTING_GLIDEPOLE."WINKS_IMAGE_TYPE,                            // 更新设置滑竿
	"UPDATESETTING_GLIDEPOLEHL."WINKS_IMAGE_TYPE,                          // 更新设置高亮滑竿
    // 亲密好友页面
	"FRIENDSETTING_ITEM."WINKS_IMAGE_TYPE,                                 // 亲密好友列表项背景
    "FRIENDSETTING_ITEMHL."WINKS_IMAGE_TYPE,                               // 亲密好友列表项高亮背景
    // 通用图片
    "PREVIEWWINKS_PICFRM."WINKS_IMAGE_TYPE,                                // 彩像预览边框
    // 弹出对话框
    "POPUPDLG_BKG."WINKS_IMAGE_TYPE,                                       // 弹出对话框背景
    "POPUPDLG_ICONSUCCESS."WINKS_IMAGE_TYPE,                               // 弹出对话框成功图标
    "POPUPDLG_ICONERROR."WINKS_IMAGE_TYPE,                                 // 弹出对话框错误图标
    "POPUPDLG_ICONWAITING1."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标1
    "POPUPDLG_ICONWAITING2."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标2
    "POPUPDLG_ICONWAITING3."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标3
    "POPUPDLG_ICONWAITING4."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标4
    "POPUPDLG_ICONWAITING5."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标5
    "POPUPDLG_ICONWAITING6."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标6
    "POPUPDLG_ICONWAITING7."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标7
    "POPUPDLG_ICONWAITING8."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标8
    "POPUPDLG_ICONWAITING9."WINKS_IMAGE_TYPE,                              // 弹出对话框等待图标9
    "POPUPDLG_ICONWAITING10."WINKS_IMAGE_TYPE,                             // 弹出对话框等待图标10
    "POPUPDLG_ICONWAITING11."WINKS_IMAGE_TYPE,                             // 弹出对话框等待图标11
    "POPUPDLG_ICONWAITING12."WINKS_IMAGE_TYPE,                             // 弹出对话框等待图标12
    // Tap控件
    "TAP_CURTAPBKG."WINKS_IMAGE_TYPE,                                      // Tap控件当前选中的标签背景
    "TAP_CURTAPHL."WINKS_IMAGE_TYPE,                                       // Tap控件当前选中的标签高亮背景
    "TAP_ICON1."WINKS_IMAGE_TYPE,                                          // Tap控件图标1
    "TAP_ICON2."WINKS_IMAGE_TYPE,                                          // Tap控件图标2
    "TAP_ICON3."WINKS_IMAGE_TYPE,                                          // Tap控件图标3
    "TAP_ICON4."WINKS_IMAGE_TYPE,                                          // Tap控件图标4
    "TAP_ICON5."WINKS_IMAGE_TYPE,                                          // Tap控件图标5
    "TAP_ICON6."WINKS_IMAGE_TYPE,                                          // Tap控件图标6
    // 普通Title控件
    "NLTITLE_BLACK_PIC."WINKS_IMAGE_TYPE,                                  // 黑-普通标题栏（全 236*31）
    "NLTITLE_BLACK_PICL1."WINKS_IMAGE_TYPE,                                // 黑-普通标题栏（左1 68*31）
    "NLTITLE_BLACK_PICL2."WINKS_IMAGE_TYPE,                                // 黑-普通标题栏（左2 91*31）
    "NLTITLE_BLACK_PICL3."WINKS_IMAGE_TYPE,                                // 黑-普通标题栏（左3 118*31）
    "NLTITLE_BLACK_PICR1."WINKS_IMAGE_TYPE,                                // 黑-普通标题栏（右1 168*31）
    "NLTITLE_BLACK_PICR2."WINKS_IMAGE_TYPE,                                // 黑-普通标题栏（右2 145*31）
    "NLTITLE_BLACK_PICR3."WINKS_IMAGE_TYPE,                                // 黑-普通标题栏（右3 118*31）
    "NLTITLE_BLUE_PIC."WINKS_IMAGE_TYPE,                                   // 蓝-普通标题栏（全 236*31） 
    "NLTITLE_BLUE_PICL1."WINKS_IMAGE_TYPE,                                 // 蓝-普通标题栏（左1 68*31） 
    "NLTITLE_BLUE_PICL2."WINKS_IMAGE_TYPE,                                 // 蓝-普通标题栏（左2 91*31） 
    "NLTITLE_BLUE_PICL3."WINKS_IMAGE_TYPE,                                 // 蓝-普通标题栏（左3 118*31）
    "NLTITLE_BLUE_PICR1."WINKS_IMAGE_TYPE,                                 // 蓝-普通标题栏（右1 168*31）
    "NLTITLE_BLUE_PICR2."WINKS_IMAGE_TYPE,                                 // 蓝-普通标题栏（右2 145*31）
    "NLTITLE_BLUE_PICR3."WINKS_IMAGE_TYPE,                                 // 蓝-普通标题栏（右3 118*31）
    "NLTITLE_HL_PIC."WINKS_IMAGE_TYPE,                                     // 高亮-普通标题栏（全 236*31） 
    "NLTITLE_HL_PICL1."WINKS_IMAGE_TYPE,                                   // 高亮-普通标题栏（左1 68*31） 
    "NLTITLE_HL_PICL2."WINKS_IMAGE_TYPE,                                   // 高亮-普通标题栏（左2 91*31） 
    "NLTITLE_HL_PICL3."WINKS_IMAGE_TYPE,                                   // 高亮-普通标题栏（左3 118*31）
    "NLTITLE_HL_PICR1."WINKS_IMAGE_TYPE,                                   // 高亮-普通标题栏（右1 168*31）
    "NLTITLE_HL_PICR2."WINKS_IMAGE_TYPE,                                   // 高亮-普通标题栏（右2 145*31）
    "NLTITLE_HL_PICR3."WINKS_IMAGE_TYPE,                                   // 高亮-普通标题栏（右3 118*31）

	"NLTITLE_SHADOW."WINKS_IMAGE_TYPE,									   // 普通标题栏下阴影（236*5）
	// 页面底边圆角遮罩
	"BOTTOM_PIC."WINKS_IMAGE_TYPE,										   // 页面底边圆角遮罩（240*10）
    // 分割线
    "SEPARATOR_H."WINKS_IMAGE_TYPE,                                        // 分割线 - 横向
    "SEPARATOR_V."WINKS_IMAGE_TYPE,                                        // 分割线 - 纵向
    // 箭头
    "ARROW_LEFT."WINKS_IMAGE_TYPE,                                         // 向左箭头
    "ARROW_RIGHT."WINKS_IMAGE_TYPE,                                        // 向右箭头
    "ARROW_LEFTHL."WINKS_IMAGE_TYPE,                                       // 高亮向左箭头
    "ARROW_RIGHTHL."WINKS_IMAGE_TYPE,                                      // 高亮向右箭头
    "ARROW_DOWN."WINKS_IMAGE_TYPE,                                         // 向下箭头
    "ARROW_DOWNHL."WINKS_IMAGE_TYPE,                                       // 高亮向下箭头
    "ARROW_DOUBLERIGHT."WINKS_IMAGE_TYPE,                                  // 向右双箭头
    "ARROW_DOUBLERIGHTHL."WINKS_IMAGE_TYPE,                                // 高亮向右双箭头
    // 文本框
    "EDIT_PIC1."WINKS_IMAGE_TYPE,                                          // 文本输入框1 36*17
    "EDIT_PIC2."WINKS_IMAGE_TYPE,                                          // 文本输入框2 91*17
    "EDIT_PIC3."WINKS_IMAGE_TYPE,                                          // 文本输入框3 142*17
    "EDIT_PIC4."WINKS_IMAGE_TYPE,                                          // 文本输入框4 153*17
    // 弹出菜单
    "POPUPMENU_ITEM."WINKS_IMAGE_TYPE,                                     // 弹出菜单项背景
    "POPUPMENU_ITEMHL."WINKS_IMAGE_TYPE,                                   // 弹出菜单项高亮背景
    // 图标
    "ICON_PHONEBOOKWHITE."WINKS_IMAGE_TYPE,                                // 白色电话本图标
    "ICON_PHONEBOOKBLACK."WINKS_IMAGE_TYPE,                                // 黑色电话本图标
    "ICON_RIGHTMARK."WINKS_IMAGE_TYPE,                                     // 对勾图标
    "ICON_LOCKGRAY."WINKS_IMAGE_TYPE,                                      // 灰色锁图标
    "ICON_LOCKGREEN."WINKS_IMAGE_TYPE                                     // 绿色锁图标
};

/************************************************************************************\
函数原型：const char* Winks_UIFM_GetImage( int ImageID )
函数介绍：
		本函数获取图片数据。
参数说明：
        参数名称	参数类型	参数介绍	备注
        ImageID	    int	        图片ID	
函数返回值：
		成功返回对应的图片数据，失败返回NULL。

使用说明：
无。
\************************************************************************************/
char Winks_Image_path[256];
const char* Winks_UIFM_GetImage( unsigned long ImageID )
{
    if(ImageID  == 0 || ImageID >= WINKS_IMGID_MAXNUM)
        return 0;
#ifdef WINKS_USE_PLATFORM_SOURCE_MANAGER
	//sprintf(Winks_Image_path,"%d", ImageID);
	sprintf(Winks_Image_path,"%s%s", WINKS_IMG_PATH, Winks_Image_a[ImageID-1]);
#else
    sprintf(Winks_Image_path,"%s%s", WINKS_IMG_PATH, Winks_Image_a[ImageID-1]);
#endif
    return Winks_Image_path;
}

/************************************************************************************\
函数原型：int Winks_UIFM_StartTimer( int Period, Winks_UIFM_Timer pfCallBack )
函数介绍：
		本函数启动一个定时器。
	    一旦时间到来，回调函数即会被调用，同时该定时器即被销毁。界面不需负责定时器的资源问题。

参数说明：
        参数名称	参数类型	        参数介绍	备注
        Period	    int	                超时时长	以毫秒为单位。
        pfCallBack	Winks_UIFM_Timer	回调函数	
函数返回值：
		成功返回定时器句柄，失败返回负数。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_StartTimer( int Period, Winks_UIFM_Timer pfCallBack )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0;

    for( i = 0; i < WK_UIFM_MAXTIMERNUM; i++ )
    {
        if( pGlobal->Timer[i].pfCallBack == NULL )
        {
            if( (pGlobal->Timer[i].ID = Winks_CreateTimer( pGlobal->channel, Period, WINKS_TIMER_DEFAULT )) == 0 )
            {
                Winks_printf( "Winks UIFM create timer error\r\n" );
                return -1;
            }
            pGlobal->Timer[i].pfCallBack = pfCallBack;
            Winks_StartTimer( pGlobal->Timer[i].ID );

            return pGlobal->Timer[i].ID;
        }
    }

    Winks_printf( "Winks UIFM start timer find timer slot failure\r\n" );
    return -1;
}
/************************************************************************************\
函数原型：int Winks_UIFM_StopTimer( int TimerID )
函数介绍：
		本函数停止一个定时器。
	    如果该定时器已经到时，界面不需调用本函数停止，定时器已经被销毁，如果尚未到时，界面本调用会造成该定时器被销毁。

参数说明：
        参数名称	参数类型	参数介绍	备注
        TimerID	    int	        定时器句柄	
函数返回值：
		成功返回0，失败返回负数。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_StopTimer( int TimerID )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0;

    if( TimerID <= 0 )
    {
        Winks_printf( "Winks UIFM Stop timer param error\r\n" );
        return -1;
    }

    for( i = 0; i < WK_UIFM_MAXTIMERNUM; i++ )
    {
        if( pGlobal->Timer[i].ID == (WINKS_TIMER_ID)TimerID )
        {
            Winks_StopTimer( pGlobal->Timer[i].ID );

            Winks_mem_set( &(pGlobal->Timer[i].ID), 0, sizeof(Winks_UIFM_Timer_s) );
            return 0;
        }
    }

    Winks_printf( "Winks UIFM stop timer find timer slot failure\r\n" );

    return -1;

}

/************************************************************************************\
函数原型：int Winks_UIFM_SetUIHandler( int Option, Winks_UIFM_UIHandler pHandler )
函数介绍：
		本函数设置界面消息处理通知函数。应用消息共有以下几种：
        WK_UIFM_MGSUSPEND		挂起消息
        WK_UIFM_MGRESUME		恢复消息
        WK_UIFM_MGPAINT		重绘消息
	    界面程序可分别为这三种消息使用本函数设置相应的消息处理函数。
	    界面消息仅仅对当前界面有效，一旦绘制新的界面(Winks_UIFM_EnterNewScreen函数被调用)，UI框架即会
        复位该消息处理函数。也就是说，界面程序不需负责该消息处理函数的失效工作。
	    特别的，如果挂起消息和恢复消息没有被设置的话，UI框架会忽略该消息。但是如果PAINT消息处理函数没
        有被设置的话，UI框架会自行找到当前界面的控件链表并自行绘制界面。

参数说明：
        参数名称	        参数类型	            参数介绍	备注
        Option	            int	                    消息数值
        WK_UIFM_MGSUSPEND	挂起消息
        WK_UIFM_MGRESUME	恢复消息
        WK_UIFM_MGPAINT		重绘消息	
        pHandler	        Winks_UIFM_UIHandler	消息通知函数。	
函数返回值：
        成功返回0，失败返回-1。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_SetUIHandler( int Option, Winks_UIFM_UIHandler pHandler )
{
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;
    
    pScreen = winks_uifm_GetCurrScreen();

    switch( Option )
    {
    case WK_UIFM_MGSUSPEND:
        pScreen->SuspendHandler = pHandler;
        break;

    case WK_UIFM_MGRESUME:
        pScreen->ResumeHandler = pHandler;
        break;

    case WK_UIFM_MGPAINT:
        pScreen->PaintHandler = pHandler;
        break;

    default:
        Winks_printf( "Winks UIFM SetUIHandler unknow Option %d\r\n", Option );
        return -1;
    }

    return 0;

}

/************************************************************************************\
函数原型：int Winks_UIFM_SetKeyHandler( int KeyCode, Winks_UIFM_KeyHandler pHandler )
函数介绍：
        本函数设置键盘消息处理通知函数。键盘码参见引擎宏定义。
	    界面程序可分别为每一个键盘按键使用本函数设置相应的消息处理函数。
	    应用消息仅仅对当前界面有效，一旦绘制新的界面(Winks_UIFM_EnterNewScreen函数被调用)，
        UI框架即会复位所有键盘按键的消息处理函数。也就是说，界面程序不需负责该消息处理函数
        的失效工作。
	    特别的，任何一个键盘按键消息到来，如果该消息处理函数未被设置，UI框架即会忽略该消息。
        也就是说，界面程序只需设置他感兴趣的键盘消息即可。

参数说明：
        参数名称	参数类型	            参数介绍	                        备注
        KeyCode	    int	                    键盘码，参见引擎宏定义(wk_osal.h)	
        pHandler	Winks_UIFM_KeyHandler	消息通知函数。	
函数返回值：
        成功返回0，失败返回-1。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_SetKeyHandler( int KeyCode, Winks_UIFM_KeyHandler pHandler )
{
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;
    
    if( KeyCode <= 0 || KeyCode >= WK_UIFM_MAXKEYNUM )
    {
        Winks_printf( "Winks UIFM SetKeyhandler unknow keycode\r\n" );
        return -1;
    }

    pScreen = winks_uifm_GetCurrScreen();
    
    pScreen->KeyHandler[KeyCode] = pHandler;

    return 0;
}

/************************************************************************************\
函数原型：int Winks_UIFM_SetPenHandler( Winks_UIFM_PenHandler pHandler )
函数介绍：
        本函数设置触摸笔消息处理通知函数。触摸笔类型参见引擎宏定义。
	    界面程序使用本函数设置触摸笔消息处理函数。
	    应用消息仅仅对当前界面有效，一旦绘制新的界面(Winks_UIFM_EnterNewScreen函数被调用)，
        UI框架即会复位所有触摸笔的消息处理函数。也就是说，界面程序不需负责该消息处理函数的
        失效工作。
	    特别的，触摸笔消息到来如果处理函数未被设置，UI框架即会忽略该消息。

参数说明：
        参数名称	参数类型	            参数介绍	    备注
        pHandler	Winks_UIFM_PenHandler	消息通知函数。	
函数返回值：
        成功返回0，失败返回-1。

使用说明：
        无。
\************************************************************************************/
#ifdef WINKS_TOUCH_PANNEL
int Winks_UIFM_SetPenHandler( Winks_UIFM_PenHandler pHandler )
{
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;
    
    pScreen = winks_uifm_GetCurrScreen();
    
    pScreen->PenHandler = pHandler;

    return 0;

}
#endif
/************************************************************************************\
函数原型：int Winks_UIFM_SetAppHandler( Winks_UIFM_AppHandler pHandler )
函数介绍：
        本函数设置应用消息处理通知函数。本类消息分为以下几种：
        WK_UIFM_MGAPN		查找APN结束
        WK_UIFM_MGFILE		查找文件结束
        WK_UIFM_MGINPUT	输入文字结束
	    界面程序使用本函数设置应用消息处理函数。界面程序当需要进行上述三种工作时，需首先调用引擎
        提供的接口启动系统界面，接口如下：
	
        查找APN	    int Winks_SelectDataAccount( WINKS_CHN_ID chn, int type, unsigned long data_account )
        查找文件	int Winks_SelectFile( WINKS_CHN_ID chn, char* defpath, int type )
        输入文字	int winks_Multiline_editor( WINKS_CHN_ID chn, char* pbuff, int buflen ,
                            winks_mmi_chset_enum input_chset, int str_id)
	    特别的，上述三个函数中chn参数目前无用。
	    上述三个函数调用以后引擎都会有新的界面弹出由用户操作，一旦用户操作完毕，界面程序设置的消息处理函数即
        会被调用，并传回相关信息。
	    应用消息仅仅对当前界面有效，一旦绘制新的界面(Winks_UIFM_EnterNewScreen函数被调用)，UI框架即会复位所
        有应用消息处理函数。也就是说，界面程序不需负责该消息处理函数的失效工作。
	    特别的，应用消息到来如果处理函数未被设置，UI框架即会忽略该消息。

参数说明：
        参数名称	参数类型	            参数介绍	    备注
        pHandler	Winks_UIFM_AppHandler	消息通知函数。	
函数返回值：
        成功返回0，失败返回-1。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_SetAppHandler( int Option, Winks_UIFM_AppHandler pHandler )
{
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;
    
    pScreen = winks_uifm_GetCurrScreen();

    switch( Option )
    {
    case WK_UIFM_MGAPN:
        pScreen->ApnHandler = pHandler;
        break;

    case WK_UIFM_MGFILE:
        pScreen->FindFileHandler = pHandler;
        break;

    case WK_UIFM_MGINPUT:
        pScreen->InputHandler = pHandler;
        break;

    case WK_UIFM_MGPIM:
        pScreen->PIMHandler = pHandler;
        break;

    default:
        Winks_printf( "Winks UIFM SetAPPHandler unknow Option %d\r\n", Option );
        return -1;
    }

    return 0;

}
/************************************************************************************\
函数原型：int Winks_UIFM_SetCallHandler( Winks_UIFM_CallHandler pHandler )
函数介绍：
        本函数设置电话消息处理通知函数。本类消息分为以下几种：
        WINKS_MSG_CALL_INCOMING				来电
        WINKS_MSG_CALL_DIAL 					拨号
        WINKS_MSG_CALL_REJECT					拒接
        WINKS_MSG_CALL_REMOTE_REJECT			远端拒接
        WINKS_MSG_CALL_ABORT					放弃
        WINKS_MSG_CALL_REMOTE_ABORT			远端放弃
        WINKS_MSG_CALL_ANSWER					接听
        WINKS_MSG_CALL_REMOTE_ANSWER			远端接听
        WINKS_MSG_CALL_HANGUP					挂断
        WINKS_MSG_CALL_REMOTE_HANGUP			远端挂断
        WINKS_MSG_CALL_ENDALL					全部挂断
        界面程序使用本函数设置电话消息处理函数。
        一旦电话事件到来，界面程序设置的消息处理函数即会被调用，并传回相关信息。
	    电话事件永久有效，应用程序只要设置了关心电话事件的函数，除非取消设置或被其他程序覆盖，
        否则电话事件一旦到来应用程序就会被通知。
	    特别的，理论上来说应该只有彩像展示界面才会关心本类消息。

参数说明：
        参数名称	参数类型	            参数介绍	    备注
        pHandler	Winks_UIFM_CallHandler	消息通知函数。	
函数返回值：
        成功返回0，失败返回-1。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_SetCallHandler( Winks_UIFM_CallHandler pHandler )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;

    pGlobal->CallHandler = pHandler;

    return 0;
}

/************************************************************************************\
函数原型：int Winks_UIFM_SetEngineHandler( int Option, Winks_UIFM_EngineHandler pHandler )
函数介绍：
        本函数设置引擎消息处理通知函数。引擎会为界面程序提供一套APP程序，实现各个具体功能，诸如DIY，激活，
        初始化等。界面程序需指定对哪一个APP程序感兴趣，而后由该应用程序产生的消息都会发送到对应的处理函数中。
        界面程序在调用了引擎，启动某一个APP工作以后，就需使用本函数设置引擎消息处理函数。
	    引擎事件永久有效，应用程序只要设置了关心电话事件的函数，除非取消设置，否则电话事件一旦到来应用程序就
        会被通知。如果有多个界面都为某一个APP设置了处理函数，则该APP发生的消息会分别传入每一个处理函数中。
	    因此，如果界面程序已经对某一个APP不再感兴趣（例如该APP已经结束或用户取消了该动作），必须取消对该APP处理
        函数的设置（将pHandler用NULL调用本函数）。否则可能会错误的收到了无用的消息。

参数说明：
        参数名称	参数类型	                参数介绍	                备注
        Option	    int	                        APP枚举值，参见引擎宏定义。	
        pHandler	Winks_UIFM_EngineHandler	消息通知函数。	
函数返回值：
        成功返回0，失败返回-1。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_SetEngineHandler( int Option, Winks_UIFM_EngineHandler pHandler )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0;

    for( i = 0; i < WK_UIFM_MAXENGINEMEGNUM; i++ )
    {
        if( pGlobal->EngineMsg[i].pfCallBack == NULL )
        {
            pGlobal->EngineMsg[i].AppID = Option;
            pGlobal->EngineMsg[i].pfCallBack = pHandler;

            return i;
        }
    }

    Winks_printf( "Winks Set Engine Handler no slot.\r\n" );

    return -1;

}

/************************************************************************************\
函数原型：int Winks_UIFM_UnSetEngineHandler( int HD )
函数介绍：
        本函数删除对某应用引擎消息的设置。引擎会为界面程序提供一套APP程序，实现各个具体功能，
        诸如DIY，激活，初始化等。界面程序需指定对哪一个APP程序感兴趣，而后由该应用程序产生的
        消息都会发送到对应的处理函数中。
	    引擎事件永久有效，应用程序只要设置了关心电话事件的函数，除非取消设置，否则电话事件一
        旦到来应用程序就会被通知。如果有多个界面都为某一个APP设置了处理函数，则该APP发生的消
        息会分别传入每一个处理函数中。
        因此，如果界面程序已经对某一个APP不再感兴趣（例如该APP已经结束或用户取消了该动作），
        必须使用本函数取消对该APP处理函数的设置。否则可能会错误的收到了无用的消息。
参数说明：
        参数名称	参数类型	参数介绍	备注
        HD	        int	        设置句柄。	
函数返回值：
        成功返回0，失败返回-1。

使用说明：
        无。
\************************************************************************************/
int Winks_UIFM_UnSetEngineHandler( int HD )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    
    if( (HD < 0) || (HD >= WK_UIFM_MAXENGINEMEGNUM) )
    {
        Winks_printf( "Winks UIFM Unset Engine Handler param error.\r\n" );
        return -1;
    }

    Winks_mem_set( &(pGlobal->EngineMsg[HD]), 0, sizeof(Winks_UIFM_EngineMsg_s) );

    return 0;
}

/************************************************************************************\
函数原型：int Winks_UIFM_ExitApp( void )
函数介绍：
本函数退出彩像应用程序。
本函数会直接引发抽象层退出函数的调用，并会清空UI框架运行状态信息。
参数说明：
		无。
函数返回值：

成功返回0，失败返回-1。

使用说明：
无。
\************************************************************************************/
int Winks_UIFM_ExitApp( void )
{
    return Winks_DestroyApp( WINKS_MINGUI_HWND_USER );
}

static int winks_uifm_CreateApp( int AppID )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;

    winks_ui_imgcacheinit();

    switch( AppID )
    {
    case WK_UIFM_USER:
        if( pGlobal->flag & WK_UIFM_FGUSERACTIVE )
        {
            Winks_fail( "Winks UIFM get user app active msg but user is actived.\r\n" );
            return -1;
        }
        pGlobal->flag |= WK_UIFM_FGUSERACTIVE;
        pGlobal->flag &= ~WK_UIFM_FGCCSHOW;
        Winks_UIFM_EnterNewScreen( WINKS_UI_LOADING_ID, NULL );
        break;
/*        
    case WK_UIFM_CALL:
        if( pGlobal->flag & WK_UIFM_FGCALLACTIVE )
        {
            Winks_fail( "Winks UIFM get call app active msg but call is actived.\r\n" );
            return -1;
        }
        pGlobal->flag |= WK_UIFM_FGCALLACTIVE;
        pGlobal->flag |= WK_UIFM_FGCCSHOW;
        //Winks_UIFM_EnterNewScreen( WINKS_UI_CCSHOW_ID );
        break;
*/
    default:
        Winks_fail( "Winks UIFM get active msg unknow app.\r\n" );
        return -1;
    
    }

    return 0;
}

static int winks_uifm_DestroyApp( int AppID )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    int i = 0;
    Winks_UIFM_ScreenList_s* pSList = NULL;
    Winks_UIFM_WdCacheItem_s* pCache = NULL;

    winks_ui_imgcachedst();

    switch( AppID )
    {
    case WK_UIFM_USER:
        if( !(pGlobal->flag & WK_UIFM_FGUSERACTIVE) )
        {
            Winks_fail( "Winks UIFM get user app deactive msg but user is not actived.\r\n" );
            return -1;
        }
        pGlobal->flag &= ~WK_UIFM_FGUSERACTIVE;
        
        /* 应用被退出时，我们首先退出当前界面，删除所有界面控件链表，同时清空当前界面数据 */
        pSList = winks_uifm_GetScreenList( pGlobal->NCurrScreen.ScreenID );
        pCache = winks_uifm_GetWidgetCache( pGlobal->NCurrScreen.ScreenID );
        pSList->pfExit( &(pCache->pLayer), WK_UIFM_OPEXIT, pGlobal->NCurrScreen.wParam, pGlobal->NCurrScreen.lParam );
        for( i = 0; i < WK_UIFM_MAXWDCACHENUM; i++ )
        {
            /*
            if( pGlobal->WdCache.Item[i].ScreenID != WINKS_UI_CCSHOW_ID )
                winks_uifm_FreeWidgetCache( i );
            else if( pGlobal->WdCache.Item[i].ScreenID == 0 )
                break;
            */
            if( pGlobal->WdCache.Item[i].ScreenID == 0 )
                break;
            winks_uifm_FreeWidgetCache( i );
        }
        Winks_mem_set( &(pGlobal->NCurrScreen), 0, sizeof(Winks_UIFM_CurrentScreen_s) );
        
        break;
/*        
    case WK_UIFM_CALL:
        if( !(pGlobal->flag & WK_UIFM_FGCALLACTIVE) )
        {
            Winks_fail( "Winks UIFM get call app deactive msg but call is not actived.\r\n" );
            return -1;
        }
        pGlobal->flag &= ~WK_UIFM_FGCALLACTIVE;
        pGlobal->flag &= ~WK_UIFM_FGCCSHOW;
        // 应用被退出时，我们首先退出当前界面，删除所有界面控件链表，同时清空当前界面数据 
        pSList = winks_uifm_GetScreenList( pGlobal->CCurrScreen.ScreenID );
        pCache = winks_uifm_GetWidgetCache( pGlobal->CCurrScreen.ScreenID );
        pSList->pfExit( &(pCache->pLayer), WK_UIFM_OPEXIT, pGlobal->CCurrScreen.wParam, pGlobal->CCurrScreen.lParam );
        for( i = 0; i < WK_UIFM_MAXWDCACHENUM; i++ )
        {
            if( pGlobal->WdCache.Item[i].ScreenID == WINKS_UI_CCSHOW_ID )
                winks_uifm_FreeWidgetCache( i );
            else if( pGlobal->WdCache.Item[i].ScreenID == 0 )
                break;
        }
        Winks_mem_set( &(pGlobal->CCurrScreen), 0, sizeof(Winks_UIFM_CurrentScreen_s) );
        
        break;
*/
    default:
        Winks_fail( "Winks UIFM get active msg unknow app.\r\n" );
        return -1;
    
    }

    return 0;
}

/* WINKS_MSG_ENGINE_REG_NOTIFY */
static int winks_uifm_OnRegNotify( void* pdata, int size )
{
    return -1;
}

/* WINKS_MSG_ENG2UI_STORAGE_LIMIT */
static int winks_uifm_OnStorageLimit( void* pdata, int size )
{
    return -1;
}

/* WINKS_MSG_USB_DETECT */
static int winks_uifm_OnUSBDetect( void* pdata, int size )
{
    return -1;
}

/* UI框架消息处理函数 */
static int winks_uifm_Handle( unsigned long msg, void *data, unsigned long size )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;
    Winks_UIFM_CurrentScreen_s* pScreen = NULL;
    Winks_UIFM_ScreenList_s* pSList = NULL;
    Winks_UIFM_WdCacheItem_s* pCache = NULL;
    WinksAppState_e Focus = 0;
    int i = 0;
    
    pScreen = winks_uifm_GetCurrScreen();

	switch(msg)
	{
	case WINKS_MSG_CHN_INIT:
		break;

	case WINKS_MSG_UI_KEY_EVENT:
        if( data == NULL )
        {
            Winks_printf( "Winks UIFM get key event no msg data.\r\n" );
            break;
        }

        if( pScreen->KeyHandler[((Winks_MsgBody_KeyEvent_s* )data)->keyCode] != NULL )
            pScreen->KeyHandler[((Winks_MsgBody_KeyEvent_s* )data)->keyCode]( ((Winks_MsgBody_KeyEvent_s* )data)->flag );
		break;

#ifdef WINKS_TOUCH_PANNEL
	case WINKS_MSG_UI_PEN_EVENT:
        if( data == NULL )
        {
            Winks_printf( "Winks UIFM get Pen Event no msg data.\r\n" );
            break;
        }
        if( (winks_GetHwndFocusFlag( WINKS_MINGUI_HWND_USER, &Focus ) == WINKS_RETURN_SUCCESS) 
            && (Focus == WINKS_APP_STATE_RUN) )
        {
            if( pScreen->PenHandler )
                pScreen->PenHandler( ((Winks_MsgBody_PenEvent_s* )data)->flag, 
                ((Winks_MsgBody_PenEvent_s* )data)->x, ((Winks_MsgBody_PenEvent_s* )data)->y );
        }
	    break;	
#endif	

	case WINKS_MSG_UI_CREATE_APP:
        winks_uifm_CreateApp( WK_UIFM_USER );
        break;	

	case WINKS_MSG_UI_DESTROY_APP:
		winks_uifm_DestroyApp( WK_UIFM_USER );
        break; 

        
	case WINKS_MSG_UI_PAINT_EVENT:
        if( pScreen->PaintHandler )
            pScreen->PaintHandler( data );
        else
        {
            /* 界面未设置重绘消息，我们获取当前界面控件链表进行绘制 */
            pCache = winks_uifm_GetWidgetCache( pScreen->ScreenID );
            Winks_Widget_ScreenDraw( pCache->pLayer );
        }
		break;

    case  WINKS_MSG_UI_PAUSE_RESUME_APP:
		{
			WinksAppPauseResume_e appFlag = *((WinksAppPauseResume_e* )data);
			/*
            if( ((appFlag->app == WINKS_MINGUI_HWND_USER) && (!(pGlobal->flag & WK_UIFM_FGUSERACTIVE)))
                || ((appFlag->app == WINKS_MINGUI_HWND_CALL) && (!(pGlobal->flag & WK_UIFM_FGCALLACTIVE))) )
            {
                Winks_fail( "Winks UIFM Get pause_resume in error hwnd\r\n" );
                return -1;
            }
            */
            if( appFlag == WINKS_APP_PAUSE )
            {
                if( pScreen->SuspendHandler )
                    pScreen->SuspendHandler( NULL );
            }
            else if( appFlag == WINKS_APP_RESUME )
            {
                if( pScreen->ResumeHandler )
                    pScreen->ResumeHandler( NULL );
            }
            else
            {
                Winks_fail( "Winks UIFM suspend_resume msg unknow flag" );
                return -1;
            }
        }
		break;

	case WINKS_MSG_CALL_INCOMING:   //来电
	case WINKS_MSG_CALL_DIAL:   //拨号
	case WINKS_MSG_CALL_REJECT:              /* 本地拒接 */
	case WINKS_MSG_CALL_REMOTE_REJECT:      /* 对方拒接 */
	case WINKS_MSG_CALL_ABORT:          /* 本地取消 */    
	case WINKS_MSG_CALL_REMOTE_ABORT:       /* 对方取消 */
	case WINKS_MSG_CALL_ANSWER:             /* 本地接听 */    
	case WINKS_MSG_CALL_REMOTE_ANSWER:     /* 对方接听 */	
	case WINKS_MSG_CALL_HANGUP:             /* 本地挂断 */
	case WINKS_MSG_CALL_REMOTE_HANGUP:     /* 对方挂断 */
	case WINKS_MSG_CALL_ENDALL:             /*挂断ALL*/		
		if( pGlobal->CallHandler )
            pGlobal->CallHandler( msg, data, size );
		break;
		
	case WINKS_MSG_TIMER_EXPIRY:
		for (i = 0;i < WK_UIFM_MAXTIMERNUM; i++)
		{
			if( (pGlobal->Timer[i].pfCallBack) && (pGlobal->Timer[i].ID == *((WINKS_TIMER_ID* )data)) )
            {
                pGlobal->Timer[i].pfCallBack( NULL );
                Winks_DestroyTimer( pGlobal->Timer[i].ID );
                pGlobal->Timer[i].ID = 0;
                pGlobal->Timer[i].pfCallBack = NULL;
                break;
            }
		}
        break;
        
    case WINKS_MSG_UI_EDITOR_RSP:
        if( pScreen->InputHandler )
            pScreen->InputHandler( data, size );
        break;

	case WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP:		 
 		if( pScreen->ApnHandler )
            pScreen->ApnHandler( data, size );
        break;

	case WINKS_MSG_UI_SELECT_FILE_RSP:
        if( pScreen->FindFileHandler )
            pScreen->FindFileHandler( data, size );
        break;

    case WINKS_MSG_UI_SELECT_PHONENUM_RSP:
        if( pScreen->PIMHandler )
            pScreen->PIMHandler( data, size );
        break;
        
	case  WINKS_MSG_ENGINE_UIRETMSG:
        for( i = 0; i < WK_UIFM_MAXENGINEMEGNUM; i++ )
        {
            if( (pGlobal->EngineMsg[i].AppID == ((Winks_EngineToUiReturnMsg* )data)->appType)
                && pGlobal->EngineMsg[i].pfCallBack )
            {
                pGlobal->EngineMsg[i].pfCallBack( data, size );
            }
        }
		break;

	case  WINKS_MSG_ENGINE_REG_NOTIFY:
        winks_uifm_OnRegNotify( data, size );
        break;

	case  WINKS_MSG_ENG2UI_STORAGE_LIMIT:
        winks_uifm_OnStorageLimit( data, size );
		break;
		

    case WINKS_MSG_USB_DETECT:
        winks_uifm_OnUSBDetect( data, size );
        break;

	default:
		Winks_printf("winks_UiIdle get unknow msg[%ld]\n",msg);
		break;
	}

	return 0;

}
