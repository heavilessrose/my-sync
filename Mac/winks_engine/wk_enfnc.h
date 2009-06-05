/*
File:       wk_enfnc.h
Author:     Huaxm
Create Time:04/03/2009
*/

#ifndef _WK_ENFNC_H_
#define _WK_ENFNC_H_


#if 1
/***********************************************/
/***********    Engine framework  **************/
/***********************************************/
#define WINKS_ENGINE_APP_QUEUE_SIZE WINKS_ENGINE_APP_PRI_NUM /*引擎应用模块的栈大小*/
#define WINKS_ENAPP_NAME_MAX_LEN 12 /*引擎应用名称长度*/
#define WINKS_ENGINE_APP_HANDLER_NULL 0 /*引擎应用空句柄*/
#define WINKS_ENGINE_APP_TIMER_NUM 2 /*引擎应用定时器个数*/

typedef enum tag_Winks_EngineAppPri_e
{
    WINKS_ENGINE_APP_PRI_NORMAL = 0x00,     /* 正常。例如:定时更新，自动升级等*/
    WINKS_ENGINE_APP_PRI_USER = 0x01,       /* 用户级别。由用户触发的操作，如激活，DIY，手动更新等*/
    WINKS_ENGINE_APP_PRI_CALL,              /* 电话级别。 由来电触发，如来电下载*/
    WINKS_ENGINE_APP_PRI_NUM                /* 优先级数。*/
}Winks_EngineAppPri_e;

typedef unsigned long WINKS_ENAPP_HND; /*引擎应用句柄*/
typedef int (*WINKS_ENGAPP_PROCESS)(WINKS_ENAPP_HND appHnd, unsigned long msg, void *msgData, unsigned long msgDataSize, void *addData); /*引擎应用回调函数*/

 /*引擎应用的控制块*/
typedef struct tag_Winks_EngineAppControl_s
{
    WINKS_ENAPP_HND         appHnd; /*引擎应用句柄*/
    WINKS_ENGAPP_PROCESS    prcFnc; /*引擎应用处理函数*/
    void *                  addData; /*引擎应用的数据*/
    Winks_EngineAppPri_e    appPri; /*引擎应用的优先级*/
    char appName[WINKS_ENAPP_NAME_MAX_LEN]; /*引擎应用的名称*/
    WINKS_TIMER_ID timer[WINKS_ENGINE_APP_TIMER_NUM]; /*引擎应用的定时器句柄*/
}Winks_EngineAppControl_s;

/*根据应用名称查找应用句柄
 * function: winks_FindEngAppHndByName
 * parameters:
 *   char * appName: 引擎应用的名称
 * return:
 *   成功返回:应用句柄(>0)
 *   失败返回:0(WINKS_ENGIEN_APP_HANDLER_NULL)
 */
WINKS_ENAPP_HND winks_FindEngAppHndByName(char * appName);

/*根据应用名称查找应用句柄
 * function: winks_GetEngAppAddData
 * parameters:
 *   WINKS_ENAPP_HND appHnd: 应用句柄(>0)
 * return:
 *   成功返回:addData指针
 *   失败返回:NULL
 */
void * winks_GetEngAppAddData(WINKS_ENAPP_HND appHnd);

 /*创建并启动引擎应用(例如激活流程和DIY流程就各是一个引擎应用。
 * function: Winks_EngAppStart
 * parameters:
 *   Winks_EngineAppPri_e appPri: 应用优先级
 *   WINKS_ENGAPP_PROCESS prcFn: 处理回调函数
 *   char * appName: 引擎应用的名称
 *   void * addData: (in)数据指针
 * return:
 *   成功返回:应用句柄(>0)
 *   失败返回:0(WINKS_ENGIEN_APP_HANDLER_NULL)
 */
WINKS_ENAPP_HND Winks_EngAppStart(Winks_EngineAppPri_e appPri, WINKS_ENGAPP_PROCESS prcFnc, char * appName, void *addData);

/*退出并注销动引擎应用(例如激活流程和DIY流程就各是一个引擎应用。
 * function: Winks_EngAppStop
 * parameters:
 *   WINKS_ENAPP_HND appHnd: 应用句柄(>0)
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_EngAppStop(WINKS_ENAPP_HND appHnd);

/*引擎应用创建TIMER。
 * function: Winks_EngCreateTimer
 * parameters:
 *   WINKS_ENAPP_HND appHnd: 应用句柄(>0)
 *   WINKS_TICKS timeOutPeriod:创建定时器的定时周期
 *   unsigned long flag: 创建定时器的属性标志。
 *                       WINKS_TIMER_DEFAULT: 默认
 *                       WINKS_TIMER_AUTO_START:自启动; 
 *                       WINKS_TIMER_AUTO_LOOP: 循环启动;
 * return:
 *   成功返回:定时器句柄(>0)
 *   失败返回:0
 */
WINKS_TIMER_ID Winks_EngCreateTimer(WINKS_ENAPP_HND appHnd, WINKS_TICKS timeOutPeriod, unsigned long flag);

/*引擎应用删除定时器。
 * function: Winks_EngDestroyTimer
 * parameters:
 *   WINKS_ENAPP_HND appHnd: 应用句柄(>0)
 *   WINKS_TIMER_ID timerId: 定时器句柄(>0)
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_EngDestroyTimer(WINKS_ENAPP_HND appHnd, WINKS_TIMER_ID timerId);
#endif/*Engine framwork end.*/


