
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
����ԭ�ͣ�int Winks_UIFM_Init( void )
�������ܣ�
        ��������ʼ��UI��ܡ�
        ���ȳ�ʼ��ȫ�����ݽṹ���ر�ģ�ȫ��ȫ�����漰��Ӧ�����б���������ʼ����ͬʱҲ���ʼ���ؼ���ܡ�
        Ȼ�󴴽�UI��Ϣͨ�����ȴ���Ϣ������

����˵����
			
��������ֵ��
		�ɹ�����0��ʧ�ܷ���-1��
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
����ԭ�ͣ�int Winks_UIFM_DeInit( void )
�������ܣ�
        ������ȥ��ʼ��UI��ܡ�
        ����ȥ��ʼ���ؼ���ܣ�ɾ�����������Ϣ���ر���Ϣͨ�����رն�ʱ����ɾ���ؼ��������ڴ档
        Ȼ���ʼ��ȫ�ֽṹ��

����˵����
			
��������ֵ��
		�ɹ�����0��ʧ�ܷ���-1��
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
����ԭ�ͣ�int Winks_UIFM_EnterNewScreen( int ScreenID, void* Param )
�������ܣ�
		����������UI��ܻ���IDΪScreenID�Ľ��档

����˵����
        ��������	��������	��������	��ע
        ScreenID	int	        ��ĻID	
��������ֵ��
		�ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
		���������Ȼ���õ�ǰ������˳�������Ȼ����յ�ǰ��Ļȫ����Ϣ��������
        1.	��ǰ��ĻID��
        2.	��ǰ��Ļ������
        3.	ȫ�����̻ص�������
        4.	�����ʻص�������
        5.	����ص�������
        6.	�ָ��ص�������
        7.	Paint�ص�������

        �������ScreenID����Ļ�Ľ��뺯��������ǰ��ĻID��λΪScreenID��
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
            /* ����ɾ���˿ؼ������������������Ϊ������ĵ�ǰ���棬�϶��������棬�����ƶ�λ�� */
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
����ԭ�ͣ�int Winks_UIFM_SetCurrentScreenParam( unsigned long* pwParam, unsigned long* plParam )
�������ܣ�
		���������õ�ǰ������Ļ������
	    Ϊ�˱�����Ӧ���ֲ�ͬƽ̨��ÿһ�����治�������Լ���ȫ�ֱ�����UI��ܿ��԰�����ǰ���汣������32λ
        ���εĲ�������ǰ����ɽ��Լ���һЩȫ����Ϣ�����籾�����״ֵ̬�ȣ����������

����˵����
        ��������	��������	    ��������	        ��ע
        pwParam	    unsigned long*	��ǰ�����һ����	Ϊ�մ����ı䡣
        plParam	    unsigned long*	��ǰ����ڶ�����	Ϊ�մ����ı䡣
��������ֵ��
		�ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        �������������õ�ǰ��Ļ���������������߿����ÿ�ָ���ʾ���޸�ĳ�������Ӷ�ʵ�ֽ�������ĳһ�������Ĺ��ܡ�
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
����ԭ�ͣ�int Winks_UIFM_GetCurrentScreenParam( int* pScreenID, unsigned long* pwParam, unsigned long* plParam )
�������ܣ�
		��������ȡ��ǰ������Ļ������
	    Ϊ�˱�����Ӧ���ֲ�ͬƽ̨��ÿһ�����治�������Լ���ȫ�ֱ�����UI��ܿ��԰�����ǰ���汣������32λ���εĲ�����
        ��ǰ����ɽ��Լ���һЩȫ����Ϣ�����籾�����״ֵ̬�ȣ����������

����˵����
        ��������	��������	    ��������	        ��ע
        pScreenID	int*	        ��ǰ��ĻID	        Ϊ�մ������ġ�
        pwParam	    unsigned long*	��ǰ�����һ����	Ϊ�մ������ġ�
        plParam	    unsigned long*	��ǰ����ڶ�����	Ϊ�մ������ġ�
