#ifndef _WK_WDT_H_
#define _WK_WDT_H_

typedef int (* Winks_Widget_InitFunc)( Winks_Widget_s* pWidget );
typedef int (* Winks_Widget_DrawFunc)( Winks_Widget_s* pWidget );
typedef int (* Winks_Widget_StateModifyFunc)( Winks_Widget_s* pWidget, int option, int ifvar );
typedef int (* Winks_Widget_KeyHandlerFunc)( Winks_Widget_s* pWidget, int KeyCode, int KeyType );
typedef int (* Winks_Widget_PenHandlerFunc)( Winks_Widget_s* pWidget, int PenType, int x, int y );
typedef int (* Winks_Widget_ChangeFunc)( Winks_Widget_s* pWidget, int conflag );

typedef struct tag_Winks_Widget_FuncItem_s
{
    //int type;
    Winks_Widget_InitFunc InitFunc;
    Winks_Widget_DrawFunc DrawFunc;
    Winks_Widget_StateModifyFunc StateModifyFunc;
    Winks_Widget_KeyHandlerFunc KeyHandlerFunc;
    Winks_Widget_PenHandlerFunc PenHandlerFunc;
    Winks_Widget_ChangeFunc ChangeFunc;
}Winks_Widget_FuncItem_s;

/*****************分页标题栏调试数据，实际ID制定好以后需要删除************************/
#define WK_UIFM_STRID_TAP00 WINKS_STRID_MAIN_MYLIBRARY//WINKS_STRID_MAIN_DIY//1
#define WK_UIFM_STRID_TAP01 WINKS_STRID_MAIN_WINKSSTORE//WINKS_STRID_MAIN_MYLIBRARY//2
#define WK_UIFM_STRID_TAP02 WINKS_STRID_MAIN_SETWINKS//WINKS_STRID_MAIN_WINKSSTORE//3
#define WK_UIFM_STRID_TAP03 WINKS_STRID_MAIN_DIY//WINKS_STRID_MAIN_SETWINKS//4
#define WK_UIFM_STRID_TAP04 WINKS_STRID_MAIN_SHARE//WINKS_STRID_MAIN_SETTINGS//5
#define WK_UIFM_STRID_TAP05 WINKS_STRID_MAIN_SETTINGS//WINKS_STRID_MAIN_SHARE//6
// #define WK_UIFM_STRID_TAP06 7
// #define WK_UIFM_STRID_TAP07 8

#define WK_UIFM_IMGID_TAP00 WINKS_IMGID_TAP_ICON1
#define WK_UIFM_IMGID_TAP01 WINKS_IMGID_TAP_ICON2
#define WK_UIFM_IMGID_TAP02 WINKS_IMGID_TAP_ICON3
#define WK_UIFM_IMGID_TAP03 WINKS_IMGID_TAP_ICON4
#define WK_UIFM_IMGID_TAP04 WINKS_IMGID_TAP_ICON5
#define WK_UIFM_IMGID_TAP05 WINKS_IMGID_TAP_ICON6
// #define WK_UIFM_IMGID_TAP06 7
// #define WK_UIFM_IMGID_TAP07 8

#define WK_UIFM_IMGID_TAPHL WINKS_IMGID_TAP_CURTAPHL//9

#define WK_UIFM_SCNID_TAP00 WINKS_UI_MYLIBRARY_ID//WINKS_UI_DIY_ID//1
#define WK_UIFM_SCNID_TAP01 WINKS_UI_WINKSSTORE_ID//WINKS_UI_MYLIBRARY_ID
#define WK_UIFM_SCNID_TAP02 WINKS_UI_SETWINKS_ID//WINKS_UI_WINKSSTORE_ID
#define WK_UIFM_SCNID_TAP03 WINKS_UI_DIY_ID//WINKS_UI_SETWINKS_ID
#define WK_UIFM_SCNID_TAP04 WINKS_UI_SHARE_ID//WINKS_UI_NORMALSETTING_ID//5
#define WK_UIFM_SCNID_TAP05 WINKS_UI_NORMALSETTING_ID//WINKS_UI_SHARE_ID
#define WK_UIFM_SCNID_TAP06 7
#define WK_UIFM_SCNID_TAP07 8
#define WK_WG_TAPNUM        6       //分页标题栏单项数目
typedef struct tag_Winks_Widget_Global_s
{
    int ifinit;
    Winks_Widget_FuncItem_s FuncItem[WK_WG_MAXWIDGETTYPENUM];
    Winks_Widget_TapTitle_s TapTitle;
    Winks_Widget_TapListItem_s TapItem[WK_WG_TAPNUM - 1];
}Winks_Widget_Global_s;

