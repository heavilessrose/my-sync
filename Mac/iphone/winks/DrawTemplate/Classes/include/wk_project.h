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
*Id: wk_project.h,v 1.00  2008/06/03 11:11:37
**************************************************************************    
* File Description
* ----------------
* Configuration about project.
**************************************************************************/
#ifndef _WK_PROJECT_H_
#define _WK_PROJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WINKS_VERSION_TAG "Winks2009-03-23 V_D1.0"
#define WINKS_MV "1.0"  /* »Ìº˛÷˜∞Ê±æ∫≈ */
#define WINKS_V  "1.1"  /*winks-protocol-version –≠“È∞Ê±æ */

//#define WINKS_REVISION_NUMBER       "10000"                         /*revision NO.*/
#include"wk_revision.h"


//WINCE ≤ª–Ë“™_WINKS_EMULATOR_£¨◊‘––undef WINKS_SIMULATOR
#ifdef _MSC_VER
#define WINKS_SIMULATOR
#endif

#ifdef __WINSCW__
#define WINKS_SIMULATOR
#endif

#ifdef WINKS_SKYWORTH_080601/*≤˙∆∑∫Íø™πÿ*/
    #define WINKS_SUPPORT_3G                                        /*3G:¿¥µÁ¡™Õ¯π¶ƒ‹°£*/
    #define WINKS_MTK
#ifdef MTK_CC_VERSION
    #define WINKS_LIST
#endif
    #define WINKS_COLLECT                                           /* π¶ƒ‹∫Í: “ªº¸ ’≤ÿ */
    #define WINKS_SMALL_DEF_CC                                      /* π”√–°µƒƒ¨»œ≤ œÒ*/
    #define WINKS_MTK_V_0_6_b                                       /*»Ìº˛∆ΩÃ®∞Ê±æ∫Íø™πÿ*/
    #define WINKS_SUPPORT_MEMORY_CARD
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
    #define WK_SYN_VAL 0x3d /*WK_SYN_MSG_KTY|WK_SYN_MSG_CREATE_APP|WK_SYN_MSG_PAUSE_RESUME_APP|WK_SYN_MSG_DESTROY_APP|WK_SYN_MSG_PAINT*/

    #define WINKS_KEY_TEXT_DOWN
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__    1
    #define __WINKS_MULTI_LAYS__
    #define __WINKS_STRINGBORDER__
    //#define WINKS_SCREEN_WIDTH          176                         /*∆¡øÌ£¨∑÷±Ê¬ */
    //#define WINKS_SCREEN_HEIGHT         220                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    extern int Winks_GetScreenWidth_mtk(void);
    extern int Winks_GetScreenHeight_mtk(void);
    #define WINKS_CCSCREEN_WIDTH         Winks_GetScreenWidth_mtk()    /*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_CCSCREEN_HEIGHT        Winks_GetScreenHeight_mtk()   /*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH           Winks_GetScreenWidth_mtk()    /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT          Winks_GetScreenHeight_mtk()   /*∆¡∏ﬂ£¨∑÷±Ê¬ */

    char* Winks_getversion(int flag);
    extern int Winks_GetSmsPromptFlag_mtk(void);
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AN                    "w-mtk-skyworth-06b"        /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
    #define WINKS_AV                    Winks_getversion(1)         /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
    #define WINKS_AF                    Winks_GetSmsPromptFlag_mtk() /* º§ªÓ ±∑¢∂Ã–≈ «∑ÒÃ· æ”√ªß */

//    #define WINKS_OS                    "MTK06b"                    /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
//    #define WINKS_HW                    "6225"                      /*hardware-id ÷’∂À”≤º˛±Í ∂ */
    #define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "240x320"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
    #define WINKS_MTK_ADC               "1001090"                   /*dist-channel «˛µ¿±‡¬Î*/
    extern char* winks_GetADC(void);
    #define WINKS_ADC                   winks_GetADC()              /*dist-channel «˛µ¿±‡¬Î*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/

    #define WINKS_ROOT_PATH             "C:\\"
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks\\"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks\\image\\"
    #define WINKS_PATH_SPACE            "\\"
  #ifdef WINKS_SIMULATOR//vcµ˜ ‘ ±º”¡À“ª–©Œƒº˛œµÕ≥µ˜ ‘
    #define WINKS_OSAL_API_SELF_DATA_LEN     32+128
  #else
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
  #endif
    #define WINKS_SYS_TICK              1                           /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝*/


#elif defined (WINKS_TIANYU_080601) || defined(WINKS_XMW_081016)    /*≤˙∆∑∫Íø™πÿ*/
    #define WINKS_MTK                                               /*∆ΩÃ®∞Ê±æ∫Íø™πÿ*/
#ifdef MTK_CC_VERSION
    #define WINKS_LIST
#endif
    #define WINKS_COLLECT                                           /* π¶ƒ‹∫Í: “ªº¸ ’≤ÿ */
    #define WINKS_SMALL_DEF_CC                                      /* π”√–°µƒƒ¨»œ≤ œÒ*/
    #ifdef WINKS_TIANYU_080601                                      /*∆ΩÃ®∞Ê±æ∫Íø™πÿ*/
    #define WINKS_MTK_V_0_6_a 
    #else
    #define WINKS_MTK_V_0_7_b    
    #define WINKS_DUAL_SIM
    #endif
    #define WINKS_SUPPORT_MEMORY_CARD
    /*∑˛ŒÒ∆˜ƒ£ƒ‚œ¬∑¢–≠“È÷∏¡Ó*/
    #define WINKS_TOUCH_PANNEL
    //#define   __240X320__
    #define WINKS_KEY_TEXT_DOWN
    #ifdef WINKS_TIANYU_080601
        #define   WINKS_GIF_IMAGE
    #endif
    #ifdef WINKS_XMW_081016
       // #define  WINKS_GIF_SMALL_ICON_IMAGE
       #define   WINKS_GIF_IMAGE
    #endif

    #define WK_SYN_VAL 0x3d /*WK_SYN_MSG_KTY|WK_SYN_MSG_CREATE_APP|WK_SYN_MSG_PAUSE_RESUME_APP|WK_SYN_MSG_DESTROY_APP|WK_SYN_MSG_PAINT*/
    #define WINKS_SUPPORT_3G                                        /*3G:¿¥µÁ¡™Õ¯π¶ƒ‹°£*/
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__    1
    #define __WINKS_MULTI_LAYS__
    #define __WINKS_STRINGBORDER__
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
    //#define WINKS_SCREEN_WIDTH          240                         /*∆¡øÌ£¨∑÷±Ê¬ */
    //#define WINKS_SCREEN_HEIGHT         320                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    extern int Winks_GetScreenWidth_mtk(void);
    extern int Winks_GetScreenHeight_mtk(void);
    #define WINKS_CCSCREEN_WIDTH         Winks_GetScreenWidth_mtk()    /*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_CCSCREEN_HEIGHT        Winks_GetScreenHeight_mtk()   /*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH           Winks_GetScreenWidth_mtk()    /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT          Winks_GetScreenHeight_mtk()   /*∆¡∏ﬂ£¨∑÷±Ê¬ */

    char* Winks_getversion(int flag);
    extern int Winks_GetSmsPromptFlag_mtk(void);
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AN                    "w-mtk-xmw-es"              /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
    #define WINKS_AV                    Winks_getversion(1)         /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
    #define WINKS_AF                    Winks_GetSmsPromptFlag_mtk() /* º§ªÓ ±∑¢∂Ã–≈ «∑ÒÃ· æ”√ªß */

