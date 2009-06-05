/**************************************************************************
* Winks Software Copyright (c) 2008 Winks Technologies, Inc.
* Licensed to Mobile Winks (China) Co., Ltd
**************************************************************************
*Id: wk_project.h,v 1.00 2008/06/03 03:05:49
*Revision: 1.00
*DateTime: 2008/06/03 11:11:37
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
#define WINKS_MV "1.0"
/*支持消息合并*/
#define WINKS_MSG_UNION

//WINCE 不需要_WINKS_EMULATOR_，自行undef WINKS_SIMULATOR
#ifdef _MSC_VER
#define WINKS_SIMULATOR
#endif

#ifdef __WINSCW__
#define WINKS_SIMULATOR
#endif
    
#ifdef WINKS_SKYWORTH_080601/*产品宏开关*/
    #define WINKS_MTK
    #define WINKS_MTK_V_0_6_b                                       /*软件平台版本宏开关*/
    /*#define WINKS_ENGINE_TEST_5PROTOCAL*/                         /*服务器模拟下发协议指令*/
    #define WINKS_SUPPORT_MEMORY_CARD
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
    /*SCREEN*/
    #define __176X220__
	#define WINKS_KEY_TEXT_DOWN
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__    1
    #define __WINKS_MULTI_LAYS__
    #define __WINKS_STRINGBORDER__
    #define WINKS_SCREEN_WIDTH          176                         /*屏宽，分辨率*/
    #define WINKS_SCREEN_HEIGHT         220                         /*屏高，分辨率*/
    #define WINKS_CC_WIDTH              176                         /*彩像宽，分辨率*/
    #define WINKS_CC_HEIGHT             140                         /*彩像高，分辨率*/
    #define WINKS_TOPBAR_HEIGHT         20                          /*定义topbar高度*/
    #define WINKS_BOTTOMBAR_HEIGHT      20                          /*定义borrombar高度*/
    #define WINKS_BUILD_VERSION         "1"                         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-MTK"                 /*package-name 终端软件包名称 */
    #define WINKS_AV                    WINKS_MV"."WINKS_BUILD_VERSION   /*version 终端软件包版本 */
//    #define WINKS_OS                    "MTK06b"                    /*client-os 终端软件操作系统信息 */
//    #define WINKS_HW                    "6225"                      /*hardware-id 终端硬件标识 */
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
//    #define WINKS_ADC                   "0"                        /*dist-channel 渠道编码*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/

    #define WINKS_ROOT_PATH             "C:\\"
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks\\"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks\\image\\"
    #define WINKS_PATH_SPACE            "\\"
  #ifdef WINKS_SIMULATOR//vc调试时加了一些文件系统调试
    #define WINKS_OSAL_API_SELF_DATA_LEN     32+128
  #else
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
  #endif
    #define WINKS_SYS_TICK              1                           /*平台1Tick对应的毫秒数*/

#elif defined (WINKS_TIANYU_080601) || defined(WINKS_XMW_081016)    /*产品宏开关*/
    #define WINKS_MTK                                               /*平台版本宏开关*/
    #ifdef WINKS_TIANYU_080601                                      /*平台版本宏开关*/
    #define WINKS_MTK_V_0_6_a 
    #else
    #define WINKS_MTK_V_0_7_b    
    #define WINKS_DUAL_SIM
    #endif
    /*#define WINKS_ENGINE_TEST_5PROTOCAL*/
    #define WINKS_SUPPORT_MEMORY_CARD
    /*服务器模拟下发协议指令*/
    #define WINKS_TOUCH_PANNEL
    #define   __240X320__
    #define WINKS_KEY_TEXT_DOWN
    #ifdef WINKS_TIANYU_080601
        #define   WINKS_GIF_IMAGE
    #endif
    #ifdef WINKS_XMW_081016
       // #define  WINKS_GIF_SMALL_ICON_IMAGE
    #endif

    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__    1
    #define __WINKS_MULTI_LAYS__
    #define __WINKS_STRINGBORDER__
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
    #define WINKS_SCREEN_WIDTH          240                         /*屏宽，分辨率*/
    #define WINKS_SCREEN_HEIGHT         320                         /*屏高，分辨率*/
    #define WINKS_CC_WIDTH              240                         /*彩像宽，分辨率*/
    #define WINKS_CC_HEIGHT             236                         /*彩像高，分辨率*/
    #define WINKS_TOPBAR_HEIGHT         20                          /*定义topbar高度*/
    #define WINKS_BOTTOMBAR_HEIGHT      20                          /*定义borrombar高度*/