typedef struct tag_Winks_Widget_Layer_s
{
    Winks_Widget_ScnLayer_s* pLayer;
    Winks_Rect_s Rect;
}Winks_Widget_Layer_s;

/*WK_WG_EDITBOX	        1   //编辑框	*/       
int wk_wg_Editbox_Init(Winks_Widget_s* p_Editbox);
int wk_wg_Editbox_Draw(Winks_Widget_s* p_Editbox);

int wk_wg_Editbox_PenHandler( Winks_Widget_s* p_Editbox, int PenType, int x, int y );

int wk_wg_Editbox_KeyHandler( Winks_Widget_s* p_Editbox, int KeyCode, int KeyType );

int wk_wg_Editbox_StateModify( Winks_Widget_s* p_Editbox, int Option, int ifvar );


/*WK_WG_BUTTON	        2   //按钮	*/   
int wk_wg_Button_Init(Winks_Widget_s* p_Button);
int wk_wg_Button_Draw(Winks_Widget_s* p_Button);

int wk_wg_Button_PenHandler( Winks_Widget_s* p_Button, int PenType, int x, int y );

int wk_wg_Button_KeyHandler( Winks_Widget_s* p_Button, int KeyCode, int KeyType );

int wk_wg_Button_StateModify( Winks_Widget_s* p_Button, int Option, int ifvar );

/*WK_WG_LISTITEM	        3   //列表项	*/     

//int wk_wg_Draw_ListItem(Winks_Widget_Listitem_s * pListItem);

/*WK_WG_LISTGROUP	        4   //列表组	*/     
int wk_wg_ListGroup_Init(Winks_Widget_s* p_ListGroup);
int wk_wg_ListGroup_Draw(Winks_Widget_s* p_ListGroup);

int wk_wg_ListGroup_PenHandler( Winks_Widget_s* p_ListGroup, int PenType, int x, int y );

int wk_wg_ListGroup_KeyHandler( Winks_Widget_s* p_ListGroup, int KeyCode, int KeyType );

int wk_wg_ListGroup_StateModify( Winks_Widget_s* p_ListGroup, int Option, int ifvar );

/*WK_WG_TAPTITLEITEM	    5   //分页标题栏单项	*/

//int wk_wg_Draw_TapTitleItem(Winks_Widget_TapListItem_s * pTapTitleItem);

/*WK_WG_TAPTITLE	        6   //分页标题栏	*/ 
int wk_wg_TapTitle_Init(Winks_Widget_s* p_ListGroup);
int wk_wg_TapTitle_Draw(Winks_Widget_s* p_TapTitle);

int wk_wg_TapTitle_PenHandler( Winks_Widget_s* p_TapTitle, int PenType, int x, int y );

int wk_wg_TapTitle_KeyHandler( Winks_Widget_s* p_TapTitle, int KeyCode, int KeyType );

int wk_wg_TapTitle_StateModify( Winks_Widget_s* p_TapTitle, int Option, int ifvar );

void Winks_UI_TapTitle_SetFocus( Winks_Widget_TapTitle_s* pTapTitle, int nFocusIndex);

/*WK_WG_NORMALTITLE	    7   //普通标题栏	*/   
int wk_wg_NormalTitle_Init(Winks_Widget_s* p_NormalTitle);
int wk_wg_NormalTitle_Draw(Winks_Widget_s* p_NormalTitle);