#ifdef WINKS_TIANYU_080601
//    #define WINKS_OS                    "MTK06a"                    /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
//    #define WINKS_HW                    "6228"                      /*hardware-id ÷’∂À”≤º˛±Í ∂ */
#else
//    #define WINKS_OS                    "MTK07b"                    /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
//    #define WINKS_HW                    "6225"                      /*hardware-id ÷’∂À”≤º˛±Í ∂ */
#endif
    #define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "240x320"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
    #define WINKS_MTK_ADC               "1001090"                   /*dist-channel «˛µ¿±‡¬Î*/
    extern char* winks_GetADC(void);
    #define WINKS_ADC                   winks_GetADC()              /*dist-channel «˛µ¿±‡¬Î*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/

    #define WINKS_ROOT_PATH            "C:\\"
    #define WINKS_APP_PATH               WINKS_ROOT_PATH"winks\\"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks\\image\\"
    #define WINKS_PATH_SPACE           "\\"
  #ifdef WINKS_SIMULATOR//vcµ˜ ‘ ±º”¡À“ª–©Œƒº˛œµÕ≥µ˜ ‘
    #define WINKS_OSAL_API_SELF_DATA_LEN     32+128
  #else
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
  #endif
    #define WINKS_SYS_TICK              1                           /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝*/

#elif defined (WINKS_BREW)
    //#define WINKS_SUPPORT_3G                                        /*3G:¿¥µÁ¡™Õ¯π¶ƒ‹°£*/
    #define WINKS_USE_PLATFORM_SOURCE_MANAGER                       /*◊ ‘¥≤…”√∆ΩÃ®µƒ µœ÷∑Ω Ω∑Ω Ω*/
    //#define  WINKS_LIST_VERSION   //∂®“Â’‚∏ˆ∫Í“‘∫Û,Õº∆¨.c Œƒº˛÷–æÕ÷ª∞¸∫¨¡Àƒ¨»œ≤ œÒ,
                                  //Õ¨ ±À‰»ª≤ª∂®“ÂWINKS_USE_PLATFORM_SOURCE_MANAGER∫Í£¨µ´ «Õº∆¨ªπ «¥”barŒƒº˛º”‘ÿ 
    //#define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/

    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    //#define WINKS_AN                    "winks-BREW-S319"           /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
	
    #define WINKS_AV                    "1.0.3"                       /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
//    #define WINKS_OS                    "BREW-S319"                 /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
//    #define WINKS_HW                    ""                          /*hardware-id ÷’∂À”≤º˛±Í ∂ */
    
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
//    #define WINKS_ADC                   "0"                         /*dist-channel «˛µ¿±‡¬Î*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/

    #define WINKS_ROOT_PATH             AEEFS_HOME_DIR              /*brew ∂‡∏ˆ”¶”√÷ªƒ‹Õ®π˝∑√Œ SharedŒƒº˛º– µœ÷π≤œÌ*/
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks/"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks/image/"
    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
	
    #define WINKS_TOUCH_PANNEL
    #define WINKS_UNSUPPORT_PROXY
	#define CDMA_NETWORK    //ƒ¨»œÕ®ª∞«∞∫ÕÕ®ª∞÷–’π æ
	#define WK_SYN_VAL 0x3F
	#define  WK_BREW_SMALL_FONT AEE_FONT_NORMAL
	#define WINKS_BREW_BAR "winkscc.bar"
	#define WINKS_BREW_GIF_CLSID AEECLSID_GIF   //gif class id
	#define WINKS_BREW_CAMERA_PHOTO_PATH "fs:/root/image/"
#ifdef WINKS_BREW_176x205
	#define __176X205__
	#define WINKS_CCSCREEN_WIDTH        176							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       205							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
	#define WINKS_SCREEN_WIDTH          176                         /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT         205                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
	#define WINKS_CL                    "176x180"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "176X220"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
	#define WINKS_AN                    "w-brew-cdma-176x220"
	//#define WINKS_BREW_BAR  "winks_brew_176x205.bar"
	#define WINKS_BREW_RESHEADER_NAME  "winks_brew_176x205.brh"
	#define WINKS_BREW_IMAGE_IN_BARFILE
	#define WINKS_EMBEDDED_BROWSER
	#define WINKS_SUPPORT_MEMORY_CARD
#elif defined(WINKS_BREW_240x320)
	#define __240X320__
	#define WINKS_CCSCREEN_WIDTH        240							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       320							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
	#define WINKS_SCREEN_WIDTH          240                         /*∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_SCREEN_HEIGHT         320                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
	#define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "240x320"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
	#define WINKS_AN                    "w-brew-cdma-240x320"
	//#define WINKS_BREW_BAR  "winks_brew_240x320.bar"
	#define WINKS_BREW_RESHEADER_NAME  "winks_brew_240x320.brh"
	#define WINKS_BREW_IMAGE_IN_BARFILE
	#define WINKS_EMBEDDED_BROWSER