typedef struct tag_WinksSrvConfig_s
{
    unsigned int    diy:1,          /*server支持功能:DIY */
                    vcard:1,        /*server支持功能:VCARD */
                    activity:1,     /*server支持功能:ACTIVITY */
                    vcash:1,        /*server支持功能:VCASH */
                    end1:1;      
    
    unsigned int    limited:1,      /*订阅模式:limited */
                    trial:1,        /*订阅模式:trial */
                    adware:1,       /*订阅模式:adware */
                    subscriber:1,   /*订阅模式:subscriber */
                    end2:1;   
}WinksSrvConfig_s;

#if 1
/***********************************************/
/***********    Engine manager    **************/
/***********************************************/
#define WINKS_HTTP_NULL (-1)        /*HTTP无效句柄*/
typedef enum tag_Winks_EngineState_e
{
    WINKS_ENGINE_STATE_INIT = 0x00,/*0000  开始状态*/
    WINKS_ENGINE_STATE_START = 0x01,/*0001  初始化状态*/
    WINKS_ENGINE_STATE_DISABLED = 0x02,/*0010  初始化完成状态*/
    WINKS_ENGINE_STATE_ENABLED = 0x06,/*0110  已激活完成状态*/
//    WINKS_ENGINE_STATE_SHOWA = 0x0a,/*1010  未激活且电话事件状态*/
//    WINKS_ENGINE_STATE_SHOWB = 0x0e,/*1110  已激活且电话事件状态*/
    WINKS_ENGINE_STATE_NUMBER
}Winks_EngineState_e;

#define Winks_IsEngStateCall(state)  (state&WINKS_BIT_FALG_D3)
#define Winks_IsEngStateRegister(state)  (state&WINKS_BIT_FALG_D2)
#define Winks_IsEngStateInitOk(state)  (state&WINKS_BIT_FALG_D1)
#define Winks_SetEngStateregister(state)  (state = state|WINKS_BIT_FALG_D2)
#define Winks_SetEngStateUnRegister(state)  (state = Winks_IsEngStateRegister(state)?state^WINKS_BIT_FALG_D2:state)


#define WINKS_MSG_ENG2UI_NONE       0       /* 无关消息 */
    
typedef enum tag_WinksEng2UiMsg_SysErr_e
{
/* 出错消息 */
    WINKS_MSG_ENG2UI_SVR_NO_ANSWER  = -127, /* 服务器无应答 */
    WINKS_MSG_ENG2UI_UNREG_USER     ,       /* 非注册用户限制 */
    WINKS_MSG_ENG2UI_ROAMING_LIMIT  ,       /* 漫游限制  */
    WINKS_MSG_ENG2UI_ENG_BUSY       ,       /* 引擎忙 */
    WINKS_MSG_ENG2UI_DIAL_INT       ,       /* 被电话中断 */
    WINKS_MSG_ENG2UI_STORAGE_LIMIT  ,       /* 存储空间不足 */
    WINKS_MSG_ENG2UI_CC_CLOSED      ,       /* 彩像软件关闭  */
#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_MCARD_NOT_EXIST,       /* 存储卡不存在 */
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    WINKS_MSG_ENG2UI_SYS_FILES_ERR  ,       /* 系统文件被破坏 */
    WINKS_MSG_ENG2UI_CC_FILES_ERR   ,       /* 彩像文件被破坏 */
#ifdef WINKS_DUAL_SIM
    WINKS_MSG_ENG2UI_SIM_SEL_ERR    ,       /* 用户未选择SIM卡 */
    WINKS_MSG_ENG2UI_SIM_UNUSABLE   ,       /* 用户选择的Sim卡不可用 */
#endif /* WINKS_DUAL_SIM */
    WINKS_MSG_ENG2UI_NO_SUBSCRIPTION,       /* 不是订阅用户 */
    WINKS_MSG_ENG2UI_USER_DISABLED   ,      /* 用户被禁用 */
    WINKS_MSG_ENG2UI_NO_NUMBER      ,       /* 没有电话号码需要更新 */

    WINKS_MSG_ENG2UI_PROT_NO        ,       /* 服务器下发协议号不匹配 */
    WINKS_MSG_ENG2UI_HTTP_ERRCODE   ,       /* 收到Http错误码 */
    WINKS_MSG_ENG2UI_SRV_ERRCODE    ,       /* 收到服务器返回错误码 */
    WINKS_MSG_ENG2UI_SRV_ERRDATA    ,       /* 服务器返回数据有错误 */
    WINKS_MSG_ENG2UI_MEM            ,       /* 内存申请出错 */
    WINKS_MSG_ENG2UI_FS             ,       /* 文件系统出错 */
    WINKS_MSG_ENG2UI_HTTP           ,       /* HTTP错误 */
    WINKS_MSG_ENG2UI_HTTP_TIMEOUT   ,       /* HTTP超时错误 */
    WINKS_MSG_ENG2UI_PROT_PACK      ,       /* 协议打包 */
    WINKS_MSG_ENG2UI_PROT_PARSE     ,       /* 协议解析 */
    WINKS_MSG_ENG2UI_XML_GEN        ,       /* 生成xml出错 */
    WINKS_MSG_ENG2UI_XML_PARSE      ,       /* xml解析出错 */
    WINKS_MSG_ENG2UI_APP_ERR        ,       /* 应用调用错 */
    WINKS_MSG_ENG2UI_ENG_ERR        ,       /* 引擎调用错 */
    
    WINKS_MSG_ENG2UI_UNKNOWN        = -80   /* 未知错误 */
}WinksEng2UiMsg_SysErr_e;