int wk_wg_NormalTitle_PenHandler( Winks_Widget_s* p_NormalTitle, int PenType, int x, int y );

int wk_wg_NormalTitle_KeyHandler( Winks_Widget_s* p_NormalTitle, int KeyCode, int KeyType );

int wk_wg_NormalTitle_StateModify( Winks_Widget_s* p_Widget, int Option, int ifvar );

/*WK_WG_POPUPMENUITEM	    8   //悬浮菜单单项	*/

//int wk_wg_Draw_PopUpItem(Winks_Widget_PopMenuItem_s * pPopUpItem);

/*WK_WG_LEFTSOFTKEY	    9   //左软键	*/       
int wk_wg_LeftSoftKey_Init(Winks_Widget_s* p_LeftSoftKey);
int wk_wg_LeftSoftKey_Draw(Winks_Widget_s* p_LeftSoftKey);

int wk_wg_LeftSoftKey_PenHandler( Winks_Widget_s* p_LeftSoftKey, int PenType, int x, int y );

int wk_wg_LeftSoftKey_KeyHandler( Winks_Widget_s* p_LeftSoftKey, int KeyCode, int KeyType );

int wk_wg_LeftSoftKey_StateModify( Winks_Widget_s* p_LeftSoftKey, int Option, int ifvar );

/*WK_WG_RIGHTSOFTKEY	    10  //右软键	*/     
int wk_wg_RightSoftKey_Init(Winks_Widget_s* p_RightSoftKey);
int wk_wg_RightSoftKey_Draw(Winks_Widget_s* p_RightSoftKey);

int wk_wg_RightSoftKey_PenHandler( Winks_Widget_s* p_RightSoftKey, int PenType, int x, int y );

int wk_wg_RightSoftKey_KeyHandler( Winks_Widget_s* p_RightSoftKey, int KeyCode, int KeyType );

int wk_wg_RightSoftKey_StateModify( Winks_Widget_s* p_RightSoftKey, int Option, int ifvar );

/*WK_WG_CENTERSOFTKEY	    11  //中间软键	*/   
int wk_wg_CenterSoftKey_Init(Winks_Widget_s* p_RightSoftKey);
int wk_wg_CenterSoftKey_Draw(Winks_Widget_s* p_CenterSoftKey);

int wk_wg_CenterSoftKey_PenHandler( Winks_Widget_s* p_CenterSoftKey, int PenType, int x, int y );

int wk_wg_CenterSoftKey_KeyHandler( Winks_Widget_s* p_CenterSoftKey, int KeyCode, int KeyType );

int wk_wg_CenterSoftKey_StateModify( Winks_Widget_s* p_CenterSoftKey, int Option, int ifvar );

/*WK_WG_POPUP	            12  //弹出式显示框	*/
int wk_wg_PopUp_Init(Winks_Widget_s* p_RightSoftKey);
int wk_wg_PopUp_Draw(Winks_Widget_s* p_PopUp);

int wk_wg_PopUp_PenHandler( Winks_Widget_s* p_PopUp, int PenType, int x, int y );

int wk_wg_PopUp_KeyHandler( Winks_Widget_s* p_PopUp, int KeyCode, int KeyType );

int wk_wg_PopUp_StateModify( Winks_Widget_s* p_PopUp, int Option, int ifvar );

int wk_wg_PopUp_Change( Winks_Widget_s* p_PopUp, int conflag );

/*WK_WG_POPUP	            12  //弹出式显示框	*/
int wk_wg_PopMenu_Init(Winks_Widget_s* p_RightSoftKey);
int wk_wg_PopMenu_Draw(Winks_Widget_s* p_PopUp);

int wk_wg_PopMenu_PenHandler( Winks_Widget_s* p_PopUp, int PenType, int x, int y );

int wk_wg_PopMenu_KeyHandler( Winks_Widget_s* p_PopUp, int KeyCode, int KeyType );

int wk_wg_PopMenu_StateModify( Winks_Widget_s* p_PopUp, int Option, int ifvar );

//int wk_wg_PopUp_Change( Winks_Widget_s* p_PopUp, int conflag );