#elif defined(WINKS_BREW_240x377)
	#define __240X377__
	#define WINKS_CCSCREEN_WIDTH        240							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       377							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
	#define WINKS_SCREEN_WIDTH          240                         /*∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_SCREEN_HEIGHT         377                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
	#define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
	#define WINKS_CS                    "240x400"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
	#define WINKS_AN                    "w-brew-cdma-240x377"
	//#define WINKS_BREW_BAR  "winks_brew_240x377.bar"
	#define WINKS_BREW_RESHEADER_NAME  "winks_brew_240x377.brh"
	#define WINKS_EMBEDDED_BROWSER
	#define WINKS_BREW_IMAGE_IN_BARFILE
	#define WINKS_SUPPORT_MEMORY_CARD
#elif defined(WINKS_BREW_240x400)
	#define __240X400__
	#define WINKS_CCSCREEN_WIDTH        240							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       400							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
	#define WINKS_SCREEN_WIDTH          240                         /*∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_SCREEN_HEIGHT         400                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
	#define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
	#define WINKS_CS                    "240x400"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
	#define WINKS_AN                    "w-brew-cdma-240x400"
	//#define WINKS_BREW_BAR  "winks_brew_240x377.bar"
	#define WINKS_BREW_RESHEADER_NAME  "winks_brew_240x377.brh"
	#define WINKS_EMBEDDED_BROWSER
	#define WINKS_BREW_IMAGE_IN_BARFILE
	#define WINKS_SUPPORT_MEMORY_CARD
#endif

#ifdef WINKS_BREW_D300
	#ifndef AEE_SIMULATOR
		#undef WK_BREW_SMALL_FONT
		#define WK_BREW_SMALL_FONT AEE_FONT_BOLD_ITALIC
	#endif
	#undef  WINKS_BREW_GIF_CLSID
	#define WINKS_BREW_GIF_CLSID AEECLSID_VIEW+8
	#define WINKS_BREW_JPG_ASYNDRAW
#endif

#ifdef WINKS_BREW_D90
	#undef WINKS_BREW_CAMERA_PHOTO_PATH
	#define WINKS_BREW_CAMERA_PHOTO_PATH "fs:/root/photo/gallery/"
#endif

#ifdef WINKS_BREW_C7100
	#define WINKS_BREW_JPG_ASYNDRAW
#endif


    #define WINKS_START_SHOWCC_ASYNC
    #define WINKS_COLLECT
	//#define WINKS_EMBEDDED_BROWSER

#elif defined (WINKS_BREW_KDDI_W63SA)
    #define WINKS_SUPPORT_3G                                        /*3G:¿¥µÁ¡™Õ¯π¶ƒ‹°£*/
    #define BREWVER  KDDI_BREW_401
    #define WINKS_USE_PLATFORM_SOURCE_MANAGER                       /*◊ ‘¥≤…”√∆ΩÃ®µƒ µœ÷∑Ω Ω∑Ω Ω*/
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
	#define WINKS_CCSCREEN_WIDTH        240							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       320							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH          240                         /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT         320                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AN                    "winks-BREW"                /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
    #define WINKS_AV                    Winks_getversion(1)         /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
//    #define WINKS_OS                    "BREW-KDDI"               /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
//    #define WINKS_HW                    "W63SA"                   /*hardware-id ÷’∂À”≤º˛±Í ∂ */
    #define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "240x320"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
//    #define WINKS_ADC                   "0"                         /*dist-channel «˛µ¿±‡¬Î*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
    #define WINKS_ROOT_PATH             AEE_SHARED_DIR"/"           /*brew ∂‡∏ˆ”¶”√÷ªƒ‹Õ®π˝∑√Œ SharedŒƒº˛º– µœ÷π≤œÌ*/
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks/"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks/image/"
    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
    #define WINKS_UNSUPPORT_PROXY
    #define WINKS_TEST_VERSION
    #define    WINKS_LEFTSOFTKEY_BUTTON_X    0
    #define    __240X320__
    #define   __240X296__
    //#define WINKS_BREW_BAR  "winks_brew_kddi_w63sa.bar"
    #define WINKS_SYS_TICK              1                           /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝*/
    #define WINKS_START_SHOWCC_ASYNC
    
#elif defined (WINKS_ARENA_V_1_2) || defined(WINKS_ARENA_V_1_4) 
    #define WINKS_SUPPORT_3G                                        /*3G:¿¥µÁ¡™Õ¯π¶ƒ‹°£*/
    char* Winks_getversion(int flag);
    #define WINKS_ARENA
    #define WINKS_DYNAMIC_UPDATE                                    /*∂ØÃ¨∞≤◊∞µƒ»Ìº˛*/
    #define WINKS_MULTITHREADING                                    /*∂‡œﬂ≥Ã*/
    #define WINKS_UNSUPPORTED_GLOBAL_VAR
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__                 /*÷˜≤Àµ•œ‘ æ∑Á∏Òµƒ∫Í£¨*/
    #define NOT_MULTI_OPEN_CLOSE_CC
    #define __240X320__
    #define WINKS_TOUCH_PANNEL                                      /*¥•√˛∆¡*/
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
	#define WINKS_CCSCREEN_WIDTH        240							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       320							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH          240                         /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT         320                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AN                    "winks-arena"               /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
    #define WINKS_AV                    Winks_getversion(1)         /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
    #define WINKS_OS                    "ARENA-1-4"                 /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
    #define WINKS_HW                    "ARENA"                     /*hardware-id ÷’∂À”≤º˛±Í ∂ */
    #define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "240x320"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
    #define WINKS_ADC                   "0"                         /*dist-channel «˛µ¿±‡¬Î*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
    #define WINKS_ROOT_PATH             "/user/user/"
    #define WINKS_APP_PATH              WINKS_ROOT_PATH"winks/"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks/image/"
    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN     128
    #define WINKS_SYS_TICK              10                          /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝.(1sys tick = 10ms)*/
    
#elif defined (WINKS_SYMBIAN_V_9_0)

    /*∫· ˙∆¡◊‘  ”¶ÃÌº”µƒΩ”ø⁄*/
