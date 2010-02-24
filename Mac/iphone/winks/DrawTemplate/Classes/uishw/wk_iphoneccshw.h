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
*Id: wk_ccshw.h,v 1.00 
**************************************************************************    
* File Description
* ----------------
* Color call show define. 
**************************************************************************/
#ifndef _WK_IPHONECCSHW_H_
#define _WK_IPHONECCSHW_H_

//#include "wk_osal.h"
#include "wk_xml.h"
#include "wk_engin.h"
 

//from other. head file h
#define WINKS_CCSHOW_TID_MAX_LEN 60

#define WINKS_TRUE  1   /* 真(是) */
#define WINKS_FALSE 0   /* 假(否) */

//from wk_osal.h
#define WK_WG_NOWRAP 0
#define WK_WG_WRAP   1//折叠

#define WK_WG_FONTRIGHTTRUNC    4       //从右边截断
#define WK_WG_GRADIENTCOLORANIMATION   1 //彩色色块在文字上滚动
#define WK_WG_FONTLEFTSCROLL    1       //从左向右滚动
#define WK_WG_FONTRIGHTSCROLL   2       //从右向左滚动
#define WK_WG_FONTUPSCROLL 3 // 从下向上滚动
#define WK_WG_TEXTCOLORSLIDE   2//每个文字每秒显示不同的颜色
#define WINKS_NEEDSCROLLING_FLAG   1   //显示文本需要滚动
#define WK_WG_STRETCH_NONE  0//默认

#define WK_WG_STRETCH_UNIFORM  1//等比拉伸
#define WK_WG_STRETCH_FILL   2//填充
#define WK_WG_STRETCH_UNIFORMFILL  3//等比剪裁

#define WINKS_PATH_MAX          512                          /*定义文件绝对路径最大长度*/

/*字符串类行*/
#define WK_STRING_TYPEASCII      1
#define WK_STRING_TYPEUNICODE    2

#define     WINKS_IMAGE_TOO_LARGE    3

#define WINKS_PIXEL_DARKGRAY              0x80808000     // dark gray     --7 

#if 1
typedef enum WINKS_SHOW_FLAG_TAG
	{
        WINKS_SHOW_LEFT = 1,
        WINKS_SHOW_CENTER,
        WINKS_SHOW_RIGHT
	}WINKS_SHOW_FLAG;
#endif

typedef enum WINKS_FONT_TAG
	{
		WINKS_SMALL_FONT = 1,
		WINKS_MEDIUM_FONT,
		WINKS_LARGE_FONT
	}WINKS_FONT;

typedef enum  Winks_line_Dashstyle
	{        
		WINKS_DASHSTYLE_NULL,   
		WINKS_LINE_SOLID,/*实线*/   
		WINKS_LINE_DOT,  /*点*/    
		WINKS_LINE_DOTDASH,/*点线*/
		WINKS_LINE_DOUBLELINE/*双线*/
	}Winks_line_Dashstyle_e;

typedef struct winks_Rect
	{
		int  v_Top;
		int  v_Left;
		int  v_Bottom;
		int  v_Right;
	} winks_Rect_s;

typedef struct tag_Winks_DateTime_s
	{
		unsigned long year;      /* 1752~2999 */
		unsigned char month;     /* 1~12 */
		unsigned char day;       /* 1~31 */
		unsigned char hour;      /* 0~23 */
		unsigned char min;       /* 0~59 */
		unsigned char sec;       /* 0~59 */
		unsigned char week;      /* 1~6,0(mon~sun)*/
	}Winks_DateTime_s;

typedef unsigned int         winks_color;
typedef unsigned long WINKS_TIMER_ID;
#define WINKS_CALL_NUMBER_LENGTH    40  /* ref "WINKS_FRIEND_NO_LEN" */
/* 字符串描述结构 */
typedef struct tag_Winks_Widget_Font_s
	{
		int style;    //粗，斜，下划线 wk_osal.h
		int size;     //大，中，小 wk_osal.h
		unsigned short type;     //黑，宋。。。
		unsigned short way;      //滚动，截断，左右(超长时)
		unsigned long color;
	}Winks_Widget_Font_s;

