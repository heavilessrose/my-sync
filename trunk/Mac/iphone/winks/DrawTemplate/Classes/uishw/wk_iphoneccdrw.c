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
*Id: wk_ccdrw.c,v 1.00 
**************************************************************************    
* File Description
* ----------------
* Color call draw functions. 
**************************************************************************/
#include <string.h>
#include "wk_public.h"
//#include "wk_osal.h"
//#include "wk_xmlgb.h"
#include "wk_engin.h"    

#include "wk_iphoneccshw.h"
#import "wk_osdraw.h"
#include "wk_template_api.h"

extern Winks_CCSW_Global_s gCCSW;


const Winks_CCDW_Convert_s Winks_CCDW_StateCvt[] = 
{
    "dialing", WK_CCSW_OUTCALL,
    "ringing", WK_CCSW_INCALL,
    "connected", WK_CCSW_CONNECT,
    "disconnected", WK_CCSW_HUNGUP,
    NULL, 0
};

const Winks_CCDW_Convert_s Winks_CCDW_FTCvt[] = 
{
    "italic", WINKS_FONT_INCLINE,
    "bold", WINKS_FONT_BOLDFACED,
    NULL, 0
};

const Winks_CCDW_Convert_s Winks_CCDW_FTSTYCvt[] = 
{
    "none", 0,
    "nowrap", 0,
    "wrap", WK_WG_WRAP,
    "char", WK_WG_FONTRIGHTTRUNC ,
    "underline", WINKS_FONT_UNDERLINE,
    "gradientcoloranimation", WK_WG_GRADIENTCOLORANIMATION,
    "textcolorslide", WK_WG_TEXTCOLORSLIDE,
    "scroll", WK_WG_FONTLEFTSCROLL,
    "scrollright", WK_WG_FONTRIGHTSCROLL,
    "left", WINKS_SHOW_LEFT,
    "center", WINKS_SHOW_CENTER,
    "right", WINKS_SHOW_RIGHT,
    NULL, 0
};

const Winks_CCDW_Convert_s Winks_CCDW_TYPECvt[] = 
{
    "rect", WK_CCDW_RGTYPE_RECT,
    "text", WK_CCDW_RGTYPE_TEXT,
    "media", WK_CCDW_RGTYPE_MEDIA,
    "winks", WK_CCDW_RGTYPE_WINKS,
    "pushButton", WK_CCDW_RGTYPE_BUTTON,
    NULL, 0
};

const Winks_CCDW_Convert_s Winks_CCDW_RectCvt[] = 
{
    "none", WINKS_DASHSTYLE_NULL,
    "solid", WINKS_LINE_SOLID,
    "dotted", WINKS_LINE_DOT,
    "dashed", WINKS_LINE_DOTDASH,
    "double", WINKS_LINE_DOUBLELINE,
    NULL, 0
};

const Winks_CCDW_Convert_s Winks_CCDW_MediaCvt[] = 
{
    "none", 0,
    "uniform", WK_CCDW_MEDSTRE_UNIFORM,
    "fill", WK_CCDW_MEDSTRE_FILL,
    "uniformfill", WK_CCDW_MEDSTRE_UNIFORMFILL,
    NULL, 0
};

const Winks_CCDW_Convert_s Winks_CCDW_ButtonCvt[] = 
{
    "PE_LEFT_KEY", WK_CCDW_BTPUSH_LSK,
    "PE_RIGHT_KEY", WK_CCDW_BTPUSH_RSK,
    "PE_ANSWER", WK_CCDW_BTPUSH_ANSWER,
    "PE_HANG_UP", WK_CCDW_BTPUSH_HANGUP,
    NULL, 0
};

static int winks_ccdw_Draw( Winks_CCDW_RgBase_s* pBase, int ifover );

static int Winks_CCDW_DrawRect( Winks_CCDW_RgBase_s* pSection );
static int Winks_CCDW_DrawText( Winks_CCDW_RgBase_s* pSection );
static int Winks_CCDW_DrawMedia( Winks_CCDW_RgBase_s* pSection );
static int Winks_CCDW_DrawButton( Winks_CCDW_RgBase_s* pSection );

static Winks_CCDW_RgBase_s* Winks_CCDW_ParseRect( Winks_XmlElement_s* pXmlele );
static Winks_CCDW_RgBase_s* Winks_CCDW_ParseText( Winks_XmlElement_s* pXmlele );
static Winks_CCDW_RgBase_s* Winks_CCDW_ParseMedia( Winks_XmlElement_s* pXmlele );
static Winks_CCDW_RgBase_s* Winks_CCDW_ParseButton( Winks_XmlElement_s* pXmlele );

//panqj add 2010.02.08 #if ((defined(WINKS_UNSUPPORTED_GLOBAL_VAR))||(defined(WINKS_DYNAMIC_LOAD_BLOCK)))
//panqj add 2010.02.08 static Winks_CCDW_DrawSectionFunc_s Winks_CCDW_DrawFunc[WK_CCDW_RGTYPE_MAX_NUM];
//panqj add 2010.02.08 #else
const Winks_CCDW_DrawSectionFunc_s Winks_CCDW_DrawFunc[] = 
{
    WK_CCDW_RGTYPE_RECT,    Winks_CCDW_DrawRect,    Winks_CCDW_ParseRect,
    WK_CCDW_RGTYPE_TEXT,    Winks_CCDW_DrawText,    Winks_CCDW_ParseText,    
    WK_CCDW_RGTYPE_MEDIA,   Winks_CCDW_DrawMedia,   Winks_CCDW_ParseMedia,
    WK_CCDW_RGTYPE_WINKS,   Winks_CCDW_DrawMedia,   Winks_CCDW_ParseMedia,
    WK_CCDW_RGTYPE_BUTTON,  Winks_CCDW_DrawButton,  Winks_CCDW_ParseButton,
    -1, NULL, NULL
};
//panqj add 2010.02.08 #endif
/* 字符串资源 */
typedef struct tag_Winks_CCSW_String_s
{
    const unsigned char* pEng;
    const unsigned char* pSc;
    const unsigned char* pTr;
}Winks_CCSW_String_s;

#if defined(__BIG_ENDIAN)
    const unsigned char pScCloseCC[] = {0x51, 0x73, 0x95, 0xED, 0x5F, 0x69, 0x50, 0xCF, 0x00, 0x00};/*chinese--Close--关闭彩像*/
    const unsigned char pEngCloseCC[] = {0x00, 0x43, 0x00, 0x6C, 0x00, 0x6F, 0x00, 0x73, 0x00, 0x65, 0x00, 0x00};/*english--Close--关闭彩像*/
    const unsigned char pScActive[] = {0x6F, 0xC0, 0x6D, 0x3B, 0x00, 0x00};/*chinese--Activate--激活*/
    const unsigned char pEngActive[] = {0x00, 0x41, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x76, 0x00, 
        0x61, 0x00, 0x74, 0x00, 0x65, 0x00, 0x00};/*english--Activate--激活*/
    const unsigned char pEngActiveNote[] = {0x00, 0x4F, 0x00, 0x70, 0x00, 0x65, 0x00, 0x72, 0x00, 0x61, 0x00, 0x74, 
        0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x20, 0x00, 0x68, 0x00, 0x61, 0x00, 0x73, 0x00, 0x20, 0x00, 0x62, 
        0x00, 0x65, 0x00, 0x65, 0x00, 0x6E, 0x00, 0x20, 0x00, 0x72, 0x00, 0x65, 0x00, 0x63, 0x00, 0x6F, 0x00, 0x72, 
        0x00, 0x64, 0x00, 0x65, 0x00, 0x64, 0x00, 0x20, 0x00, 0x61, 0x00, 0x6E, 0x00, 0x64, 0x00, 0x20, 0x00, 0x77, 
        0x00, 0x69, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x20, 0x00, 0x63, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x74, 0x00, 0x69, 
        0x00, 0x6E, 0x00, 0x75, 0x00, 0x65, 0x00, 0x20, 0x00, 0x61, 0x00, 0x66, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 
        0x00, 0x20, 0x00, 0x74, 0x00, 0x68, 0x00, 0x65, 0x00, 0x20, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x6C, 
        0x00, 0x2E, 0x00, 0x00}; 
     /*english--Operation has been recorded and will continue after the call.--激活操作已被记录，将在您挂机后开始激活。*/
    const unsigned char pEngActiveSug[] = {0x00, 0x4E, 0x00, 0x6F, 0x00, 0x74, 0x00, 0x20, 0x00, 0x61, 0x00, 0x20, 
        0x00, 0x57, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x6B, 0x00, 0x73, 0x00, 0x20, 0x00, 0x75, 0x00, 0x73, 0x00, 0x65, 
        0x00, 0x72, 0x00, 0x3F, 0x00, 0x41, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x76, 0x00, 0x61, 0x00, 0x74, 
        0x00, 0x65, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x77, 0x00, 0x20, 0x00, 0x74, 0x00, 0x6F, 0x00, 0x20, 
        0x00, 0x65, 0x00, 0x6E, 0x00, 0x6A, 0x00, 0x6F, 0x00, 0x79, 0x00, 0x20, 0x00, 0x74, 0x00, 0x68, 0x00, 0x65, 
        0x00, 0x20, 0x00, 0x65, 0x00, 0x78, 0x00, 0x63, 0x00, 0x69, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x67, 
        0x00, 0x20, 0x00, 0x57, 0x00, 0x49, 0x00, 0x4E, 0x00, 0x4B, 0x00, 0x53, 0x00, 0x21, 0x00, 0x00};
    /*english--Not a Winks user?Activate now to enjoy the exciting WINKS!--欢迎使用彩像，您尚未激活，马上激活体验彩像的精彩吧。*/
	
    const unsigned char pScActiveNote[] = {0x6F, 0xC0, 0x6D, 0x3B, 0x64, 0xCD, 0x4F, 0x5C, 0x5D, 0xF2, 0x88, 0xAB, 
        0x8B, 0xB0, 0x5F, 0x55, 0xFF, 0x0C, 0x5C, 0x06, 0x57, 0x28, 0x60, 0xA8, 0x63, 0x02, 0x67, 0x3A, 0x54, 0x0E, 
        0x5F, 0x00, 0x59, 0xCB, 0x6F, 0xC0, 0x6D, 0x3B, 0x30, 0x02, 0x00, 0x00}; 
    /*chinese--Operation has been recorded and will continue after the call.--激活操作已被记录，将在您挂机后开始激活。*/
    const unsigned char pScActiveSug[] = {0x6B, 0x22, 0x8F, 0xCE, 0x4F, 0x7F, 0x75, 0x28, 0x5F, 0x69, 0x50, 0xCF, 
        0xFF, 0x0C, 0x60, 0xA8, 0x5C, 0x1A, 0x67, 0x2A, 0x6F, 0xC0, 0x6D, 0x3B, 0xFF, 0x0C, 0x9A, 0x6C, 0x4E, 0x0A, 
        0x6F, 0xC0, 0x6D, 0x3B, 0x4F, 0x53, 0x9A, 0x8C, 0x5F, 0x69, 0x50, 0xCF, 0x76, 0x84, 0x7C, 0xBE, 0x5F, 0x69, 
        0x54, 0x27, 0x30, 0x02, 0x00, 0x00};
   /*chinese--Not a Winks user?Activate now to enjoy the exciting WINKS!--欢迎使用彩像，您尚未激活，马上激活体验彩像的精彩吧。*/