typedef enum tag_WinksEng2UiMsg_Regist_e
{
/* 1 注册及初始化过程 */
    WINKS_MSG_ENG2UI_SETUP_SVR_FAIL = -9   /* 连接配置服务器失败 */
    ,WINKS_MSG_ENG2UI_REG_FAIL              /* 注册失败 */
    ,WINKS_MSG_ENG2UI_ULPB_FAIL             /* 上传电话簿失败 */
    ,WINKS_MSG_ENG2UI_INIT_FAIL             /* 初始化失败 */
    
    ,WINKS_MSG_ENG2UI_SETUP_SVR      = 1    /* 正在连接配置服务器... */
    ,WINKS_MSG_ENG2UI_SETUP_SVR_OK          /* 连接配置服务器成功 */
    ,WINKS_MSG_ENG2UI_SMS_REG               /* 未注册，需要发送注册短信 */
    ,WINKS_MSG_ENG2UI_WAIT_PUSH             /* 未注册，等待接收push */
    ,WINKS_MSG_ENG2UI_REG                   /* 正在注册... */
    ,WINKS_MSG_ENG2UI_REG_OK                /* 注册OK [注册成功问候语(utf8格式)] */
    ,WINKS_MSG_ENG2UI_ULPB_CONFIRM          /* 用户确认是否上传电话簿 */
    ,WINKS_MSG_ENG2UI_ULPB                  /* 正在上传电话簿... */
    ,WINKS_MSG_ENG2UI_ULPB_OK               /* 上传电话簿完毕 */
    ,WINKS_MSG_ENG2UI_INIT                  /* 正在初始化... */
    ,WINKS_MSG_ENG2UI_INIT_OK               /* 初始化完毕 */
    ,WINKS_MSG_ENG2UI_QUERY_OK              /* 查询用户状态完成 */
}WinksEng2UiMsg_Regist_e;

typedef enum tag_WinksEng2UiMsg_Friend_e
{
/* 2 更新好友录 */
    WINKS_MSG_ENG2UI_FRIEND_FAIL    = -79,  /* 更新好友录失败 */
    
    WINKS_MSG_ENG2UI_FRIEND         = 71,   /* 正在更新好友录...[WinksProgress_s] */
    WINKS_MSG_ENG2UI_FRIEND_OK              /* 好友录更新完毕 */
}WinksEng2UiMsg_Friend_e;
    
typedef enum tag_WinksEng2UiMsg_Diy_e
{
/* 3 上传DIY */
    WINKS_MSG_ENG2UI_DIY_FAIL       = -29,  /* 上传DIY失败 */
    WINKS_MSG_ENG2UI_SCRIPT_FAIL,           /* 定制彩像失败 */

    WINKS_MSG_ENG2UI_DIY            = 21,   /* 正在上传DIY... */
    WINKS_MSG_ENG2UI_DIY_OK,                /* 上传DIY成功[wid彩像id] */
    WINKS_MSG_ENG2UI_SCRIPT_OK              /* 定制彩像成功 */
}WinksEng2UiMsg_Diy_e;

typedef enum tag_WinksEng2UiMsg_Recommend_e
{
/* 4 推荐给好友 */
    WINKS_MSG_ENG2UI_RECOMMEND_FAIL = -39,  /* 推荐失败 */

    WINKS_MSG_ENG2UI_RECOMMEND      = 30,   /* 正在推荐... */
    WINKS_MSG_ENG2UI_RECOMMEND_OK           /* 推荐成功 */
}WinksEng2UiMsg_Recommend_e;

typedef enum tag_WinksEng2UiMsg_Call_e
{
/* 5 电话下载 */
    WINKS_MSG_ENG2UI_CALL_FAIL      = -49,  /* 获取通话用户彩像失败 */
    WINKS_MSG_ENG2UI_CALL_TIMEOUT   ,       /* 获取通话用户彩像超时 */

    WINKS_MSG_ENG2UI_CALL           = 41,   /* 正在获取通话用户彩像... */
    WINKS_MSG_ENG2UI_CALL_OK                /* 获取通话用户彩像成功 */
}WinksEng2UiMsg_Call_e;

typedef enum tag_WinksEng2UiMsg_Setup_e
{
/* 6 同步本地配置到服务器 */
    WINKS_MSG_ENG2UI_SETUP_FAIL     = -59,  /* 同步本地配置失败 */
    WINKS_MSG_ENG2UI_SRVCFG_FAIL,           /* 服务器配置失败 */
    WINKS_MSG_ENG2UI_USERCFG_FAIL,          /* 本地配置失败 */
    WINKS_MSG_ENG2UI_ULSTAT_FAIL,           /* 统计信息上传失败 */
    WINKS_MSG_ENG2UI_ULLOG_FAIL,            /* log信息上传失败 */
    
    WINKS_MSG_ENG2UI_SETUP          = 51,   /* 正在同步本地配置... */
    WINKS_MSG_ENG2UI_SETUP_OK,              /* 同步本地配置成功 */
    WINKS_MSG_ENG2UI_SRVCFG_OK,             /* 服务器配置成功 */
    WINKS_MSG_ENG2UI_USERCFG_OK,            /* 本地配置成功 */
    WINKS_MSG_ENG2UI_ULSTAT_OK,             /* 统计信息上传成功 */
    WINKS_MSG_ENG2UI_ULLOG_OK               /* log信息上传成功 */
}WinksEng2UiMsg_Setup_e;