/*WK_WG_STATIC	        14  //静态框*/         
int wk_wg_Static_Init(Winks_Widget_s* p_RightSoftKey);
int wk_wg_Static_Draw(Winks_Widget_s* p_Static);

int wk_wg_Static_PenHandler( Winks_Widget_s* p_Static, int PenType, int x, int y );

int wk_wg_Static_KeyHandler( Winks_Widget_s* p_Static, int KeyCode, int KeyType );

int wk_wg_Static_StateModify( Winks_Widget_s* p_Static, int Option, int ifvar );

/*WK_WG_MATRIXMENUITEM	15  //矩阵菜单单项	*/ 

int wk_wg_MatrixMenuItem_Init(Winks_Widget_s* p_RightSoftKey);

int wk_wg_MatrixMenuItem_Draw(Winks_Widget_s* p_MatrixMenu);

int wk_wg_MatrixMenuItem_PenHandler( Winks_Widget_s* p_MatrixMenu, int PenType, int x, int y );

int wk_wg_MatrixMenuItem_KeyHandler( Winks_Widget_s* p_MatrixMenu, int KeyCode, int KeyType );

int wk_wg_MatrixMenuItem_StateModify( Winks_Widget_s* p_MatrixMenu, int Option, int ifvar );

/*WK_WG_MATRIXMENU	    16  //矩阵菜单列表  */ 
int wk_wg_MatrixMenu_Init(Winks_Widget_s* p_RightSoftKey);

int wk_wg_MatrixMenu_Draw(Winks_Widget_s* p_MatrixMenu);

int wk_wg_MatrixMenu_PenHandler( Winks_Widget_s* p_MatrixMenu, int PenType, int x, int y );

int wk_wg_MatrixMenu_KeyHandler( Winks_Widget_s* p_MatrixMenu, int KeyCode, int KeyType );

int wk_wg_MatrixMenu_StateModify( Winks_Widget_s* p_MatrixMenu, int Option, int ifvar );

/*WK_WG_MATRIXMENU      18  //旋钮控件  */ 
int wk_wg_SpinBox_Init(Winks_Widget_s* p_SpinBox);

int wk_wg_SpinBox_Draw(Winks_Widget_s* p_SpinBox);

int wk_wg_SpinBox_PenHandler( Winks_Widget_s* p_SpinBox, int PenType, int x, int y );

int wk_wg_SpinBox_KeyHandler( Winks_Widget_s* p_SpinBox, int KeyCode, int KeyType );

int wk_wg_SpinBox_StateModify( Winks_Widget_s* p_SpinBox, int Option, int ifvar );

/*WK_WG_GLIDEPOLE	    19  //滑杆  */ 
int wk_wg_GlidePole_Init(Winks_Widget_s* p_RightSoftKey);

int wk_wg_GlidePole_Draw(Winks_Widget_s* p_MatrixMenu);

int wk_wg_GlidePole_PenHandler( Winks_Widget_s* p_MatrixMenu, int PenType, int x, int y );

int wk_wg_GlidePole_KeyHandler( Winks_Widget_s* p_MatrixMenu, int KeyCode, int KeyType );

int wk_wg_GlidePole_StateModify( Winks_Widget_s* p_MatrixMenu, int Option, int ifvar );
#if 0
/* softkey */
//#define    WINKS_LEFTSOFTKEY_BUTTON_X    0
#define    WINKS_LEFTSOFTKEY_BUTTON_X    0
#define    WINKS_LEFTSOFTKEY_BUTTON_Y   (WINKS_SCREEN_HEIGHT -  WINKS_SOFTKEY_HEIGHT)//(WINKS_CLIENT_Y + WINKS_CLIENT_HEIGHT + 1)//198