//    const unsigned char pScUnknowNum[] = {0x67, 0x2A, 0x77, 0xE5, 0x53, 0xF7, 0x78, 0x01, 0x00, 0x00};
    const unsigned char pScUnknowNum[] = {0x79,0xC1,0x4E,0xBA,0x53,0xF7,0x78,0x01,0x00,0x00};/*chinese--Private Number--私人号码*/	
    const unsigned char pScAnswer[] = {0x63, 0xA5, 0x54, 0x2C, 0x00, 0x00};/*chinese--Answer--接听*/
    const unsigned char pScHangup[] = {0x63, 0x02, 0x65, 0xAD, 0x00, 0x00};/*chinese--Hang up--挂断*/
//    const unsigned char pEngUnknowNum[] = {0x00, 0x55, 0x00, 0x6E, 0x00, 0x6B, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x77, 
//        0x00, 0x6E, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x75, 0x00, 0x6D, 0x00, 0x62, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00};
    const unsigned char pEngUnknowNum[] = {0x00,0x50,0x00,0x72,0x00,0x69,0x00,0x76,0x00,0x61,0x00,0x74,0x00,
         0x65,0x00,0x20,0x00,0x4E,0x00,0x75,0x00,0x6D,0x00,0x62,0x00,0x65,0x00,0x72,0x00,0x00};/*english--Private Number--私人号码*/
    const unsigned char pEngAnswer[] = {0x00, 0x41, 0x00, 0x6E, 0x00, 0x73, 0x00, 0x77, 0x00, 0x65, 0x00, 0x72, 
        0x00, 0x00};/*english--Answer--接听*/
    const unsigned char pEngHangup[] = {0x00, 0x48, 0x00, 0x61, 0x00, 0x6E, 0x00, 0x67, 0x00, 0x20, 0x00, 0x75, 
        0x00, 0x70, 0x00, 0x00};/*english--Hang up--挂断*/
   const unsigned char pScCollect[] = {0x65,0x36,0x85,0xCF,0x00,0x00};/*chinese--Collection--收藏*/
   const unsigned char pEngCollect[] = {0x00,0x43,0x00,0x6F,0x00,0x6C,0x00,0x6C,0x00,0x65,0x00,0x63,0x00,0x74,0x00,0x69,0x00,0x6F,0x00,0x6E,0x00,0x00};/*english--Collection*/
   const unsigned char pScCollected[] = {0x5D,0xF2,0x65,0x36,0x85,0xCF,0x00,0x00};/*chinese--Collection--已收藏*/
   const unsigned char pEngCollected[] = {0x00,0x49,0x00,0x6E,0x00,0x20,0x00,0x43,0x00,0x6F,0x00,0x6C,0x00,0x6C,0x00,0x65,0x00,
	0x63,0x00,0x74,0x00,0x69,0x00,0x6F,0x00,0x6E,0x00,0x00};/*english--In Collection--已收藏*/
    


#else
    const unsigned short pScCloseCC[] = {0x5173, 0x95ED, 0x5F69, 0x50CF, 0x0000};//保证分配在偶地址/*chinese----关闭彩像*/
    //const unsigned char pScCloseCC[] = {0x73, 0x51, 0xED, 0x95, 0x69, 0x5F, 0xCF, 0x50, 0x00, 0x00};
    const unsigned char pEngCloseCC[] = {0x43, 0x00, 0x6C, 0x00, 0x6F, 0x00, 0x73, 0x00, 0x65, 0x00, 0x00, 0x00};/*english--Close--关闭彩像*/
    const unsigned char pScActive[] = {0xC0, 0x6F, 0x3B, 0x6D, 0x00, 0x00};/*chinese--Activate--激活*/
    const unsigned char pEngActive[] = {0x41, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x76, 0x00, 0x61, 0x00, 
        0x74, 0x00, 0x65, 0x00, 0x00, 0x00};/*english--Activate--激活*/
    const unsigned char pEngActiveNote[] = {0x4F, 0x00, 0x70, 0x00, 0x65, 0x00, 0x72, 0x00, 0x61, 0x00, 
        0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x20, 0x00, 0x68, 0x00, 0x61, 0x00, 0x73, 0x00, 
        0x20, 0x00, 0x62, 0x00, 0x65, 0x00, 0x65, 0x00, 0x6E, 0x00, 0x20, 0x00, 0x72, 0x00, 0x65, 0x00, 
        0x63, 0x00, 0x6F, 0x00, 0x72, 0x00, 0x64, 0x00, 0x65, 0x00, 0x64, 0x00, 0x20, 0x00, 0x61, 0x00, 
        0x6E, 0x00, 0x64, 0x00, 0x20, 0x00, 0x77, 0x00, 0x69, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x20, 0x00, 
        0x63, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x75, 0x00, 0x65, 0x00, 
        0x20, 0x00, 0x61, 0x00, 0x66, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x74, 0x00, 
        0x68, 0x00, 0x65, 0x00, 0x20, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x2E, 0x00, 0x00, 0x00}; 
    /*english--Operation has been recorded and will continue after the call.--激活操作已被记录，将在您挂机后开始激活。*/
    const unsigned char pEngActiveSug[] = {0x4E, 0x00, 0x6F, 0x00, 0x74, 0x00, 0x20, 0x00, 0x61, 0x00, 
        0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x6B, 0x00, 0x73, 0x00, 0x20, 0x00, 0x75, 0x00, 
        0x73, 0x00, 0x65, 0x00, 0x72, 0x00, 0x3F, 0x00, 0x41, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 
        0x76, 0x00, 0x61, 0x00, 0x74, 0x00, 0x65, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x77, 0x00, 
        0x20, 0x00, 0x74, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x65, 0x00, 0x6E, 0x00, 0x6A, 0x00, 0x6F, 0x00, 
        0x79, 0x00, 0x20, 0x00, 0x74, 0x00, 0x68, 0x00, 0x65, 0x00, 0x20, 0x00, 0x65, 0x00, 0x78, 0x00, 
        0x63, 0x00, 0x69, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x67, 0x00, 0x20, 0x00, 0x57, 0x00, 
        0x49, 0x00, 0x4E, 0x00, 0x4B, 0x00, 0x53, 0x00, 0x21, 0x00, 0x00, 0x00};
    /*english--Not a Winks user?Activate now to enjoy the exciting WINKS!--欢迎使用彩像，您尚未激活，马上激活体验彩像的精彩吧。*/	
    const unsigned char pScActiveNote[] = {0xC0, 0x6F, 0x3B, 0x6D, 0xCD, 0x64, 0x5C, 0x4F, 0xF2, 0x5D, 
        0xAB, 0x88, 0xB0, 0x8B, 0x55, 0x5F, 0x0C, 0xFF, 0x06, 0x5C, 0x28, 0x57, 0xA8, 0x60, 0x02, 0x63, 
        0x3A, 0x67, 0x0E, 0x54, 0x00, 0x5F, 0xCB, 0x59, 0xC0, 0x6F, 0x3B, 0x6D, 0x02, 0x30, 0x00, 0x00}; 
     /*chinese--Operation has been recorded and will continue after the call.--激活操作已被记录，将在您挂机后开始激活。*/
    const unsigned char pScActiveSug[] = {0x22, 0x6B, 0xCE, 0x8F, 0x7F, 0x4F, 0x28, 0x75, 0x69, 0x5F, 
        0xCF, 0x50, 0x0C, 0xFF, 0xA8, 0x60, 0x1A, 0x5C, 0x2A, 0x67, 0xC0, 0x6F, 0x3B, 0x6D, 0x0C, 0xFF, 
        0x6C, 0x9A, 0x0A, 0x4E, 0xC0, 0x6F, 0x3B, 0x6D, 0x53, 0x4F, 0x8C, 0x9A, 0x69, 0x5F, 0xCF, 0x50, 
        0x84, 0x76, 0xBE, 0x7C, 0x69, 0x5F, 0x27, 0x54, 0x02, 0x30, 0x00, 0x00};
    /*chinese--Not a Winks user?Activate now to enjoy the exciting WINKS!--欢迎使用彩像，您尚未激活，马上激活体验彩像的精彩吧。*/	 
//    const unsigned char pScUnknowNum[] = {0x2A, 0x67, 0xE5, 0x77, 0xF7, 0x53, 0x01, 0x78, 0x00, 0x00};
    const unsigned char pScUnknowNum[] = {0xC1,0x79,0xBA,0x4E,0xF7,0x53 ,0x01,0x78, 0x00, 0x00};/*chinese--Private Number--私人号码*/
    const unsigned char pScAnswer[] = {0xA5, 0x63, 0x2C, 0x54, 0x00, 0x00};/*chinese--Answer--接听*/
    const unsigned char pScHangup[] = {0x02, 0x63, 0xAD, 0x65, 0x00, 0x00};/*chinese--Hang up--挂断*/