int Winks_GetCCScreenWidth();										/*ªÒ»°≤ œÒ’π æΩÁ√Ê∆¡ƒªøÌ∂»*/
int Winks_GetCCScreenHeight();										/*ªÒ»°≤ œÒ’π æΩÁ√Ê∆¡ƒª∏ﬂ∂»*/
int Winks_GetScreenWidth();                                         /*ªÒ»°∆¡ƒªøÌ∂»*/
int Winks_GetScreenHeight();                                        /*ªÒ»°∆¡ƒª∏ﬂ∂»*/
char* Winks_GetCLSize();                                            /*ªÒ»°≤ œÒ≥ﬂ¥Á*/
char* Winks_GetCSSize();                                            /*ªÒ»°≤ œÒ’π æª≠≤º≥ﬂ¥Á*/
char* Winks_GetImagePath();                                         /*ªÒ»°Õº∆¨◊ ‘¥¬∑æ∂*/
char* Winks_GetAN();                                                /*ªÒ»°∆¡ƒªAN÷µ*/
int Winks_GetDMFlag();                                              /* ÷ª˙÷˜≤Ÿ◊˜ƒ£ Ωœ¬£¨∫· ˙∆¡±Í÷æ£¨0Œ™ ˙∆¡£¨1Œ™∫·∆¡*/

#ifdef WINKS_SIMULATOR
    #define WINKS_DBFILE_MAX_SIZE       0x100000/*1M*/              /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x19000/*100K*/            /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
#else
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x10000/*65K*/             /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
#endif
	#define WINKS_CCSCREEN_WIDTH        Winks_GetCCScreenWidth()    /*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       Winks_GetCCScreenHeight()   /*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH          Winks_GetScreenWidth()        /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT         Winks_GetScreenHeight()       /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_ROOT_PATH               "c:\\system\\apps\\"
    //#define WINKS_ROOT_PATH             "c:\\data\\others\\"
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks\\"

    #define WINKS_IMG_PATH                Winks_GetImagePath()
    #define WINKS_PARTNER_LEN             16
    #define WINKS_PARTNER_PATH            WINKS_ROOT_PATH"winks\\partner.txt"
    #define WINKS_AN_PATH                 WINKS_ROOT_PATH"winks\\an.txt"
    #define WINKS_PATH_SPACE              "\\"
    #define WINKS_OSAL_API_SELF_DATA_LEN  32
    #define WINKS_SYS_TICK                1                         /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝*/

char* Winks_getversion(int flag);
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AV                    Winks_getversion(1)         //"1.1.2000" //    /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
    #define WINKS_OS                    "Symbian-9x"                /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
char *Symbian_GetPhoneType();
    #define WINKS_HW                    Symbian_GetPhoneType()      /*hardware-id ÷’∂À”≤º˛±Í ∂ */
    #define WINKS_AN_EXT                ".sis"                      /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ ¿©’π√˚*/

    #define WINKS_AN                    Winks_GetAN()               /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
    #define WINKS_CL                    Winks_GetCLSize()                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    Winks_GetCSSize()                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
    #define WINKS_VHFLAG                Winks_GetDMFlag()           /* ÷ª˙÷˜≤Ÿ◊˜ƒ£ Ωœ¬£¨∫· ˙∆¡±Í÷æ£¨0Œ™ ˙∆¡£¨1Œ™∫·∆¡*/

    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
char *Symbian_GetADC();
    #define WINKS_ADC                   Symbian_GetADC()    //"1" // /*dist-channel ∞≤◊∞«˛µ¿±‡¬Î,ø…”√∫Ø ˝ªÒ»°*/
int Symbian_GetAvtiveFlag();/*∞≤◊∞«˛µ¿∫≈µ⁄“ªŒªY/N◊¥Ã¨£¨Y∑µªÿ1£¨N∑µªÿ0£¨ƒ¨»œ∑µªÿ1*/
    #define WINKS_AF                   Symbian_GetAvtiveFlag()      ///*∞≤◊∞«˛µ¿∫≈µ⁄“ªŒªY/N◊¥Ã¨£¨Y∑µªÿ1£¨N∑µªÿ0£¨ƒ¨»œ∑µªÿ1*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/

#define WK_SYN_VAL 0

#elif defined (WINKS_IPHONE)
	#define WINKS_UNSUPPORT_PROXY
    #define WINKS_USE_PLATFORM_SOURCE_MANAGER
    #ifdef WINKS_SIMULATOR
        #define WINKS_DBFILE_MAX_SIZE       0x100000/*1M*/          /*debugfileµƒ◊Ó¥Ûsize*/
        #define WINKS_DBFILE_WRITE_BACK_SIZE 0x19000/*100K*/        /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
    #else
        #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/         /*debugfileµƒ◊Ó¥Ûsize*/
        #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/           /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
    #endif
       
    //#define WINKS_DEBUG
    //#define WINKS_CHECK_ASSERTIONS
    //#define WINKS_DEMO_VERSION
    //#define WINKS_SHOW_LOG 
    #define WINKS_SYS_TICK                1                         /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝*/
    
    #define WINKS_BUILD_VERSION         "2.0"                       /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AV                    "1.1.0"      //"1.1.2000" //  /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
    #define WINKS_AN                    "w-ip-gen-320x480" /*temporary name*//*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
    #define WINKS_CL                    "320x480"
    #define WINKS_CS                    "320x480"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á 320x270 */
    #define WINKS_MT                    "gif,png,jpeg"
    
    #define WINKS_OS                    "iPhone-2.x"                /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */

    #define WINKS_AN_EXT                ".deb"                      /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ ¿©’π√˚*/

    #define WINKS_ROOT_PATH             "/var/mobile/"
    #define WINKS_APP_PATH              "/var/mobile/winks/"
    #define WINKS_IMG_PATH              WINKS_APP_PATH"image/"

    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN 32
    
    
    #define WINKS_DB_FILE           WINKS_APP_PATH"debugf.txt"
    #define _SIZE_T_DEFINED
	#define WINKS_SUPPORT_3G 
    #include <ctype.h>
    #include <stdarg.h>

    