typedef struct winks_text_colorlist
	{
		unsigned short  tal_colornum;  //集合的颜色数
		unsigned short  curr_index;//最先使用颜色
		winks_color * colorlist;    //颜色列表
	} winks_text_colorlist_s;

typedef struct winks_scrolling_text
	{
		int x, y;
		int width, height;
		
		winks_color backgroud_color;
		int  bgimage_x;
		int  bgimage_y;
		char * draw_backgroundimage;
		Winks_Widget_Font_s  font_feature;
		char* text;
		short txtshadow_x;
		short txtshadow_y;
		winks_color txtshadow_color;
		unsigned char show_flags;/*WINKS_SHOW_LEFT        1   //居左显示    WINKS_SHOW_CENTER    2   //居中显示    WINKS_SHOW_RIGHT    3   //居右显示*/   
		unsigned char effect_flags;//特效
		unsigned char wrapflags;  //WK_WG_NOWRAP   0//不折叠    WK_WG_WRAP   1//折叠 
		unsigned char lineflags; // WK_WG_ONLYLINE  0//单行 WK_WG_MULLINE   1//多行
		
		unsigned long backgroudcolorflags; //  0 忽略背景颜色// 1 绘制背景颜色
		winks_text_colorlist_s text_colorlist;
		
		/* 下面可不填写*/
		long int ismul_line_flags;//是否是多行数据
		int text_width, text_height;
		long int flags;
		int scroll_size;
		int scroll_gap;
		int scroll_pause;
		//compatable with old ui
		WINKS_TIMER_ID timer_id;
		struct winks_scrolling_text * p_next;
		
		
		int scroll_x;
		int scroll_counter;
		
		
		
		int      v_Stringline_talNum;
		int      v_FirstlineIndx;
		
		int      line_number_area;
		char * first_inshow_text;
		int      string_y;
		//int scroll_time;
		//Winks_Hwnd Hwnd;  //
		
		
	} winks_scrolling_text_s;

typedef enum WINKS_FONT_FORMAT
	{
		WINKS_FONT_STANDARD = 1,
		WINKS_FONT_BOLDFACED = 2,
		WINKS_FONT_INCLINE = 4,
		WINKS_FONT_UNDERLINE = 8
		
	}WINKS_FONT_FORMAT_E;



//end

//from wk_ospub.h
//#define Winks_salAlloc         malloc
//#define Winks_mem_set          memset
//#define Winks_mem_cpy         memcpy
//end

/* 显示标记 */
#define WK_CCSW_RUN         0x01
#define WK_CCSW_SHOW        0x02
#define WK_CCSW_SUSPEND     0x04
#define WK_CCSW_NEEDSHOW    0x08

#define WK_CCSW_LSKACTIVE   0x10
#define WK_CCSW_LSKACTIVED  0x20
#ifdef WINKS_COLLECT
#define WK_CCSW_LSKCOLLECT  0x40
#define WK_CCSW_LSKCOLLECTED    0x80
#endif

/* 呼叫状态 */
#define WK_CCSW_IDLE        0       //无效
#define WK_CCSW_INCALL      0x01       //呼叫到来
#define WK_CCSW_OUTCALL     0x02       //呼出
#define WK_CCSW_WAITCONNECT 0x04       //等待接通（MTK）
#define WK_CCSW_CONNECT     0x08       //接通
#define WK_CCSW_HUNGUP      0x10       //挂断

/* 显示类型 */
#define WK_CCSW_TITLE       1       //片头彩像
#define WK_CCSW_USER        2       //用户彩像
#define WK_CCSW_DEFAULT     3       //默认彩像
#define WK_CCSW_BUS         4       //运营商彩像

/* 心跳tick */
#define WK_CCSW_TICK        WINKS_MILLISECONDS2TICKS(300)

/* 挂断后最大显示时间 */
#define WK_CCSW_HUMAXTIME      3599      //1h

#define WK_CCSW_PNAMELEN    64

typedef struct tag_Winks_CCSW_Info_s
{
	Winks_DateTime_s titletime;
    Winks_DateTime_s hanguptime;
    unsigned short showtype;
    unsigned short hanguplen;
    unsigned short titlelen;
    unsigned short tick;
    unsigned short elapse;
    unsigned short wconnlen;
    char name[WK_CCSW_PNAMELEN];
    char number[WINKS_CALL_NUMBER_LENGTH + 4];
}Winks_CCSW_Info_s;