typedef enum tag_WinksEng2UiMsg_Upgrade_e
{
/* 7 版本升级 */
    WINKS_MSG_ENG2UI_RUPGRADE_FAIL = -69,  /* 升级失败 */

    WINKS_MSG_ENG2UI_RUPGRADE      = 61,   /* 正在检查新版本... */
    WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW  ,   /* 没有新版本 */
    WINKS_MSG_ENG2UI_RUPGRADE_NEW      ,   /* 有新版本 */
    WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK       /* 新版本下载成功，安装 */
}WinksEng2UiMsg_Upgrade_e;


#define WINKS_NOTIFY_MASK       0x0000001f
#define WINKS_NOTIFY_newver     0x00000001
#define WINKS_NOTIFY_scfg       0x00000002
#define WINKS_NOTIFY_ccfg       0x00000004
#define WINKS_NOTIFY_upldstat   0x00000008
#define WINKS_NOTIFY_upldlog    0x00000010

#define WINKS_NOTIFY_BITS       5   

typedef struct tag_Winks_SrvNotify_s
{
    unsigned long   Notify;         /* 高16位表示正在处理哪条消息;低16为表示接收到的消息 
                                       消息按位标记，参考上面WINKS_NOTIFY_MASK */
    unsigned long   filenumber_log; /* 文件号 (WINKS_NOTIFY_upldstat/upldlog有效) */
    unsigned long   filenumber_stat;/* 文件号 (WINKS_NOTIFY_upldstat/upldlog有效) */

    int             Bit;            /* 0~WINKS_NOTIFY_BITS,按位读取消息并处理 */
}Winks_SrvNotify_s;


Winks_MemPool_s * Winks_EngMemPoolCreate(Winks_MemPool_s ** ppMemPool);
int Winks_EngMemPoolDestory(Winks_MemPool_s * pMemPool);
void * Winks_EngMemPoolAlloc(Winks_MemPool_s* pMemPool, int AllocSize, int Align);

#define HTTP_HEAD_CONTENTLEN     "Content-Length"
#define HTTP_HEAD_CONTENTTYPE    "Content-Type"
#define HTTP_HEAD_WINKSNOTIFY    "Winks-Notify"
#define HTTP_HEAD_LANGUAGE       "Accept-Language"

#define HTTP_CONTENTTYPE_gif     "image/gif"
#define HTTP_CONTENTTYPE_jpeg    "image/jpeg"
#define HTTP_CONTENTTYPE_png     "image/x-png"
#define HTTP_CONTENTTYPE_mpeg    "video/mpeg"
#define HTTP_CONTENTTYPE_avi     "video/x-msvideo"
#define HTTP_CONTENTTYPE_flash   "application/x-shockwave-flash"
#define HTTP_CONTENTTYPE_bin     "application/octet-stream"
#define HTTP_CONTENTTYPE_octet   "application/"
#define HTTP_CONTENTTYPE_xml     "application/xml"

#define HTTP_CONTENTTYPE_xml_post     "text/xml"

#define HTTP_WINKSNOTIFY_newver     "Software-Update"
#define HTTP_WINKSNOTIFY_ccfg       "User-Config"
#define HTTP_WINKSNOTIFY_upldstat   "Upload-Stat-"
#define HTTP_WINKSNOTIFY_upldlog    "Upload-Log-"
#define HTTP_WINKSNOTIFY_scfg       "Service-Config"

#define WINKS_DB_FILE_IN    WINKS_APP_PATH"dbgin.txt"
#define WINKS_FILE_CCSSET   WINKS_APP_PATH"ccsset.txt"

char * Winks_Ext2ContType(char *ext);

typedef struct tag_WinksStrList_s
{
	struct tag_WinksStrList_s * pnext;	//链表指针
	char  * str;    /* 电话号码 */
}WinksStrList_s;

#include "wk_prot_if.h"
#include "wk_strg_if.h"

typedef struct tag_WinksEng2AppMsg_s
{
    int     AppMsg;     /* WinksEng2UiMsg_SysErr_e or WinksProtRetType_e */
    void *  pData;
}WinksEng2AppMsg_s;

typedef struct tag_WinksProgress_s
{
    unsigned short Finished;
    unsigned short Total;
}WinksProgress_s;