#define    WINKS_SOFTKEYBAR_WIDTH   WINKS_SCREEN_WIDTH_UI//176
#define    WINKS_SOFTKEYBAR_HEIGHT    WINKS_SOFTKEY_HEIGHT// 22
#define    WINKS_SOFTKEY_WIDTH
#ifdef     __176X205__
#define    WINKS_SOFTKEY_HEIGHT      16
#else
#define    WINKS_SOFTKEY_HEIGHT      winks_get_coordinate(WINKS_SOFTKEY_HEIGHT_ID)//20
#endif
#endif
int Winks_Widget_Init( void );
int Winks_Widget_DeInit( void );



typedef struct winks_icon_button
{
	int x, y;
	int width, height;
	winks_color focussed_down_color;
	winks_color normal_color;
	int flags;
	
	int normal_up_icon;
	int normal_down_icon;
	
	int  icon_x, icon_y;
	
}winks_icon_button_s;

typedef enum
{
    winks_MMI_LEFT_SOFTKEY,
		winks_MMI_RIGHT_SOFTKEY,
#ifdef __WINKS_UI_CSK_ENABLE__
		winks_MMI_CENTER_SOFTKEY,
#endif 
		winks_WGUI_MAX_SOFTKEYS
} winks_WGUI_SOFTKEY_ENUM;


typedef struct winks_vertical_scrollbar
{
	int x, y;
	int width, height;
	int range;
	int value;
	int scale;
	int flags;
	winks_color background_color;
	
	//winks_icon_button_s  up_button;
	//winks_icon_button_s  down_button;
	winks_icon_button_s  scroll_button;//Winks_Widget_Button_s scroll_button;//
	winks_color   v_scroll_bg;
	
} winks_vertical_scrollbar_s;

typedef struct winks_pen_point_struct_tag
{
    int Pen_x;
    int Pen_y;
} winks_pen_point_struct;



/* title */
#define   WINKS_TITLE_ICON_X       2
#define   WINKS_TITLE_ICON_Y       30
#define   WINKS_TITLE_STRING_X    85
#define   WINKS_TITLE_STRING_Y    45
#define   WINKS_TITLE_HEIGHT       65
#define   WINKS_TITLE_DIGIT_X      3
#define   WINKS_TITLE_DIGIT_Y      WINKS_TITLE_STRING_Y

/* Client */
#define    WINKS_CLIENT_X               0
#define    WINKS_CLIENT_Y                 85//88//90//WINKS_TITLE_HEIGHT + 5
#ifdef  __240X296__
#define    WINKS_CLIENT_Y                 80//88//90//WINKS_TITLE_HEIGHT + 5
#define    WINKS_CLIENT_HEIGHT            189
#else
#define    WINKS_CLIENT_Y                 85//88//90//WINKS_TITLE_HEIGHT + 5
#define    WINKS_CLIENT_HEIGHT            205
#endif
#define    WINKS_CLIENT_WIDTH           239

/* mainmenu */

#ifdef __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__

#define   WINKS_MAINMENU_ICON_WIDTH     45
#define   WINKS_MAINMENU_ICON_HEIGHT    45

#else

//#define   WINKS_MATRIXMAINMENU_MAX_NUMBER  6
#define   WINKS_FIRST_MATRIXMAINMENU_X  33
#define   WINKS_FIRST_MATRIXMAINMENU_Y  60

#define   WINKS_MAINMENU_FIRSTICON_X   20
#define   WINKS_MAINMENU_FIRSTICON_Y   40
#define   WINKS_MAINMENU_ICON_WIDTH     41
#define   WINKS_MAINMENU_ICON_HEIGHT    41

#define    WINKS_MAINMENU_WIDTH_NUMBER   2
#define    WINKS_MAINMENU_HEIGHT_NUMBER   3

#endif


/* softkey */
#define    WINKS_LEFTSOFTKEY_BUTTON_X    0
#ifdef  __240X296__
#define    WINKS_LEFTSOFTKEY_BUTTON_Y    270
#else
#define    WINKS_LEFTSOFTKEY_BUTTON_Y    292
#endif

#define    WINKS_SOFTKEYBAR_WIDTH   239
#define    WINKS_SOFTKEYBAR_HEIGHT     30
#define    WINKS_SOFTKEY_WIDTH   50
#ifdef  __240X296__
#define    WINKS_SOFTKEY_HEIGHT    26
#else
#define    WINKS_SOFTKEY_HEIGHT      30
#endif