char* Winks_getversion(int flag);
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-MTK"                 /*package-name 终端软件包名称 */
    #define WINKS_AV                    Winks_getversion(1)         /*version 终端软件包版本 */
#ifdef WINKS_TIANYU_080601
//    #define WINKS_OS                    "MTK06a"                    /*client-os 终端软件操作系统信息 */
//    #define WINKS_HW                    "6228"                      /*hardware-id 终端硬件标识 */
#else
//    #define WINKS_OS                    "MTK07b"                    /*client-os 终端软件操作系统信息 */
//    #define WINKS_HW                    "6225"                      /*hardware-id 终端硬件标识 */
#endif
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
//    #define WINKS_ADC                   "0"                        /*dist-channel 渠道编码*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/

    #define WINKS_ROOT_PATH            "C:\\"
    #define WINKS_APP_PATH               WINKS_ROOT_PATH"winks\\"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks\\image\\"
    #define WINKS_PATH_SPACE           "\\"
  #ifdef WINKS_SIMULATOR//vc调试时加了一些文件系统调试
    #define WINKS_OSAL_API_SELF_DATA_LEN     32+128
  #else
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
  #endif
    #define WINKS_SYS_TICK              1                           /*平台1Tick对应的毫秒数*/

#elif defined (WINKS_BREW_S319)
    #define WINKS_USE_PLATFORM_SOURCE_MANAGER                       /*资源采用平台的实现方式方式*/
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
    #define WINKS_SCREEN_WIDTH          240                         /*屏宽，分辨率*/
    #define WINKS_SCREEN_HEIGHT         320                         /*屏高，分辨率*/
    #define WINKS_CC_WIDTH              240                         /*彩像宽，分辨率*/
    #define WINKS_CC_HEIGHT             236                         /*彩像高，分辨率*/
    #define WINKS_TOPBAR_HEIGHT         20                          /*定义topbar高度*/
    #define WINKS_BOTTOMBAR_HEIGHT      20                          /*定义borrombar高度*/
    #define WINKS_BUILD_VERSION         "1"                         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-BREW-S319"           /*package-name 终端软件包名称 */
    #define WINKS_AV                    WINKS_MV"."WINKS_BUILD_VERSION   /*version 终端软件包版本 */
//    #define WINKS_OS                    "BREW-S319"                    /*client-os 终端软件操作系统信息 */
//    #define WINKS_HW                    ""                      /*hardware-id 终端硬件标识 */
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
//    #define WINKS_ADC                   "0"                        /*dist-channel 渠道编码*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/

    #define WINKS_ROOT_PATH             AEE_SHARED_DIR"/"              /*brew 多个应用只能通过访问Shared文件夹实现共享*/
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks/"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks/image/"
    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
    #define __176X205__
    #define WINKS_UNSUPPORT_PROXY
    #define  WINKS_TEST_VERSION
    #define  WINKS_BREW_BAR  "winks_brew_s319.bar"
    #define WINKS_SYS_TICK              1                           /*平台1Tick对应的毫秒数*/
	#define WINKS_START_SHOWCC_ASYNC

#elif defined (WINKS_BREW_KDDI_W63SA)
    #define BREWVER  KDDI_BREW_401
    #define WINKS_USE_PLATFORM_SOURCE_MANAGER                       /*资源采用平台的实现方式方式*/
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
    #define WINKS_SCREEN_WIDTH          240                         /*屏宽，分辨率*/
    #define WINKS_SCREEN_HEIGHT         320                         /*屏高，分辨率*/
    #define WINKS_CC_WIDTH              240                         /*彩像宽，分辨率*/
    #define WINKS_CC_HEIGHT             236                         /*彩像高，分辨率*/
    #define WINKS_TOPBAR_HEIGHT         20                          /*定义topbar高度*/
    #define WINKS_BOTTOMBAR_HEIGHT      20                          /*定义borrombar高度*/
    #define WINKS_BUILD_VERSION         "1"                         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-BREW"                /*package-name 终端软件包名称 */
    #define WINKS_AV                    WINKS_MV"."WINKS_BUILD_VERSION   /*version 终端软件包版本 */