/* CCDW structures */
typedef struct tag_Winks_CCDW_Convert_s
{
#if ((defined(WINKS_UNSUPPORTED_GLOBAL_VAR))||(defined(WINKS_DYNAMIC_LOAD_BLOCK)))
    char pstr[24];
#else
    char* pstr;
#endif
    unsigned long value;
}Winks_CCDW_Convert_s;

/*
#define WK_CCDW_SDIAL   1
#define WK_CCDW_RING    2
#define WK_CCDW_CONNECT 3
#define WK_CCDW_DISCONN 4
*/

#define WK_CCDW_DEFCOLOR    0xffffffff

typedef struct tag_Winks_CCDW_Section_s
{
    int x;              //区域坐标x
    int y;              //区域坐标y
    int w;              //区域宽
    int h;              //区域高
    unsigned long state;    //区域显示状态
    int z;              //区域z（一版不支持）
}Winks_CCDW_Section_s;

#define WK_CCDW_FTSMALLMAX  16
#define WK_CCDW_FTMEDIUMMAX 20
#define WK_CCDW_FTLARGEMAX  24

/*
#define WK_CCDW_FTITALIC    0x01    //斜体
#define WK_CCDW_FTBOLD  0x02        //粗体
*/
/*
typedef struct tag_Winks_CCDW_Font_s
{
    unsigned short family;  //字体家族
    unsigned short size;    //字体大小（点阵）
    unsigned long style;    //字体类型（粗，斜）
}Winks_CCDW_Font_s;
*/
/*
#define WK_CCDW_FTALLEFT    0       //左对齐
#define WK_CCDW_FTALCENTER  1       //居中
#define WK_CCDW_FTALRIGHT   2       //右对齐

#define WK_CCDW_FTWRAP  0x01        //折叠
#define WK_CCDW_FTTRIM  0x02        //裁剪
#define WK_CCDW_FTUNDERLINE 0x04    //下划线
*/
/*
#define WK_CCDW_FTEFFECT_BGAN   1   //背景变换
#define WK_CCDW_FTEFFECT_TXTAN  2   //文字闪动
#define WK_CCDW_FTSCROLL_LEFT   0   //左——右，上——下
#define WK_CCDW_FTSCROLL_RIGHT  1   //右——左
*/
/*
typedef struct tag_Winks_CCDW_TxtStyle_s
{
    int align;              //对齐模式，左，中，右
    unsigned long flags;    //文字显示标志，折叠，裁剪，下划线，是否有背景色
    unsigned long bgcolor;  //背景色
    Winks_CCDW_Font_s font; //字体信息
    int shadowcolor;        //阴影颜色
    int shadowx;            //阴影位移
    int shadowy;            //阴影位移
    int effect;             //字体特效，背景色块变化，字体闪动
    int scrolltype;         //滚动效果，左——右，右——左，上——下
//    char* pdata;            //字体数据
}Winks_CCDW_TxtStyle_s;

typedef struct tag_Winks_CCDW_TxtShadow_s
{
    unsigned long color;
    int x;
    int y;
}Winks_CCDW_TxtShadow_s;
*/
#define WK_CCDW_RGTYPE_RECT     1       //矩形区域
#define WK_CCDW_RGTYPE_TEXT     2       //文本
#define WK_CCDW_RGTYPE_MEDIA    3       //图片
#define WK_CCDW_RGTYPE_WINKS    4       //彩像=图片WK_CCDW_RGTYPE_IMAGE
#define WK_CCDW_RGTYPE_BUTTON   5       //问候语=文本WK_CCDW_RGTYPE_TEXT
#define WK_CCDW_RGTYPE_MAX_NUM  6       //最大数

#define WK_CCDW_RGFLAG_BGCOLOR  0x01
#define WK_CCDW_RGFLAG_REDRAW   0x02
#define WK_CCDW_RGFLAG_SREF     0x10
#define WK_CCDW_RGFLAG_CREF     0x20
#define WK_CCDW_RGFLAG_V        0x40
#define WK_CCDW_RGFLAG_OVERB    0x100   //画前面交叠的区域
#define WK_CCDW_RGFLAG_OVERA    0x200   //画后面交叠的区域

#define WK_CCDW_OVERRECNUM      32

