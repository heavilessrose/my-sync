/**************************************************************************
Copyright (C) Beijing Mobile Winks Co., Ltd
All rights reserved.
 
This source code is part of winks software and is licensed by WINKS SOFTWARE LICENSE AGREEMENT.
 
The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.
 
THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL ANY PERSON WHO HAS CONTRIBUTED TO OR IS THE OWNER OF ANY PART OF THIS SOFTWARE BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************
*Id: wk_uifm.h,v 1.00 
**************************************************************************    
* File Description
* ----------------
* Color call UI framework global define. 
**************************************************************************/

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
#define WK_WG_EDITBOX            1   //�༭��    
#define WK_WG_BUTTON            2   //��ť    
#define WK_WG_LISTITEM            3   //�б���    
#define WK_WG_LISTGROUP            4   //�б���    ��һ��LISTITEM��ɣ��������������Ϣ���������ɿؼ���ܻ��ơ�
#define WK_WG_TAPTITLEITEM        5   //��ҳ����������    
#define WK_WG_TAPTITLE            6   //��ҳ������    ��һ���ҳ������������ɡ�
#define WK_WG_NORMALTITLE        7   //��ͨ������    
#define WK_WG_POPUPMENUITEM        8   //�����˵�����    
#define WK_WG_POPMENU           9   //����ʽ�˵�
#define WK_WG_LEFTSOFTKEY        10   //�����    ��һ����ť��һ�������˵�������ɡ�ʹ�ù��õ�����ṹ��Winks_Widget_SoftKey_s��
#define WK_WG_RIGHTSOFTKEY        11  //�����    ��һ����ť��һ�������˵�������ɡ�ʹ�ù��õ�����ṹ��Winks_Widget_SoftKey_s����
#define WK_WG_CENTERSOFTKEY        12  //�м����    ��һ����ť��һ�������˵�������ɡ�ʹ�ù��õ�����ṹ��Winks_Widget_SoftKey_s����
#define WK_WG_POPUP                14  //����ʽ��ʾ��    
#define WK_WG_STATIC            15  //��̬��    ����ʾͼƬ���ı������У���
#define WK_WG_MATRIXMENUITEM    16  //����˵�����    
#define WK_WG_MATRIXMENU        17  //����˵��б�    ��һ�����˵�������ɣ�������ʾ�Ź���˵���
#define WK_WG_SPINBOX   18  //��ť�ؼ�
#define WK_WG_GLIDEPOLE   19 //����
#define WK_WG_EMBEDDED_BROWSER   20 //��Ƕ�����

#define WK_WG_MAXWIDGETTYPENUM  21  //ö�����ֵ

//�ؼ����ö��
#define WK_WG_FGGRAY            0x01   //�Ƿ��ûң���Ч��    
#define WK_WG_FGFOCUS            0x02   //�Ƿ��ý���    
#define WK_WG_FGPUSH            0x04   //�Ƿ���    BUTTON
#define WK_WG_FGGETFOCUS        0x08   //�Ƿ���ܻ�ý���    ������ͨ�������������޷���ý���Ŀؼ���
#define WK_WG_FGPENSTATE        0x10   //������״̬��pendown��penmove�Ժ��penup������ܲ�ͬ��
#define WK_WG_FGNOTSHOW         0x20   //����ʾ
#define WK_WG_PLAYING            0x40   //��ʼ����GIF�ļ�

//�������ö��
#define WK_WG_SLSHOW            0x01   //�Ƿ���ʾ    �ò��Ƿ����ڱ�������
#define WK_WG_SLPOPUP            0x02   //����ʽ��ʾ��    
#define WK_WG_SLPOPMENU         0x04   //����ʽ�˵�

//�б�����ö��
#define WK_LIST_TYPE_TEXT        0x01   //��ʾ����    
#define WK_LIST_TYPE_TEXT_ICON    0x02  //��ʾ���ֺ�ͼƬ    ������ʾͼƬ������
#define WK_LIST_TYPE_RADIO        0x04   //��ѡ    
#define WK_LIST_TYPE_CHECKBOX    0x08   //��ѡ    

//�б���������ʾ����ö��
#define WK_LIST_TYPE_TEXT_INFO_UP_DOWN        0x10   //������������    
#define WK_LIST_TYPE_TEXT_INFO_LEFT_RIGHT     0x20  //������������    


//��̬����ʾ��������ö��
#define WK_STATIC_TYPE_IMAGE            1   //��ʾͼƬ     
#define WK_STATIC_TYPE_TEXT                2   //��ʾ����(����ɱ�ѡ��)    
#define WK_STATIC_TYPE_ONLY_READ_TEXT    3   //��ʾֻ���ı�(���ᱻѡ��)    
#define WK_STATIC_TYPE_SINGLE_SCROLLING_TEXT    4   //��ʾ���й����ı�(���ᱻѡ��)    