//    #define WINKS_OS                    "BREW-KDDI"               /*client-os 终端软件操作系统信息 */
//    #define WINKS_HW                    "W63SA"                   /*hardware-id 终端硬件标识 */
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
//    #define WINKS_ADC                   "0"                        /*dist-channel 渠道编码*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
    #define WINKS_ROOT_PATH             AEE_SHARED_DIR"/"              /*brew 多个应用只能通过访问Shared文件夹实现共享*/
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks/"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks/image/"
    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
    #define WINKS_UNSUPPORT_PROXY
    #define WINKS_TEST_VERSION
    #define    WINKS_LEFTSOFTKEY_BUTTON_X    0
    #define    __240X320__
    #define   __240X296__
    #define WINKS_BREW_BAR  "winks_brew_kddi_w63sa.bar"
    #define WINKS_SYS_TICK              1                           /*平台1Tick对应的毫秒数*/
	#define WINKS_START_SHOWCC_ASYNC
    
#elif defined (WINKS_ARENA_V_1_2) || defined(WINKS_ARENA_V_1_4) 
    char* Winks_getversion(int flag);
    #define WINKS_ARENA
    #define WINKS_DYNAMIC_UPDATE                                    /*动态安装的软件*/
    #define WINKS_MULTITHREADING                                    /*多线程*/
    #define WINKS_UNSUPPORTED_GLOBAL_VAR
    #define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__                 /*主菜单显示风格的宏，*/
    #define NOT_MULTI_OPEN_CLOSE_CC
    #define __240X320__
    #define WINKS_TOUCH_PANNEL                                      /*触摸屏*/
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
    #define WINKS_SCREEN_WIDTH          240                         /*屏宽，分辨率*/
    #define WINKS_SCREEN_HEIGHT         320                         /*屏高，分辨率*/
    #define WINKS_CC_WIDTH              240                         /*彩像宽，分辨率*/
    #define WINKS_CC_HEIGHT             236                         /*彩像高，分辨率*/
    #define WINKS_TOPBAR_HEIGHT         20                          /*定义topbar高度*/
    #define WINKS_BOTTOMBAR_HEIGHT      20                          /*定义borrombar高度*/
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-arena"                /*package-name 终端软件包名称 */
    #define WINKS_AV                    Winks_getversion(1)         /*version 终端软件包版本 */
    #define WINKS_OS                    "ARENA-1-4"                 /*client-os 终端软件操作系统信息 */
    #define WINKS_HW                    "ARENA"                     /*hardware-id 终端硬件标识 */
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
    #define WINKS_ADC                   "0"                        /*dist-channel 渠道编码*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
    #define WINKS_ROOT_PATH             "/user/user/"
    #define WINKS_APP_PATH              WINKS_ROOT_PATH"winks/"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks/image/"
    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN     128
    #define WINKS_SYS_TICK              10                          /*平台1Tick对应的毫秒数.(1sys tick = 10ms)*/
    
#elif defined (WINKS_SYMBIAN_V_9_0)
    //以下注释掉的宏都放在symbian的mmp文件中定义了
    //#define WINKS_DYNAMIC_UPDATE                                    /*动态安装的软件*/
    //#define WINKS_CC                                              /*支持彩像展示*/
    //#define WINKS_TEST_VERSION                                    /*测试版本*/
    //#define WINKS_DEBUG                                           /*输出调试信息*/
    //#define WINKS_UNSUPPORTED_GLOBAL_VAR                          /*平台不支持全局变量*/
    //#define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__               /**/
    //#define WINKS_SUPPORT_MEMORY_CARD                             /*支持存储卡*/
    //#define NOT_MULTI_OPEN_CLOSE_CC                                /*彩像展示不需要多次打开和关闭红板*/
    //#define WINKS_APN_WITH_PROXY_IN_PLATFORM                      /*平台中可以读取ip地址和端口*/
    //#define WINKS_USE_PLATFORM_SOURCE_MANAGER                     /*使用平台自己的图片资源*/
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
    #define WINKS_SCREEN_WIDTH          winks_screen_width()        /*屏宽，分辨率*/
    #define WINKS_SCREEN_HEIGHT         winks_screen_height()       /*屏高，分辨率*/