#elif defined (WINKS_WM_PPC)
#ifdef WINKS_SIMULATOR
#undef WINKS_SIMULATOR
#endif
    //#define WINKS_DYNAMIC_UPDATE                                    /*∂ØÃ¨∞≤◊∞µƒ»Ìº˛*/
    //#define WINKS_CC
    //#define WINKS_SYMBIAN 
    //#define WINKS_TEST_VERSION 
    //#define __240X320__
    //#define __320X240__
    //#define __176X220__
    //#define WINKS_DEBUG 
    //#define WINKS_UNSUPPORTED_GLOBAL_VAR 
    //#define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__ 
    //#define WINKS_SUPPORT_MEMORY_CARD

    //#define WINKS_SUPPORT_3G                                        /*3G:¿¥µÁ¡™Õ¯π¶ƒ‹°£*/
    //#define NOT_MULTI_OPEN_CLOSE_CC                                 /*≤ œÒ’π æ≤ª–Ë“™∂‡¥Œ¥Úø™∫Õπÿ±’∫Ï∞Â*/
    //#define WINKS_APN_WITH_PROXY_IN_PLATFORM                        /*∆ΩÃ®÷–ø…“‘∂¡»°ipµÿ÷∑∫Õ∂Àø⁄*/
    //#define WINKS_USE_PLATFORM_SOURCE_MANAGER                       /* π”√∆ΩÃ®◊‘º∫µƒÕº∆¨◊ ‘¥*/
	#define WINKS_SMARTPHONE_CCKEYFLAG								/* π”√∏√∫Í£¨¥˙±ÌœµÕ≥µÁª∞≥Ã–Úø…“‘ΩÿªÒΩ”Õ®π“∂œ∞¥º¸œ˚œ¢*/
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/

	/*∫· ˙∆¡◊‘  ”¶ÃÌº”µƒΩ”ø⁄*/
int Winks_GetScreenWidth();											/*ªÒ»°∆¡ƒªøÌ∂»*/
int Winks_GetScreenHeight();										/*ªÒ»°∆¡ƒª∏ﬂ∂»*/
int Winks_GetCCScreenWidth();										/*ªÒ»°≤ œÒ’π æΩÁ√Ê∆¡ƒªøÌ∂»*/
int Winks_GetCCScreenHeight();										/*ªÒ»°≤ œÒ’π æΩÁ√Ê∆¡ƒª∏ﬂ∂»*/
char* Winks_GetCLSize();											/*ªÒ»°≤ œÒ≥ﬂ¥Á*/
char* Winks_GetCSSize();                                            /*ªÒ»°≤ œÒ’π æª≠≤º≥ﬂ¥Á*/
char* Winks_GetImagePath();											/*ªÒ»°Õº∆¨◊ ‘¥¬∑æ∂*/
char* Winks_GetAN();												/*ªÒ»°∆¡ƒªAN÷µ*/
int Winks_GetDMFlag();												/* ÷ª˙÷˜≤Ÿ◊˜ƒ£ Ωœ¬£¨∫· ˙∆¡±Í÷æ£¨0Œ™ ˙∆¡£¨1Œ™∫·∆¡*/

#if defined(__240X320__) || defined(__480X800__) || defined(__360X640__) || defined(__480X640__) || defined(__240X400__) || defined(__240X377__)
	#define WINKS_CCSCREEN_WIDTH        Winks_GetCCScreenWidth()    /*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       Winks_GetCCScreenHeight()   /*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH          Winks_GetScreenWidth()      /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT         Winks_GetScreenHeight()     /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_CL                    Winks_GetCLSize()           /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    Winks_GetCSSize()           /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
	#define WINKS_VHFLAG				Winks_GetDMFlag()			/* ÷ª˙÷˜≤Ÿ◊˜ƒ£ Ωœ¬£¨∫· ˙∆¡±Í÷æ£¨0Œ™ ˙∆¡£¨1Œ™∫·∆¡*/
#elif defined(__176X205__)
	#define WINKS_CCSCREEN_WIDTH        176							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       205							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH          176                         /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT         205                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "240x320"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
	#define WINKS_VHFLAG				0
#endif // #ifdef __240X320__
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AN_EXT                ".cab"                      /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ ¿©’π√˚*/
    
    /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
	#define WINKS_AN					Winks_GetAN()

    extern char* Winks_getversion(int flag);
    #define WINKS_AV                    Winks_getversion(1)         /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
    #define WINKS_OS                    "winmobile"                 /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
    #define WINKS_HW                    ""                          /*hardware-id ÷’∂À”≤º˛±Í ∂ */
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
    extern char *WM_GetADC();
    #define WINKS_ADC                   WM_GetADC()                 /*dist-channel ∞≤◊∞«˛µ¿±‡¬Î,ø…”√∫Ø ˝ªÒ»°*/
int WM_GetAvtiveFlag(); /*∞≤◊∞«˛µ¿∫≈µ⁄“ªŒªY/N◊¥Ã¨£¨Y∑µªÿ1£¨N∑µªÿ0£¨ƒ¨»œ∑µªÿ1*/
    #define WINKS_AF                    WM_GetAvtiveFlag()        ///*∞≤◊∞«˛µ¿∫≈µ⁄“ªŒªY/N◊¥Ã¨£¨Y∑µªÿ1£¨N∑µªÿ0£¨ƒ¨»œ∑µªÿ1*/
char *WM_GetPhoneType();
    #define WINKS_BUA                   WM_GetPhoneType()           /*base64-user-agent:client default browser user-agent string*/
    #define WINKS_ROOT_PATH             "\\"
    //#define WINKS_ROOT_PATH             "c:\\data\\others\\"
    #define WINKS_APP_PATH              WINKS_ROOT_PATH"Program Files\\Winks\\"
    #define WINKS_IMG_PATH              Winks_GetImagePath()//WINKS_APP_PATH"image\\"
    #define WINKS_PATH_SPACE            "\\"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
    #define WINKS_SYS_TICK              1                           /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝*/
    

#elif defined(WINKS_SPTRUM_MOCOR)
  #define WINKS_MULTITHREADING 
#ifdef WINKS_LIST_VERSION
    #define WINKS_LIST
    //#define WINKS_USE_PLATFORM_SOURCE_MANAGER
    #define WINKS_SMALL_DEF_CC                                      /* π”√–°µƒƒ¨»œ≤ œÒ*/
#else
    #define WINKS_USE_PLATFORM_SOURCE_MANAGER                 //full ui version use sptrum platform resource manager
#endif
    //#define WINKS_SUPPORT_3G                                        /*3G:¿¥µÁ¡™Õ¯π¶ƒ‹°£*/
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__                 /*÷˜≤Àµ•œ‘ æ∑Á∏Òµƒ∫Í£¨*/
    #define __240X320__
    #define WINKS_TOUCH_PANNEL                                      /*¥•√˛∆¡*/
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
	#define WINKS_CCSCREEN_WIDTH        240							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       320							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH          240                         /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT         320                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AN                    "w-st-amoi-a616"            /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
    #define WINKS_AV                    Winks_getversion(1)         /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
