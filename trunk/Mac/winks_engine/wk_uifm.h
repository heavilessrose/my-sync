#ifndef _WK_UIFM_H_
#define _WK_UIFM_H_

/* ��Դͷ�ļ� */
#include "wk_image.h"
#include "wk_string.h"

   
//�ؼ�API����ֵ
#define WK_UIFM_FAIL                -1      //ʧ��
#define WK_UIFM_SUCCESS             0       //�ɹ�
#define WK_UIFM_FOCUSCHANGE         1       //���㷢���仯
#define WK_UIFM_CONTENTCHANGE       2       //���ݷ����仯
#define WK_UIFM_FCCHANGE            3       //��������ݶ������仯

//�ؼ�����ö��
#define WK_WG_EDITBOX	        1   //�༭��	
#define WK_WG_BUTTON	        2   //��ť	
#define WK_WG_LISTITEM	        3   //�б���	
#define WK_WG_LISTGROUP	        4   //�б���	��һ��LISTITEM��ɣ��������������Ϣ���������ɿؼ���ܻ��ơ�
#define WK_WG_TAPTITLEITEM	    5   //��ҳ����������	
#define WK_WG_TAPTITLE	        6   //��ҳ������	��һ���ҳ������������ɡ�
#define WK_WG_NORMALTITLE	    7   //��ͨ������	
#define WK_WG_POPUPMENUITEM	    8   //�����˵�����	
#define WK_WG_POPMENU           9   //����ʽ�˵�
#define WK_WG_LEFTSOFTKEY	    10   //�����	��һ����ť��һ�������˵�������ɡ�ʹ�ù��õ�����ṹ��Winks_Widget_SoftKey_s��
#define WK_WG_RIGHTSOFTKEY	    11  //�����	��һ����ť��һ�������˵�������ɡ�ʹ�ù��õ�����ṹ��Winks_Widget_SoftKey_s����
#define WK_WG_CENTERSOFTKEY	    12  //�м����	��һ����ť��һ�������˵�������ɡ�ʹ�ù��õ�����ṹ��Winks_Widget_SoftKey_s����
#define WK_WG_POPUP	            14  //����ʽ��ʾ��	
#define WK_WG_STATIC	        15  //��̬��	����ʾͼƬ���ı������У���
#define WK_WG_MATRIXMENUITEM	16  //����˵�����	
#define WK_WG_MATRIXMENU	    17  //����˵��б�	��һ�����˵�������ɣ�������ʾ�Ź���˵���
#define WK_WG_SPINBOX   18  //��ť�ؼ�
#define WK_WG_GLIDEPOLE   19 //����

#define WK_WG_MAXWIDGETTYPENUM  20  //ö�����ֵ

//�ؼ����ö��
#define WK_WG_FGGRAY	        0x01   //�Ƿ��ûң���Ч��	
#define WK_WG_FGFOCUS	        0x02   //�Ƿ��ý���	
#define WK_WG_FGPUSH	        0x04   //�Ƿ���	BUTTON
#define WK_WG_FGGETFOCUS	    0x08   //�Ƿ���ܻ�ý���	������ͨ�������������޷���ý���Ŀؼ���
#define WK_WG_FGPENSTATE        0x10   //������״̬��pendown��penmove�Ժ��penup������ܲ�ͬ��
#define WK_WG_FGNOTSHOW         0x20   //����ʾ

//�������ö��
#define WK_WG_SLSHOW	        0x01   //�Ƿ���ʾ	�ò��Ƿ����ڱ�������
#define WK_WG_SLPOPUP	        0x02   //����ʽ��ʾ��	
#define WK_WG_SLPOPMENU         0x04   //����ʽ�˵�

//�б�����ö��
#define WK_LIST_TYPE_TEXT	    0x01   //��ʾ����	
#define WK_LIST_TYPE_TEXT_ICON	0x02  //��ʾ���ֺ�ͼƬ	������ʾͼƬ������
#define WK_LIST_TYPE_RADIO	    0x04   //��ѡ	
#define WK_LIST_TYPE_CHECKBOX	0x08   //��ѡ	

//�б���������ʾ����ö��
#define WK_LIST_TYPE_TEXT_INFO_UP_DOWN	    0x10   //������������	
#define WK_LIST_TYPE_TEXT_INFO_LEFT_RIGHT 	0x20  //������������	


//��̬����ʾ��������ö��
#define WK_STATIC_TYPE_IMAGE	        1   //��ʾͼƬ 	
#define WK_STATIC_TYPE_TEXT	            2   //��ʾ����(����ɱ�ѡ��)	
#define WK_STATIC_TYPE_ONLY_READ_TEXT	3   //��ʾֻ���ı�(���ᱻѡ��)	

//��ʾ��ʽö��
#define WINKS_SHOW_LEFT	    1   //������ʾ	
#define WINKS_SHOW_CENTER	2   //������ʾ	
#define WINKS_SHOW_RIGHT	3   //������ʾ	