#define    WINKS_LEFTSOFTKEY_STRING_X    20
#define    WINKS_SOFTKEY_STRING_Y            (WINKS_LEFTSOFTKEY_BUTTON_Y + ((WINKS_SOFTKEYBAR_HEIGHT - WINKS_GETFONTHEIGHT) >> 1))
#define    WINKS_SOFTKEY_STRING_EXCURSION_X           2
#define    WINKS_SOFTKEY_STRING_EXCURSION_Y           2
#define    WINKS_LEFTSOFTKEY_BUTTON_WIDTH  80
#define    WINKS_MIDDLESOFTKEY_BUTTON_WIDTH 80

#define    WINKS_MIDDLESOFTKEY_BUTTON_X    WINKS_LEFTSOFTKEY_BUTTON_WIDTH
#define    WINKS_RIGHTSOFTKEY_BUTTON_X    WINKS_LEFTSOFTKEY_BUTTON_WIDTH + WINKS_MIDDLESOFTKEY_BUTTON_WIDTH

#define    WINKS_SOFTKEYBAR_COLOR  WINKS_PIXEL_RED
/* list */
#define    WINKS_LIST_Y                   WINKS_CLIENT_Y//95
#define    WINKS_LIST_FIRSTSTRING_X       35
#define    WINKS_LIST_FIRSTSTRING_Y       93//95
#define    WINKS_LIST_FIRSTSTRINGONLY_X   33
#define    WINKS_LIST_FIRSTSTRINGONLY_Y    WINKS_LIST_FIRSTSTRING_Y
#define    WINKS_LIST_STRINGONLY_SCROOBAR_GAP 2

#define SMALL_ICON_X 3
#define SMALL_ICON_Y 4
#define SMALL_ICON_WIDTH WINKS_LIST_ICON_WIDTH
#define SMALL_ICON_HEIGHT WINKS_LIST_ICON_HEIGHT
#define INFO_ICON_X 3
#define INFO_ICON_Y 4
#define INFO_ICON_WIDTH 20
#define INFO_ICON_HEIGHT 20
#define ICON_GAP_X 3

#define    WINKS_LIST_CKECKICON_WIDTH    10//radio or checkbox same as icon
#define    WINKS_LIST_CKECKICON_HEIGHT    10//radio or checkbox same as icon

#define    WINKS_LIST_ICON_X    10//radio or checkbox same as icon
#define    WINKS_LIST_ICON_Y    93//95
#define    WINKS_LIST_ICON_WIDTH   46//( 46 /60)//radio or checkbox same as icon
#define    WINKS_LIST_ICON_HEIGHT  46//  ( 46 /60)
#define    WINKS_LIST_MAX_NUMBER  6
#define    WINKS_HIGHLIGHT_LISTITEM_HEIGHT   32

#define    WINKS_HIGHLIGHT_HEIGHT       (WINKS_CLIENT_HEIGHT/WINKS_LIST_MAX_NUMBER)
#define    WINKS_HIGHLIGHT_EXCURSION_HEIGHT  6

/* scroll bar */
#define    WINKS_UPSCROLLBAR_X   229
#define    WINKS_UPSCROLLBAR_Y   90
#define    WINKS_DOWNSCROLLBAR_X   WINKS_UPSCROLLBAR_X
#define    WINKS_DOWNSCROLLBAR_Y   270
#define    WINKS_UPSCROLLBAR_WIDTH    11
#define    WINKS_UPSCROLLBAR_HEIGHT   12
#define    WINKS_SLIDESCROLLBAR_X   WINKS_UPSCROLLBAR_X
#define    WINKS_SLIDESCROLLBAR_Y   (WINKS_UPSCROLLBAR_Y + WINKS_UPSCROLLBAR_HEIGHT)
#define    WINKS_SLIDESCROLLBAR_WIDTH 11


#define WINKS_UI_BUTTON_STATE_DOWN  1
/* popup */

