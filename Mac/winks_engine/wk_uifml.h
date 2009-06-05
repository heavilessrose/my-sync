
#ifndef _WK_UIFML_H_
#define _WK_UIFML_H_
#include "wk_wdt.h"
//内部宏定义枚举：
#define WK_UIFM_MAXKEYNUM	        (WINKS_KEY_POWER + 1)   //最大键盘码数值	借用引擎键盘码
#define WK_UIFM_MAXWDCACHENUM	    10                      //最大控件链表缓存数	
#define WK_UIFM_MAXSCREENNUM	    100                     //最大屏幕数 @_@Should change	
#define WK_UIFM_MAXTIMERNUM	        10                      //最大定时器数	
#define WK_UIFM_MAXENGINEMEGNUM	    20                      //最大APP消息守候数值	
#define WK_UIFM_MAXCACHESIZE        (10 * 1024)             //最大控件链表缓存长度

//全局flag预定义
#define WK_UIFM_FGCCSHOW        0x01        //当前界面是CCShow界面
#define WK_UIFM_FGUSERACTIVE    0x02        //普通界面活动
#define WK_UIFM_FGCALLACTIVE    0x04        //CCShow界面活动

#define WK_UIFM_USER            1           //普通界面
#define WK_UIFM_CALL            2           //CCShow界面

//全局当前界面信息
typedef struct tag_Winks_UIFM_CurrentScreen_s
{
    int ScreenID;
    unsigned long wParam;
    unsigned long lParam;
    Winks_UIFM_KeyHandler KeyHandler[WK_UIFM_MAXKEYNUM];
#ifdef WINKS_TOUCH_PANNEL
    Winks_UIFM_PenHandler PenHandler;
#endif
    Winks_UIFM_UIHandler SuspendHandler;
    Winks_UIFM_UIHandler ResumeHandler;
    Winks_UIFM_UIHandler PaintHandler;
    Winks_UIFM_AppHandler ApnHandler;
    Winks_UIFM_AppHandler FindFileHandler;
    Winks_UIFM_AppHandler InputHandler;
    Winks_UIFM_AppHandler PIMHandler;
}Winks_UIFM_CurrentScreen_s;

//全局全部界面列表结构
typedef struct tag_Winks_UIFM_ScreenList_s
{
    int ScreenID;
    Winks_UIFM_EnterScreen pfEnter;
    Winks_UIFM_ExitScreen pfExit;
}Winks_UIFM_ScreenList_s;

//定时器管理列表结构
typedef struct tag_Winks_UIFM_Timer_s
{
    WINKS_TIMER_ID ID;
    Winks_UIFM_Timer pfCallBack;
}Winks_UIFM_Timer_s;

//引擎消息处理函数结构
typedef struct tag_Winks_UIFM_EngineMsg_s
{
    int AppID;
    Winks_UIFM_EngineHandler pfCallBack;
}Winks_UIFM_EngineMsg_s;


//控件缓存单项结构
typedef struct tag_Winks_UIFM_WdCacheItem_s
{
    int ScreenID;
    int size;
    Winks_Widget_ScnLayer_s* pLayer;
}Winks_UIFM_WdCacheItem_s;

//控件缓存结构
typedef struct tag_Winks_UIFM_WdCache_s
{
    int TotalSize;
    int CurrSize;
    Winks_UIFM_WdCacheItem_s Item[WK_UIFM_MAXWDCACHENUM];
}Winks_UIFM_WdCache_s;

//UI框架全局变量结构
typedef struct tag_Winks_UIFM_Global_s
{
    int ifinit;
    int flag;       
    WINKS_CHN_ID channel;
    Winks_UIFM_ScreenList_s ScreenList[WK_UIFM_MAXSCREENNUM];
    Winks_UIFM_CurrentScreen_s NCurrScreen;     //当前界面是普通界面时
    Winks_UIFM_CurrentScreen_s CCurrScreen;     //当前界面是CCShow界面时
    Winks_UIFM_Timer_s Timer[WK_UIFM_MAXTIMERNUM];
    Winks_UIFM_CallHandler CallHandler;
    Winks_UIFM_EngineMsg_s EngineMsg[WK_UIFM_MAXENGINEMEGNUM];
    Winks_UIFM_WdCache_s WdCache;
#if 1
	winks_text_String_rect_s  text_String_rect;
	//winks_text_String_rect_s  text_String_rect_infobox;
	winks_pen_point_struct prior_Pen_Pos;
	winks_scrolling_text_s  winks_scroll_string;
	winks_scrolling_text_s * winks_scroll_string_head;//winks_scroll_string_colorcall;
	  winks_vertical_scrollbar_s  gv_scrollbar;
	  int scrooltext_flag;
	  int gv_OneTimeFlag;
#endif
}Winks_UIFM_Global_s;

int Winks_Widget_Init( void );
int Winks_Widget_DeInit( void );
int Winks_UI_InitCCShow( void );
int Winks_UI_DeInitCCShow( void );




#endif