//    #define WINKS_OS                    "SPTRUM"                    /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
//    #define WINKS_HW                    "SPTRUM"                    /*hardware-id ÷’∂À”≤º˛±Í ∂ */
    #define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "240x320"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
//    #define WINKS_ADC                   "0"                         /*dist-channel ∞≤◊∞«˛µ¿±‡¬Î,ø…”√∫Ø ˝ªÒ»°*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
    #ifdef WIN32
    //#define __BIG_ENDIAN
    //#define  WINKS_LITTLEENDIAN
    #define WINKS_ROOT_PATH             "D:\\"
    #else
        #ifndef __BIG_ENDIAN
            #define __BIG_ENDIAN
        #endif  
    #define WINKS_ROOT_PATH             "D:\\"
    #endif
    #define WINKS_APP_PATH              WINKS_ROOT_PATH"winks\\"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks\\image\\"
    #define WINKS_PATH_SPACE            "\\"
    #define WINKS_OSAL_API_SELF_DATA_LEN     40
    #define WINKS_SYS_TICK              1                           /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝.(1sys tick = 10ms)*/
    //#define WINKS_DEMO_VERSION
    //#define NOT_MULTI_OPEN_CLOSE_CC
    //#define WINKS_START_SHOWCC_ASYNC
    #define WINKS_DUAL_SIM
#else
    //#define WINKS_MULTITHREADING                                  /*∂‡œﬂ≥Ã*/
    #define WINKS_SUPPORT_3G                                        /*3G:¿¥µÁ¡™Õ¯π¶ƒ‹°£*/
    #define WINKS_UNSUPPORTED_GLOBAL_VAR
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfileµƒ◊Ó¥Ûsize*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfileπ˝¥Û ±ªÿ–¥≥ﬂ¥Á*/
	#define WINKS_CCSCREEN_WIDTH        240							/*≤ œÒ’π æΩÁ√Ê∆¡øÌ£¨∑÷±Ê¬ */
	#define WINKS_CCSCREEN_HEIGHT       320							/*≤ œÒ’π æΩÁ√Ê∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_WIDTH          240                         /*∆¡øÌ£¨∑÷±Ê¬ */
    #define WINKS_SCREEN_HEIGHT         320                         /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*±‡“Î∞Ê±æ∫≈*/
    #define WINKS_AN                    "winks-SPTRUM"              /*package-name ÷’∂À»Ìº˛∞¸√˚≥∆ */
    #define WINKS_AV                    Winks_getversion(1)         /*version ÷’∂À»Ìº˛∞¸∞Ê±æ */
//    #define WINKS_OS                    "SPTRUM"                    /*client-os ÷’∂À»Ìº˛≤Ÿ◊˜œµÕ≥–≈œ¢ */
//    #define WINKS_HW                    "SPTRUM"                    /*hardware-id ÷’∂À”≤º˛±Í ∂ */
    #define WINKS_CL                    "240x236"                   /*canvasList ≤ œÒ√ΩÃÂ’π æ≥ﬂ¥Á¡–±Ì*/
    #define WINKS_CS                    "240x320"                   /* canvasSize ’π æ«¯”Ú(ª≠≤º)≥ﬂ¥Á */
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes √ΩÃÂ∏Ò Ω¡–±Ì*/
//    #define WINKS_ADC                   "0"                         /*dist-channel ∞≤◊∞«˛µ¿±‡¬Î,ø…”√∫Ø ˝ªÒ»°*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
    #define WINKS_ROOT_PATH             "/flash/app2/user/"
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks/"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks\\image\\"
    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
    #define WINKS_SYS_TICK              1                           /*∆ΩÃ®1Tick∂‘”¶µƒ∫¡√Î ˝*/
#endif
#define WINKS_USRAGENT              "UA-winks1.0"                   /*USER AGENT for HTTP.*/


#if (!defined(WINKS_AN) || !defined(WINKS_AV) || !defined(WINKS_BUILD_VERSION))
#error "You must define WINKS_AN, WINKS_AV and WINKS_BUILD_VERSION."
#endif

#ifdef WINKS_DYNAMIC_UPDATE
#if !defined(WINKS_ADC)
#error "Dynamic setup must define WINKS_ADC."
#endif
#if ( !defined(WINKS_BUA) && (!defined(WINKS_OS) || !defined(WINKS_HW)) )
#error "Dynamic setup must define WINKS_BUA or (WINKS_OS and WINKS_HW)."
#endif
#endif


#ifndef WINKS_DYNAMIC_LOAD_BLOCK
#ifdef WINKS_MTK
    /*SCREEN*/
    #if defined(__MMI_MAINLCD_176X220__)
        #define __176X220__
        #define WINKS_SCREEN_WIDTH_int         176                  /*∆¡øÌ£¨∑÷±Ê¬ */
        #define WINKS_SCREEN_HEIGHT_int        220                  /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #elif defined(__MMI_MAINLCD_240X320__)
        #define __240X320__
        #define WINKS_SCREEN_WIDTH_int         240                  /*∆¡øÌ£¨∑÷±Ê¬ */
        #define WINKS_SCREEN_HEIGHT_int        320                  /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #elif defined(__MMI_MAINLCD_320X240__)
        #define __320X240__
        #define WINKS_SCREEN_WIDTH_int         320                  /*∆¡øÌ£¨∑÷±Ê¬ */
        #define WINKS_SCREEN_HEIGHT_int        240                  /*∆¡∏ﬂ£¨∑÷±Ê¬ */
    #else
        #error "Winks NOT support the current resolution"
    #endif
#endif
#else/*WINKS_DYNAMIC_LOAD_BLOCK*/
        #define __240X320__
        #define WINKS_SCREEN_WIDTH_int         240                  /*∆¡øÌ£¨∑÷±Ê¬ */
        #define WINKS_SCREEN_HEIGHT_int        320                  /*∆¡∏ﬂ£¨∑÷±Ê¬ */
#endif/*WINKS_DYNAMIC_LOAD_BLOCK*/


//wq added for brew version 08-12-16
#ifdef WINKS_BREW
#define KDDI_BREW_401  4010
#define CN_BREW_315    3151
#endif
//wq added for brew version end