//��ʾ��ʽö��
#define WINKS_SHOW_LEFT        1   //������ʾ    
#define WINKS_SHOW_CENTER    2   //������ʾ    
#define WINKS_SHOW_RIGHT    3   //������ʾ    

//�Ի���ö��
#define WK_DIALOG_TYPE_NOMAL    1   //��ͨ�ĶԻ���    
#define WK_DIALOG_TYPE_SCREEN    2   //��Ļʽ�ĶԻ���    

//�ؼ��޸�����
//WK_WG_POPUP
#define WK_WG_PPCF_TTSTR        1   //��������
#define WK_WG_PPCF_UPSTR        2   //�ϲ�����
#define WK_WG_PPCF_ICON            3   //�в�ͼ��
#define WK_WG_PPCF_DNSTR        4   //�²�����
#define WK_WG_PPCF_INFO          5   //�ٷֱ�

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
    unsigned short   v_offx;//�ַ���ʾλ����������ϵ��ƫ��
    unsigned short d_flag; //left | right | center
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
    short             v_IconID_Width;  // IconID width || WK_LIST_TYPE_TEXT����ʱ��������ƫ�ơ�
    short             v_IconID_Height;  
    unsigned long     v_IconID;  //filename image
    int               v_NextScreenId;  //menu's next screen
    winks_scrolling_text_s scrolling_text;
}Winks_Widget_Listitem_s;

//�б������ݽṹ
typedef struct tag_Winks_Widget_ListGroup_s
{
    Winks_Widget_s bwidget;
    int v_TotalItemsNum;
    int v_FirstItemIndx;
    int v_FocusItemIndex;//the index number
    int v_PendownIndex;
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
    winks_scrolling_text_s scrolling_text;
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
    int v_MenuItemHeight;
    int v_LeftBottom_x;
    int v_LeftBottom_y;
    int v_Pen_Down_Index;
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
    char *            pinfo_str;//�ٷֱ�
    WINKS_ANIM_HANDLE anim_handle;
}Winks_Widget_PopUp_s;

//��̬�����ݽṹ
typedef struct tag_Winks_Widget_Static_s
{
    Winks_Widget_s bwidget;
    unsigned short d_flag; //left | right | center
    unsigned short c_flag; //image | text
    unsigned long v_ContentID;
     unsigned long string_type;

    //Ϊ�ؼ��ڲ�ʹ��
   winks_text_String_rect_s rect_text_String;
   winks_scrolling_text_s scrolling_text;
   WINKS_ANIM_HANDLE anim_handle;
}Winks_Widget_Static_s;

//����˵����Ź��񣩵������ݽṹ
typedef struct tag_Winks_Widget_MatrixMenuItem_s
{
    Winks_Widget_s bwidget;
    unsigned long nmIconID;
    unsigned long hlIconID;
    unsigned long v_StringID;
    int v_NextScreenID;
    int v_type;//0:�Ź���1:�б�
    int v_separator_icon_height;
}Winks_Widget_MatrixMenuItem_s;