#define WK_CCDW_SKMARGIN 5

typedef struct tag_Winks_CCDW_RgBase_s
{
    struct tag_Winks_CCDW_RgBase_s* pnext;
    unsigned short type;            //区域类型
    unsigned short flags;           //区域标记
    unsigned long color;            //填充颜色
    Winks_CCDW_Section_s Section;   //区域信息
    char* pdata;        //server ref
    unsigned short overb;           //前面交叠的区域数
    unsigned short overa;           //后面交叠的区域数
    //struct tag_Winks_CCDW_RgBase_s* pOver[WK_CCDW_OVERRECNUM];
}Winks_CCDW_RgBase_s;

typedef struct tag_Winks_CCDW_Rect_s
{
    Winks_CCDW_RgBase_s base;       //基类
    int bwidth;          //边框宽度
    int bstyle;          //边框类型（借用抽象层线形）
    unsigned long bcolor;           //边框颜色
}Winks_CCDW_Rect_s;

#define WK_CCDW_FTSHADOW    0x01    //字体是否有阴影
#define WK_CCDW_FTBGCOLOR   0x02    //字体背景填充颜色
#define WK_CCDW_FTNEEDSCROLL  0x04    //是否需要滚动（滚动接口要求）
#define WK_CCDW_FTALLOC     0x08

#define WK_CCDW_MAXEFFECTCOLOR_NUM  8
#define WK_CCDW_TEXTBUFLEN  204


typedef struct tag_Winks_CCDW_Text_s
{
    Winks_CCDW_RgBase_s base;       //基类
    unsigned long flags;
    winks_scrolling_text_s txtstyle;    //文字信息
    //Winks_CCDW_TxtShadow_s txtshadow;
    winks_color effcolor[WK_CCDW_MAXEFFECTCOLOR_NUM];
    char pbuf[WK_CCDW_TEXTBUFLEN];
    //Winks_CCDW_TxtStyle_s txtstyle; //文字信息
}Winks_CCDW_Text_s;

#define WK_CCDW_MEDSTRE_UNIFORM     1   //等比拉伸
#define WK_CCDW_MEDSTRE_FILL        2   //填充
#define WK_CCDW_MEDSTRE_UNIFORMFILL 3   //等比裁剪

#define WINKS_ANIM_HANDLE unsigned long 

typedef struct tag_Winks_CCDW_Media_s
{
    Winks_CCDW_RgBase_s base;       //基类
    char CCTid[60];
    WINKS_ANIM_HANDLE anim_handle;
    int stretch;                    //图片填充形式
    char* pdata;
    char pbuf[512];
//    char* psource;                  //图片源
}Winks_CCDW_Media_s;

#define WK_CCDW_BTPUSH_LSK  0
#define WK_CCDW_BTPUSH_RSK  1
#define WK_CCDW_BTPUSH_ANSWER   2
#define WK_CCDW_BTPUSH_HANGUP   3
typedef struct tag_Winks_CCDW_Button_s
{
    Winks_CCDW_RgBase_s base;       //基类
    int OnPush;
}Winks_CCDW_Button_s;

typedef int (* Winks_CCDW_DrawSection)( Winks_CCDW_RgBase_s* pSection );
typedef Winks_CCDW_RgBase_s* (* Winks_CCDW_ParseSection)( Winks_XmlElement_s* pXmlele );

typedef struct tag_Winks_CCDW_DrawSectionFunc_s
{
    int type;
    Winks_CCDW_DrawSection pDrawFunc;
    Winks_CCDW_ParseSection pParseFunc;
}Winks_CCDW_DrawSectionFunc_s;

typedef struct tag_Winks_CCDW_Global_s1
{
    Winks_CCDW_RgBase_s* pRegion;
    Winks_CCDW_Media_s* pWinks;
    Winks_CCDW_Text_s* pGreet;
    Winks_CCDW_Text_s* pPeernum;
    Winks_CCDW_Button_s* pLsk;
    Winks_CCDW_Button_s* pRsk;
    Winks_CCDW_Button_s* pAnswer;
    Winks_CCDW_Button_s* pHangup;
    //char ltelnum[WINKS_CALL_NUMBER_LENGTH + 4];
    char stelnum[WINKS_CALL_NUMBER_LENGTH * 2 + WK_CCSW_PNAMELEN * 2 + 2];	
    unsigned short flag1; //for multi-thread free
#if 0    
    unsigned short flag2;
#endif	
    Winks_Xml_s* pXml;
}Winks_CCDW_Global_s1;