/*APNµƒ…Ë÷√*/
#if defined(WINKS_SIMULATOR)
    #define WINKS_PROXY_SERVER_IP "0.0.0.0"
    #define WINKS_PROXY_SERVER_PORT 0
    #define WINKS_DEFAULT_ACCESS_POINT 0
    #define WINKS_DEFAULT_ACCESS ""
#else /* ’Êª˙ */
  #if defined(WINKS_MTK)
    #define WINKS_DEFAULT_ACCESS_POINT 14
    #define WINKS_DEFAULT_ACCESS "cmwap"
    #define WINKS_PROXY_SERVER_IP "10.0.0.172"
    #define WINKS_PROXY_SERVER_PORT 80
  #elif defined(WINKS_ARENA)
    #define WINKS_DEFAULT_ACCESS_POINT 1
    #define WINKS_DEFAULT_ACCESS "cmwap"
    #define WINKS_PROXY_SERVER_IP "10.0.0.172"
    #define WINKS_PROXY_SERVER_PORT 80
  #else /*WINKS_SPTRUM_MOCOR|WINKS_SYMBIAN_V_9_0|WINKS_WM_PPC|other*/ 
    #define WINKS_DEFAULT_ACCESS_POINT 0
    #define WINKS_DEFAULT_ACCESS "cmwap"
    #define WINKS_PROXY_SERVER_IP "10.0.0.172"
    #define WINKS_PROXY_SERVER_PORT 80
  #endif
#endif /* WINKS_SIMULATOR */

/* …Ë÷√–Ë“™Õ¨≤Ωµ˜”√µƒœ˚œ¢ */
#ifndef WK_SYN_VAL 
//#define WK_SYN_MSG_KTY 0x1
//#define WK_SYN_MSG_PEN 0x2
//#define WK_SYN_MSG_CREATE_APP 0x4
//#define WK_SYN_MSG_PAUSE_RESUME_APP 0x8
//#define WK_SYN_MSG_DESTROY_APP 0x10
//#define WK_SYN_MSG_PAINT 0x20
#define WK_SYN_VAL 0x35 /*WK_SYN_MSG_KTY|WK_SYN_MSG_CREATE_APP|WK_SYN_MSG_DESTROY_APP|WK_SYN_MSG_PAINT*/
#endif







/*≈‰÷√∑˛ŒÒ∆˜µƒ…Ë÷√*/
#if 0
/* v1.0 sp1∆ΩÃ®ª∑æ≥ */
#define WINKS_URL_DEMO "http://ip.winksi.com:8100/winksWS"
#define WINKS_URL_TEST "http://ip.winksi.cn:8100/winksWS"
#define WINKS_URL_DEVICE "http://ip91.winksi.com:18081/winksWS"
#else
/* v1.0 sp3∆ΩÃ®ª∑æ≥ */
#define WINKS_URL_DEMO "http://ipng.winksi.com:8100/winksWS"
#define WINKS_URL_TEST "http://ipng.winksi.cn:8100/winksWS"
#define WINKS_URL_DEVICE "http://ip91.winksi.com:18081/winksWS"
#endif

#include "wk_srvurl.h"
#ifndef WINKS_HOST_SERVER_URL
    #ifdef WINKS_DEMO_VERSION
        #define WINKS_HOST_SERVER_URL WINKS_URL_DEMO
    #elif defined (WINKS_TEST_VERSION)
        #define WINKS_HOST_SERVER_URL WINKS_URL_TEST
    #else
        #define WINKS_HOST_SERVER_URL WINKS_URL_DEVICE
    #endif
#endif

/*Õº∆¨◊ ‘¥CACHE size*/
#ifdef WINKS_SIMULATOR
#define WINK_IMG_CACHELEN    (300 * 1024)
#else
#define WINK_IMG_CACHELEN    (50 * 1024)
#endif

#define WINKS_FILE_DFPATH   ((char* )"root")                /*Œƒº˛π‹¿Ì∆˜ƒ¨»œµƒ∆ º¬∑æ∂*/
#define WINKS_MSG_UNION                                     /*÷ß≥÷œ˚œ¢∫œ≤¢*/
#if ((defined(WINKS_DEBUG)) || (defined(WINKS_SIMULATOR)))
    #define WINKS_PRINT_BASE_LEVEL WINKS_PRINT_LEVEL_INFO   /*∂®“Â¥Ú”°ª˘◊ºº∂±*/
#elif (defined(WINKS_SHOW_LOG))
    #define WINKS_PRINT_BASE_LEVEL WINKS_PRINT_LEVEL_WARN   /*∂®“Â¥Ú”°ª˘◊ºº∂±*/
#else 
    #define WINKS_PRINT_BASE_LEVEL WINKS_PRINT_LEVEL_NONE   /*∂®“Â¥Ú”°ª˘◊ºº∂±*/
#endif

/*≤ œÒ»Ìº˛∞Ê±æ∫≈£¨”√”⁄»Ìº˛…˝º∂*/
#define WINKS_SW_VERSION 0x01000300                 /*1.0.3.0->1.0sp3∞Ê±æ*/
/*”¶”√¡¨Õ¯Œﬁ∑¥”¶ ±º‰≥¨ ±*/
#define WINKS_HTTP_TIME_LIMIT (120)
/*∂® ±¡¨Õ¯ ±º‰÷‹∆⁄µƒ ±º‰œﬁ÷∆*/
#define WINKS_TIME_CONNECT_PERIOD_DEF (43200)       /*“ªÃÏ∏¸–¬¡Ω¥Œ*/
#define WINKS_TIME_CONNECT_PERIOD_MAX (86400)       /*≤ªƒ‹≥¨π˝4294967£¨49»’∂‡£¨ªªÀ„≥…∫¡√Î∫Û“—æ≠ «2^32*/
#define WINKS_TIME_CONNECT_PERIOD_MIN (1000)        /*≤ªµΩ17∑÷÷”*/
/*∂® ±¡¨Õ¯≥¨ ± ±º‰œﬁ÷∆*/
#define WINKS_TIME_CONNECT_OVERTIME_DEF (120)         /*2∑÷÷”*/
#define WINKS_TIME_CONNECT_OVERTIME_MAX (600)        /*10∑÷÷”*/
#define WINKS_TIME_CONNECT_OVERTIME_MIN (30)          /*30√Î*/
/*PUSH¡¨Õ¯∑≈∆˙÷’∂À ±º‰œﬁ÷∆*/
//#define WINKS_PUSH_GIVE_UP_TIMER_DEF (8640000)       /*100ÃÏ*/
//#define WINKS_PUSH_GIVE_UP_TIMER_MAX (207360000)     /*100ƒÍ*/
//#define WINKS_PUSH_GIVE_UP_TIMER_MIN (600)           /*10∑÷÷”*/
/*∆¨Õ∑≤•∑≈ ±º‰œﬁ÷∆*/
//#define WINKS_SPLASH_SHOW_TIMER_DEF (5)         /*5"*/
//#define WINKS_SPLASH_SHOW_TIMER_MAX (30)        /*30"*/
#define WINKS_SPLASH_SHOW_TIMER_MIN (0)         /*not show*/
/*π“∂œ∫Û∏¸–¬—” ± ±≥§*/
#define WINKS_HUNGUP_UPDATE_DELAY_TIME (600)    /*π“∂œ∫Û∏¸–¬µƒ—” ±10∑÷÷”*/