��������ֵ��
		�ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        ������������ȡ��ǰ��ĻID�����������������߿����ÿ�ָ���ʾ������ĳ�������Ӷ�ʵ�ֽ�����ȡĳ���������Ĺ��ܡ�
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
����ԭ�ͣ�Winks_Widget_ScnLayer_s* Winks_UIFM_GetScreenLayer( int ScreenID )
�������ܣ�
		��������ȡĳһ�����������������øý������пؼ�������Ϣ��
	    Ϊ�˿����ڴ����Ĳ����ӳ�������Ч�ʣ�UI����ṩһ�׽���ؼ���������ơ�
	    ÿ��ĳһ��������뺯��������ʱ�����������������ͼ�ڿؼ���������Ѱ�ұ�����Ŀؼ�����
        ����Ҳ����ٽ��б�����ؼ�����Ĵ�����������������������������ؼ��������С����ú������¡�

����˵����
        ��������	��������	��������	��ע
        ScreenID	int 	    ��ĻID	    Ϊ0����ǰ��Ļ��
��������ֵ��
		�ɹ�����ָ������������Ϣͷָ�룬ʧ�ܷ���NULL��

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_SetScreenLayer( int ScreenID, Winks_Widget_ScnLayer_s* pScnLayer, int memlen )
�������ܣ�
		����������ĳһ������������������øý������пؼ�������Ϣ��
	    Ϊ�˿����ڴ����Ĳ����ӳ�������Ч�ʣ�UI����ṩһ�׽���ؼ���������ơ�
	    ÿ��ĳһ��������뺯��������ʱ�����������������ͼ�ڿؼ���������Ѱ�ұ�����Ŀؼ�����
        ����Ҳ����ٽ��б�����ؼ�����Ĵ�����������������������ʹ�ñ�����������ؼ��������С�
        �Ա��´ν��뱾��Ļʱֱ�ӻ�ȡʹ�á�

����˵����
        ��������	��������	                ��������	                ��ע
        ScreenID	int	                        ��ĻID	                    Ϊ0����ǰ��Ļ��
        pScnLayer	Winks_Widget_ScnLayer_s*	������Ľ����ͷָ�롣	
        memlen	    int	                        �����ؼ�����ռ�õ��ڴ�ռ䡣	
��������ֵ��
        �ɹ�����0��ʧ�ܷ���-1������ý���Ŀؼ������ڻ������Ѿ����ڣ��򱾴����ûḲ��ԭ�������á�
        �ر�ģ����ڴ�ʱ��Ҫ����ɾ��ԭ�����õĿؼ�����ռ�õ��ڴ�ռ䣬�ý�����˳������ᱻ��
        WK_UIFM_OPFREEΪ�������á�

ʹ��˵����
        �ޡ�
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
            /* �ҵ��գ�����һ�����ǿ� */
            break;
        }
        if( pGlobal->WdCache.Item[i].ScreenID == ScreenID )
        {
            /* �б���Ļ��Ϣ��ɾ���ɵ� */
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
            /* ȫ����ջ��ǲ��������������ܴ�С��ȷ��������ӳɹ� */
            pGlobal->WdCache.TotalSize = memlen;
            break;
        }
        iffull = 0;
    }

    /* ɾ���ն� */
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
����ԭ�ͣ�const char* Winks_UIFM_GetString( int StringID )
�������ܣ�
		���������յ�ǰ���Ի�ȡ�ַ����������ݡ�
	

����˵����
        ��������	��������	��������	��ע
        StringID	int	        �ַ���ID	
��������ֵ��
		�ɹ����ض�Ӧ���ַ������ݣ�ʧ�ܷ���NULL��

ʹ��˵����
�ޡ�
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
    // δ����ҳ��
    "UNACTIVATED_BTN."WINKS_IMAGE_TYPE,                                    // δ����ҳ�水ť����
    "UNACTIVATED_BTNHL."WINKS_IMAGE_TYPE,                                  // δ����ҳ�水ť����
    "UNACTIVATED_PICFRM."WINKS_IMAGE_TYPE,                                 // δ����ҳ��չʾͼƬ�߿�
    // ��ҳ��
    "MAINMENU_ICON1."WINKS_IMAGE_TYPE,                                     // ������ͼ��1
    "MAINMENU_ICON2."WINKS_IMAGE_TYPE,                                     // ������ͼ��2
    "MAINMENU_ICON3."WINKS_IMAGE_TYPE,                                     // ������ͼ��3
    "MAINMENU_ICON4."WINKS_IMAGE_TYPE,                                     // ������ͼ��4
    "MAINMENU_ICON5."WINKS_IMAGE_TYPE,                                     // ������ͼ��5
    "MAINMENU_ICON6."WINKS_IMAGE_TYPE,                                     // ������ͼ��6
    "MAINMENU_HLBKG."WINKS_IMAGE_TYPE,                                     // ���������ͼ�걳��
    // ��ͨ����ҳ��
    "NORMALSETTING_ITEMBKG."WINKS_IMAGE_TYPE,                              // ��ͨ�����б����
    "NORMALSETTING_ITEMHL."WINKS_IMAGE_TYPE,                               // ��ͨ�����б����������
    "NORMALSETTING_ICON1."WINKS_IMAGE_TYPE,                                // ��ͨ����ͼ��1
    "NORMALSETTING_ICON2."WINKS_IMAGE_TYPE,                                // ��ͨ����ͼ��2
    "NORMALSETTING_ICON3."WINKS_IMAGE_TYPE,                                // ��ͨ����ͼ��3
    "NORMALSETTING_ICON4."WINKS_IMAGE_TYPE,                                // ��ͨ����ͼ��4
    // �߼�����ҳ��
    "ADVANCESETTING_GPITEMBKG."WINKS_IMAGE_TYPE,                           // �߼��������б����
	"ADVANCESETTING_GPITEMBKG2."WINKS_IMAGE_TYPE,                          // �߼��������б����2
	"ADVANCESETTING_GPITEMBKG3."WINKS_IMAGE_TYPE,                          // �߼��������б����3
    "ADVANCESETTING_ITEMBKG."WINKS_IMAGE_TYPE,                             // �߼������б����
    "ADVANCESETTING_ITEMHL."WINKS_IMAGE_TYPE,                              // �߼������б����������
    // ��������ҳ��
    "UPDATESETTING_ITEMHL."WINKS_IMAGE_TYPE,                               // ���������б����������
    "UPDATESETTING_SELBLOCK."WINKS_IMAGE_TYPE,                             // �������û���
	"UPDATESETTING_GLIDEPOLE."WINKS_IMAGE_TYPE,                            // �������û���
	"UPDATESETTING_GLIDEPOLEHL."WINKS_IMAGE_TYPE,                          // �������ø�������
    // ���ܺ���ҳ��
	"FRIENDSETTING_ITEM."WINKS_IMAGE_TYPE,                                 // ���ܺ����б����
    "FRIENDSETTING_ITEMHL."WINKS_IMAGE_TYPE,                               // ���ܺ����б����������
    // ͨ��ͼƬ
    "PREVIEWWINKS_PICFRM."WINKS_IMAGE_TYPE,                                // ����Ԥ���߿�
    // �����Ի���
    "POPUPDLG_BKG."WINKS_IMAGE_TYPE,                                       // �����Ի��򱳾�
    "POPUPDLG_ICONSUCCESS."WINKS_IMAGE_TYPE,                               // �����Ի���ɹ�ͼ��
    "POPUPDLG_ICONERROR."WINKS_IMAGE_TYPE,                                 // �����Ի������ͼ��
    "POPUPDLG_ICONWAITING1."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��1
    "POPUPDLG_ICONWAITING2."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��2
    "POPUPDLG_ICONWAITING3."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��3
    "POPUPDLG_ICONWAITING4."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��4
    "POPUPDLG_ICONWAITING5."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��5
    "POPUPDLG_ICONWAITING6."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��6
    "POPUPDLG_ICONWAITING7."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��7
    "POPUPDLG_ICONWAITING8."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��8
    "POPUPDLG_ICONWAITING9."WINKS_IMAGE_TYPE,                              // �����Ի���ȴ�ͼ��9
    "POPUPDLG_ICONWAITING10."WINKS_IMAGE_TYPE,                             // �����Ի���ȴ�ͼ��10
    "POPUPDLG_ICONWAITING11."WINKS_IMAGE_TYPE,                             // �����Ի���ȴ�ͼ��11
    "POPUPDLG_ICONWAITING12."WINKS_IMAGE_TYPE,                             // �����Ի���ȴ�ͼ��12
    // Tap�ؼ�
    "TAP_CURTAPBKG."WINKS_IMAGE_TYPE,                                      // Tap�ؼ���ǰѡ�еı�ǩ����
    "TAP_CURTAPHL."WINKS_IMAGE_TYPE,                                       // Tap�ؼ���ǰѡ�еı�ǩ��������
    "TAP_ICON1."WINKS_IMAGE_TYPE,                                          // Tap�ؼ�ͼ��1
    "TAP_ICON2."WINKS_IMAGE_TYPE,                                          // Tap�ؼ�ͼ��2
    "TAP_ICON3."WINKS_IMAGE_TYPE,                                          // Tap�ؼ�ͼ��3
    "TAP_ICON4."WINKS_IMAGE_TYPE,                                          // Tap�ؼ�ͼ��4
    "TAP_ICON5."WINKS_IMAGE_TYPE,                                          // Tap�ؼ�ͼ��5
    "TAP_ICON6."WINKS_IMAGE_TYPE,                                          // Tap�ؼ�ͼ��6
    // ��ͨTitle�ؼ�
    "NLTITLE_BLACK_PIC."WINKS_IMAGE_TYPE,                                  // ��-��ͨ��������ȫ 236*31��
    "NLTITLE_BLACK_PICL1."WINKS_IMAGE_TYPE,                                // ��-��ͨ����������1 68*31��
    "NLTITLE_BLACK_PICL2."WINKS_IMAGE_TYPE,                                // ��-��ͨ����������2 91*31��
    "NLTITLE_BLACK_PICL3."WINKS_IMAGE_TYPE,                                // ��-��ͨ����������3 118*31��
    "NLTITLE_BLACK_PICR1."WINKS_IMAGE_TYPE,                                // ��-��ͨ����������1 168*31��
    "NLTITLE_BLACK_PICR2."WINKS_IMAGE_TYPE,                                // ��-��ͨ����������2 145*31��
    "NLTITLE_BLACK_PICR3."WINKS_IMAGE_TYPE,                                // ��-��ͨ����������3 118*31��
    "NLTITLE_BLUE_PIC."WINKS_IMAGE_TYPE,                                   // ��-��ͨ��������ȫ 236*31�� 
    "NLTITLE_BLUE_PICL1."WINKS_IMAGE_TYPE,                                 // ��-��ͨ����������1 68*31�� 
    "NLTITLE_BLUE_PICL2."WINKS_IMAGE_TYPE,                                 // ��-��ͨ����������2 91*31�� 
    "NLTITLE_BLUE_PICL3."WINKS_IMAGE_TYPE,                                 // ��-��ͨ����������3 118*31��
    "NLTITLE_BLUE_PICR1."WINKS_IMAGE_TYPE,                                 // ��-��ͨ����������1 168*31��
    "NLTITLE_BLUE_PICR2."WINKS_IMAGE_TYPE,                                 // ��-��ͨ����������2 145*31��
    "NLTITLE_BLUE_PICR3."WINKS_IMAGE_TYPE,                                 // ��-��ͨ����������3 118*31��
    "NLTITLE_HL_PIC."WINKS_IMAGE_TYPE,                                     // ����-��ͨ��������ȫ 236*31�� 
    "NLTITLE_HL_PICL1."WINKS_IMAGE_TYPE,                                   // ����-��ͨ����������1 68*31�� 
    "NLTITLE_HL_PICL2."WINKS_IMAGE_TYPE,                                   // ����-��ͨ����������2 91*31�� 
    "NLTITLE_HL_PICL3."WINKS_IMAGE_TYPE,                                   // ����-��ͨ����������3 118*31��
    "NLTITLE_HL_PICR1."WINKS_IMAGE_TYPE,                                   // ����-��ͨ����������1 168*31��
    "NLTITLE_HL_PICR2."WINKS_IMAGE_TYPE,                                   // ����-��ͨ����������2 145*31��
    "NLTITLE_HL_PICR3."WINKS_IMAGE_TYPE,                                   // ����-��ͨ����������3 118*31��

	"NLTITLE_SHADOW."WINKS_IMAGE_TYPE,									   // ��ͨ����������Ӱ��236*5��
	// ҳ��ױ�Բ������
	"BOTTOM_PIC."WINKS_IMAGE_TYPE,										   // ҳ��ױ�Բ�����֣�240*10��
    // �ָ���
    "SEPARATOR_H."WINKS_IMAGE_TYPE,                                        // �ָ��� - ����
    "SEPARATOR_V."WINKS_IMAGE_TYPE,                                        // �ָ��� - ����
    // ��ͷ
    "ARROW_LEFT."WINKS_IMAGE_TYPE,                                         // �����ͷ
    "ARROW_RIGHT."WINKS_IMAGE_TYPE,                                        // ���Ҽ�ͷ
    "ARROW_LEFTHL."WINKS_IMAGE_TYPE,                                       // ���������ͷ
    "ARROW_RIGHTHL."WINKS_IMAGE_TYPE,                                      // �������Ҽ�ͷ
    "ARROW_DOWN."WINKS_IMAGE_TYPE,                                         // ���¼�ͷ
    "ARROW_DOWNHL."WINKS_IMAGE_TYPE,                                       // �������¼�ͷ
    "ARROW_DOUBLERIGHT."WINKS_IMAGE_TYPE,                                  // ����˫��ͷ
    "ARROW_DOUBLERIGHTHL."WINKS_IMAGE_TYPE,                                // ��������˫��ͷ
    // �ı���
    "EDIT_PIC1."WINKS_IMAGE_TYPE,                                          // �ı������1 36*17
    "EDIT_PIC2."WINKS_IMAGE_TYPE,                                          // �ı������2 91*17
    "EDIT_PIC3."WINKS_IMAGE_TYPE,                                          // �ı������3 142*17
    "EDIT_PIC4."WINKS_IMAGE_TYPE,                                          // �ı������4 153*17
    // �����˵�
    "POPUPMENU_ITEM."WINKS_IMAGE_TYPE,                                     // �����˵����
    "POPUPMENU_ITEMHL."WINKS_IMAGE_TYPE,                                   // �����˵����������
    // ͼ��
    "ICON_PHONEBOOKWHITE."WINKS_IMAGE_TYPE,                                // ��ɫ�绰��ͼ��
    "ICON_PHONEBOOKBLACK."WINKS_IMAGE_TYPE,                                // ��ɫ�绰��ͼ��
    "ICON_RIGHTMARK."WINKS_IMAGE_TYPE,                                     // �Թ�ͼ��
    "ICON_LOCKGRAY."WINKS_IMAGE_TYPE,                                      // ��ɫ��ͼ��
    "ICON_LOCKGREEN."WINKS_IMAGE_TYPE                                     // ��ɫ��ͼ��
};