#define     WINKS_POPUP_BOTTOM_X    10
#ifdef  __240X296__
#define     WINKS_POPUP_BOTTOM_Y    265
#else
#define     WINKS_POPUP_BOTTOM_Y    285
#endif

#define     WINKS_POPUP_WIDTH    60
#define     WINKS_POPUP_ITEM_HEIGHT    30
#define     WINKS_POPUP_ITEM_GAP    3
#define     WINKS_POPUP_STRING_X    15

/* infobox */
#define     WINKS_INFOBOX_X         12
#define     WINKS_INFOBOX_Y         120

#define     WINKS_INFOBOX_ICON_X         25
#define     WINKS_INFOBOX_ICON_Y         130

#define     WINKS_INFOBOX_STRING_X         20//32
#define     WINKS_INFOBOX_STRING_Y         132//130
#define     WINKS_INFOBOX_STRING_WIDTH       200//190  
#define     WINKS_INFOBOX_STRING_HEIGHT      96//  90

/* editor */
#define     WINKS_EDITOR_CURSOR_X        30
#define     WINKS_EDITOR_CURSOR_Y        130
#define     WINKS_EDITOR_RECT_X        20
#define     WINKS_EDITOR_RECT_Y        150
#define     WINKS_EDITOR_RECT_WIDTH        230
#define     WINKS_EDITOR_RECT_HEIGHT        20

/*callor call*/

#define    WINKS_MAX_TEL_NUMBER    11
#define    WINKS_COLORCALL_NAME_X  4
#define    WINKS_COLORCALL_NAME_Y 4
#define    WINKS_COLORCALL_NAME_WIDTH  80
#define    WINKS_COLORCALL_NAME_HEIGHT  25
#define    WINKS_COLORCALL_TIME_X   200
#define    WINKS_COLORCALL_TIME_Y    4
#define    WINKS_COLORCALL_TIME_GAP    3
#define    WINKS_COLORCALL_PIC_X   0
#define    WINKS_COLORCALL_PIC_Y   26//28//25
#define    WINKS_COLORCALL_PIC_WIGHT   240
#define    WINKS_COLORCALL_PIC_HEIGHT   236//248//150
#define    WINKS_COLORCALL_GREETSTRING_X    20
#define    WINKS_COLORCALL_GREETSTRING_Y    270//266//270
#define    WINKS_COLORCALL_GREETSTRING_WIGHT 200
#define    WINKS_COLORCALL_GREETSTRING_HEIGHT  26//30

#define    WINKS_COLORCALLSOFTKEY_BUTTON_Y            300
#define    WINKS_COLORCALL_HEIGHT                               20
#define    WINKS_COLORCALL_SOFTKEY_STRING_Y           (WINKS_COLORCALLSOFTKEY_BUTTON_Y + ((WINKS_COLORCALL_HEIGHT - WINKS_GETFONTHEIGHT) >> 1))//295
/*diy */
#define    WINKS_DIY_PIC_X   0
#define    WINKS_DIY_PIC_Y   66
#define    WINKS_DIY_PIC_WIDTH   240
#define    WINKS_DIY_PIC_HEIGHT   220
#define    WINKS_DIY_GREETSTRING_X    12
#define    WINKS_DIY_GREETSTRING_Y    185
#define    WINKS_DIY_GREETSTRING_WIGHT 216
#define    WINKS_DIY_GREETSTRING_HEIGHT  30

/*show numberl*/
#define    WINKS_SHOW_NUMBER_X       WINKS_LIST_FIRSTSTRING_X
#define    WINKS_SHOW_NUMBER_Y       WINKS_LIST_FIRSTSTRING_Y
/*multiline text*/
#define WINKS_MULTILINE_MAX_NUMBER ((WINKS_MULTILINE_TEXT_HEIGHT) /(WINKS_GETFONTHEIGHT + WINKS_LINE_GAP))
#define WINKS_MULTILINE_TEXT_X       20
#define WINKS_MULTILINE_TEXT_Y       88
#define WINKS_MULTILINE_TEXT_WIDTH   208
#define WINKS_MULTILINE_TEXT_HEIGHT  198
#define WINKS_LINE_GAP               0//3