/* ∑¢ÀÕ∂Ã–≈∂Ã÷‹∆⁄ ±º‰ Ù–‘ */
#define WINKS_SMS_SHORT_PRIEOD_DFT      20
#define WINKS_SMS_SHORT_PRIEOD_MIN      10
#define WINKS_SMS_SHORT_PRIEOD_MAX      60

/* ∑¢ÀÕ∂Ã–≈≥§÷‹∆⁄ ±º‰ Ù–‘ */
#define WINKS_SMS_LONG_PRIEOD_DFT       40
#define WINKS_SMS_LONG_PRIEOD_MIN       40
#define WINKS_SMS_LONG_PRIEOD_MAX       180

/* ”√ªß∫√”—¡–±Ì∫≈¬Î–≈œ¢ Ù–‘ */
#define WINKS_FRIENDS_MAX_NUMBER        20

/* ∑÷øÈœ¬¥´∫√”—–≈œ¢ Ù–‘-◊‹øÈ ˝ */
#define WINKS_FRIEND_INFO_BLOCK_DFT     1
#define WINKS_FRIEND_INFO_BLOCK_MIN     0
#define WINKS_FRIEND_INFO_BLOCK_MAX     WINKS_FRIENDS_MAX_NUMBER      /* FIXME: »Áπ˚∫√”—π˝∂‡ */
/* ÷˜Ω–≤ œÒ◊‹’≈ ˝ */
#define WINKS_OUT_CC_NUM_MIN            1
#define WINKS_OUT_CC_NUM_MAX            4
/* ±ªΩ–≤ œÒ◊‹’≈ ˝ */
#define WINKS_IN_CC_NUM_MIN             1
#define WINKS_IN_CC_NUM_MAX             4
/* ≤ œÒ≤•∑≈º‰∏Ù ±º‰ */
#define WINKS_CC_PLAY_INTERVAL_DFT      1
#define WINKS_CC_PLAY_INTERVAL_MIN      1
#define WINKS_CC_PLAY_INTERVAL_MAX      5
/* ≤ œÒ≤•∑≈º‰∏Ù ±º‰ */
#define WINKS_TAOCC_PLAY_TIME_DFT       3
#define WINKS_TAOCC_PLAY_TIME_MIN       1
#define WINKS_TAOCC_PLAY_TIME_MAX       5
/*  ±º‰¥¡µƒ◊Ó¥Û≥§∂»*/
#define WINKS_CONN_TIME_FLAG_MAX_LEN    19

/* Õ∆ºˆ◊Ó¥Û∏ˆ ˝*/
#define WINKS_RECOMMAND_MAX_NUM 5       /*Õ∆ºˆ◊Ó¥Û∏ˆ ˝*/
/*º¸≈Ã¿˙ ∑’ª≥§∂»*/
#ifdef WINKS_SHOW_LOG
#define WINKS_TEST_KEY_BASE_LEN 9           /* "*#100022#" */
#define WINKS_TEST_KEY_WORDS_LEN 4          /* "????" */
#define WINKS_TEST_KEY_HISTORY_STACK_LEN 14 /* "*#100022#????#" */
#define WINKS_TEST_KEY_STR "*#100022#" /* "*#100022#????#" */
#define WINKS_TEST_PEN_HISTORY_STACK_LEN 4  /*…œ…œ…œ…œ*/
#define WINKS_TEST_PEN_PRECISION 80  /*“∆∂Øæ´∂»“™¥Û”⁄80*/
#endif
#define WINKS_FLASHSPACE_RESERVED       61440   /* 61K (0xF000)*/
#define EACH_CC_TAKEUP_SIZE             30720   /* 30K = 30*1024,√ø∏ˆ≤ œÒ30K */
#define WINKS_INIT_NEED_FLASH_SIZE      (0x10000)   /*(64*1024) winks≥ı ºªØ–Ë“™µƒ◊Ó–°¥≈≈Ãø’º‰ */
#define WINKS_LARGE_FLASH_SIZE          (0x4000000) /*(64*1024*1024) winks»Ìº˛«¯∑÷¥Û–°»›¡ø¥≈≈Ã∑÷ΩÁ>64MŒ™¥Û»›¡ø */

#if defined(WINKS_HELP_APP) || defined(WINKS_BREW)
/*************************************************
∞Ô÷˙“≥√Ê√∂æŸ
**************************************************/
typedef enum _helptype_id
{
    WINKS_HELPTYPE_MAIN = 1,
    WINKS_HELPTYPE_OVERVIEW,
    WINKS_HELPTYPE_MYLIBRARY,
    WINKS_HELPTYPE_WINKSSTORE,
    WINKS_HELPTYPE_SETWINKS,
    WINKS_HELPTYPE_DIY,
    WINKS_HELPTYPE_SHARE,
    WINKS_HELPTYPE_NORMALSETTING,
    WINKS_HELPTYPE_SHOWSETTING,
    WINKS_HELPTYPE_ADVANCESETTING,
    WINKS_HELPTYPE_UPDATESETTING,
    WINKS_HELPTYPE_ADDFRIENDS,
    WINKS_HELPTYPE_SELECTAPN
}helptype_id;
#endif //#if defined(WINKS_HELP_APP) || defined(WINKS_BREW)

#ifdef __cplusplus
}
#endif

#endif/*_WK_PROJECT_H_*/