/* CCSW DRAW Option */
#define WK_CCSW_DWALL   1   //first draw, all element need draw
#define WK_CCSW_DWSTATE 2   //draw because state change
#define WK_CCSW_DWTIME  3   //draw because timer

/* CCSW String ID */
typedef enum tag_Winks_CCSW_StringID_s
{
    WK_CCSW_CLOSECC_STRID = 0,
    WK_CCSW_ACTIVE_STRID,
    WK_CCSW_ACTIVENOTE_STRID,
    WK_CCSW_ACTIVESUGGEST_STRID,
    WK_CCSW_UNKNOWNUM_STRID,
    WK_CCSW_ANSWER_STRID,
    WK_CCSW_HANGUP_STRID,
    WK_CCSW_COLLECT_STRID,
    WK_CCSW_COLLECTED_STRID,
    WK_CCSW_MAX_STRID
}Winks_CCSW_StringID_s;

#ifdef WINKS_COLLECT
#define WINKS_CC_ID_LEN 16
#endif

/* CCSW global structure */
/*
typedef struct tag_Winks_CCSW_Global_s1
{
    int ifinit;
    WINKS_CHN_ID channel;
    WINKS_TIMER_ID timer;
    unsigned short flag;
    unsigned short callstate;
    Winks_CCSW_Info_s Info;
    Winks_CCDW_Global_s1* pCCDWglb;
	
#ifdef WINKS_COLLECT
    char wid[WINKS_CC_ID_LEN]; //for collection
#endif

#ifdef WINKS_SMARTPHONE_CCKEYFLAG
    unsigned short penflag;
#endif
}Winks_CCSW_Global_s1;
*/


//彩像展示模板内容
typedef struct tag_Winks_CCDW_Global_s
	{
		Winks_CCDW_RgBase_s* pRegion;
		Winks_CCDW_Media_s* pWinks;
		Winks_CCDW_Text_s* pGreet;			
		Winks_Xml_s* pXml;
	}Winks_CCDW_Global_s;



//彩像展示的全局结构体
typedef struct tag_Winks_CCSW_Global_s
	{	
		unsigned short timerflag;      /*定时器标志信息 1:定时器启动 0:定时器关闭*/
		unsigned short callstate;     /*电话事件状态WinksCallEvent_e*/
		unsigned short showstate;      /*彩像展示状态信息 1:展示 0:不展示*/
		short Socketstate;      /*彩像socket状态信息 1:可用 -1:不可用*/
		unsigned short flag;    /*用户状态*/
		unsigned short titleflag;/*是否展示片头标志；1展示；0不展示*/
		char* number;		
		Winks_CCSW_Info_s Info; /*彩像信息*/
		Winks_CCDW_Global_s *pCCDWglb;/*彩像模板内容*/
	}Winks_CCSW_Global_s;

int Winks_stricmp(const char *dst, const char *src);

#if 0
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_CCSWGETGLOBALVAR (get_module_glob(WK_MODULE_CCSW))
#else
extern Winks_CCSW_Global_s Winks_CCSW_Global_GB;
#define WINKS_CCSWGETGLOBALVAR (&Winks_CCSW_Global_GB)
#endif
#endif

Winks_CCDW_RgBase_s* winks_ccsw_ParseXml( Winks_Xml_s* pXml );
int winks_ccsw_FreeXml( Winks_CCDW_RgBase_s* pBase );
int winks_ccsw_Draw( Winks_CCDW_Global_s* pCCDW, int option );
int winks_ccsw_FreezeRg( Winks_CCDW_Global_s* pCCDW );
const unsigned char* winks_ccdw_GetString( int STRID );
char* winks_ccsw_GetShowNum( int option );
Winks_CCDW_RgBase_s* winks_ccsw_GetRegion( char* PhoneNumber);

//from wk_xml.c
//int Winks_xml_parse( Winks_XmlMem_s* pdata, Winks_XmlInt_s** ppxml );
//int Winks_xml_free( Winks_XmlInt_s* pxml );
#endif