//�Ի���ö��
#define WK_DIALOG_TYPE_NOMAL	1   //��ͨ�ĶԻ���	
#define WK_DIALOG_TYPE_SCREEN	2   //��Ļʽ�ĶԻ���	

//�ؼ��޸�����
//WK_WG_POPUP
#define WK_WG_PPCF_TTSTR        1   //��������
#define WK_WG_PPCF_UPSTR        2   //�ϲ�����
#define WK_WG_PPCF_ICON	        3   //�в�ͼ��
#define WK_WG_PPCF_DNSTR        4   //�²�����

//�ؼ��������ݽṹ
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

//��������ݽṹ
typedef struct tag_Winks_Widget_ScnLayer_s
{
    struct tag_Winks_Widget_ScnLayer_s* pnext;
    unsigned long flag;
    int LayerID;
    int layernum; 
    Winks_Widget_s* pWidget;
}Winks_Widget_ScnLayer_s;

//�༭�����ݽṹ
typedef struct tag_Winks_Widget_EditBox_s
{
    Winks_Widget_s bwidget; 
    unsigned long dfstring;
    winks_scrolling_text_s scrolling_text;
}Winks_Widget_EditBox_s;

//��ť���ݽṹ
typedef struct tag_Winks_Widget_Button_s
{
    Winks_Widget_s bwidget;
	unsigned long v_stringId;//ID is null, not display 
   	unsigned long normal_up_icon;
    unsigned long normal_down_icon;
    int v_nextscreenID;
}Winks_Widget_Button_s;

//�б������ݽṹ
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

//�б������ݽṹ
typedef struct tag_Winks_Widget_ListGroup_s
{
    Winks_Widget_s bwidget;
	int v_TotalItemsNum;
	int v_FirstItemIndx;
	int v_FocusItemIndex;//the index number
    int ifScrollBar;
    Winks_Widget_Listitem_s* pa_ItemListData;
}Winks_Widget_ListGroup_s;

//��ҳ�������������ݽṹ
typedef struct tag_Winks_Widget_TapListItem_s
{
    Winks_Widget_s bwidget;
	unsigned long v_IconID;
	unsigned long v_string;
	int v_ScreenID;
}Winks_Widget_TapListItem_s;

//��ҳ���������ݽṹ
typedef struct tag_Winks_Widget_TapTitle_s
{
    Winks_Widget_s bwidget;
	int v_TotalItemsNum;
	int v_FocusItemIndex;
    unsigned long v_HighlightImageId;
    Winks_Widget_TapListItem_s a_ItemListData[1];
}Winks_Widget_TapTitle_s;

//��ͨ���������ݽṹ
typedef struct tag_Winks_Widget_NormalTitle_s
{
    Winks_Widget_s bwidget;
	unsigned long v_IconID;
    unsigned long v_TitleStringID;
    unsigned long v_icondigit;
}Winks_Widget_NormalTitle_s;

//�����˵��������ݽṹ
typedef struct tag_Winks_Widget_PopMenuItem_s
{
    Winks_Widget_s bwidget;
	unsigned long v_IconID;
    unsigned long v_StringID;
    int v_NextScreenId;
}Winks_Widget_PopMenuItem_s;

//����ʽ�˵����ݽṹ
typedef struct tag_Winks_Widget_PopMenu_s
{
    Winks_Widget_s bwidget;//�������½ǵ�������Ϊ(bwidget.lux, bwidget.luy)�����Բ���width, high
    int v_TotalItemsNum;
    int v_FocusItemIndex;//the index number
    Winks_Widget_PopMenuItem_s a_ItemList[1];
}Winks_Widget_PopMenu_s;

//������ݽṹ
typedef struct tag_Winks_Widget_SoftKey_s
{
    Winks_Widget_s bwidget;
    unsigned long v_IconID;
    unsigned long v_StringID;
   	unsigned long normal_up_icon;
    unsigned long normal_down_icon;
}Winks_Widget_SoftKey_s;

//����ʽ��ʾ�����ݽṹ
typedef struct tag_Winks_Widget_PopUp_s
{
    Winks_Widget_s bwidget;
    int flag; //dialog | screen
    unsigned long v_TitleStringID;
   	unsigned long v_UpStringID;
    unsigned long v_Icon;
    unsigned long v_DownStringID;
}Winks_Widget_PopUp_s;

//��̬�����ݽṹ
typedef struct tag_Winks_Widget_Static_s
{
    Winks_Widget_s bwidget;
    unsigned short d_flag; //left | right | center
    unsigned short c_flag; //image | text
    unsigned long v_ContentID;

	//Ϊ�ؼ��ڲ�ʹ��
   winks_text_String_rect_s rect_text_String;
}Winks_Widget_Static_s;