/************************************************************************************\
����ԭ�ͣ�const char* Winks_UIFM_GetImage( int ImageID )
�������ܣ�
		��������ȡͼƬ���ݡ�
����˵����
        ��������	��������	��������	��ע
        ImageID	    int	        ͼƬID	
��������ֵ��
		�ɹ����ض�Ӧ��ͼƬ���ݣ�ʧ�ܷ���NULL��

ʹ��˵����
�ޡ�
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
����ԭ�ͣ�int Winks_UIFM_StartTimer( int Period, Winks_UIFM_Timer pfCallBack )
�������ܣ�
		����������һ����ʱ����
	    һ��ʱ�䵽�����ص��������ᱻ���ã�ͬʱ�ö�ʱ���������١����治�踺��ʱ������Դ���⡣

����˵����
        ��������	��������	        ��������	��ע
        Period	    int	                ��ʱʱ��	�Ժ���Ϊ��λ��
        pfCallBack	Winks_UIFM_Timer	�ص�����	
��������ֵ��
		�ɹ����ض�ʱ�������ʧ�ܷ��ظ�����

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_StopTimer( int TimerID )
�������ܣ�
		������ֹͣһ����ʱ����
	    ����ö�ʱ���Ѿ���ʱ�����治����ñ�����ֹͣ����ʱ���Ѿ������٣������δ��ʱ�����汾���û���ɸö�ʱ�������١�

����˵����
        ��������	��������	��������	��ע
        TimerID	    int	        ��ʱ�����	
��������ֵ��
		�ɹ�����0��ʧ�ܷ��ظ�����

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_SetUIHandler( int Option, Winks_UIFM_UIHandler pHandler )
�������ܣ�
		���������ý�����Ϣ����֪ͨ������Ӧ����Ϣ�������¼��֣�
        WK_UIFM_MGSUSPEND		������Ϣ
        WK_UIFM_MGRESUME		�ָ���Ϣ
        WK_UIFM_MGPAINT		�ػ���Ϣ
	    �������ɷֱ�Ϊ��������Ϣʹ�ñ�����������Ӧ����Ϣ��������
	    ������Ϣ�����Ե�ǰ������Ч��һ�������µĽ���(Winks_UIFM_EnterNewScreen����������)��UI��ܼ���
        ��λ����Ϣ��������Ҳ����˵����������踺�����Ϣ��������ʧЧ������
	    �ر�ģ����������Ϣ�ͻָ���Ϣû�б����õĻ���UI��ܻ���Ը���Ϣ���������PAINT��Ϣ������û
        �б����õĻ���UI��ܻ������ҵ���ǰ����Ŀؼ��������л��ƽ��档

����˵����
        ��������	        ��������	            ��������	��ע
        Option	            int	                    ��Ϣ��ֵ
        WK_UIFM_MGSUSPEND	������Ϣ
        WK_UIFM_MGRESUME	�ָ���Ϣ
        WK_UIFM_MGPAINT		�ػ���Ϣ	
        pHandler	        Winks_UIFM_UIHandler	��Ϣ֪ͨ������	
��������ֵ��
        �ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_SetKeyHandler( int KeyCode, Winks_UIFM_KeyHandler pHandler )
�������ܣ�
        ���������ü�����Ϣ����֪ͨ������������μ�����궨�塣
	    �������ɷֱ�Ϊÿһ�����̰���ʹ�ñ�����������Ӧ����Ϣ��������
	    Ӧ����Ϣ�����Ե�ǰ������Ч��һ�������µĽ���(Winks_UIFM_EnterNewScreen����������)��
        UI��ܼ��Ḵλ���м��̰�������Ϣ��������Ҳ����˵����������踺�����Ϣ������
        ��ʧЧ������
	    �ر�ģ��κ�һ�����̰�����Ϣ�������������Ϣ������δ�����ã�UI��ܼ�����Ը���Ϣ��
        Ҳ����˵���������ֻ������������Ȥ�ļ�����Ϣ���ɡ�

����˵����
        ��������	��������	            ��������	                        ��ע
        KeyCode	    int	                    �����룬�μ�����궨��(wk_osal.h)	
        pHandler	Winks_UIFM_KeyHandler	��Ϣ֪ͨ������	
��������ֵ��
        �ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_SetPenHandler( Winks_UIFM_PenHandler pHandler )
�������ܣ�
        ���������ô�������Ϣ����֪ͨ���������������Ͳμ�����궨�塣
	    �������ʹ�ñ��������ô�������Ϣ��������
	    Ӧ����Ϣ�����Ե�ǰ������Ч��һ�������µĽ���(Winks_UIFM_EnterNewScreen����������)��
        UI��ܼ��Ḵλ���д����ʵ���Ϣ��������Ҳ����˵����������踺�����Ϣ��������
        ʧЧ������
	    �ر�ģ���������Ϣ�������������δ�����ã�UI��ܼ�����Ը���Ϣ��

����˵����
        ��������	��������	            ��������	    ��ע
        pHandler	Winks_UIFM_PenHandler	��Ϣ֪ͨ������	
��������ֵ��
        �ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_SetAppHandler( Winks_UIFM_AppHandler pHandler )
�������ܣ�
        ����������Ӧ����Ϣ����֪ͨ������������Ϣ��Ϊ���¼��֣�
        WK_UIFM_MGAPN		����APN����
        WK_UIFM_MGFILE		�����ļ�����
        WK_UIFM_MGINPUT	�������ֽ���
	    �������ʹ�ñ���������Ӧ����Ϣ�����������������Ҫ�����������ֹ���ʱ�������ȵ�������
        �ṩ�Ľӿ�����ϵͳ���棬�ӿ����£�
	
        ����APN	    int Winks_SelectDataAccount( WINKS_CHN_ID chn, int type, unsigned long data_account )
        �����ļ�	int Winks_SelectFile( WINKS_CHN_ID chn, char* defpath, int type )
        ��������	int winks_Multiline_editor( WINKS_CHN_ID chn, char* pbuff, int buflen ,
                            winks_mmi_chset_enum input_chset, int str_id)
	    �ر�ģ���������������chn����Ŀǰ���á�
	    �����������������Ժ����涼�����µĽ��浯�����û�������һ���û�������ϣ�����������õ���Ϣ��������
        �ᱻ���ã������������Ϣ��
	    Ӧ����Ϣ�����Ե�ǰ������Ч��һ�������µĽ���(Winks_UIFM_EnterNewScreen����������)��UI��ܼ��Ḵλ��
        ��Ӧ����Ϣ��������Ҳ����˵����������踺�����Ϣ��������ʧЧ������
	    �ر�ģ�Ӧ����Ϣ�������������δ�����ã�UI��ܼ�����Ը���Ϣ��

����˵����
        ��������	��������	            ��������	    ��ע
        pHandler	Winks_UIFM_AppHandler	��Ϣ֪ͨ������	
��������ֵ��
        �ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_SetCallHandler( Winks_UIFM_CallHandler pHandler )
�������ܣ�
        ���������õ绰��Ϣ����֪ͨ������������Ϣ��Ϊ���¼��֣�
        WINKS_MSG_CALL_INCOMING				����
        WINKS_MSG_CALL_DIAL 					����
        WINKS_MSG_CALL_REJECT					�ܽ�
        WINKS_MSG_CALL_REMOTE_REJECT			Զ�˾ܽ�
        WINKS_MSG_CALL_ABORT					����
        WINKS_MSG_CALL_REMOTE_ABORT			Զ�˷���
        WINKS_MSG_CALL_ANSWER					����
        WINKS_MSG_CALL_REMOTE_ANSWER			Զ�˽���
        WINKS_MSG_CALL_HANGUP					�Ҷ�
        WINKS_MSG_CALL_REMOTE_HANGUP			Զ�˹Ҷ�
        WINKS_MSG_CALL_ENDALL					ȫ���Ҷ�
        �������ʹ�ñ��������õ绰��Ϣ��������
        һ���绰�¼�����������������õ���Ϣ���������ᱻ���ã������������Ϣ��
	    �绰�¼�������Ч��Ӧ�ó���ֻҪ�����˹��ĵ绰�¼��ĺ���������ȡ�����û��������򸲸ǣ�
        ����绰�¼�һ������Ӧ�ó���ͻᱻ֪ͨ��
	    �ر�ģ���������˵Ӧ��ֻ�в���չʾ����Ż���ı�����Ϣ��

����˵����
        ��������	��������	            ��������	    ��ע
        pHandler	Winks_UIFM_CallHandler	��Ϣ֪ͨ������	
��������ֵ��
        �ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        �ޡ�
\************************************************************************************/
int Winks_UIFM_SetCallHandler( Winks_UIFM_CallHandler pHandler )
{
    Winks_UIFM_Global_s* pGlobal = WINKS_UIFMGETGLOBALVAR;

    pGlobal->CallHandler = pHandler;

    return 0;
}