/*应用类别*/
typedef enum tag_Winks_EngProType_e
{
    WINKS_ENGPROTYPE_REGISTER = 0x00    /*激活*/
    ,WINKS_ENGPROTYPE_INIT              /*初始化*/
    ,WINKS_ENGPROTYPE_REGULARLY_UPDATE  /*定时更新*/
    ,WINKS_ENGPROTYPE_DIY               /*DIY设置*/
    ,WINKS_ENGPROTYPE_RECOMMEND         /*推荐*/
    ,WINKS_ENGPROTYPE_QUREY_US          /*用户类型查询*/
    ,WINKS_ENGPROTYPE_UPDATE_FRIENDS    /*更新好友*/
    ,WINKS_ENGPROTYPE_UPLOAD_PB         /*上传电话本*/
    ,WINKS_ENGPROTYPE_UPDATE_SW         /*软件升级*/
    ,WINKS_ENGPROTYPE_CALL_UPDATE       /*电话事件更新*/
    ,WINKS_ENGPROTYPE_HUNGUP_UPDATE     /*挂断后更新*/
    ,WINKS_ENGPROTYPE_UPDATE_SC         /*更新服务器配置信息*/
    ,WINKS_ENGPROTYPE_UPDATE_UC         /*更新用户配置信息*/
    ,WINKS_ENGPROTYPE_UPDATE_LS         /*更新本地设置项*/
    ,WINKS_ENGPROTYPE_UPLOAD_LOG        /*上传log日志*/
    ,WINKS_ENGPROTYPE_UPLOAD_STA        /*上传统计信息*/
    ,WINKS_ENGPROTYPE_NUMBER
}Winks_EngProType_e;

/*消息WINKS_MSG_ENGINE_APP_RETMSG/WINKS_MSG_ENGINE_UIRETMSG的结构，引擎联网操作返回的消息体.*/
typedef struct tag_Winks_EngineToUiReturnMsg_s
{
    int     code;
    int     appType;/*Winks_EngProType_e*/
    char    body[4];
}Winks_EngineToUiReturnMsg;

typedef struct tag_WinksStrgFileMap_s
{
    char    SelfNoBcd[6];
    unsigned long   CacheTime;
    void * pUserMap;                /* 用户占用 */
    void * pFriendMap;              /* 好友占用 */
    unsigned short UserNum;         /* 用户数 */
    unsigned short FriendNum;       /* 好友数 */
    unsigned char * pCCCache;       /* 动态下载图片Cache */
    unsigned long   CCCacheOff;     /* 图片Cache长度 */
    char CCCacheNoBcd[6];
}WinksStrgFileMap_s;

#define WINKS_CACHETIMER_RETRY_SLOT     90
#define WINKS_CALLTIMER_WAIT_SLOT       5
#define WINKS_PERIODTIMER_RETRY_SLOT    120
#define WINKS_SRVMSG_RETRY_SLOT         60

#define WINKS_HTTP_RETRY_MAX            1

typedef enum tag_WinksCallCCStatus_e
{
    WINKS_CALLCC_STATUS_NONE = 0,
    WINKS_CALLCC_STATUS_GETINFO,
    WINKS_CALLCC_STATUS_GETCC,
    WINKS_CALLCC_STATUS_OK,
    WINKS_CALLCC_STATUS_FAIL,
    WINKS_CALLCC_STATUS_MAX
}WinksCallCCStatus_e;

/*用户DIY设置*/
typedef struct tag_Winks_DiyInfo_s
{
    int     action;  /* 0 - 只上传不定制; 1 - 上传并定制 */
    char  * pnolist; /* 在action为1时使用，定制好友的手机号码串，以|分隔，以|结尾；为NULL表示所有人 */
    unsigned char name[40]; /* 用户设置的彩像名称(unicode编码) */
    unsigned char path[WINKS_PATH_MAX]; /* 文件路径(unicode编码) */
    unsigned char greet[202];         /* 问候语(unicode编码) */
} Winks_DiyInfo_s;

typedef struct tag_Winks_RecommendInfo_s
{
	unsigned char name[16]; /* 姓名(unicode编码) */
	char    pno[16];        /* 电话号码 */
} Winks_RecommendInfo_s;

typedef enum tag_Winks_UrlType_e
{
    WINKS_URLTYPE_CCSHOP =0,  /* 彩像商城 */
    WINKS_URLTYPE_FAVORITE,   /* 我的彩像库 */
    WINKS_URLTYPE_SCRIPT,     /* 定制业务 */
    WINKS_URLTYPE_PURCHASE,   /* 购买彩像 */

    WINKS_URLTYPE_INTRO,      /* 在线体验 */
    WINKS_URLTYPE_HELP,       /* 帮助页面 */
    WINKS_URLTYPE_ABOUT,      /* 关于页面 */
    WINKS_URLTYPE_SETTINGS,   /* 彩像设置 */
    WINKS_URLTYPE_DIYCUSTOMIZE,/* DIY定制 */

    WINKS_URLTYPE_MAX
}Winks_UrlType_e;

typedef struct tag_WinksEngCall_s
{
    int              CallCCStatus;      /* 下载彩像状态 WinksCallCCStatus_e */
    int              CallType;          /* 来电或去电 */
    Winks_DateTime_s CallStartTime;     /* 电话事件开始时刻 */
    char             NO[WINKS_STRG_PHONE_LEN];    /* 电话号码字符串 */
    WinksStrgFile_CCShow_s  CCShow;     /* 彩像展示 */
}WinksEngCall_s;

