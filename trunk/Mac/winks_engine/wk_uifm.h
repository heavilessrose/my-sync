#ifndef _WK_UIFM_H_
#define _WK_UIFM_H_

/* 资源头文件 */
#include "wk_image.h"
#include "wk_string.h"

   
//控件API返回值
#define WK_UIFM_FAIL                -1      //失败
#define WK_UIFM_SUCCESS             0       //成功
#define WK_UIFM_FOCUSCHANGE         1       //焦点发生变化
#define WK_UIFM_CONTENTCHANGE       2       //内容发生变化
#define WK_UIFM_FCCHANGE            3       //焦点和内容都发生变化

//控件类型枚举
#define WK_WG_EDITBOX	        1   //编辑框	
#define WK_WG_BUTTON	        2   //按钮	
#define WK_WG_LISTITEM	        3   //列表项	
#define WK_WG_LISTGROUP	        4   //列表组	由一组LISTITEM组成，并包含焦点等信息。滚动条由控件框架绘制。
#define WK_WG_TAPTITLEITEM	    5   //分页标题栏单项	
#define WK_WG_TAPTITLE	        6   //分页标题栏	由一组分页标题栏单项组成。
#define WK_WG_NORMALTITLE	    7   //普通标题栏	
#define WK_WG_POPUPMENUITEM	    8   //悬浮菜单单项	
#define WK_WG_POPMENU           9   //弹出式菜单
#define WK_WG_LEFTSOFTKEY	    10   //左软键	由一个按钮和一组悬浮菜单单项组成。使用公用的软键结构（Winks_Widget_SoftKey_s）
#define WK_WG_RIGHTSOFTKEY	    11  //右软键	由一个按钮和一组悬浮菜单单项组成。使用公用的软键结构（Winks_Widget_SoftKey_s）。
#define WK_WG_CENTERSOFTKEY	    12  //中间软键	由一个按钮和一组悬浮菜单单项组成。使用公用的软键结构（Winks_Widget_SoftKey_s）。
#define WK_WG_POPUP	            14  //弹出式显示框	
#define WK_WG_STATIC	        15  //静态框	可显示图片，文本（多行）。
#define WK_WG_MATRIXMENUITEM	16  //矩阵菜单单项	
#define WK_WG_MATRIXMENU	    17  //矩阵菜单列表	由一组矩阵菜单单项组成，用来显示九宫格菜单。
#define WK_WG_SPINBOX   18  //旋钮控件
#define WK_WG_GLIDEPOLE   19 //滑杆

#define WK_WG_MAXWIDGETTYPENUM  20  //枚举最大值

//控件标记枚举
#define WK_WG_FGGRAY	        0x01   //是否置灰（无效）	
#define WK_WG_FGFOCUS	        0x02   //是否获得焦点	
#define WK_WG_FGPUSH	        0x04   //是否按下	BUTTON
#define WK_WG_FGGETFOCUS	    0x08   //是否可能获得焦点	例如普通标题栏即属于无法获得焦点的控件。
#define WK_WG_FGPENSTATE        0x10   //触摸笔状态（pendown和penmove以后的penup处理可能不同）
#define WK_WG_FGNOTSHOW         0x20   //不显示

//界面层标记枚举
#define WK_WG_SLSHOW	        0x01   //是否显示	该层是否正在被弹出。
#define WK_WG_SLPOPUP	        0x02   //弹出式显示框	
#define WK_WG_SLPOPMENU         0x04   //弹出式菜单

//列表类型枚举
#define WK_LIST_TYPE_TEXT	    0x01   //显示文字	
#define WK_LIST_TYPE_TEXT_ICON	0x02  //显示文字和图片	可有提示图片和文字
#define WK_LIST_TYPE_RADIO	    0x04   //单选	
#define WK_LIST_TYPE_CHECKBOX	0x08   //复选	

//列表项文字显示类型枚举
#define WK_LIST_TYPE_TEXT_INFO_UP_DOWN	    0x10   //文字上下排列	
#define WK_LIST_TYPE_TEXT_INFO_LEFT_RIGHT 	0x20  //文字左右排列	


//静态框显示内容类型枚举
#define WK_STATIC_TYPE_IMAGE	        1   //显示图片 	
#define WK_STATIC_TYPE_TEXT	            2   //显示文字(区域可被选中)	
#define WK_STATIC_TYPE_ONLY_READ_TEXT	3   //显示只读文本(不会被选中)	

//显示方式枚举
#define WINKS_SHOW_LEFT	    1   //居左显示	
#define WINKS_SHOW_CENTER	2   //居中显示	
#define WINKS_SHOW_RIGHT	3   //居右显示	

//对话框枚举
#define WK_DIALOG_TYPE_NOMAL	1   //普通的对话框	
#define WK_DIALOG_TYPE_SCREEN	2   //屏幕式的对话框	