//    #define WINKS_CC_WIDTH              240                         /*彩像宽，分辨率*/
//    #define WINKS_CC_HEIGHT             236                         /*彩像高，分辨率*/
//    #define WINKS_SOFTWARE_VERSION      "Symbian S60 V2.0"              /*软件版本*/
//    #define WINKS_HARDWARE_VERSION      "Symbian os 9.0 XXX"                 /*硬件版本*/
//    #define WINKS_PHONE_ID              (0x000D0001UL)              /*手机型号代码*/
//    #define WINKS_SODRWARE_TYPE         (3)                         /*软件类型 1:Smart; 2:Java; 3:Feature. */
//    #define WINKS_SUPPORTED_PB_INFO     (0x00004000UL)              /*支持名片的控制项*/
    //#define WINKS_ROOT_PATH             "c:\\system\\apps\\"
    #define WINKS_ROOT_PATH             "c:\\data\\others\\"
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks\\"
    #define WINKS_IMG_PATH                WINKS_ROOT_PATH"winks\\image\\"
    #define WINKS_PARTNER_LEN             16
    #define WINKS_PARTNER_PATH            WINKS_ROOT_PATH"winks\\partner.txt"
    #define WINKS_PATH_SPACE            "\\"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
    #define WINKS_SYS_TICK              1                           /*平台1Tick对应的毫秒数*/

char* Winks_getversion(int flag);
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-s60v3"             /*package-name 终端软件包名称 */
    #define WINKS_AV                    "1.1.2000" //Winks_getversion(1)     //    /*version 终端软件包版本 */
    #define WINKS_OS                    "Symbian-9x"                  /*client-os 终端软件操作系统信息 */
char *Symbian_GetPhoneType();
    #define WINKS_HW                    Symbian_GetPhoneType()                   /*hardware-id 终端硬件标识 */
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
char *Symbian_GetADC();
    #define WINKS_ADC                   "1" //Symbian_GetADC()      //                  /*dist-channel 安装渠道编码,可用函数获取*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/

#elif defined (WINKS_WM_PPC)
#ifdef WINKS_SIMULATOR
#undef WINKS_SIMULATOR
#endif
    #define WINKS_DYNAMIC_UPDATE                                    /*动态安装的软件*/
    //#define WINKS_CC
    //#define WINKS_SYMBIAN 
    //#define WINKS_TEST_VERSION 
    #define __240X320__ 
    //#define __176X220__
    //#define WINKS_DEBUG 
    //#define WINKS_UNSUPPORTED_GLOBAL_VAR 
    //#define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__ 
    //#define WINKS_SUPPORT_MEMORY_CARD

    //#define NOT_MULTI_OPEN_CLOSE_CC                                  /*彩像展示不需要多次打开和关闭红板*/
	//#define WINKS_APN_WITH_PROXY_IN_PLATFORM                        /*平台中可以读取ip地址和端口*/
    //#define WINKS_USE_PLATFORM_SOURCE_MANAGER                       /*使用平台自己的图片资源*/
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
    #define WINKS_SCREEN_WIDTH          240                         /*屏宽，分辨率*/
    #define WINKS_SCREEN_HEIGHT         320                         /*屏高，分辨率*/
    #define WINKS_CC_WIDTH              240                         /*彩像宽，分辨率*/
    #define WINKS_CC_HEIGHT             236                         /*彩像高，分辨率*/
    #define WINKS_TOPBAR_HEIGHT         20                          /*定义topbar高度*/
    #define WINKS_BOTTOMBAR_HEIGHT      20                          /*定义borrombar高度*/
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-WMPPC"                /*package-name 终端软件包名称 */
    #define WINKS_AV                    Winks_getversion(1)         /*version 终端软件包版本 */
    #define WINKS_OS                    "winmobile"                  /*client-os 终端软件操作系统信息 */
    #define WINKS_HW                    "winmobile"                   /*hardware-id 终端硬件标识 */
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
    #define WINKS_ADC                   "0"                        /*dist-channel 安装渠道编码,可用函数获取*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
    #define WINKS_ROOT_PATH             "\\"
    //#define WINKS_ROOT_PATH             "c:\\data\\others\\"
    #define WINKS_APP_PATH              WINKS_ROOT_PATH"Program Files\\Winks\\"
    #define WINKS_IMG_PATH              WINKS_APP_PATH"image\\"
    #define WINKS_PATH_SPACE            "\\"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
    #define WINKS_SYS_TICK              1                           /*平台1Tick对应的毫秒数*/
    

#elif defined(WINKS_SPTRUM_MOCOR)
  #define WINKS_MULTITHREADING 
#ifdef WINKS_LIST_VERSION
    //#define WINKS_USE_PLATFORM_SOURCE_MANAGER
	#define WINKS_DESTROY_MSG