//    const unsigned char pEngUnknowNum[] = {0x55, 0x00, 0x6E, 0x00, 0x6B, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x77, 0x00, 
//        0x6E, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x75, 0x00, 0x6D, 0x00, 0x62, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00};
    const unsigned char pEngUnknowNum[] = {0x50,0x00,0x72,0x00,0x69,0x00,0x76,0x00,0x61,0x00,0x74,0x00,
         0x65,0x00,0x20,0x00,0x4E,0x00,0x75,0x00,0x6D,0x00,0x62,0x00,0x65,0x00,0x72,0x00,0x00,0x00};/*english--Private Number--私人号码*/	
    const unsigned char pEngAnswer[] = {0x41, 0x00, 0x6E, 0x00, 0x73, 0x00, 0x77, 0x00, 0x65, 0x00, 0x72, 0x00, 
        0x00, 0x00};/*english--Answer--接听*/
    const unsigned char pEngHangup[] = {0x48, 0x00, 0x61, 0x00, 0x6E, 0x00, 0x67, 0x00, 0x20, 0x00, 0x75, 0x00, 
        0x70, 0x00, 0x00, 0x00};/*chinese--Hang up--挂断*/
    const unsigned char pScCollect[] = {0x36,0x65,0xCF,0x85,0x00,0x00};/*chinese--Collection--收藏*/
    const unsigned char pEngCollect[] = {0x43,0x00,0x6F,0x00,0x6C,0x00,0x6C,0x00,0x65,0x00,0x63,0x00,0x74,0x00,0x69,0x00,0x6F,0x00,0x6E,0x00,0x00,0x00};/*english--Collection*/
    const unsigned char pScCollected[] = {0xF2,0x5D,0x36,0x65,0xCF,0x85,0x00,0x00};/*chinese--Collection--已收藏*/
    const unsigned char pEngCollected[] = {0x49,0x00,0x6E,0x00,0x20,0x00,0x43,0x00,0x6F,0x00,0x6C,0x00,0x6C,0x00,0x65,0x00,
	0x63,0x00,0x74,0x00,0x69,0x00,0x6F,0x00,0x6E,0x00,0x00,0x00};/*english--In Collection--已收藏*/                  	
#endif

//#if ((defined(WINKS_UNSUPPORTED_GLOBAL_VAR))||(defined(WINKS_DYNAMIC_LOAD_BLOCK)))
//static Winks_CCSW_String_s pCCSW_String[WK_CCSW_MAX_STRID];
//#else
const Winks_CCSW_String_s pCCSW_String[] = 
{
    pEngCloseCC, (const unsigned char*)pScCloseCC, (const unsigned char*)pEngCloseCC,
    pEngActive, pScActive, pEngActive,
    pEngActiveNote, pScActiveNote, pEngActiveNote,
    pEngActiveSug, pScActiveSug, pEngActiveSug,
    pEngUnknowNum, pScUnknowNum, pEngUnknowNum,
    pEngAnswer, pScAnswer, pEngAnswer,
    pEngHangup, pScHangup, pEngHangup,
    pEngCollect, pScCollect, pEngCollect,
    pEngCollected, pScCollected, pEngCollected
};
//#endif
#if 0
#if ((defined(WINKS_UNSUPPORTED_GLOBAL_VAR))||(defined(WINKS_DYNAMIC_LOAD_BLOCK)))
int Winks_ccdrwInit(void)
{
    int i;
    
    {/******************    Init Winks_CCDW_DrawFunc     ******************/
    memset(Winks_CCDW_DrawFunc, 0, sizeof(Winks_CCDW_DrawFunc)); 
    Winks_CCDW_DrawFunc[0].type = WK_CCDW_RGTYPE_RECT;
    Winks_CCDW_DrawFunc[0].pDrawFunc = Winks_CCDW_DrawRect;
    Winks_CCDW_DrawFunc[0].pParseFunc = Winks_CCDW_ParseRect;
    Winks_CCDW_DrawFunc[1].type = WK_CCDW_RGTYPE_TEXT;
    Winks_CCDW_DrawFunc[1].pDrawFunc = Winks_CCDW_DrawText;
    Winks_CCDW_DrawFunc[1].pParseFunc = Winks_CCDW_ParseText;
    Winks_CCDW_DrawFunc[2].type = WK_CCDW_RGTYPE_MEDIA;
    Winks_CCDW_DrawFunc[2].pDrawFunc = Winks_CCDW_DrawMedia;
    Winks_CCDW_DrawFunc[2].pParseFunc = Winks_CCDW_ParseMedia;
    memcpy(&Winks_CCDW_DrawFunc[3], &Winks_CCDW_DrawFunc[2], sizeof(Winks_CCDW_DrawFunc[2]));
    Winks_CCDW_DrawFunc[3].type = WK_CCDW_RGTYPE_WINKS;
    Winks_CCDW_DrawFunc[4].type = WK_CCDW_RGTYPE_BUTTON;
    Winks_CCDW_DrawFunc[4].pDrawFunc = Winks_CCDW_DrawButton;
    Winks_CCDW_DrawFunc[4].pParseFunc = Winks_CCDW_ParseButton;
    Winks_CCDW_DrawFunc[5].type = -1;
    }

    {/******************    Init pCCSW_String     ******************/
    memset(pCCSW_String, 0, sizeof(pCCSW_String));
    pCCSW_String[WK_CCSW_CLOSECC_STRID].pEng = pEngCloseCC;
    pCCSW_String[WK_CCSW_CLOSECC_STRID].pSc = (const unsigned char*)pScCloseCC;
    pCCSW_String[WK_CCSW_CLOSECC_STRID].pTr = pEngCloseCC;
    pCCSW_String[WK_CCSW_ACTIVE_STRID].pEng = pEngActive;
    pCCSW_String[WK_CCSW_ACTIVE_STRID].pSc = pScActive;
    pCCSW_String[WK_CCSW_ACTIVE_STRID].pTr = pEngActive;
    pCCSW_String[WK_CCSW_ACTIVENOTE_STRID].pEng = pEngActiveNote;
    pCCSW_String[WK_CCSW_ACTIVENOTE_STRID].pSc = pScActiveNote;
    pCCSW_String[WK_CCSW_ACTIVENOTE_STRID].pTr = pEngActiveNote;
    pCCSW_String[WK_CCSW_ACTIVESUGGEST_STRID].pEng = pEngActiveSug;
    pCCSW_String[WK_CCSW_ACTIVESUGGEST_STRID].pSc = pScActiveSug;
    pCCSW_String[WK_CCSW_ACTIVESUGGEST_STRID].pTr = pEngActiveSug;
    pCCSW_String[WK_CCSW_UNKNOWNUM_STRID].pEng = pEngUnknowNum;
    pCCSW_String[WK_CCSW_UNKNOWNUM_STRID].pSc = pScUnknowNum;
    pCCSW_String[WK_CCSW_UNKNOWNUM_STRID].pTr = pEngUnknowNum;
    pCCSW_String[WK_CCSW_ANSWER_STRID].pEng = pEngAnswer;
    pCCSW_String[WK_CCSW_ANSWER_STRID].pSc = pScAnswer;
    pCCSW_String[WK_CCSW_ANSWER_STRID].pTr = pEngAnswer;
    pCCSW_String[WK_CCSW_HANGUP_STRID].pEng = pEngHangup;
    pCCSW_String[WK_CCSW_HANGUP_STRID].pSc = pScHangup;
    pCCSW_String[WK_CCSW_HANGUP_STRID].pTr = pEngHangup;
    pCCSW_String[WK_CCSW_COLLECT_STRID].pEng = pEngCollect;
    pCCSW_String[WK_CCSW_COLLECT_STRID].pSc = pScCollect;
    pCCSW_String[WK_CCSW_COLLECT_STRID].pTr = pEngCollect;
    pCCSW_String[WK_CCSW_COLLECTED_STRID].pEng = pEngCollected;
    pCCSW_String[WK_CCSW_COLLECTED_STRID].pSc = pScCollected;
    pCCSW_String[WK_CCSW_COLLECTED_STRID].pTr = pEngCollected;
    }
    return 0;
}

#endif
#endif

int Winks_stricmp(const char *dst, const char *src)
{
	int ch1, ch2;
	
	do{
		if ( ((ch1 = (unsigned char)(*(dst++))) >= 'A') &&(ch1 <= 'Z') )
			ch1 += 0x20;
		if ( ((ch2 = (unsigned char)(*(src++))) >= 'A') &&(ch2 <= 'Z') )
			ch2 += 0x20;
	} while ( ch1 && (ch1 == ch2) );
	return(ch1 - ch2);
}

const unsigned char* winks_ccdw_GetString( int STRID )
{
    const unsigned char* pret = NULL;

    if( STRID < 0 || STRID >= WK_CCSW_MAX_STRID )
        return NULL;

    switch( Winks_GetCurrentLanguage() )
    {
    case WINKS_LANG_CHINESE:
        pret = pCCSW_String[STRID].pSc;
        break;

    case WINKS_LANG_CHINESETW:
        pret = pCCSW_String[STRID].pTr;
        break;

    default:
        /* default is english */
        pret = pCCSW_String[STRID].pEng;
        break;
    }

    Winks_assert( pret != NULL );

    return pret;
    
}


////////////////////////////////////////////////////////////////////////////////////