//控件修改内容
//WK_WG_POPUP
#define WK_WG_PPCF_TTSTR        1   //标题文字
#define WK_WG_PPCF_UPSTR        2   //上部文字
#define WK_WG_PPCF_ICON	        3   //中部图标
#define WK_WG_PPCF_DNSTR        4   //下部文字

//控件基类数据结构
typedef struct tag_Winks_Widget_s
{
    struct tag_Winks_Widget_s* pnext;
    unsigned short type;
    unsigned char xindex;
    unsigned char yindex;
    unsigned long flag;
    unsigned long userflag;
    int WidgetID;
    Winks_Widget_Font_s font;
    int lux;
    int luy;
    int width;
    int high;
    unsigned long bgimage;
    unsigned long hlimage;
    unsigned long bgcolor;
    unsigned long hlcolor;
    unsigned long bgfontcolor;
    unsigned long hlfontcolor;
}Winks_Widget_s;

//界面层数据结构
typedef struct tag_Winks_Widget_ScnLayer_s
{
    struct tag_Winks_Widget_ScnLayer_s* pnext;
    unsigned long flag;
    int LayerID;
    int layernum; 
    Winks_Widget_s* pWidget;
}Winks_Widget_ScnLayer_s;

//编辑框数据结构
typedef struct tag_Winks_Widget_EditBox_s
{
    Winks_Widget_s bwidget; 
    unsigned long dfstring;
    winks_scrolling_text_s scrolling_text;
}Winks_Widget_EditBox_s;

//按钮数据结构
typedef struct tag_Winks_Widget_Button_s
{
    Winks_Widget_s bwidget;
	unsigned long v_stringId;//ID is null, not display 
   	unsigned long normal_up_icon;
    unsigned long normal_down_icon;
    int v_nextscreenID;
}Winks_Widget_Button_s;

//列表项数据结构
typedef struct tag_Winks_Widget_Listitem_s
{ 
    Winks_Widget_s bwidget;
    struct tag_Winks_Widget_Listitem_s* pnext;
	unsigned long     v_ItemSetFlag;  //radio or checkbox is or not select
	int               v_ListType;   //radio or checkbox
	unsigned long     v_ItemStrID;  //string id
    unsigned long     v_info_string ;//info string
    unsigned long     v_info_imageId; //right small icon
    unsigned long     v_hlinfo_imageId; //right small icon
    unsigned long     v_ItemSelected_Icon;
    unsigned long     v_ItemNoselected_Icon;
    unsigned long     v_IconID;  //filename image
	int         	  v_NextScreenId;  //menu's next screen
	winks_scrolling_text_s scrolling_text;
}Winks_Widget_Listitem_s;

//列表组数据结构
typedef struct tag_Winks_Widget_ListGroup_s
{
    Winks_Widget_s bwidget;
	int v_TotalItemsNum;
	int v_FirstItemIndx;
	int v_FocusItemIndex;//the index number
    int ifScrollBar;
    Winks_Widget_Listitem_s* pa_ItemListData;
}Winks_Widget_ListGroup_s;

//分页标题栏单项数据结构
typedef struct tag_Winks_Widget_TapListItem_s
{
    Winks_Widget_s bwidget;
	unsigned long v_IconID;
	unsigned long v_string;
	int v_ScreenID;
}Winks_Widget_TapListItem_s;

//分页标题栏数据结构
typedef struct tag_Winks_Widget_TapTitle_s
{
    Winks_Widget_s bwidget;
	int v_TotalItemsNum;
	int v_FocusItemIndex;
    unsigned long v_HighlightImageId;
    Winks_Widget_TapListItem_s a_ItemListData[1];
}Winks_Widget_TapTitle_s;

//普通标题栏数据结构
typedef struct tag_Winks_Widget_NormalTitle_s
{
    Winks_Widget_s bwidget;
	unsigned long v_IconID;
    unsigned long v_TitleStringID;
    unsigned long v_icondigit;
}Winks_Widget_NormalTitle_s;

//悬浮菜单单项数据结构
typedef struct tag_Winks_Widget_PopMenuItem_s
{
    Winks_Widget_s bwidget;
	unsigned long v_IconID;
    unsigned long v_StringID;
    int v_NextScreenId;
}Winks_Widget_PopMenuItem_s;

//弹出式菜单数据结构
typedef struct tag_Winks_Widget_PopMenu_s
{
    Winks_Widget_s bwidget;//传入左下角的坐标作为(bwidget.lux, bwidget.luy)，忽略参数width, high
    int v_TotalItemsNum;
    int v_FocusItemIndex;//the index number
    Winks_Widget_PopMenuItem_s a_ItemList[1];
}Winks_Widget_PopMenu_s;