//����˵����Ź������ݽṹ
typedef struct tag_Winks_Widget_Matrixmenu_s
{
    Winks_Widget_s bwidget;
    int v_TotalMatrixItemsNum;
    int v_FocusMatrixItemIndex;//the index number of the screen
    int v_numberbycolumn;
    int v_numberbyline;
    int v_separator_icon_width;
    int v_separator_icon_height;
    int v_separator_icon_id;
    int v_Pen_Down_Index;
    int v_type;//0:�Ź���1:�б�
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

//��Ƕ�����
typedef struct tag_Winks_Widget_embedded_browser_s
{
    Winks_Widget_s bwidget; 
    char *  p_url;
    unsigned long  browser_handle;
    
}Winks_Widget_embedded_browser_s;

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
int Winks_Widget_ActiveWidget_Ex( Winks_Widget_s* pRootWidget, int WidgetID );
int Winks_Widget_ActiveWidget( Winks_Widget_ScnLayer_s* pLayer, int WidgetID );


//�����˳���������ѡ�
#define WK_UIFM_OPEXIT    1   //�˳����档    
#define WK_UIFM_OPFREE    2   //�����ͷſؼ�������Դ��    

//������Ϣö�٣�
#define WK_UIFM_MGSUSPEND    1   //������Ϣ    ���汻���ǡ�
#define WK_UIFM_MGRESUME    2   //�ָ���Ϣ    ����������ʾ��
#define WK_UIFM_MGPAINT        3   //�ػ���Ϣ  
#define WK_UIFM_MGMODIFY 4//�޸���Ϣ  

//Ӧ����Ϣö�٣�
#define WK_UIFM_MGAPN        1   //APN���ҽ��    
#define WK_UIFM_MGFILE        2   //�ļ����ҽ��    
#define WK_UIFM_MGINPUT        3   //���뷨������
#define WK_UIFM_MGPIM       4   //�绰�����ҽ�� 

#define WK_UIFM_MGINPUTSTATE        5   //���뷨״̬
#define WK_UIFM_EMBEDDEDBROWSER        6  //��Ƕ�����

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
//�����ʼ���ɹ��ص�����
typedef int (* Winks_UIFM_EngineRegHandler)( int Result );

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
#else
#define Winks_UIFM_SetPenHandler(a)
#endif
int Winks_UIFM_SetEngineRegHandler( Winks_UIFM_EngineRegHandler pHandler );
int Winks_UIFM_SetAppHandler( int Option, Winks_UIFM_AppHandler pHandler );
int Winks_UIFM_SetCallHandler( Winks_UIFM_CallHandler pHandler );
int Winks_UIFM_SetEngineHandler( int Option, Winks_UIFM_EngineHandler pHandler );
int Winks_UIFM_UnSetEngineHandler( int HD );
int Winks_UIFM_ExitApp( void );

//�û��Ƿ��Ѿ����һ������״̬��1Ϊ�Ѿ������0Ϊδ���
int Winks_UIFM_GetActiveFlag(void);
int Winks_UIFM_SetActiveFlag(int flag);

int Winks_GetDisplayMode();											/*��ȡ��Ļ��ʾ��ʽ��0Ϊ������1Ϊ����*/
#define WINKS_DM					Winks_GetDisplayMode()

#ifdef WINKS_COLLECT
//�û��Ƿ��Ѿ����һ���ղع��ܣ�NULLΪδ���,�Ѿ��������wid
char* Winks_UIFM_GetCollectWid(void);
int Winks_UIFM_SetCollectWid(char *wid);
#endif

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

enum winks_errortype
{
    WK_ERROR_DISABLE = 0,
    WK_ERROR_NOSIM,
    WK_ERROR_OTHER
};

// ����Ź���������
enum TMainMenuEnterType
{
    EMainMenuOther = 0, //����������봦��ֱ����ʾ�Ź���
    EMainMenuUploadPhone, //��һ�μ����ʾ�ϴ��绰��
    EMainMenuLoadingError, //��Loadingҳ�������룬��Ҫ��ʾ�û�ѡ������
    EMainMenuLoadingOk //��Loadingҳ���������룬��Ҫ�������Ƿ�ر�
};

// ����δ����ҳ�����
enum TUnActiveEnterType
{
    EUnActiveOther = 0, //����������봦��ֱ����ʾδ����ҳ�棬����ӹ��ڷ���
    EUnActiveLoadingError, //��Loadingҳ�������룬��Ҫ��ʾ�û�ѡ������
    EUnActiveLoadingOk //��Loadingҳ���������룬����ADC��Ϣ�ж��Ƿ���ʾ�û�������Զ�����
};

#define WK_TAPTITLE_WIGDETID            1

#ifdef __240X320__

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_V                 2
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_V                 2
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_V                236//(240 - 4)  
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_V             24//((320 * 8 - 160) / 100);

// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    2
#define WINKS_SCRPOS_BGGRAY_YPOS                    2
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 4)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 23)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_V                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_V                298//((320 * 93 + 40) / 100)
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_V                240
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_V                21//((320 * 6 + 180) / 100)

//#elif defined(__320X240__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_H                2
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_H                2
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_H              316//  (320 - 4)
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_H            24//(240 * 10 / 100)

#if 0
// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    2
#define WINKS_SCRPOS_BGGRAY_YPOS                    2
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 4)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 23)
#endif

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_H                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_H                219//(240 * 91 / 100 + 1)
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_H                320
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_H               21// ((240 * 9 / 100)

#elif defined(__480X800__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_V                 5
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_V                 4
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_V                470
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_V             59

// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    5
#define WINKS_SCRPOS_BGGRAY_YPOS                    4
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 10)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 52 - 4)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_V                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_V                748
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_V                480
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_V                52

//#elif defined(__320X240__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_H                5
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_H                4
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_H				790
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_H				59

#if 0
// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    2
#define WINKS_SCRPOS_BGGRAY_YPOS                    2
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 4)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 23)
#endif

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_H                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_H                428
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_H                800
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_H               52

#elif defined(__360X640__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_V                 3
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_V                 3
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_V                354
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_V				44

// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    3
#define WINKS_SCRPOS_BGGRAY_YPOS                    3
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 6)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 42)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_V                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_V                598
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_V                360
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_V                42