/*apn */
#define    WINKS_APN_MAX_NUMBER  3
#define WINKS_APN_TITLE_X               30
#define WINKS_APN_TITLE_Y				85
#define WINKS_APN_TITLE_TEXT_GAP        ((WINKS_APN_ITEM_HEIGHT - WINKS_GETFONTHEIGHT) >> 1)
#define WINKS_APN_TITLE_CONTEN_GAP      3
#define WINKS_APN_ITEM_HEIGHT           30
#define WINKS_APN_CONTENTITEM_WIDTH     190
#define WINKS_APN_NEXTITEM_HEIGHT ((WINKS_APN_ITEM_HEIGHT + WINKS_APN_TITLE_CONTEN_GAP) << 1)

#if 0
#define    WINKS_LEFTSOFTKEY_STRING_X    (int)(WINKS_SCREEN_WIDTH_UI * 0.0568)//10
#define    WINKS_SOFTKEY_STRING_Y          (WINKS_LEFTSOFTKEY_BUTTON_Y + ((WINKS_SOFTKEYBAR_HEIGHT - WINKS_GETFONTHEIGHT) >> 1))
#define    WINKS_SOFTKEY_STRING_EXCURSION_X           (int)(WINKS_SCREEN_WIDTH_UI * 0.01)/*1*/
#define    WINKS_SOFTKEY_STRING_EXCURSION_Y            (int)(WINKS_SCREEN_WIDTH_UI * 0.01)/*1*/
#define    WINKS_LEFTSOFTKEY_BUTTON_WIDTH  (int)(WINKS_SCREEN_WIDTH_UI * 0.284)//50
#define    WINKS_MIDDLESOFTKEY_BUTTON_WIDTH (int)(WINKS_SCREEN_WIDTH_UI * 0.4318)//5076

#define    WINKS_MIDDLESOFTKEY_BUTTON_X    WINKS_LEFTSOFTKEY_BUTTON_WIDTH
#define    WINKS_RIGHTSOFTKEY_BUTTON_X    WINKS_LEFTSOFTKEY_BUTTON_WIDTH + WINKS_MIDDLESOFTKEY_BUTTON_WIDTH

#endif

#define DIALOG_SHOW_PIC_HBLANK		  20
#define DIALOG_SHOW_PIC_VBLANK		  25
#define DIALOG_SHOW_PIC_WIDTH		  50
#define DIALOG_SHOW_AREA_BLANKWIDTH   22
#define DIALOG_TITLE_EXCURSION_Y  4

#define TAPTITLE_EXCURSION_X  3
#define TAPTITLE_ICON_HEIGHT  18
#define TAPTITLE_ICON_WIDTH  18
#define HILITAPTITLE_WIDTH    96
#define HILITAPTITLE_HEIGHT   24//31
int  winks_show_string_in_rect(winks_text_String_rect_s * rect_text_String, winks_Rect_s* pRect, unsigned long TextAlign);

#define WK_WGAPI_MAXLAYER   8   //默认屏幕层数最大值
#define WK_WGAPI_MAXMATRIXX 18   //默认控件矩阵横坐标最大值
#define WK_WGAPI_MAXMATRIXY 18   //默认控件矩阵纵坐标最大值

#define WK_GLIDEPOLE_DFPOSX		30
#define WK_GLIDEPOLE_DFPOSY		210
#define WK_GLIDEPOLE_DFWIDTH	178
#define WK_GLIDEPOLE_DFHEIGHT	28
#define WK_GLIDEPOLE_SLIDERWIDTH	35

int winks_widget_findupline( Winks_Widget_s** ppWidget, int thisline );
int winks_widget_finddownline( Winks_Widget_s** ppWidget, int thisline );
int winks_widget_findbeforecolumn( Winks_Widget_s** ppWidget, int thisline, int thiscolumn );
int winks_widget_findaftercolumn( Winks_Widget_s** ppWidget, int thisline, int thiscolumn );

#endif