/************************************************************************************\
����ԭ�ͣ�int Winks_UIFM_SetEngineHandler( int Option, Winks_UIFM_EngineHandler pHandler )
�������ܣ�
        ����������������Ϣ����֪ͨ�����������Ϊ��������ṩһ��APP����ʵ�ָ������幦�ܣ�����DIY�����
        ��ʼ���ȡ����������ָ������һ��APP�������Ȥ�������ɸ�Ӧ�ó����������Ϣ���ᷢ�͵���Ӧ�Ĵ������С�
        ��������ڵ��������棬����ĳһ��APP�����Ժ󣬾���ʹ�ñ���������������Ϣ��������
	    �����¼�������Ч��Ӧ�ó���ֻҪ�����˹��ĵ绰�¼��ĺ���������ȡ�����ã�����绰�¼�һ������Ӧ�ó����
        �ᱻ֪ͨ������ж�����涼Ϊĳһ��APP�����˴����������APP��������Ϣ��ֱ���ÿһ���������С�
	    ��ˣ������������Ѿ���ĳһ��APP���ٸ���Ȥ�������APP�Ѿ��������û�ȡ���˸ö�����������ȡ���Ը�APP����
        ���������ã���pHandler��NULL���ñ���������������ܻ������յ������õ���Ϣ��

����˵����
        ��������	��������	                ��������	                ��ע
        Option	    int	                        APPö��ֵ���μ�����궨�塣	
        pHandler	Winks_UIFM_EngineHandler	��Ϣ֪ͨ������	
��������ֵ��
        �ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_UnSetEngineHandler( int HD )
�������ܣ�
        ������ɾ����ĳӦ��������Ϣ�����á������Ϊ��������ṩһ��APP����ʵ�ָ������幦�ܣ�
        ����DIY�������ʼ���ȡ����������ָ������һ��APP�������Ȥ�������ɸ�Ӧ�ó��������
        ��Ϣ���ᷢ�͵���Ӧ�Ĵ������С�
	    �����¼�������Ч��Ӧ�ó���ֻҪ�����˹��ĵ绰�¼��ĺ���������ȡ�����ã�����绰�¼�һ
        ������Ӧ�ó���ͻᱻ֪ͨ������ж�����涼Ϊĳһ��APP�����˴����������APP��������
        Ϣ��ֱ���ÿһ���������С�
        ��ˣ������������Ѿ���ĳһ��APP���ٸ���Ȥ�������APP�Ѿ��������û�ȡ���˸ö�������
        ����ʹ�ñ�����ȡ���Ը�APP�����������á�������ܻ������յ������õ���Ϣ��
����˵����
        ��������	��������	��������	��ע
        HD	        int	        ���þ����	
��������ֵ��
        �ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
        �ޡ�
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
����ԭ�ͣ�int Winks_UIFM_ExitApp( void )
�������ܣ�
�������˳�����Ӧ�ó���
��������ֱ������������˳������ĵ��ã��������UI�������״̬��Ϣ��
����˵����
		�ޡ�
��������ֵ��

�ɹ�����0��ʧ�ܷ���-1��

ʹ��˵����
�ޡ�
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
        
        /* Ӧ�ñ��˳�ʱ�����������˳���ǰ���棬ɾ�����н���ؼ�����ͬʱ��յ�ǰ�������� */
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
        // Ӧ�ñ��˳�ʱ�����������˳���ǰ���棬ɾ�����н���ؼ�����ͬʱ��յ�ǰ�������� 
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

/* UI�����Ϣ������ */
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
            /* ����δ�����ػ���Ϣ�����ǻ�ȡ��ǰ����ؼ�������л��� */
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

	case WINKS_MSG_CALL_INCOMING:   //����
	case WINKS_MSG_CALL_DIAL:   //����
	case WINKS_MSG_CALL_REJECT:              /* ���ؾܽ� */
	case WINKS_MSG_CALL_REMOTE_REJECT:      /* �Է��ܽ� */
	case WINKS_MSG_CALL_ABORT:          /* ����ȡ�� */    
	case WINKS_MSG_CALL_REMOTE_ABORT:       /* �Է�ȡ�� */
	case WINKS_MSG_CALL_ANSWER:             /* ���ؽ��� */    
	case WINKS_MSG_CALL_REMOTE_ANSWER:     /* �Է����� */	
	case WINKS_MSG_CALL_HANGUP:             /* ���عҶ� */
	case WINKS_MSG_CALL_REMOTE_HANGUP:     /* �Է��Ҷ� */
	case WINKS_MSG_CALL_ENDALL:             /*�Ҷ�ALL*/		
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