//软键数据结构
typedef struct tag_Winks_Widget_SoftKey_s
{
    Winks_Widget_s bwidget;
    unsigned long v_IconID;
    unsigned long v_StringID;
   	unsigned long normal_up_icon;
    unsigned long normal_down_icon;
}Winks_Widget_SoftKey_s;

//弹出式显示框数据结构
typedef struct tag_Winks_Widget_PopUp_s
{
    Winks_Widget_s bwidget;
    int flag; //dialog | screen
    unsigned long v_TitleStringID;
   	unsigned long v_UpStringID;
    unsigned long v_Icon;
    unsigned long v_DownStringID;
}Winks_Widget_PopUp_s;

//静态框数据结构
typedef struct tag_Winks_Widget_Static_s
{
    Winks_Widget_s bwidget;
    unsigned short d_flag; //left | right | center
    unsigned short c_flag; //image | text
    unsigned long v_ContentID;

	//为控件内部使用
   winks_text_String_rect_s rect_text_String;
}Winks_Widget_Static_s;

//矩阵菜单（九宫格）单项数据结构
typedef struct tag_Winks_Widget_MatrixMenuItem_s
{
    Winks_Widget_s bwidget;
    unsigned long nmIconID;
    unsigned long hlIconID;
    unsigned long v_StringID;
    int v_NextScreenID;
}Winks_Widget_MatrixMenuItem_s;

//矩阵菜单（九宫格）数据结构
typedef struct tag_Winks_Widget_Matrixmenu_s
{
    Winks_Widget_s bwidget;
	int v_TotalMatrixItemsNum;
	int v_FocusMatrixItemIndex;//the index number of the screen
	int v_numberbycolumn;
	Winks_Widget_MatrixMenuItem_s a_MenuItem[1];
}Winks_Widget_MatrixMenu_s;

//旋钮控件数据结构
typedef struct tag_Winks_Widget_SpinBox_s
{
    Winks_Widget_s bwidget;
    int v_TotalItemNums;//Static控件总数
    int v_FocusItemIndex;//当前焦点index
    int bg_iconid;//背景框图片
    int left_iconid;//左箭头
    int hl_left_iconid;//左箭头高亮
    int right_iconid;//右箭头
    int hl_right_iconid;//右箭头高亮
    Winks_Widget_Static_s a_Static[1];
}Winks_Widget_SpinBox_s;

//滑动杆数据结构
typedef struct tag_Winks_Widget_GlidePole_s
{
    Winks_Widget_s bwidget;
	unsigned long v_SliderIconID;
	int v_MinNum;
	int v_MaxNum;
	int v_StepNum;
	int v_CurNum;
}Winks_Widget_GlidePole_s;

int Winks_Widget_Draw( Winks_Widget_s* pWidget );
int Winks_Widget_StateModify( Winks_Widget_s* pWidget, int option, int ifvar );
int Winks_Widget_KeyHandler( Winks_Widget_s* pWidget, int KeyCode, int KeyType );
int Winks_Widget_PenHandler( Winks_Widget_s* pWidget, int PenType, int x, int y );
int Winks_Widget_Change( Winks_Widget_s* pWidget, int conflag );
int Winks_Widget_ScreenDraw( Winks_Widget_ScnLayer_s* pScnLayer );
Winks_Widget_ScnLayer_s* Winks_Widget_ScreenFindFirstLayer( Winks_Widget_ScnLayer_s* pScnLayer );
int Winks_Widget_ScreenNavKeyDeal( Winks_Widget_ScnLayer_s* pScnLayer, int KeyCode, int KeyType, 
                                  Winks_Widget_s** ppLostFocus, Winks_Widget_s** ppGetFocus );
int Winks_Widget_ScreenPenDeal( Winks_Widget_ScnLayer_s* pScnLayer, int PenType, int x, int y, 
                               Winks_Widget_s** ppLostFocus, Winks_Widget_s** ppGetFocus, Winks_Widget_s** ppPoint );
Winks_Widget_s* Winks_Widget_GetFocusWidget( Winks_Widget_s* pWidget );
Winks_Widget_s* Winks_Widget_GetTypeWidget( Winks_Widget_s* pWidget, int WidgetType );
Winks_Widget_s* Winks_Widget_WidgetAlloc( int type, int lux, int luy, int width, int high, 
                                         int xindex, int yindex, int allocsize );
Winks_Widget_ScnLayer_s* Winks_Widget_FindLayerByID( Winks_Widget_ScnLayer_s* pHead, int ID );
Winks_Widget_s* Winks_Widget_FindWidgetByID( Winks_Widget_ScnLayer_s* pHead, int ID );
Winks_Widget_ScnLayer_s* Winks_Widget_FindLayerByWidget( Winks_Widget_ScnLayer_s* pHead, int ID );
int Winks_Widget_ActiveLayer( Winks_Widget_ScnLayer_s* pScnLayer, int LayerID, int ifac );