#endif
	#define __WINKS_UI_MATRIXMENU_WITH_BACKGROUND__                 /*主菜单显示风格的宏，*/
	#define __240X320__
	#define WINKS_TOUCH_PANNEL                                      /*触摸屏*/
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
	#define WINKS_SCREEN_WIDTH          240                         /*屏宽，分辨率*/
	#define WINKS_SCREEN_HEIGHT         320                         /*屏高，分辨率*/
	#define WINKS_CC_WIDTH              240                         /*彩像宽，分辨率*/
	#define WINKS_CC_HEIGHT             236                         /*彩像高，分辨率*/
	#define WINKS_TOPBAR_HEIGHT         20                          /*定义topbar高度*/
	#define WINKS_BOTTOMBAR_HEIGHT      20                          /*定义borrombar高度*/
    #define WINKS_BUILD_VERSION         Winks_getversion(0)         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-SPTRUM"              /*package-name 终端软件包名称 */
    #define WINKS_AV                    Winks_getversion(1)         /*version 终端软件包版本 */
//    #define WINKS_OS                    "SPTRUM"                    /*client-os 终端软件操作系统信息 */
//    #define WINKS_HW                    "SPTRUM"                    /*hardware-id 终端硬件标识 */
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
//    #define WINKS_ADC                   "0"                        /*dist-channel 安装渠道编码,可用函数获取*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
	#ifdef WIN32
	//#define __BIG_ENDIAN
	#define  WINKS_LITTLEENDIAN
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
	#define WINKS_SYS_TICK              1                          /*平台1Tick对应的毫秒数.(1sys tick = 10ms)*/
    #define WINKS_DEMO_VERSION
	//#define NOT_MULTI_OPEN_CLOSE_CC
	#define WINKS_START_SHOWCC_ASYNC
	#define WINKS_DUAL_SIM
#else
    //#define WINKS_MULTITHREADING                                  /*多线程*/
    #define WINKS_UNSUPPORTED_GLOBAL_VAR
    #define WINKS_DBFILE_MAX_SIZE       0x4b000/*300K*/             /*debugfile的最大size*/
    #define WINKS_DBFILE_WRITE_BACK_SIZE 0x1000/*4K*/               /*debugfile过大时回写尺寸*/
    #define WINKS_SCREEN_WIDTH          240                         /*屏宽，分辨率*/
    #define WINKS_SCREEN_HEIGHT         320                         /*屏高，分辨率*/
    #define WINKS_CC_WIDTH              240                         /*彩像宽，分辨率*/
    #define WINKS_CC_HEIGHT             236                         /*彩像高，分辨率*/
    #define WINKS_TOPBAR_HEIGHT         20                          /*定义topbar高度*/
    #define WINKS_BOTTOMBAR_HEIGHT      20                          /*定义borrombar高度*/
    #define WINKS_BUILD_VERSION         "1"                         /*编译版本号*/
    #define WINKS_V                     "1.0"                       /*winks-protocol-version 协议版本 */
    #define WINKS_AN                    "winks-SPTRUM"              /*package-name 终端软件包名称 */
    #define WINKS_AV                    WINKS_MV"."WINKS_BUILD_VERSION   /*version 终端软件包版本 */
//    #define WINKS_OS                    "SPTRUM"                    /*client-os 终端软件操作系统信息 */
//    #define WINKS_HW                    "SPTRUM"                    /*hardware-id 终端硬件标识 */
    #define WINKS_CL                    "240x236"                   /*canvasList 彩像媒体展示尺寸列表*/
    #define WINKS_MT                    "gif,png,jpeg"              /*mediaTypes 媒体格式列表*/
//    #define WINKS_ADC                   "0"                        /*dist-channel 安装渠道编码,可用函数获取*/
//    #define WINKS_BUA                   "WAPUSERAGENT"              /*base64-user-agent:client default browser user-agent string*/
    #define WINKS_ROOT_PATH             "/flash/app2/user/"
    #define WINKS_APP_PATH                WINKS_ROOT_PATH"winks/"
    #define WINKS_IMG_PATH              WINKS_ROOT_PATH"winks\\image\\"
    #define WINKS_PATH_SPACE            "/"
    #define WINKS_OSAL_API_SELF_DATA_LEN     32
    #define WINKS_SYS_TICK              1                           /*平台1Tick对应的毫秒数*/