static int winks_ccdw_GetValueByString( char* pstr, Winks_CCDW_Convert_s* pcvt )
{
    int i = 0;

    while( pcvt[i].pstr )
    {
        if( Winks_stricmp( pstr, pcvt[i].pstr ) == 0 )
            return pcvt[i].value;

        i++;
    }

    /* Always, zero is default */
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////

static int Winks_CCDW_DrawRect( Winks_CCDW_RgBase_s* pSection )
{
    Winks_CCDW_Rect_s* pRect = (Winks_CCDW_Rect_s* )pSection;
    int rectoff = 0;

    Winks_printfInfo( "WK CDWW Draw Rect color %x\r\n", pRect->bcolor );
    winks_ui_drawDashRect( pRect->base.Section.x, pRect->base.Section.y, 
        (pRect->base.Section.x + pRect->base.Section.w), (pRect->base.Section.y + pRect->base.Section.h),
        pRect->bcolor, pRect->bwidth, pRect->bstyle );

    rectoff = (pRect->bstyle == WINKS_LINE_DOUBLELINE) ? (3 * pRect->bwidth) : pRect->bwidth;
    if( pRect->base.flags & WK_CCDW_RGFLAG_BGCOLOR )
    {
        winks_ui_fillRect( pRect->base.Section.x + rectoff, pRect->base.Section.y + rectoff, 
            (pRect->base.Section.x + pRect->base.Section.w - rectoff), (pRect->base.Section.y + pRect->base.Section.h - rectoff),
            pRect->base.color );
    }

    //Winks_printfInfo( "WK CCDW Draw Rect %d, %d, %d, %d color is %x\r\n", pRect->base.Section.x, pRect->base.Section.y, 
    //    (pRect->base.Section.w), (pRect->base.Section.h), pRect->bcolor );
    return 0;
}

/* 检查pBase是否和pSection有交叠 */
static int winks_ccdw_IfRectOverlaped( Winks_CCDW_RgBase_s* pSection, Winks_CCDW_RgBase_s* pBase )
{
    winks_Rect_s Sect; 
    winks_Rect_s Base;

    Sect.v_Left = pSection->Section.x;
    Sect.v_Top = pSection->Section.y;
    Sect.v_Right = pSection->Section.x + pSection->Section.w;
    Sect.v_Bottom = pSection->Section.y + pSection->Section.h;
    
    Base.v_Left = pBase->Section.x;
    Base.v_Top = pBase->Section.y;
    Base.v_Right = pBase->Section.x + pBase->Section.w;
    Base.v_Bottom = pBase->Section.y + pBase->Section.h;

    if( (Sect.v_Left >= Base.v_Right) || (Sect.v_Right <= Base.v_Left)
        || (Sect.v_Top >= Base.v_Bottom) || (Sect.v_Bottom <= Base.v_Top) )
        return 0;
    else
        return 1;
    
/*
    if( ((Sect.v_Left >= Base.v_Left) && (Sect.v_Left < Base.v_Right))
        || ((Sect.v_Right > Base.v_Left) && (Sect.v_Right < Base.v_Right)) )
    {
        if( ((Sect.v_Top >= Base.v_Top) && (Sect.v_Top < Base.v_Bottom)) 
            || ((Sect.v_Bottom > Base.v_Top) && (Sect.v_Bottom < Base.v_Bottom)) )
            return 1;
    }
    if( (Sect.v_Left <= Base.v_Left) && (Sect.v_Right >= Base.v_Right)
        && (Sect.v_Top <= Base.v_Top) && (Sect.v_Bottom >= Base.v_Bottom) )
        return 1;

    return 0;
*/
}

static int winks_ccdw_DrawOverlap( Winks_CCDW_RgBase_s* pSection, int option )
{
    Winks_CCDW_RgBase_s* pBase = NULL;
    Winks_CCSW_Global_s* pGlobal = &gCCSW;
    int overb = 0, overa = 0, ifdraw = 0;
    
    if( option & WK_CCDW_RGFLAG_OVERB )
    {
        pBase = pGlobal->pCCDWglb->pRegion;

        if( (pSection->flags & WK_CCDW_RGFLAG_OVERB) )
            ifdraw = ((overb = pSection->overb) == 0) ? 0 : 1;
        else
        {
            pSection->overb = 0;
            ifdraw = 1;
        }

        while( pBase && ifdraw )
        {
            if( pBase == pSection )
                break;
            if( winks_ccdw_IfRectOverlaped( pSection, pBase ) )
            {
                if( (pBase->Section.state == 0) || (pBase->Section.state & pGlobal->callstate) )
                {
                    WINKS_CLIP_ON( pSection->Section.x, pSection->Section.y, 
                        pSection->Section.x + pSection->Section.w, pSection->Section.y + pSection->Section.h);
                    winks_ccdw_Draw( pBase, 1 );
                    WINKS_CLIP_OFF( pSection->Section.x, pSection->Section.y, 
                        pSection->Section.x + pSection->Section.w, pSection->Section.y + pSection->Section.h);
                }
                if( (pSection->flags & WK_CCDW_RGFLAG_OVERB) )
                {
                    if( --overb == 0 )
                        break;
                }
                else
                {
                    pSection->overb ++;
                }
            }
            pBase = pBase->pnext;
        }

        if( pBase == NULL )
            Winks_fail( "Winks CCDRW OverLap find section first failure\r\n" );
        pSection->flags |= WK_CCDW_RGFLAG_OVERB;
    }
    
    if( option & WK_CCDW_RGFLAG_OVERA )
    {
        pBase = pSection->pnext;

        if( (pSection->flags & WK_CCDW_RGFLAG_OVERA) )
            ifdraw = ((overa = pSection->overa) == 0) ? 0 : 1;
        else
        {
            pSection->overa = 0;
            ifdraw = 1;
        }

        while( pBase && ifdraw )
        {
            if( winks_ccdw_IfRectOverlaped( pSection, pBase ) )
            {
                if( (pBase->Section.state == 0) || (pBase->Section.state & pGlobal->callstate) )
                {
                    WINKS_CLIP_ON( pSection->Section.x, pSection->Section.y, 
                        pSection->Section.x + pSection->Section.w, pSection->Section.y + pSection->Section.h);
                    winks_ccdw_Draw( pBase, 1 );
                    WINKS_CLIP_OFF( pSection->Section.x, pSection->Section.y, 
                        pSection->Section.x + pSection->Section.w, pSection->Section.y + pSection->Section.h);
                }
                if( (pSection->flags & WK_CCDW_RGFLAG_OVERA) )
                {
                    if( --overa == 0 )
                        break;
                }
                else
                {
                    pSection->overa ++;
                }
            }
            pBase = pBase->pnext;
        }

        pSection->flags |= WK_CCDW_RGFLAG_OVERA;
    }

    return 0;

#if 0
    if( (pSection->flags & (WK_CCDW_RGFLAG_OVERED | WK_CCDW_RGFLAG_OVERMORE)) == 0 )
    {
        //Winks_printfInfo( "WK CCDW Draw overlap rect new, find over rect\r\n" );
        
        pBase = pGlobal->pCCDWglb->pRegion;
        
        while( pBase && (pBase != pSection) )
        {
            if( (pBase->Section.state == 0) || (pBase->Section.state & pGlobal->callstate) )
            {
                if( winks_ccdw_IfRectOverlaped( pSection, pBase ) )
                {
                    pSection->pOver[i] = pBase;
                    i ++;
                }
            }
            pBase = pBase->pnext;
            if( (i == WK_CCDW_OVERRECNUM) && pBase && (pBase != pSection) )
            {
                pSection->flags |= WK_CCDW_RGFLAG_OVERMORE;
                break;
            }
        }
        
        if( (pBase == NULL) || (pBase == pSection) )
            pSection->flags |= WK_CCDW_RGFLAG_OVERED;
    }
    
    i = 0;
    while( (i < WK_CCDW_OVERRECNUM) && pSection->pOver[i] )
    {
        //Winks_printfInfo( "WK CCDW Draw overlap rect \r\n" );
        WINKS_CLIP_ON( pSection->Section.x, pSection->Section.y, 
            pSection->Section.x + pSection->Section.w, pSection->Section.y + pSection->Section.h);
        if ((pSection->pOver[i]->Section.state == 0)  || (pSection->pOver[i]->Section.state & pGlobal->callstate))	
        {
            winks_ccdw_Draw( pSection->pOver[i] );
        }
        else
        {
            pSection->flags &= ~WK_CCDW_RGFLAG_OVERED;
            pSection->pOver[i] = NULL;
        }
        
        WINKS_CLIP_OFF( pSection->Section.x, pSection->Section.y, 
            pSection->Section.x + pSection->Section.w, pSection->Section.y + pSection->Section.h);
        i++;
    }
    
    if( (pSection->flags & WK_CCDW_RGFLAG_OVERMORE) )
    {
        //Winks_printfInfo( "WK CCDW Draw overlap rect moreover, draw other\r\n" );
        Winks_assert( i == WK_CCDW_OVERRECNUM );
        Winks_assert( pSection->pOver[i - 1] );
        
        pBase = pSection->pOver[i - 1];
        
        while( pBase && (pBase != pSection) )
        {
            if( (pBase->Section.state == 0) || (pBase->Section.state & pGlobal->callstate) )
            {
                if( winks_ccdw_IfRectOverlaped( pSection, pBase ) )
                {
                    Winks_printfInfo( "WK CCDW Draw overlap rect \r\n" );
                    WINKS_CLIP_ON( pSection->Section.x, pSection->Section.y, 
                        pSection->Section.x + pSection->Section.w, pSection->Section.y + pSection->Section.h);
                    winks_ccdw_Draw( pBase );
                    WINKS_CLIP_OFF( pSection->Section.x, pSection->Section.y, 
                        pSection->Section.x + pSection->Section.w, pSection->Section.y + pSection->Section.h);
                }
            }
            pBase = pBase->pnext;
        }
        
    }
    
    return 0;
#endif
}

static int Winks_CCDW_DrawText( Winks_CCDW_RgBase_s* pSection )
{
    Winks_CCDW_Text_s* pText = (Winks_CCDW_Text_s* )pSection;
    //winks_scrolling_text_s Text;
    int dlen = 0;
    unsigned long fflag = 0;
    char* pdata = NULL;
#define WK_CCDW_TIMEBUFLEN  16
    char ptbuf[WK_CCDW_TIMEBUFLEN];
    Winks_DateTime_s v_dateTime;
    Winks_CCSW_Global_s* pGlobal = &gCCSW;

    //Winks_mem_set( &Text, 0, sizeof(winks_scrolling_text_s) );
    //Winks_assert( pText->base != NULL );

    Winks_printfInfo( "WK CDWW Draw Text %s\r\n", pText->base.pdata );

    if( (pText->base.flags & WK_CCDW_RGFLAG_SREF) && (pText->txtstyle.text == NULL) )
    {
        /* get data from server ref */
        dlen = WK_CCDW_TEXTBUFLEN;
        /* deal greeting sepreatedly */
        if( (Winks_stricmp(pText->base.pdata, "greeting" ) == 0) && (pGlobal->flag & WK_CCSW_LSKACTIVE) )
            pText->txtstyle.text = (char* )winks_ccdw_GetString( WK_CCSW_ACTIVESUGGEST_STRID );
        else if( (Winks_stricmp(pText->base.pdata, "greeting" ) == 0) && (pGlobal->flag & WK_CCSW_LSKACTIVED) )
            pText->txtstyle.text = (char* )winks_ccdw_GetString( WK_CCSW_ACTIVENOTE_STRID );
        else if( (Winks_GetShowElement( winks_ccsw_GetShowNum(pGlobal->Info.showtype), WINKS_TRUE, 
            pText->base.pdata, &fflag, pText->pbuf, (unsigned long* )(&dlen), NULL, 0 ) < 0) )
        {
            if( dlen > WK_CCDW_TEXTBUFLEN )
            {
                pdata = Winks_salAlloc( dlen + 2 );
                memset( pdata, 0, dlen + 2 );
                if( Winks_GetShowElement( winks_ccsw_GetShowNum(pGlobal->Info.showtype), WINKS_TRUE, 
                    pText->base.pdata, &fflag, pdata, (unsigned long* )(&dlen), NULL, 0 ) < 0 )
                {
                    Winks_salFree( pdata );
                    return -1;
                }
                pText->flags |= WK_CCDW_FTALLOC;
                pText->txtstyle.text = pdata;
            }
            else
                return -1;
        }
        else
            pText->txtstyle.text = pText->pbuf;
    }
    else if( pText->base.flags & WK_CCDW_RGFLAG_CREF )
    {
        /* get data from client ref */
        if( (pText->txtstyle.text == NULL) && (Winks_stricmp(pText->base.pdata, "peerNumber") == 0) )
        {
            /* get telnum string */
            //pText->txtstyle.text = pGlobal->pCCDWglb->stelnum;
        }
        else if( Winks_stricmp(pText->base.pdata, "systemTime") == 0 )
        {
            if( pText->txtstyle.text == NULL )
                pText->txtstyle.text = pText->pbuf;
            Winks_GetSysDateTime( &v_dateTime );
            sprintf( ptbuf, "%02d:%02d:%02d", v_dateTime.hour, v_dateTime.min, v_dateTime.sec );
            Winks_Ascii2UnicodeString( ptbuf, pText->txtstyle.text, WK_CCDW_TEXTBUFLEN );
            pText->base.flags |= WK_CCDW_RGFLAG_REDRAW;
        }
        else if( Winks_stricmp(pText->base.pdata, "connectedTime") == 0 )
        {
            if( pText->txtstyle.text == NULL )
                pText->txtstyle.text = pText->pbuf;
            Winks_GetCallTime( NULL, &v_dateTime );   
            sprintf( ptbuf, "%02d:%02d:%02d", v_dateTime.hour, v_dateTime.min, v_dateTime.sec );
            Winks_Ascii2UnicodeString( ptbuf, pText->txtstyle.text, WK_CCDW_TEXTBUFLEN );
            pText->base.flags |= WK_CCDW_RGFLAG_REDRAW;
        }
    }
    else
    {
        if( pText->txtstyle.text == NULL )
        {
			if (pText->base.pdata != NULL)
            dlen = strlen( pText->base.pdata ) * 2 + 2;
            if( dlen < WK_CCDW_TEXTBUFLEN )
            {
                pText->txtstyle.text = pText->pbuf;
                Winks_Utf2UnicodeString( pText->base.pdata, pText->txtstyle.text, dlen );
            }
            else
            {
                pText->txtstyle.text = Winks_salAlloc( dlen );
                memset( pText->txtstyle.text, 0, dlen );
                Winks_Utf2UnicodeString( pText->base.pdata, pText->txtstyle.text, dlen );
                pText->flags |= WK_CCDW_FTALLOC;
            }
        }
    }

    Winks_assert( pText->txtstyle.text );
    /*
    if( !(pText->flags & WK_CCDW_FTBGCOLOR) )
    {
        winks_ui_fillRect( pText->txtstyle.x, pText->txtstyle.y, 
            pText->txtstyle.x + pText->txtstyle.width, pText->txtstyle.y + pText->txtstyle.height, WK_CCDW_DEFCOLOR );
        //winks_ccdw_DrawOverlap( (Winks_CCDW_RgBase_s* )pText );
    }
    else
    {
        winks_ui_fillRect( pText->base.Section.x, pText->base.Section.y, 
            pText->base.Section.x + pText->base.Section.w, pText->base.Section.y + pText->base.Section.h,
            pText->txtstyle.backgroud_color );
    }
    
    if( pText->txtshadow.x || pText->txtshadow.y )
    {
        unsigned long lcolor = 0;
        lcolor = pText->txtstyle.font_feature.color;
        pText->txtstyle.x += pText->txtshadow.x;
        pText->txtstyle.y += pText->txtshadow.y;
        pText->txtstyle.font_feature.color = pText->txtshadow.color;
        if( pText->flags & WK_CCDW_FTNEEDSCROLL )
            winks_ui_handle_scrolling_text( &(pText->txtstyle) );
        else
            Winks_showtext_in_area( &(pText->txtstyle) );
        pText->txtstyle.font_feature.color = lcolor;
        pText->txtstyle.x -= pText->txtshadow.x;
        pText->txtstyle.y -= pText->txtshadow.y;
    }
    */
    if( pText->flags & WK_CCDW_FTNEEDSCROLL )
    {
        winks_ui_handle_scrolling_text( &(pText->txtstyle) );
        pText->base.flags |= WK_CCDW_RGFLAG_REDRAW;
    }
    else
    {
        if( Winks_showtext_in_area( &(pText->txtstyle) ) == WINKS_NEEDSCROLLING_FLAG )
        {
            pText->flags |= WK_CCDW_FTNEEDSCROLL;
            pText->base.flags |= WK_CCDW_RGFLAG_REDRAW;
        }
    }

    //Winks_printfInfo( "WK CCDW Draw Text %d, %d, %d, %d ref is %s\r\n", pText->base.Section.x, pText->base.Section.y, 
    //    pText->base.Section.w, pText->base.Section.h, pText->base.pdata );
    
    return 0;

}

static int winks_ccdw_ShowImgTooLarge( Winks_CCDW_Media_s* pMedia )
{
    Winks_fail( "Winks CCSW Img too large" );
    return -1;
}

static int Winks_CCDW_DrawMedia( Winks_CCDW_RgBase_s* pSection )
{
    Winks_CCSW_Global_s* pGlobal = &gCCSW;
    Winks_CCDW_Media_s* pMedia = (Winks_CCDW_Media_s* )pSection;
    int dlen = WINKS_PATH_MAX, gflag = 0;
/*
#define WK_WG_STRETCH_NONE  0//默认
#define WK_WG_STRETCH_UNIFORM  1//等比拉伸
#define WK_WG_STRETCH_FILL   2//填充
#define WK_WG_STRETCH_UNIFORMFILL  3//等比剪裁
*/
    unsigned long sflag = WK_WG_STRETCH_NONE, fflag = 0;
    //char pbuf[WINKS_PATH_MAX];
    char* pdata = NULL;

    Winks_printfInfo( "WK CDWW Draw Media %s\r\n", pMedia->base.pdata );

    if( pMedia->pdata == NULL )
    {
        if( pMedia->base.flags & WK_CCDW_RGFLAG_SREF )
            gflag = WINKS_TRUE;
        else if( pMedia->base.flags & (WK_CCDW_RGFLAG_CREF | WK_CCDW_RGFLAG_V) )
            gflag = WINKS_FALSE;
        else
        {
            Winks_fail( "WK CDWW Draw Media can not get file\r\n" );
            return -1;
        }
        
        if( (Winks_GetShowElement( winks_ccsw_GetShowNum(pGlobal->Info.showtype), gflag, pMedia->base.pdata, 
            &fflag, pMedia->pbuf, (unsigned long* )(&dlen), pMedia->CCTid, WINKS_CCSHOW_TID_MAX_LEN ) < 0) )
        {
            if( dlen > WINKS_PATH_MAX )
            {
                pdata = Winks_salAlloc( dlen + 2 );
                memset( pdata, 0, dlen + 2 );
                if( Winks_GetShowElement( winks_ccsw_GetShowNum(pGlobal->Info.showtype), gflag, pMedia->base.pdata, 
                    &fflag, pdata, (unsigned long* )(&dlen), pMedia->CCTid, WINKS_CCSHOW_TID_MAX_LEN ) < 0 )
                {
                    Winks_salFree( pdata );
                    return -1;
                }
                else
                    pMedia->pdata = pdata;
            }
            else
                return -1;
        }
        else
            pMedia->pdata = pMedia->pbuf;
        
    }
#ifdef WINKS_COLLECT
    if( stricmp(pMedia->base.pdata, "winks") == 0)
        memcpy(pGlobal->wid,pMedia->CCTid,sizeof(pGlobal->wid));
#endif
 
    switch( pMedia->stretch )
    {
    case WK_CCDW_MEDSTRE_UNIFORM:
        sflag = WK_WG_STRETCH_UNIFORM;
        break;
    case WK_CCDW_MEDSTRE_FILL:
        sflag = WK_WG_STRETCH_FILL;
        break;
    case WK_CCDW_MEDSTRE_UNIFORMFILL:
        sflag = WK_WG_STRETCH_UNIFORMFILL;
        break;
    default:
        sflag = WK_WG_STRETCH_NONE;
        break;
    }

    if( pMedia->base.flags & WK_CCDW_RGFLAG_BGCOLOR )
        winks_ui_fillRect( pMedia->base.Section.x, pMedia->base.Section.y,
            pMedia->base.Section.x + pMedia->base.Section.w, pMedia->base.Section.y + pMedia->base.Section.h, 
            pMedia->base.color );
    
    if( pMedia->anim_handle )
    {
        Winks_printfInfo( "WK CCDW Pmedia %s redraw\r\n", pdata );
        winks_image_stop_animation( pMedia->anim_handle );
    }
    if( winks_ui_drawanimation_by_filenametype( pMedia->pdata, WK_STRING_TYPEASCII, 
        pMedia->base.Section.x, pMedia->base.Section.y,pMedia->base.Section.w, pMedia->base.Section.h,
        sflag, &(pMedia->anim_handle) ) == WINKS_IMAGE_TOO_LARGE)
    {
        winks_ccdw_ShowImgTooLarge( pMedia );
    };

    //Winks_printfInfo( "WK CCDW Draw Image %d, %d, %d, %d file is %s\r\n", pMedia->base.Section.x, pMedia->base.Section.y, 
    //    pMedia->base.Section.w, pMedia->base.Section.h, pdata );


    return 0;
}

static int Winks_CCDW_DrawButton( Winks_CCDW_RgBase_s* pSection )
{
    /* @_@draw sk string */
    Winks_CCDW_Button_s* pButton = (Winks_CCDW_Button_s* )pSection;
    char* pStr = NULL;
    winks_scrolling_text_s Text;
    Winks_CCSW_Global_s* pGlobal = &gCCSW;

    memset( &Text, 0, sizeof(winks_scrolling_text_s) );

    Text.x = pButton->base.Section.x;
    Text.y = pButton->base.Section.y;
    Text.width = pButton->base.Section.w;
    Text.height = pButton->base.Section.h;
    Text.show_flags = WINKS_SHOW_CENTER;
    Text.font_feature.size = WINKS_MEDIUM_FONT;
    //Text.font_feature.style = WINKS_FONT_BOLDFACED;
    
    Winks_printfInfo( "WK CDWW Draw Button %s\r\n", pButton->base.pdata );
    switch( pButton->OnPush )
    {
    case WK_CCDW_BTPUSH_LSK:
        if( pGlobal->flag & WK_CCSW_LSKACTIVE )
        {
            pStr = (char* )winks_ccdw_GetString( WK_CCSW_ACTIVE_STRID );
        }
        else if( pGlobal->flag & WK_CCSW_LSKACTIVED )
        {
            pStr = (char* )winks_ccdw_GetString( WK_CCSW_ACTIVE_STRID );
            Text.font_feature.color = WINKS_PIXEL_DARKGRAY;
        }
#ifdef WINKS_COLLECT
        else if( pGlobal->flag & WK_CCSW_LSKCOLLECT )
        {
            pStr = (char* )winks_ccdw_GetString( WK_CCSW_COLLECT_STRID );
        }
        else if( pGlobal->flag & WK_CCSW_LSKCOLLECTED )
        {
            pStr = (char* )winks_ccdw_GetString( WK_CCSW_COLLECTED_STRID );
            Text.font_feature.color = WINKS_PIXEL_DARKGRAY;
        }
#endif //WINKS_COLLECT
        Text.show_flags = WINKS_SHOW_LEFT;
        break;

    case WK_CCDW_BTPUSH_RSK:
        pStr = (char* )winks_ccdw_GetString( WK_CCSW_CLOSECC_STRID );
	 Text.show_flags = WINKS_SHOW_RIGHT;	
        break;

    case WK_CCDW_BTPUSH_ANSWER:
        pStr = (char* )winks_ccdw_GetString( WK_CCSW_ANSWER_STRID );
	 Text.show_flags = WINKS_SHOW_LEFT;	
        break;

    case WK_CCDW_BTPUSH_HANGUP:
        pStr = (char* )winks_ccdw_GetString( WK_CCSW_HANGUP_STRID );
	 Text.show_flags = WINKS_SHOW_RIGHT;	
        break;

    default:
        Winks_fail( "WK CCDW draw button unknow onpush\r\n" );
        break;
    }

    if( pStr )
    {
        //winks_ui_fillRect( Text.x, Text.y, 
        //    Text.x + Text.width, Text.y + Text.height, WK_CCDW_DEFCOLOR );

        //winks_ccdw_DrawOverlap( (Winks_CCDW_RgBase_s* )pButton );
        Text.text = pStr;
	 
        Winks_showtext_in_area( &Text );
    }

    //Winks_printfInfo( "WK CCDW Draw Button %d, %d, %d, %d OnPush is %d\r\n", pButton->base.Section.x, pButton->base.Section.y, 
    //    pButton->base.Section.w, pButton->base.Section.h, pButton->OnPush );
    
    return 0;
}

static int winks_ccdw_Draw( Winks_CCDW_RgBase_s* pBase, int ifover )
{
    int i = 0;
    Winks_CCDW_Text_s* pText = NULL;

    while( Winks_CCDW_DrawFunc[i].type > 0 )
    {
        if( Winks_CCDW_DrawFunc[i].type == pBase->type )
        {
            if( Winks_CCDW_DrawFunc[i].pDrawFunc )
            {
                if( pBase->type == WK_CCDW_RGTYPE_TEXT )
                {
                    pText = (Winks_CCDW_Text_s* )pBase;
                }
                if( (pText && !(pText->flags & WK_CCDW_FTBGCOLOR))
                    || (pBase->type == WK_CCDW_RGTYPE_BUTTON) )
                {
                    winks_ui_fillRect( pBase->Section.x, pBase->Section.y, 
                        pBase->Section.x + pBase->Section.w, pBase->Section.y + pBase->Section.h, WK_CCDW_DEFCOLOR );
                }
                if( (ifover == 0) && ((pBase->type == WK_CCDW_RGTYPE_TEXT) || (pBase->type == WK_CCDW_RGTYPE_BUTTON)) )
                    winks_ccdw_DrawOverlap( pBase, WK_CCDW_RGFLAG_OVERB );
                Winks_CCDW_DrawFunc[i].pDrawFunc( pBase );
                if(ifover == 0)
                    winks_ccdw_DrawOverlap( pBase, WK_CCDW_RGFLAG_OVERA );

                return 0;
            }
            else
            {
                Winks_fail( "WK CCDW draw section no function\r\n" );
                return -1;
            }
        }

        i++;
    }

    return -1;
}

/////////////////////////////////////////////////////////////////////////////////////
static int winks_ccdw_Gethexbychar( char c )
{
    if( c >= '0' && c <='9' )
        return (c - '0');

    switch( c )
    {
    case 'a':
    case 'A':
        return 10;

    case 'b':
    case 'B':
        return 11;

    case 'c':
    case 'C':
        return 12;

    case 'd':
    case 'D':
        return 13;

    case 'e':
    case 'E':
        return 14;

    case 'f':
    case 'F':
        return 15;
    }

    return -1;
}

static int winks_ccdw_Gethexbystring( char* ps, int* pvalue )
{
    int ret = 0, hex = 0, i = 0;
/*    
    while( winks_ccdw_Gethexbychar( *ps ) < 0 )
    {
        if( *ps == 0 )
        {
            Winks_fail( "WK CCDW get int error\r\n" );
            return -1;
        }
        ps ++;
        i++;
    }

    while( (hex = winks_ccdw_Gethexbychar( *ps )) >= 0 )
    {
        ret = (ret << 4) + hex;
        ps ++;
        i++;
    }
*/
    while( *ps )
    {
        if( (hex = winks_ccdw_Gethexbychar( *ps )) >= 0 )
            ret = (ret << 4) + hex;
        else
        {
            if( (*ps == 'x') || (*ps == 'X') || (*ps == '#') )
                ret = 0;
            else if( *ps == ',' )
                break;
            else if( (*ps != ' ') && (*ps != 0x0d) && (*ps != 0x0a) && (*ps != 0x09) )
            {
                Winks_fail( "WK CCDW get int error\r\n" );
                return -1;
            }
        }
        i++;
        ps++;
    }
    *pvalue = ret;

    return i;

}

static int winks_ccdw_Getintbystring( char* ps, int* pvalue )
{
    int ret = 0, i = 0;

    while( (*ps < '0') || (*ps > '9') )
    {
        if( *ps == 0 )
        {
            Winks_fail( "WK CCDW get int error\r\n" );
            return -1;
        }
        ps ++;
        i++;
    }

    while( (*ps >= '0') && (*ps <= '9') )
    {
        ret = ret * 10 + (*ps - '0');
        ps ++;
        i++;
    }

    *pvalue = ret;

    return i;
    
}

static Winks_CCDW_RgBase_s* winks_ccdw_ParseBase( Winks_XmlElement_s* pXmlele, int stroff )
{
    Winks_XmlElement_s* pele = NULL;
    Winks_CCDW_RgBase_s* pBase = NULL;
    int conoff = 0, dlen = 0, mlen = 0, soff = 0;
    unsigned short flags = 0;
    char* pcon = NULL;
#define WK_CCDW_STATELEN    32
    char pstate[WK_CCDW_STATELEN];
    char* pdata = NULL;
    
    pele = pXmlele->pchild;

    while( pele )
    {
        if( Winks_stricmp( pele->ptag, "sref" ) == 0 )
        {
            pdata = pele->pcontent;
            flags = WK_CCDW_RGFLAG_SREF;
        }
        else if( Winks_stricmp( pele->ptag, "cref" ) == 0 )
        {
            pdata = pele->pcontent;
            flags = WK_CCDW_RGFLAG_CREF;
        }
        else if( Winks_stricmp( pele->ptag, "v" ) == 0 )
        {
            pdata = pele->pcontent;
            flags = WK_CCDW_RGFLAG_V;
        }
        if( pdata )
            dlen = strlen( pdata );
        pele = pele->pbrother;
    }

    /*
    pBase = Winks_salAlloc( 1024 );
    Winks_mem_set( pBase, 0, 1024 );
    */
    mlen = sizeof(Winks_CCDW_RgBase_s) + stroff + dlen + 1;
    if( mlen < 256 )
        mlen = 256;
    pBase = Winks_salAlloc( mlen );
    memset( pBase, 0, mlen );
    
    pBase->flags = flags;

    if( pdata )
    {
        pBase->pdata = (char* )((char* )pBase + sizeof(Winks_CCDW_RgBase_s) + stroff);
        memcpy( pBase->pdata, pdata, dlen );
    }

    pele = pXmlele->pchild;

    while( pele )
    {
        if( Winks_stricmp( pele->ptag, "d" ) == 0 )
        {
            pcon = pele->pcontent;
            if( (conoff = winks_ccdw_Getintbystring( pcon, &(pBase->Section.x) )) < 0 )
            {
                Winks_fail( "WK CCDW parse section x error\r\n" );
                Winks_salFree( pBase );
                return NULL;
            }
            pcon += conoff;
            if( (conoff = winks_ccdw_Getintbystring( pcon, &(pBase->Section.y) )) < 0 )
            {
                Winks_fail( "WK CCDW parse section y error\r\n" );
                Winks_salFree( pBase );
                return NULL;
            }
            pcon += conoff;
            if( (conoff = winks_ccdw_Getintbystring( pcon, &(pBase->Section.w) )) < 0 )
            {
                Winks_fail( "WK CCDW parse section w error\r\n" );
                Winks_salFree( pBase );
                return NULL;
            }
            pcon += conoff;
            if( (conoff = winks_ccdw_Getintbystring( pcon, &(pBase->Section.h) )) < 0 )
            {
                Winks_fail( "WK CCDW parse section h error\r\n" );
                Winks_salFree( pBase );
                return NULL;
            }
            pcon += conoff;
        }
        else if( Winks_stricmp( pele->ptag, "state" ) == 0 )
        {
            pcon = pele->pcontent;
            while( *pcon )
            {
                if( *pcon != ',' )
                {
                    pstate[soff] = *pcon;
                    soff ++;
                }
                else
                {
                    pstate[soff] = 0;
                    pBase->Section.state |= winks_ccdw_GetValueByString( pstate, (Winks_CCDW_Convert_s* )Winks_CCDW_StateCvt );
                    soff = 0;
                }
                pcon ++;
            }
            pstate[soff] = 0;
            pBase->Section.state |= winks_ccdw_GetValueByString( pstate, (Winks_CCDW_Convert_s* )Winks_CCDW_StateCvt );
        }
        else if( Winks_stricmp( pele->ptag, "z" ) == 0 )
        {
            if( (conoff = winks_ccdw_Getintbystring( pele->pcontent, &(pBase->Section.z) )) < 0 )
            {
                Winks_fail( "WK CCDW parse section z error\r\n" );
                Winks_salFree( pBase );
                return NULL;
            }
        }
        else if( Winks_stricmp( pele->ptag, "color" ) == 0 )
        {
            if( (conoff = winks_ccdw_Gethexbystring( pele->pcontent, (int* )(&(pBase->color)) )) < 0 )
            {
                Winks_fail( "WK CCDW parse section z error\r\n" );
                Winks_salFree( pBase );
                return NULL;
            }
            pBase->flags |= WK_CCDW_RGFLAG_BGCOLOR;
        }
        pele = pele->pbrother;
    }

    return pBase;
}

static Winks_CCDW_RgBase_s* Winks_CCDW_ParseRect( Winks_XmlElement_s* pXmlele )
{
    Winks_CCDW_Rect_s* pRect = NULL;
    //int ifbwidth = 0;
    Winks_XmlElement_s* pele = NULL;

    if( (pRect = (Winks_CCDW_Rect_s* )(winks_ccdw_ParseBase( pXmlele, 0 ))) == NULL )
    {
        Winks_fail( "WK CCDW Parse Rect Base error\r\n" );
        return NULL;
    }

    pRect->base.type = WK_CCDW_RGTYPE_RECT;

    pele = pXmlele->pchild;

    while( pele )
    {
        if( Winks_stricmp( pele->ptag, "border-width" ) == 0 )
        {
            if( winks_ccdw_Getintbystring( pele->pcontent, &(pRect->bwidth) ) < 0 )
            {
                Winks_fail( "WK CCDW parse rect bwidth error\r\n" );
                Winks_salFree( pRect );
                return NULL;
            }
            //ifbwidth = 1;
        }
        else if( Winks_stricmp( pele->ptag, "border-style" ) == 0 )
        {
            pRect->bstyle = winks_ccdw_GetValueByString( pele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_RectCvt );
        }
        else if( Winks_stricmp( pele->ptag, "border-color" ) == 0 )
        {
            if( winks_ccdw_Gethexbystring( pele->pcontent, (int* )(&(pRect->bcolor)) ) < 0 )
            {
                Winks_fail( "WK CCDW parse rect bcolor error\r\n" );
                Winks_salFree( pRect );
                return NULL;
            }
        }

        pele = pele->pbrother;
    }
    
    //if( !ifbwidth )
        /* border width default is 1 */
        //pRect->bwidth = 1;

    return (Winks_CCDW_RgBase_s* )pRect;
}

static Winks_CCDW_RgBase_s* Winks_CCDW_ParseText( Winks_XmlElement_s* pXmlele )
{
    Winks_CCDW_Text_s* pText = NULL;
    Winks_XmlElement_s* pele = NULL;
    Winks_XmlElement_s* pcele = NULL;
    char* pcon = NULL;
    unsigned long color = 0, clen = 0;
    int soff = 0;

    if( (pText = (Winks_CCDW_Text_s* )(winks_ccdw_ParseBase( pXmlele, (sizeof(Winks_CCDW_Text_s) - sizeof(Winks_CCDW_RgBase_s)) ))) == NULL )
    {
        Winks_fail( "WK CCDW Parse Rect Base error\r\n" );
        return NULL;
    }

    pText->base.type = WK_CCDW_RGTYPE_TEXT;
    
    pText->txtstyle.x = pText->base.Section.x;
    pText->txtstyle.y = pText->base.Section.y;
    pText->txtstyle.width = pText->base.Section.w;
    pText->txtstyle.height = pText->base.Section.h;

    pText->txtstyle.show_flags = WINKS_SHOW_CENTER;

    if( pText->base.flags & WK_CCDW_RGFLAG_BGCOLOR )
        pText->txtstyle.font_feature.color = pText->base.color;

    pele = pXmlele->pchild;

    while( pele )
    {
        if( Winks_stricmp( pele->ptag, "align" ) == 0 )
        {
            pText->txtstyle.show_flags = (unsigned char)winks_ccdw_GetValueByString( pele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_FTSTYCvt );
        }
        else if( (Winks_stricmp( pele->ptag, "wrap" ) == 0) )
        {
            pText->txtstyle.wrapflags = (unsigned char)winks_ccdw_GetValueByString( pele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_FTSTYCvt );
        }
        else if( (Winks_stricmp( pele->ptag, "trim" ) == 0) )
        {
            pText->txtstyle.font_feature.way = (unsigned short)winks_ccdw_GetValueByString( pele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_FTSTYCvt );
        }
        else if( Winks_stricmp( pele->ptag, "overflow" ) == 0 )
        {
            pText->txtstyle.font_feature.way = (unsigned short)winks_ccdw_GetValueByString( pele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_FTSTYCvt );
        }
        else if( (Winks_stricmp( pele->ptag, "decoration" ) == 0) )
        {
            pText->txtstyle.font_feature.style |= winks_ccdw_GetValueByString( pele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_FTSTYCvt );
        }
        else if( Winks_stricmp( pele->ptag, "font" ) == 0 )
        {
            pcele = pele->pchild;
            while( pcele )
            {
                if( Winks_stricmp( pcele->ptag, "family" ) == 0 )
                {
                    /* @_@ */
                    Winks_printfInfo( "WK CCDW parse font family not support\r\n" );
                }
                else if( Winks_stricmp( pcele->ptag, "size" ) == 0 )
                {
                    int fsize = 0;
                    if( winks_ccdw_Getintbystring(pcele->pcontent, &(fsize)) < 0 )
                    {
                        Winks_fail( "WK CCDW parse text font size error\r\n" );
                        Winks_salFree( pText );
                        return NULL;
                    }
					
                    pText->txtstyle.font_feature.size  = fsize;
                }
                else if( (Winks_stricmp( pcele->ptag, "style" ) == 0) || (Winks_stricmp( pcele->ptag, "weight" ) == 0) )
                {
                    pText->txtstyle.font_feature.style |= winks_ccdw_GetValueByString( pcele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_FTCvt );
                }
                pcele = pcele->pbrother;
            }
        }
        else if( Winks_stricmp( pele->ptag, "background-color" ) == 0 )
        {
            if( winks_ccdw_Gethexbystring( pele->pcontent, (int* )(&(pText->txtstyle.backgroud_color)) ) < 0 )
            {
                Winks_fail( "WK CCDW parse text bcolor error\r\n" );
                Winks_salFree( pText );
                return NULL;
            }
            pText->txtstyle.backgroudcolorflags = 1;
            pText->flags |= WK_CCDW_FTBGCOLOR;
            if( (pText->txtstyle.backgroud_color == pText->txtstyle.font_feature.color) )
            {
                Winks_printfInfo( "WK CCDW text color same as bgcolor, we verified it\r\n" );
                pText->txtstyle.font_feature.color = ~(pText->txtstyle.backgroud_color);
            }
        }
        else if( Winks_stricmp( pele->ptag, "shadow" ) == 0 )
        {
            pcele = pele->pchild;
            while( pcele )
            {
                if( Winks_stricmp( pcele->ptag, "color" ) == 0 )
                {
                    if( winks_ccdw_Gethexbystring(pcele->pcontent, (int* )(&(pText->txtstyle.txtshadow_color))) < 0 )
                    {
                        Winks_fail( "WK CCDW parse text shadow color error\r\n" );
                        Winks_salFree( pText );
                        return NULL;
                    }
                }
                else if( Winks_stricmp( pcele->ptag, "x" ) == 0 )
                {
                    if( winks_ccdw_Getintbystring(pcele->pcontent, &(soff)) < 0 )
                    {
                        Winks_fail( "WK CCDW parse text shadow x error\r\n" );
                        Winks_salFree( pText );
                        return NULL;
                    }
                    else
                        pText->txtstyle.txtshadow_x = (short)soff;
                }
                else if( Winks_stricmp( pcele->ptag, "y" ) == 0 )
                {
                    if( winks_ccdw_Getintbystring(pcele->pcontent, &(soff)) < 0 )
                    {
                        Winks_fail( "WK CCDW parse text shadow y error\r\n" );
                        Winks_salFree( pText );
                        return NULL;
                    }
                    else
                        pText->txtstyle.txtshadow_y = (short)soff;
                }
                pcele = pcele->pbrother;
            }
            pText->flags |= WK_CCDW_FTSHADOW;
        }
        else if( Winks_stricmp( pele->ptag, "effect" ) == 0 )
        {
            pcele = pele->pchild;
            /*
            while( pcele )
            {
                if( Winks_stricmp( pcele->ptag, "predefined" ) == 0 )
                {
                    pText->txtstyle.effect_flags = winks_ccdw_GetValueByString( pcele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_FTCvt );
                }
                pcele = pcele->pbrother;
            }
            */
            while( pcele )
            {
                if( Winks_stricmp(pcele->ptag, "gradient-color-flow") == 0 )
                {
                    pText->txtstyle.effect_flags = WK_WG_GRADIENTCOLORANIMATION;
                    break;
                }
                else if( Winks_stricmp(pcele->ptag, "text-color-slide") == 0 )
                {
                    pText->txtstyle.effect_flags = WK_WG_TEXTCOLORSLIDE;
                    break;
                }

                pcele = pcele->pbrother;
            }

            if( pText->txtstyle.effect_flags )
            {
                pcele = pcele->pchild;
                if( Winks_stricmp(pcele->ptag, "color-set") == 0 )
                {
                    pcon = pcele->pcontent;
                    pText->txtstyle.text_colorlist.colorlist = pText->effcolor;
                    while( *pcon )
                    {
                        clen = winks_ccdw_Gethexbystring(pcon, (int* )(&color));
                        pText->txtstyle.text_colorlist.colorlist[pText->txtstyle.text_colorlist.tal_colornum] = color;
                        pcon += clen;
                        while( (*pcon == ' ') || (*pcon == ',') )
                            pcon ++;
                        pText->txtstyle.text_colorlist.tal_colornum ++;
                        if( pText->txtstyle.text_colorlist.tal_colornum >= WK_CCDW_MAXEFFECTCOLOR_NUM )
                            break;
                    }
                }
            }
        }


        pele = pele->pbrother;
    }

    return (Winks_CCDW_RgBase_s* )pText;
}

static Winks_CCDW_RgBase_s* Winks_CCDW_ParseMedia( Winks_XmlElement_s* pXmlele )
{
    Winks_CCDW_Media_s* pMedia = NULL;
    Winks_XmlElement_s* pele = NULL;

    if( (pMedia = (Winks_CCDW_Media_s* )(winks_ccdw_ParseBase( pXmlele, (sizeof(Winks_CCDW_Media_s) - sizeof(Winks_CCDW_RgBase_s)) ))) == NULL )
    {
        Winks_fail( "WK CCDW Parse Rect Base error\r\n" );
        return NULL;
    }

    pMedia->base.type = WK_CCDW_RGTYPE_MEDIA;

    pele = pXmlele->pchild;

    while( pele )
    {
        if( Winks_stricmp( pele->ptag, "stretch" ) == 0 )
        {
            pMedia->stretch = winks_ccdw_GetValueByString( pele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_MediaCvt );
        }
        pele = pele->pbrother;
    }

    return (Winks_CCDW_RgBase_s* )pMedia;

}
static Winks_CCDW_RgBase_s* Winks_CCDW_ParseButton( Winks_XmlElement_s* pXmlele )
{
    Winks_CCDW_Button_s* pButton = NULL;
    Winks_XmlElement_s* pele = NULL;

    if( (pButton = (Winks_CCDW_Button_s* )(winks_ccdw_ParseBase( pXmlele, (sizeof(Winks_CCDW_Button_s) - sizeof(Winks_CCDW_RgBase_s)) ))) == NULL )
    {
        Winks_fail( "WK CCDW Parse Rect Base error\r\n" );
        return NULL;
    }

    pButton->base.type = WK_CCDW_RGTYPE_BUTTON;

    pele = pXmlele->pchild;

    while( pele )
    {
        if( Winks_stricmp( pele->ptag, "onPush" ) == 0 )
        {
            pButton->OnPush = winks_ccdw_GetValueByString( pele->pcontent, (Winks_CCDW_Convert_s* )Winks_CCDW_ButtonCvt );
        }
        pele = pele->pbrother;
    }

    return (Winks_CCDW_RgBase_s* )pButton;

}

////////////////////////////////////////////////////////////////////////////////////

Winks_CCDW_RgBase_s* winks_ccsw_ParseXml( Winks_Xml_s* pXml )
{
    Winks_XmlElement_s* pele = NULL;
    Winks_CCDW_RgBase_s* pbase = NULL;
    Winks_CCDW_RgBase_s* plast = NULL;
    Winks_CCDW_RgBase_s* ptemp = NULL;
    int rtype = 0, i = 0;

    pele = pXml->pelement;
    if( Winks_stricmp( pele->ptag, "page" ) != 0 )
    {
        Winks_fail( "WK CCDW Parse data no page tag\r\n" );
        return NULL;
    }
    pele = pele->pchild;
    if( Winks_stricmp( pele->ptag, "canvas" ) != 0 )
    {
        Winks_fail( "WK CCDW Parse data no canvas tag\r\n" );
        return NULL;
    }
    pele = pele->pchild;

    while( pele )
    {
        rtype = winks_ccdw_GetValueByString( pele->ptag, (Winks_CCDW_Convert_s* )Winks_CCDW_TYPECvt );
        i = 0;
        while( Winks_CCDW_DrawFunc[i].type > 0 )
        {
            if( Winks_CCDW_DrawFunc[i].type == rtype )
            {
                if( Winks_CCDW_DrawFunc[i].pParseFunc )
                {
                    if( (ptemp = Winks_CCDW_DrawFunc[i].pParseFunc( pele )) != NULL )
                    {
                        if( plast == NULL )
                        {
                            pbase = ptemp;
                            plast = ptemp;
                        }
                        else
                        {
                            plast->pnext = ptemp;
                            plast = ptemp;
                        }
                    }
                    else
                    {
                        Winks_fail( "WK CCDW Parse data error\r\n" );
                    }
                }
                else
                {
                    Winks_fail( "WK CCDW Parse Function error\r\n" );
                    return NULL;
                }
                break;
            }

            i++;
        }
        pele = pele->pbrother;
    }

    return pbase;
}

int winks_ccsw_FreeXml( Winks_CCDW_RgBase_s* pBase )
{
    Winks_CCDW_RgBase_s* pFree = pBase;
    Winks_CCDW_Text_s* pText = NULL;
    Winks_CCDW_Media_s* pMedia = NULL;

    while( pBase )
    {
        pBase = pBase->pnext;
        if( pFree->type == WK_CCDW_RGTYPE_TEXT )
        {
            pText = (Winks_CCDW_Text_s* )pFree;
            if( pText->flags & WK_CCDW_FTALLOC )
                Winks_salFree( pText->txtstyle.text );
        }
        else if( pFree->type == WK_CCDW_RGTYPE_MEDIA )
        {
            pMedia = (Winks_CCDW_Media_s* )pFree;
            if( (pMedia->pdata) && (pMedia->pdata != pMedia->pbuf) )
                Winks_salFree( pMedia->pdata );
        }
        Winks_salFree( pFree );
        pFree = pBase;
    }

    return 0;
}

int winks_ccsw_Draw( Winks_CCDW_Global_s* pCCDW, int option )
{
    Winks_CCDW_RgBase_s* pBase = pCCDW->pRegion;
    Winks_CCSW_Global_s* pGlobal = &gCCSW;

    Winks_printfInfo("[wk_ccdrw] [wk_ccsw_draw] enter\r\n");

    //pCCDW->flag1 = 0;
#if 0	
    pCCDW->flag2 = 0;
#endif	
   
   //panqj ????? WINKS_CC_BUFFER_ON;
    if( option == WK_CCSW_DWALL )
        winks_ui_fillRect( 0, 0, WINKS_SCREEN_WIDTH, WINKS_SCREEN_HEIGHT, WK_CCDW_DEFCOLOR );
    while( pBase )
    {
        switch( option )
        {
        case WK_CCSW_DWALL:
            if( (pBase->Section.state == 0) || (pBase->Section.state & pGlobal->callstate) )
                winks_ccdw_Draw( pBase, 0 );
            break;
        case WK_CCSW_DWSTATE:
            if( pBase->Section.state & pGlobal->callstate )
                winks_ccdw_Draw( pBase, 0 );
            break;
        case WK_CCSW_DWTIME:
            if( ((pBase->Section.state == 0) || (pBase->Section.state & pGlobal->callstate))
                && (pBase->flags & WK_CCDW_RGFLAG_REDRAW) )
            {
                pBase->flags &= ~WK_CCDW_RGFLAG_REDRAW;
                winks_ccdw_Draw( pBase, 0 );
            }
            break;
        default:
            Winks_fail( "WK CCSW Draw option unknow\r\n" );
            break;
        }
        pBase = pBase->pnext;
    }
   //panqj ??? WINKS_CC_BUFFER_OFF;

   // pCCDW->flag1 = 1;
#if 0	
    if (pCCDW->flag2 != 0)
    {
         Winks_printfInfo("[WK CC SHOW] [winks_ccdrw_draw] pGlobal->pCCDWglb->flag2 != 0 ");
         pCCDW->flag2 = 0;
    }
#endif
    Winks_printfInfo("[wk_ccdrw] [wk_ccsw_draw] exit\r\n");

    return 0;
}

int winks_ccsw_FreezeRg( Winks_CCDW_Global_s* pCCDW )
{
    Winks_CCDW_RgBase_s* pBase = pCCDW->pRegion;
    Winks_CCDW_Media_s* pMedia = NULL;

    while( pBase )
    {
        if( pBase->type == WK_CCDW_RGTYPE_MEDIA )
        {
            pMedia = (Winks_CCDW_Media_s* )pBase;
            if( pMedia->anim_handle )
            {
                winks_image_stop_animation( pMedia->anim_handle );
                pMedia->anim_handle = 0;
            }
        }

        pBase = pBase->pnext;
    }

    return 0;
}

#if 1
char* winks_ccsw_GetShowNum( int option )
{
    Winks_CCSW_Global_s* pGlobal = &gCCSW;
    
    switch( option )
    {
    case WK_CCSW_TITLE:
        return WINKS_PRI_CC_NO;
    case WK_CCSW_USER:
        return pGlobal->Info.number;
    case WK_CCSW_DEFAULT:
        return WINKS_DFT_CC_NO;
    case WK_CCSW_BUS:
        return WINKS_BUS_CC_NO;
    default:
        Winks_fail( "WK get region unknow option\r\n" );
        return NULL;
    }

    return NULL;
}
#endif