//界面退出函数操作选项：
#define WK_UIFM_OPEXIT	1   //退出界面。	
#define WK_UIFM_OPFREE	2   //请求释放控件链表资源。	

//界面消息枚举：
#define WK_UIFM_MGSUSPEND	1   //挂起消息	界面被覆盖。
#define WK_UIFM_MGRESUME	2   //恢复消息	界面重新显示。
#define WK_UIFM_MGPAINT	    3   //重绘消息	

//应用消息枚举：
#define WK_UIFM_MGAPN	    1   //APN查找结果	
#define WK_UIFM_MGFILE	    2   //文件查找结果	
#define WK_UIFM_MGINPUT	    3   //输入法输入结果
#define WK_UIFM_MGPIM       4   //电话本查找结果	

//界面进入函数原型
typedef int (*Winks_UIFM_EnterScreen)( int Opcode );
//界面退出函数原型
typedef int (* Winks_UIFM_ExitScreen)( Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam );
//定时器回调函数原型
typedef int (*Winks_UIFM_Timer)( void* Param );
//界面消息处理通知函数原型
typedef int (* Winks_UIFM_UIHandler)( void* Param );
//键盘消息处理通知函数原型
typedef int (* Winks_UIFM_KeyHandler)( int KeyType );
#ifdef WINKS_TOUCH_PANNEL
//触摸笔消息处理通知函数原型
typedef int (*Winks_UIFM_PenHandler)( int PenType, int x, int y );
#endif
//应用消息处理通知函数原型
typedef int (* Winks_UIFM_AppHandler)( void* pData, unsigned long size );
//引擎消息处理通知函数原型
typedef int (*Winks_UIFM_EngineHandler)( void* Param, unsigned long size );
//电话消息处理通知函数原型
typedef int (* Winks_UIFM_CallHandler)( unsigned long Msg, void* pData, unsigned long size );

//界面管理接口
int Winks_UIFM_EnterNewScreen( int ScreenID, void* Param );
int Winks_UIFM_SetCurrentScreenParam( unsigned long* pwParam, unsigned long* plParam );
int Winks_UIFM_GetCurrentScreenParam( int* pScreenID, unsigned long* pwParam, unsigned long* plParam );
Winks_Widget_ScnLayer_s* Winks_UIFM_GetScreenLayer( int ScreenID );
int Winks_UIFM_SetScreenLayer( int ScreenID, Winks_Widget_ScnLayer_s* pScnLayer, int memlen );
//资源管理接口
const char* Winks_UIFM_GetString( unsigned long StringID );
const char* Winks_UIFM_GetImage( unsigned long ImageID );
//定时器管理接口
int Winks_UIFM_StartTimer( int Period, Winks_UIFM_Timer pfCallBack );
int Winks_UIFM_StopTimer( int TimerID );
//消息处理函数设置接口
int Winks_UIFM_SetUIHandler( int Option, Winks_UIFM_UIHandler pHandler );
int Winks_UIFM_SetKeyHandler( int KeyCode, Winks_UIFM_KeyHandler pHandler );
#ifdef WINKS_TOUCH_PANNEL
int Winks_UIFM_SetPenHandler( Winks_UIFM_PenHandler pHandler );
#endif
int Winks_UIFM_SetAppHandler( int Option, Winks_UIFM_AppHandler pHandler );
int Winks_UIFM_SetCallHandler( Winks_UIFM_CallHandler pHandler );
int Winks_UIFM_SetEngineHandler( int Option, Winks_UIFM_EngineHandler pHandler );
int Winks_UIFM_UnSetEngineHandler( int HD );
int Winks_UIFM_ExitApp( void );

#include "wk_screen.h"

Winks_Widget_TapTitle_s* Winks_Widget_GetTapTitle( void );
/*****************分页标题栏调试数据，实际ID制定好以后需要删除************************/

enum winks_enterscreen_flag
{
	WK_ENTERSCREENFLAG_NORMAL = 0,
	WK_ENTERSCREENFLAG_TAPTITLE
};

enum winks_TapTitle_ItemIndex
{
	WK_TAPTITLE_ITEMINDEX_MYLIBRARY = 0,
	WK_TAPTITLE_ITEMINDEX_WINKSSTORE,
	WK_TAPTITLE_ITEMINDEX_SETWINKS,
	WK_TAPTITLE_ITEMINDEX_DIY,
	WK_TAPTITLE_ITEMINDEX_SHARE,
	WK_TAPTITLE_ITEMINDEX_SETTINGS
};

#define WK_TAPTITLE_WIGDETID			1

// 左右软键功能条 控件位置
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS				0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS				((WINKS_SCREEN_HEIGHT * 93 + 40) / 100)
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH				WINKS_SCREEN_WIDTH
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT				((WINKS_SCREEN_HEIGHT * 6 + 180) / 100)

#endif