#endif
#define WINKS_USRAGENT              "UA-winks1.0"               /*USER AGENT for HTTP.*/


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


//wq added for brew version 08-12-16
#ifdef WINKS_BREW
#define KDDI_BREW_401  4010
#define CN_BREW_315    3151
#endif
//wq added for brew version end

/*配置服务器的设置*/
//#define WINKS_URL_DEMO "http://ip.izhayan.com:8100"
//#define WINKS_URL_DEMO "http://ip91.winksi.com:18080/winksWS"

#if 0
#ifdef WINKS_SIMULATOR
#define WINKS_URL_TEST "http://ip.winksi.com:8100"
#define WINKS_URL_DEVICE "http://192.168.10.11:5051"
#else/*WINKS_SIMULATOR*/
#define WINKS_URL_TEST "http://ip.izhayan.com:8100"
#define WINKS_URL_DEVICE "http://219.143.46.226:5051"
#endif/*WINKS_SIMULATOR*/
#endif
#define WINKS_URL_DEMO "http://ip.winksi.cn:8100/winksWS"
#define WINKS_URL_TEST  "http://ip.winksi.cn:8100/winksWS"
#define WINKS_URL_DEVICE "http://ip91.winksi.com:18080/winksWS"

#ifdef WINKS_DEMO_VERSION
#define WINKS_HOST_SERVER_URL WINKS_URL_DEMO
#elif defined (WINKS_TEST_VERSION)
#define WINKS_HOST_SERVER_URL WINKS_URL_TEST
#else
#define WINKS_HOST_SERVER_URL WINKS_URL_DEVICE
#endif



/*APN的设置*/
#if defined(WINKS_SIMULATOR) && !defined(WINKS_WM_PPC) /* 模拟器 */
    #define WINKS_PROXY_SERVER_IP "0.0.0.0"
    #define WINKS_PROXY_SERVER_PORT 0
    #define WINKS_DEFAULT_ACCESS_POINT 0
#else /* 真机 */

#if defined(WINKS_MTK)
    #define WINKS_PROXY_SERVER_IP "10.0.0.172"
    #define WINKS_PROXY_SERVER_PORT 80
    #define WINKS_DEFAULT_ACCESS_POINT 14
#elif defined(WINKS_ARENA)
    #define WINKS_PROXY_SERVER_IP "10.0.0.172"
    #define WINKS_PROXY_SERVER_PORT 80
    #define WINKS_DEFAULT_ACCESS_POINT 1
#elif defined(WINKS_SPTRUM_MOCOR)
    #define WINKS_PROXY_SERVER_IP "10.0.0.172"
    #define WINKS_PROXY_SERVER_PORT 80
    #define WINKS_DEFAULT_ACCESS_POINT 0
#elif defined(WINKS_SYMBIAN_V_9_0)
    #define WINKS_PROXY_SERVER_IP "0.0.0.0"
    #define WINKS_PROXY_SERVER_PORT 0
    #define WINKS_DEFAULT_ACCESS_POINT 0
#elif defined(WINKS_WM_PPC)
    #define WINKS_PROXY_SERVER_IP "0.0.0.0"
    #define WINKS_PROXY_SERVER_PORT 0
    #define WINKS_DEFAULT_ACCESS_POINT 1131344196
#else
    #error "U should give default value"
#endif

#endif /* WINKS_SIMULATOR */

#ifdef WINKS_ENGINE_TEST_5PROTOCAL
#ifdef WINKS_SIMULATOR
#define WINKS_COMM_SERVER_URL "http://192.168.10.11:6052"
#else
#define WINKS_COMM_SERVER_URL "http://219.143.46.226:5053"
#endif
#endif

/*图片资源CACH size*/
#ifdef MMI_ON_WIN32
#define WINK_IMG_CACHELEN    (300 * 1024)
#else
#define WINK_IMG_CACHELEN    (50 * 1024)
#endif
#define WINKS_FILE_DFPATH   ((char* )"root")            /*文件管理器默认的起始路径*/