typedef struct tag_Winks_EngineContext_s
{
    WINKS_CHN_ID    engChn;                 /* 引擎消息通道*/
    Winks_EngineState_e curState;           /* 当前引擎状态*/
    unsigned long   engAppSequence;
    Winks_EngineAppControl_s engAppStack[WINKS_ENGINE_APP_QUEUE_SIZE];     /*引擎应用控制块栈*/
    int             http;                   /* 引擎调用的HTTP句柄*/
    WINKS_TIMER_ID  httpTimer;              /* http连网定时器，用于监控调用http的时间*/
    unsigned char * pSaveSendData;          /* 保存发送数据，以便重新发送 */

    unsigned long   ProtNo;                 /* 协议号WinksProtocolEnum_e */
    unsigned long   FrameNo;                /* 记录帧号 */
    unsigned char   ifRemindRegister;       /* 标志:是否提示用户"注册". o:不提示;1:提示*/
    unsigned char   ifHttpReady;            /* 判断是否成功联网过一次? */
    unsigned short  getImsiCount;           /* 取IMSI失败后，延时重取TIMER */
    WINKS_TIMER_ID  getImsiTimer;           /* 取IMSI失败次数 */

    WINKS_TIMER_ID  CallWaitTimer;          /* 电话事件等待服务器返回彩像的Timer */
    WINKS_TIMER_ID  PeriodPushTimer;        /* 定时更新Timer */
    WINKS_TIMER_ID  BuildCacheTimer;        /* 创建彩像Cache的Timer */
    WINKS_TIMER_ID  ServerMsgTimer;         /* 处理服务消息的Timer */
    Winks_SrvNotify_s SrvNotify;            /* 服务器通知的消息 */
    WinksEngCall_s  *pCall;                 /* 记录电话事件相关信息 */
    unsigned short  SupportDimNum;          
    unsigned short  SupportExtNum;          
    unsigned short  SupportDim[16][2];      /* 支持的宽高比 */
    unsigned char   SupportExt[16];         /* 支持的文件扩展名类型 */
#ifdef WINKS_DUAL_SIM
    unsigned short  SimStatus;              /* 高字节低2位表示已获取哪张卡的IMSI，d0--卡1 d1--卡2
                                               高字节高4位表示获取哪张卡的IMSI失败，d4--卡1 d5--卡2
                                               低字节表示当前激活的是哪张卡，      1--卡1  2--卡2 */
    int             CurInquireSimNo;        /* 当前查询的Sim卡号: 
                                               1--Sim卡1(对应Winks_GetImsi); 
                                               2--Sim卡2(对应Winks_GetImsi_2) */
    int             OtherSimRetryCnt;       /* 获取1卡imsi后, 重试读取另一卡imsi的次数, 2次未取到认为卡不存在 */
    int             NeedRestoreWapSimSel;   /* 保存手机平台的Wap Sim选择 */
    int             SavePhoneWapSimSel;     /* 保存手机平台的Wap Sim选择 */
#endif /* WINKS_DUAL_SIM */
    WinksStrgFileMap_s  StrgFileMap;
#ifdef WINKS_DEBUG
    unsigned long usbFlagHistory[2];         /* 用于测试插拔USB线的状态。[0]记录次数,[1]记录状态位运算 from d0,*/
    Winks_DateTime_s v_dateTime;             /* 记录上一次定时更新的时间 */
    Winks_DateTime_s v_dateTime1;            /* 记录上一次定时更新服务关闭的时间 */
    int PTCloseCause;              /* 记录定时更新服务关闭原因 1--用户关 2--状态回滚 3--服务器不支持 4--Timer资源申请不到 5--彩像关闭*/
    char    ImsiCode[2][16];
    void  * TstProtMem;
    int     DbgCount;
#endif
}Winks_EngineContext;
#endif/*Engine manager.*/

/* 协议请求: 请求引擎按指定的协议发送到服务器
输入:
    ProtocolNo  : 需要打包的协议号 
    indata      : 需要输入的参数(无参数可以为空)
    inLen       : 输入参数的长度(无参数可以为0)
输出:
返回:
    成功或失败。
*/
int Winks_EngProtReq(WinksProtocolEnum_e ProtocolNo, void * inData, int inLen);

int Winks_SendLogToServer(void);
/* 
    读取电话簿
    调用前创建内存池pMemPool以便存储电话本，等打包发送完毕需要释放内存池
*/
int Winks_EngReadPhoneBook( Winks_MemPool_s * pMemPool,
                                       WinksProt_phoneBook_s ** ppphoneBook );
#ifdef WINKS_DUAL_SIM
/* 高字节低2位表示发现哪张卡，  b0--卡1; b1--卡2
   低字节表示当前激活的是哪张卡，1--卡1;  2--卡2
*/
unsigned short Winks_GetSimStatus(void);
#endif
/*
    注册完毕调用，引擎需要置状态
*/
int Winks_EngRegistOk(void);
/*
    上传电话簿及下载片头运营商彩像结束，引擎启动定时更新
*/
int Winks_EngInitOk(void);