//����˵����Ź��񣩵������ݽṹ
typedef struct tag_Winks_Widget_MatrixMenuItem_s
{
    Winks_Widget_s bwidget;
    unsigned long nmIconID;
    unsigned long hlIconID;
    unsigned long v_StringID;
    int v_NextScreenID;
}Winks_Widget_MatrixMenuItem_s;

//����˵����Ź������ݽṹ
typedef struct tag_Winks_Widget_Matrixmenu_s
{
    Winks_Widget_s bwidget;
	int v_TotalMatrixItemsNum;
	int v_FocusMatrixItemIndex;//the index number of the screen
	int v_numberbycolumn;
	Winks_Widget_MatrixMenuItem_s a_MenuItem[1];
}Winks_Widget_MatrixMenu_s;

//��ť�ؼ����ݽṹ
typedef struct tag_Winks_Widget_SpinBox_s
{
    Winks_Widget_s bwidget;
    int v_TotalItemNums;//Static�ؼ�����
    int v_FocusItemIndex;//��ǰ����index
    int bg_iconid;//������ͼƬ
    int left_iconid;//���ͷ
    int hl_left_iconid;//���ͷ����
    int right_iconid;//�Ҽ�ͷ
    int hl_right_iconid;//�Ҽ�ͷ����
    Winks_Widget_Static_s a_Static[1];
}Winks_Widget_SpinBox_s;

//���������ݽṹ
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

//�����˳���������ѡ�
#define WK_UIFM_OPEXIT	1   //�˳����档	
#define WK_UIFM_OPFREE	2   //�����ͷſؼ�������Դ��	

//������Ϣö�٣�
#define WK_UIFM_MGSUSPEND	1   //������Ϣ	���汻���ǡ�
#define WK_UIFM_MGRESUME	2   //�ָ���Ϣ	����������ʾ��
#define WK_UIFM_MGPAINT	    3   //�ػ���Ϣ	

//Ӧ����Ϣö�٣�
#define WK_UIFM_MGAPN	    1   //APN���ҽ��	
#define WK_UIFM_MGFILE	    2   //�ļ����ҽ��	
#define WK_UIFM_MGINPUT	    3   //���뷨������
#define WK_UIFM_MGPIM       4   //�绰�����ҽ��	

//������뺯��ԭ��
typedef int (*Winks_UIFM_EnterScreen)( int Opcode );
//�����˳�����ԭ��
typedef int (* Winks_UIFM_ExitScreen)( Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam );
//��ʱ���ص�����ԭ��
typedef int (*Winks_UIFM_Timer)( void* Param );
//������Ϣ����֪ͨ����ԭ��
typedef int (* Winks_UIFM_UIHandler)( void* Param );
//������Ϣ����֪ͨ����ԭ��
typedef int (* Winks_UIFM_KeyHandler)( int KeyType );
#ifdef WINKS_TOUCH_PANNEL
//��������Ϣ����֪ͨ����ԭ��
typedef int (*Winks_UIFM_PenHandler)( int PenType, int x, int y );
#endif
//Ӧ����Ϣ����֪ͨ����ԭ��
typedef int (* Winks_UIFM_AppHandler)( void* pData, unsigned long size );
//������Ϣ����֪ͨ����ԭ��
typedef int (*Winks_UIFM_EngineHandler)( void* Param, unsigned long size );
//�绰��Ϣ����֪ͨ����ԭ��
typedef int (* Winks_UIFM_CallHandler)( unsigned long Msg, void* pData, unsigned long size );

//�������ӿ�
int Winks_UIFM_EnterNewScreen( int ScreenID, void* Param );
int Winks_UIFM_SetCurrentScreenParam( unsigned long* pwParam, unsigned long* plParam );
int Winks_UIFM_GetCurrentScreenParam( int* pScreenID, unsigned long* pwParam, unsigned long* plParam );
Winks_Widget_ScnLayer_s* Winks_UIFM_GetScreenLayer( int ScreenID );
int Winks_UIFM_SetScreenLayer( int ScreenID, Winks_Widget_ScnLayer_s* pScnLayer, int memlen );
//��Դ����ӿ�
const char* Winks_UIFM_GetString( unsigned long StringID );
const char* Winks_UIFM_GetImage( unsigned long ImageID );
//��ʱ������ӿ�
int Winks_UIFM_StartTimer( int Period, Winks_UIFM_Timer pfCallBack );
int Winks_UIFM_StopTimer( int TimerID );
//��Ϣ���������ýӿ�
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
/*****************��ҳ�������������ݣ�ʵ��ID�ƶ����Ժ���Ҫɾ��************************/

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

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS				0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS				((WINKS_SCREEN_HEIGHT * 93 + 40) / 100)
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH				WINKS_SCREEN_WIDTH
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT				((WINKS_SCREEN_HEIGHT * 6 + 180) / 100)

#endif