//#elif defined(__320X240__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_H                3
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_H                3
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_H              634
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_H				44

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_H                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_H                318
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_H                640
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_H               42

#elif defined(__480X640__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_V                 5
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_V                 4
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_V                470
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_V             59

// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    5
#define WINKS_SCRPOS_BGGRAY_YPOS                    4
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 10)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 52)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_V                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_V                588
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_V                480
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_V                52

//#elif defined(__320X240__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_H                5
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_H                4
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_H              630
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_H            59

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_H                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_H                428
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_H                640
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_H               52

#elif defined(__240X400__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_V                 2
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_V                 2
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_V                236//(240 - 4)  
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_V             24//((320 * 8 - 160) / 100);

// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    2
#define WINKS_SCRPOS_BGGRAY_YPOS                    2
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 4)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 26)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_V                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_V                374
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_V                240
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_V                26

//#elif defined(__320X240__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_H                2
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_H                2
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_H              396//  (320 - 4)
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_H            24//(240 * 10 / 100)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_H                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_H                214
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_H                400
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_H               26

#elif defined(__176X205__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_V               0
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_V                0
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_V                176
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_V               30

// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                      0
#define WINKS_SCRPOS_BGGRAY_YPOS                      0
#define WINKS_SCRPOS_BGGRAY_WIDTH                     WINKS_SCREEN_WIDTH
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 28)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_V				  0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_V                  177//(205 - 28)
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_V                 176
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_V                28

//if defined(__205X176__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_H                2
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_H                2
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_H              205//  (320 - 4)
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_H            24//(240 * 10 / 100)
#if 0

// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    2
#define WINKS_SCRPOS_BGGRAY_YPOS                    2
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 4)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 23)
#endif

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_H                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_H                155//(240 * 91 / 100 + 1)
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_H                215
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_H               21// ((240 * 9 / 100)

#elif defined(__240X377__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_V                 2
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_V                 2
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_V                236//(240 - 4)  
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_V             24//((320 * 8 - 160) / 100);

// ������ɫ���ο� λ��
#define WINKS_SCRPOS_BGGRAY_XPOS                    2
#define WINKS_SCRPOS_BGGRAY_YPOS                    2
#define WINKS_SCRPOS_BGGRAY_WIDTH                    (WINKS_SCREEN_WIDTH - 4)
#define WINKS_SCRPOS_BGGRAY_HEIGHT                    (WINKS_SCREEN_HEIGHT - 26)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_V                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_V                351
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_V                240
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_V                26

//#elif defined(__320X240__)

// TopTitle �ؼ�λ��
#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS_H                2
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS_H                2
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH_H              396//  (320 - 4)
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT_H            24//(240 * 10 / 100)

// ������������� �ؼ�λ��
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS_H                0
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS_H                214
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH_H                400
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT_H               26

#endif // #ifdef __240X320__
extern const int TAPTITLE[5][2];

#define WINKS_SCRPOS_UIFMTOPTITLE_XPOS                ((int )TAPTITLE[0][WINKS_DM])//0
#define WINKS_SCRPOS_UIFMTOPTITLE_YPOS                ((int )TAPTITLE[1][WINKS_DM])//0
#define WINKS_SCRPOS_UIFMTOPTITLE_WIDTH              ((int )TAPTITLE[2][WINKS_DM]) //WINKS_SCREEN_WIDTH
#define WINKS_SCRPOS_UIFMTOPTITLE_HEIGHT             ((int )TAPTITLE[3][WINKS_DM])// 30

extern const int SOFTKEYBAR[4][2];
#define WINKS_SCRPOS_SOFTKEYBAR_XPOS                ((int )SOFTKEYBAR[0][WINKS_DM])
#define WINKS_SCRPOS_SOFTKEYBAR_YPOS                ((int )SOFTKEYBAR[1][WINKS_DM])
#define WINKS_SCRPOS_SOFTKEYBAR_WIDTH             ((int )SOFTKEYBAR[2][WINKS_DM])
#define WINKS_SCRPOS_SOFTKEYBAR_HEIGHT            ((int )SOFTKEYBAR[3][WINKS_DM])

#define WINKS_IFISID(Strid) ((Strid <= 255 && Strid > 0)?1:0)
#endif

#ifdef WINKS_EMBEDDED_BROWSER
/*��Ƕ���������*/
typedef struct tag_Winks_EBrowser_Param_s
{
	int nScreenID;								/*���ô˽������ĻID*/
	char* wid;									/*URL����*/
	int bHelpFlag;								/*Helpҳ���ʾ��1����Helpҳ�棬0Ϊ��Helpҳ��*/
}Winks_EBrowser_Param_s;

#endif //#ifdef WINKS_EMBEDDED_BROWSER