typedef enum tag_WinksResultCode_e
{
    WINKS_RST_CODE_OK   = 0,

/* 1~999    系统调用错误 */
    RESOURCE_NOT_FOUND  = 1,    /* 资源未找到 */
    ACCESS_DENIED       = 2,    /* 访问被拒绝 */
    INVALID_PARAMETER   = 3,    /* 调用参数(形参)错 */
    BAD_ARGUMENTS       = 4,    /* 参数错 */
    NOT_IMPLEMENTED     = 5,    /* 功能尚未实现 */
    DATABASE_EXCEPTION  = 201,  /* 数据库访问异常 */

/* 1001~1999    网络通信错误 */
    PROTOCOL_VERSION_NOT_SUPPORTED  = 1001, /* 不支持的协议版本 */

/* 5001~5999    彩像业务逻辑错误 */
    SERVICE_NO_SUBSCRIPTION         = 5001, /* 不是订阅用户 */
    SERVICE_MEMBER_DISABLED         = 5002, /* 用户被禁用 */
    SERVICE_NOT_MEMBER              = 5003, /* 不是彩像用户 */
    
    WINKS_RST_CODE_MAX
}WinksResultCode_e;

/* 
    彩像下载保存
*/
typedef struct tag_Winks_fileinfo_s
{
    WinksFileExt_e  ext;        /* 文件扩展名 */
    unsigned long   filesize;   /*  */
    Winks_MemPool_s* pMem;      /* 存放下载的文件数据 */
    unsigned long   blocklen;   /* 当前下载块总大小(应该与内存列表中pMem->offset累加值相等) */
}Winks_fileinfo_s;

typedef enum tag_WinksCCDownType_e
{
    WINKS_CCDOWN_TYPE_PERIOD = 0,
    WINKS_CCDOWN_TYPE_CCCACHE,
    WINKS_CCDOWN_TYPE_RECRUIT,
    WINKS_CCDOWN_TYPE_FRIEND,
    WINKS_CCDOWN_TYPE_INIT,
    WINKS_CCDOWN_TYPE_CALL,
    WINKS_CCDOWN_TYPE_NUMBER,
    WINKS_CCDOWN_TYPE_MAX
}WinksCCDownType_e;

typedef enum tag_GetCCStep_e
{
    GETCC_STEP_GETCCINFO = 0,
    GETCC_STEP_GETURLFILE,
    GETCC_STEP_MAX
}GetCCStep_e;


typedef struct tag_WinksCCDownCtxt_s
{
    int     Step;   /* 获取一个号码的彩像步骤,GetCCStep_e */
    int     Pri;    /* 应用优先级 */
    int     AppType;/* 应用类型 */
    char  * AppName;
/* 原始输入 */
    WinksCCDownType_e CCDownType;     /*  */
    WinksStrList_s  * pPNoRoot;       /* 电话号码根列表 */
    WinksStrList_s  * pPNoCurr;       /* 当前电话号码 */
    unsigned short    Total;          /* 电话号码数 */
    unsigned short    Curr;           /* 当前第几个 */
/* 第1步输出结果 */
    WinksFileType_s * mediaurl;       /* 存放彩像的url */
    WinksFileType_s * thumburl;       /* 存放缩略图的url */
    unsigned char     media_num;      /* 彩像张数 */
    unsigned char     media_idx;      /* 彩像序列号 */
    unsigned char     thumbnail_num;  /* 缩略图数 */
    unsigned char     thumbnail_idx;  /* 缩略图序列号 */
    char              wid[WINKS_PROT_WID_MAX];/* 彩像ID */
    unsigned char     greet[202];     /* 问候语(unicode编码) */
    unsigned char     greetlen;       /* 问候语长度(字节数)，当等于255表示删除当前问候语 */
    unsigned char     img_type;       /* 图片类型 WinksFileExt_e */
    unsigned short    interval;       /* 图片播放间隔 */
    int               status;         /* 我的状态 */
    unsigned char     statustxt[100]; /* 我的状态字符串(unicode编码) */
    int               statustxtlen;   /* 我的状态字符串长(字节数) */
/* 保存文件需要的数据 */
    unsigned short    CCIdx;          /* 彩像ID */
    unsigned short    SaveIdx;        /* 保存文件的序列号，第1次保存需初始化CCImage，再打开文件句柄，
                                         保存完一个文件后关闭文件句柄SaveIdx置0 */
    WinksStrgFile_CCImage_s CCImage;      /* 供存储文件使用的文件信息 */
}WinksCCDownCtxt_s;

typedef enum tag_Winks_PeriodUpdateMethod_e
{
    WINKS_PERIODUPDATEMETHOD_SAVEMONEY =0,  /* 节省费用更新 */
    WINKS_PERIODUPDATEMETHOD_DATANEW,       /* 数据最新更新 */
    WINKS_PERIODUPDATEMETHOD_CUSTOM,        /* 用户自定义 */
    WINKS_PERIODUPDATEMETHOD_MAX
}Winks_PeriodUpdateMethod_e;

typedef struct tag_Winks_PeriodUpdateInfo_s
{
    int             Method;     /* 更新方式 Winks_PeriodUpdateMethod_e */
    unsigned char   Hour;       /* 更新时刻--时 */
    unsigned char   Min;        /* 更新时刻--分 */
    unsigned short  Period;     /* 更新周期(以分钟为单位) */
}Winks_PeriodUpdateInfo_s;

#define PU_SAVEMONEY_DFT_PERIOD     (24*60)     /* 1天 */
#define PU_DATANEW_DFT_PERIOD       (2*60)      /* 2小时 */