/*应用连网无反应时间超时*/
#define WINKS_HTTP_TIME_LIMIT (120)
/*定时连网时间周期的时间限制*/
#define WINKS_TIME_CONNECT_PERIOD_DEF (43200)       /*一天更新两次*/
#define WINKS_TIME_CONNECT_PERIOD_MAX (86400)       /*不能超过4294967，49日多，换算成毫秒后已经是2^32*/
#define WINKS_TIME_CONNECT_PERIOD_MIN (1000)        /*不到17分钟*/
/*定时连网超时时间限制*/
#define WINKS_TIME_CONNECT_OVERTIME_DEF (120)         /*2分钟*/
#define WINKS_TIME_CONNECT_OVERTIME_MAX (600)        /*10分钟*/
#define WINKS_TIME_CONNECT_OVERTIME_MIN (30)          /*30秒*/
/*PUSH连网放弃终端时间限制*/
//#define WINKS_PUSH_GIVE_UP_TIMER_DEF (8640000)       /*100天*/
//#define WINKS_PUSH_GIVE_UP_TIMER_MAX (207360000)     /*100年*/
//#define WINKS_PUSH_GIVE_UP_TIMER_MIN (600)           /*10分钟*/
/*片头播放时间限制*/
#define WINKS_SPLASH_SHOW_TIMER_DEF (3)         /*3"*/
#define WINKS_SPLASH_SHOW_TIMER_MAX (15)        /*15"*/
#define WINKS_SPLASH_SHOW_TIMER_MIN (0)         /*not show*/

/* 发送短信短周期时间属性 */
#define WINKS_SMS_SHORT_PRIEOD_DFT      20
#define WINKS_SMS_SHORT_PRIEOD_MIN      10
#define WINKS_SMS_SHORT_PRIEOD_MAX      60

/* 发送短信长周期时间属性 */
#define WINKS_SMS_LONG_PRIEOD_DFT       40
#define WINKS_SMS_LONG_PRIEOD_MIN       40
#define WINKS_SMS_LONG_PRIEOD_MAX       180

/* 用户好友列表号码信息属性 */
#define WINKS_FRIENDS_MAX_NUMBER        10

/* 分块下传好友信息属性-总块数 */
#define WINKS_FRIEND_INFO_BLOCK_DFT     1
#define WINKS_FRIEND_INFO_BLOCK_MIN     0
#define WINKS_FRIEND_INFO_BLOCK_MAX     WINKS_FRIENDS_MAX_NUMBER      /* FIXME: 如果好友过多 */
/* 主叫彩像总张数 */
#define WINKS_OUT_CC_NUM_MIN            1
#define WINKS_OUT_CC_NUM_MAX            4
/* 被叫彩像总张数 */
#define WINKS_IN_CC_NUM_MIN             1
#define WINKS_IN_CC_NUM_MAX             4
/* 彩像播放间隔时间 */
#define WINKS_CC_PLAY_INTERVAL_DFT      1
#define WINKS_CC_PLAY_INTERVAL_MIN      1
#define WINKS_CC_PLAY_INTERVAL_MAX      5
/* 彩像播放间隔时间 */
#define WINKS_TAOCC_PLAY_TIME_DFT       3
#define WINKS_TAOCC_PLAY_TIME_MIN       1
#define WINKS_TAOCC_PLAY_TIME_MAX       5
/* 时间戳的最大长度*/
#define WINKS_CONN_TIME_FLAG_MAX_LEN    19

/*键盘历史栈长度*/
#ifdef WINKS_DEBUG
#define WINKS_TEST_KEY_BASE_LEN 1           /* "#" */
#define WINKS_TEST_KEY_WORDS_LEN 4          /* "???#" */
#define WINKS_TEST_KEY_HISTORY_STACK_LEN 5 /* "*#100022#???#" */
#define WINKS_TEST_KEY_STR "*" /* "*#100022#???#" */
#else
#define WINKS_TEST_KEY_BASE_LEN 9           /* "*#100022#" */
#define WINKS_TEST_KEY_WORDS_LEN 4          /* "???#" */
#define WINKS_TEST_KEY_HISTORY_STACK_LEN 13 /* "*#100022#???#" */
#define WINKS_TEST_KEY_STR "*#100022#" /* "*#100022#???#" */
#endif


#define WINKS_FLASHSPACE_RESERVED       61440   /* 61K (0xF000)*/

#define EACH_CC_TAKEUP_SIZE             30720   /* 30K = 30*1024,每个彩像30K */

#define WINKS_INIT_NEED_FLASH_SIZE      (0x10000)   /*(64*1024) winks初始化需要的最小磁盘空间 */
#define WINKS_LARGE_FLASH_SIZE          (0x4000000) /*(64*1024*1024) winks软件区分大小容量磁盘分界>64M为大容量 */

#ifdef __cplusplus
}
#endif

#endif/*_WK_PROJECT_H_*/