int Winks_EngCCDownCtxtCreate(WinksCCDownType_e CCDownType, char *pNo, WinksCCDownCtxt_s ** ppCCDownCtxt);
int Winks_EngCCDownCtxtDestory(WinksCCDownCtxt_s * pCCDownCtxt);
int Winks_EngCCDownCtxtSaveUrl( WinksFileType_s ** ppurlList, 
                                           WinksFileType_s * pWinksUrl );
int Winks_EngCCDownCtxtSaveVCard(VcardType_s * vcard);
/* 保存问候语且查找号码及彩像信息，
   成功返回是否需要下载彩像 */
int Winks_EngSearchCCInfo(WinksCCDownCtxt_s *pDownCC, int NeedWashOut);
/* 收到下载彩像的第1块时调用，
   成功返回彩像IDX */
int Winks_EngSaveCCInfo(WinksCCDownCtxt_s *pDownCC, int NeedWashOut);
/* 当下载通话号码的彩像后调用该函数 */
int Winks_EngUpdateCCShow(void);
/* 当下载通话号码的彩像失败 */
int Winks_EngUpdateCCShowFail(void);
/* 当彩像Cache创建成功调用 */
int Winks_EngCCCacheOk(void);
/* 当注册完毕初始化片头运营商彩像时调用, Pri为优先级 */
int Winks_EngInitSpecialCC(int Pri);
/* 服务器返回的错误码处理函数
   retUIMsg用来存放返回UI的消息
   返回值: -1 -- 处理失败
            0 -- 未处理
            1 -- 已处理
*/
int Winks_EngResultErrDftHandler(WinksProt_result_s * presult, int *retUIMsg);

/* 
    打包发给UI的包含body的消息
*/
int Winks_EngPostMsgToUI(int Pri, Winks_EngProType_e appType, int msg, void *data, int size);

int winks_StrgDeleteUnUsedCC(unsigned short *CCIdxLeft, int CCLeftCnt);
int Winks_GetCCStart(int Pri, WinksCCDownType_e CCDownType, char * pNo);
int Winks_GetCCStop(WinksCCDownType_e CCDownType);

/*加密算法(base64)
 * function: Winks_Encode
 * parameters:
 *   char *src: 明文
 *   int src_len: 明文长度
 *   char *dst: 密文
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_Encode(char *src, unsigned int src_len, char *dst, unsigned int dst_len);

typedef enum tag_SynsetType_e
{
    SYNSET_TYPE_SERVICECONFIG = 0,
    SYNSET_TYPE_USERCONFIG,
    SYNSET_TYPE_UPLOAD_STAT,
    SYNSET_TYPE_UPLOAD_LOG,
    SYNSET_TYPE_SETUSERCONFIG,
    SYNSET_TYPE_UPDUSERCONFIG,
    SYNSET_TYPE_MAX
}SynsetType_e;

/*
输入:
    Pri --- 优先级
    Type--- SynsetType_e
    pData:当 Type == SYNSET_TYPE_UPLOAD_STAT或SYNSET_TYPE_UPLOAD_LOG时,该项为UploadFile.fileNumber
             Type == SYNSET_TYPE_SETUSERCONFIG时,该项为WinksProt_userConfig_s结构
*/
int Winks_SynsetStart(int Pri, SynsetType_e Type, void * pData);
int Winks_SynsetStop(void);

/*-------------------------
        激活流程接口
---------------------------*/
#define WINKS_SMS_GATEWAY_NUM 2         /*短信网关最大个数*/
#define WINKS_SMS_GATEWAY_LEN 16        /*短信网关最大长度*/
#define WINKS_SMS_PREFIX_LEN 16          /*短信前缀最大长度*/

#define WINKS_ENGAPP_REG_FREGIST 0x01   /*前台激活*/
#define WINKS_ENGAPP_REG_BREGIST 0x02   /*后台激活*/
#define WINKS_ENGAPP_REG_QUERYUS 0x03   /*用户属性查询，是否激活、禁用、退订用户*/
#define WINKS_ENGAPP_REG_UPLOADPB 0x04  /*上传电话本*/

#define WINKS_REG_RUNCTR_PAUSE 1
#define WINKS_REG_RUNCTR_STOP 2

int Winks_UserRegistExt(unsigned long controller);
/*用户注册彩像业务，供UI使用。返回值:0成功,-1失败。*/
int Winks_UserRegist(void);
/*联网获取用户激活状态。待联网返回后需调用Winks_UserGetState获取状态。*/
int Winks_UserCheckState(void);
/*读取本地用户激活状态。调用Winks_UserCheckState后会更新本地状态*/
int Winks_UserGetState(int *state);
/*退出激活流程、或联网查询用户状态流程*/
int Winks_UserRegistStop(void);
/*用户确认注册，发送注册短信息,0:取消注册，1:发送注册*/
int Winks_UserSendRegSms(int reg_flag);
/*用户确认是否发送电话簿。返回值:0成功 其他失败。
  非注册流程也可以调用该函数上传电话簿
ulpb_flag: 0取消上传，1上传电话簿*/
int Winks_UserUploadPhoneBook(int ulpb_flag);
/*    取引擎当前的状态。*/
int Winks_GetEngineCurrentState(void);
/*    设置引擎当前的状态。*/
int Winks_SetEngineCurrentState(int state);
#endif 
