/*
File:       wk_enfnc.h
Author:     Huaxm
Create Time:01/04/2008
*/
#ifndef _WK_ENFNC_H_
#define _WK_ENFNC_H_

#ifndef _WK_PUBLIC_H_
#include "wk_public.h"
#endif

#define WINKS_IMSI_DISABLED 0x00
#define WINKS_IMSI_ENABLED 0x01

typedef enum tag_Winks_Eng2UiMsg_e
{
    WINKS_MSG_ENG2UI_NONE           = 0,    /* XX 无关消息 */
    
/* 成功及进度消息 */
    
/* 1 注册及初始化过程 */
    WINKS_MSG_ENG2UI_SETUP_SVR_OK   = 0x11, /* XX 连接配置服务器成功 */
    WINKS_MSG_ENG2UI_SMS_REG        ,       /* 未注册，需要发送注册短信     [Winks_UserSendRegSms] */
    WINKS_MSG_ENG2UI_SMS_REG_OK     ,       /* 短信注册OK */
    WINKS_MSG_ENG2UI_INIT           ,       /* XX 正在初始化... */
    WINKS_MSG_ENG2UI_INIT_OK        ,       /* 初始化完毕 */
/* 2 更新好友录 */
    WINKS_MSG_ENG2UI_FL_OK          = 0x21, /* XX 好友列表下载完毕 */
    WINKS_MSG_ENG2UI_FL_NOTIFY      ,       /* 通知用户有好友更新 */
    WINKS_MSG_ENG2UI_FL_SEL_FRIEND  ,       /* 好友列表下载完毕，选择好友录 [Winks_UserSetFriendList] */
    WINKS_MSG_ENG2UI_FRIEND_SET     ,       /* XX 上传好友录成功 */
    WINKS_MSG_ENG2UI_FRIEND_UPD     ,       /* XX 上传好友录及下载好友数据 */
    WINKS_MSG_ENG2UI_DL_FRIENDCC    ,       /* 正在下载好友彩像... */
    WINKS_MSG_ENG2UI_FRIEND_OK      ,       /* 好友录更新完毕 */
#if 0//def WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_FL_SEL_MCARD   ,       /* 好友列表下载完毕，选择存储卡 [Winks_UserSelMCard] */
#endif
    
/* 3 同步本地配置到服务器 */
    WINKS_MSG_ENG2UI_SETUP_OK       = 0x31, /* 同步本地配置成功 */
    
/* 4 上传DIY */
    WINKS_MSG_ENG2UI_DIY            = 0x41, /* XX 正在上传DIY... */
    WINKS_MSG_ENG2UI_DIY_OK         ,       /* 上传DIY成功 */
    
/* 5 重新激活，查询服务器注册状态 */
    WINKS_MSG_ENG2UI_REREG_OK       = 0x51, /* 重新激活成功 */

/* 6 电话下载 */
    WINKS_MSG_ENG2UI_CALLCC_GRT_OK  = 0x61, /* 获取用户问候语成功 */
    WINKS_MSG_ENG2UI_CALLCC_OK      ,       /* 获取用户问候语成功 */
    WINKS_MSG_ENG2UI_CALLCC_OTHER   ,       /* XX获取片头运营商彩像 */
    
/* 7 后台服务 */
    WINKS_MSG_ENG2UI_PUSH           = 0x71, /* XX 后台正在定时更新服务... */
    WINKS_MSG_ENG2UI_PUSH_FRIEND    ,       /* XX 后台正在更新好友... */
    WINKS_MSG_ENG2UI_PUSH_OK        ,       /* XX 后台定时更新服务完毕 */
    WINKS_MSG_ENG2UI_PUSH_CHECK_FL  ,       /* XX 后台定时更新服务完毕,但需要检查好友列表 */

    WINKS_MSG_ENG2UI_INITCACHE_FL   ,       /* XX 后台正在下载好友列表以便初始化Cache... */
    WINKS_MSG_ENG2UI_INITCACHE      ,       /* XX 后台正在初始化Cache... */
    WINKS_MSG_ENG2UI_INITCACHE_OK   ,       /* XX 后台初始化Cache完成 */

    WINKS_MSG_ENG2UI_RECACHE_TRIM   ,       /* XX 后台正在下载基本信息以重建Cache... */
    WINKS_MSG_ENG2UI_RECACHE        ,       /* XX 后台正在重建Cache... */
    WINKS_MSG_ENG2UI_RECACHE_OK     ,       /* XX 后台重建Cache完成 */

    WINKS_MSG_ENG2UI_RECRUIT_DL     ,       /* XX 后台正在补充下载电话彩像... */
    WINKS_MSG_ENG2UI_RECRUIT_DL_OK  ,       /* XX 后台补充下载电话彩像完成 */

/* 出错消息 */

/* 1 注册及初始化过程 */
    WINKS_MSG_ENG2UI_REG_FAIL       = 0x8011,   /* 注册失败 */
    WINKS_MSG_ENG2UI_INIT_FAIL      ,           /* 初始化失败 */

/* 2 更新好友录 */
    WINKS_MSG_ENG2UI_FL_FAIL        = 0x8021,   /* 更新好友录失败 */

/* 3 同步本地配置到服务器 */
    WINKS_MSG_ENG2UI_SETUP_FAIL     = 0x8031,   /* 同步本地配置失败 */

/* 4 上传DIY */
    WINKS_MSG_ENG2UI_DIY_FAIL       = 0x8041,   /* 上传DIY失败 */

/* 5 重新激活，查询服务器注册状态 */
    WINKS_MSG_ENG2UI_REREG_FAIL     = 0x8051,   /* 重新激活失败 */

/* 6 电话下载 */
    WINKS_MSG_ENG2UI_CALLCC_GRT_FAIL  = 0x8061, /* 问候语下载失败 */
    WINKS_MSG_ENG2UI_CALLCC_FAIL      ,         /* 用户彩像下载失败 */
    WINKS_MSG_ENG2UI_CALLCC_TIMEOUT   ,         /* 用户彩像下载超时 */
    WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL ,         /* XX获取片头运营商彩像失败 */

/* 7 后台服务 */
    WINKS_MSG_ENG2UI_PUSH_FAIL      = 0x8071,   /* XX 后台定时更新服务失败 */

    WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL      ,   /* XX 后台下载好友列表失败 */
    WINKS_MSG_ENG2UI_INITCACHE_FAIL         ,   /* XX 后台初始化Cache失败 */

    WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL      ,   /* XX 后台下载基本信息失败 */
    WINKS_MSG_ENG2UI_RECACHE_FAIL           ,   /* XX 后台重建Cache失败 */

    WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL        ,   /* XX 后台补充下载电话彩像失败 */

/* 引擎协议错误 */
    WINKS_ENG_ERROR_CALL_FUNC = 0x8101,     /* 调用函数出错 */
    WINKS_ENG_ERROR_PROT_NO     ,           /* 服务器下发协议号不匹配 */
    WINKS_ENG_ERROR_SVR_RET     ,           /* 服务器返回数据错误 */
    WINKS_ENG_ERROR_DATA_PARSE  ,           /* 服务器返回数据解析错误 */
    WINKS_ENG_ERROR_DATA_RSV    ,           /* 消化数据失败*/
/* 系统错误 */
    WINKS_MSG_ENG2UI_SVR_NO_ANSWER  = 0x8201,   /* 服务器无应答 */
    WINKS_MSG_ENG2UI_UNREG_USER     ,           /* 非注册用户 */
    WINKS_MSG_ENG2UI_ROAMING_LIMIT  ,           /* 漫游限制  */
    WINKS_MSG_ENG2UI_ENG_BUSY       ,           /* 引擎忙,稍候再试 */
    WINKS_MSG_ENG2UI_DIAL_INT       ,           /* 引擎被电话中断 */
    WINKS_MSG_ENG2UI_STORAGE_LIMIT  ,           /* 存储空间不足 */
    WINKS_MSG_ENG2UI_CC_CLOSED      ,           /* 彩像软件关闭  */
#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_MCARD_NOT_EXIST,           /* 存储卡不存在 */
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    WINKS_MSG_ENG2UI_SYS_FILES_ERR  ,           /* 系统文件被破坏 */
//#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_CC_FILES_ERR   ,           /* 彩像文件被破坏 */
//#endif /* WINKS_SUPPORT_MEMORY_CARD */
#ifdef WINKS_DUAL_SIM
    WINKS_MSG_ENG2UI_SIM_SEL_ERR    ,           /* 提示用户选择要激活的Sim卡 */
    WINKS_MSG_ENG2UI_SIM_UNUSABLE   ,           /* 用户选择的Sim卡不可用 */
#endif /* WINKS_DUAL_SIM */

}Winks_Eng2UiMsg_e;

#define WINKS_ENG_DS_HEAD_LEN  (34)
#define WINKS_ENG_DS_4BYTES_LEN  (4)
#define WINKS_ENG_DS_2BYTES_LEN  (2)
#define WINKS_ENG_DS_LT_LEN  (WINKS_ENG_DS_4BYTES_LEN + WINKS_ENG_DS_2BYTES_LEN)
#define WINKS_HTTP_NULL (-1)
#if 0
#define WINKS_ENGDS_HEAD_SIZE  (8)
#define WINKS_ENGDS_1_MAX_LEN  (1024)
#define WINKS_ENGDS_2_MAX_LEN  (4096)
#define WINKS_ENGDS_3_MAX_LEN  (8192)
typedef struct tag_Winks_EngDSItem_s
{
    unsigned long mem_len;
    struct tag_Winks_EngDSItem_s *next;
    unsigned char data[1];
}Winks_EngDSItem_s;

typedef struct tag_Winks_EngDS_s
{
    unsigned long ds_id;
    unsigned long len;
    Winks_EngDSItem_s *mem_item;
    struct tag_Winks_EngDS_s *next;
}Winks_EngDS_s;

#define WINKS_ENGDS_1_MAX_NUM (84)/*((WINKS_ENGDS_1_MAX_LEN -12)/12)*/
#define WINKS_ENGDS_2_MAX_NUM (340)/*((WINKS_ENGDS_2_MAX_LEN -12)/12)*/
typedef struct tag_Winks_EngDSAItem_s
{
    unsigned long start;
    unsigned long len;
    unsigned char msg_type[2];
    unsigned short next;
}Winks_EngDSAItem_s;

typedef struct tag_Winks_EngDSA_s
{
    struct tag_Winks_EngDSA_s *next;
    unsigned long total_num;
    unsigned long used_num;
    Winks_EngDSAItem_s item[1];
}Winks_EngDSA_s;
#endif

typedef struct tag_Winks_EngDSABaseInfo_s
{
    unsigned long size;
    unsigned char version;
    unsigned char way;
    unsigned short msg_type;
    unsigned short sequence_num;
}Winks_EngDSABaseInfo_s;


#define WINKS_SEND_FRAME_LEN          1024
typedef struct tagWinks_SendFrame_s{
    unsigned char *pSendBuf;    /*存放整个桢数据*/
    unsigned char baseBuff[WINKS_SEND_FRAME_LEN];   /*如果桢比较大,另申请大空间,保存原来申请的地址,等发送完桢再恢复*/
    unsigned long   Len;         /*总长度*/
    unsigned long   Off;         /*偏移*/
    int   ErrCode;     /* 组桢过程出现的出错信息号 (WINKS_ENG_ERROR_CODE_x) */
}Winks_SendFrame_s;

/* 用于对好友列表排序 */
typedef struct {
    char data[WINKS_FRIEND_NO_LEN];
    unsigned int index;
    unsigned int flag;    /* 标识不同的号码类型 */
} wk_SortItem;




 typedef enum tag_Winks_EngineState_e
{
    WINKS_ENGINE_STATE_INIT = 0x00,
    WINKS_ENGINE_STATE_START = 0x01,
    WINKS_ENGINE_STATE_DISABLED = 0x02,
    WINKS_ENGINE_STATE_SHOWA = 0x03,
    WINKS_ENGINE_STATE_ENABLED = 0x04,
    WINKS_ENGINE_STATE_SHOWB = 0x05,
    WINKS_ENGINE_STATE_NUMBER
}Winks_EngineState_e;

typedef enum tag_Winks_EngineSubState_e
{
    WINKS_ENGINE_SUBSTATE_ZERO = 0x00,
    WINKS_ENGINE_SUBSTATE_ONE = 0x01,
    WINKS_ENGINE_SUBSTATE_TOW = 0x02,
    WINKS_ENGINE_SUBSTATE_THREE = 0x03,
    WINKS_ENGINE_SUBSTATE_FOUR = 0x04,
    WINKS_ENGINE_SUBSTATE_FIVE = 0x05,
    WINKS_ENGINE_SUBSTATE_SIX = 0x06,
    WINKS_ENGINE_SUBSTATE_SEVEN = 0x07,
    WINKS_ENGINE_SUBSTATE_EIGHT = 0x08,
    WINKS_ENGINE_SUBSTATE_NINE = 0x09,
    WINKS_ENGINE_SUBSTATE_TEN = 0x0a,
    WINKS_ENGINE_SUBSTATE_ELEVEN = 0x0b,
    WINKS_ENGINE_SUBSTATE_TWELVE = 0x0c,
    WINKS_ENGINE_SUBSTATE_COMPLETE = 0xff,
    WINKS_ENGINE_SUBSTATE_NUMBER
}Winks_EngineSubState_e;

 typedef enum tag_Winks_EngineApp_e
{
    WINKS_ENGINE_APP_NULL = 0x00,       /* 无 */
    WINKS_ENGINE_APP_REG = 0x01,        /* 激活 */
    WINKS_ENGINE_APP_UPDATE_FRIEND,     /* 更新好友录 */
    WINKS_ENGINE_APP_SYNC_SETTING,      /* 同步设置 */
    WINKS_ENGINE_APP_UPLOAD_DIY,        /* 上传DIY */
    WINKS_ENGINE_APP_REREG,             /* 重新激活 */
#ifdef WINKS_SUPPORT_3G
    WINKS_ENGINE_APP_CALL_DOWNLOAD,     /* 电话下载 */
#endif /* WINKS_SUPPORT_3G */
/* 后台服务 */
    WINKS_ENGINE_APP_INIT_CACHE,        /* 初始化Cache */
    WINKS_ENGINE_APP_REBULID_CACHE,     /* 重建Cache */
    WINKS_ENGINE_APP_PERIOD_UPDATE,     /* 定时更新 */
    WINKS_ENGINE_APP_RECRUIT_DOWNLOAD,  /* 补充下载 */
    WINKS_ENGINE_APP_NUMBER
}Winks_EngineApp_e;

typedef struct tag_Winks_ConnectServerInfo_s
{
    unsigned char srv;                                /*0:配置服务器; 1:通讯服务器*/
    unsigned char staDataSrv;                   /*连接通讯服务器的起始下标*/
    unsigned char tryCnt;                           /*试连次数*/
    unsigned char datainFlag;                    /*连接通讯服务器后收到是否收到数据标志*/
    char *data;
    unsigned long len;
}Winks_ConnectServerInfo_s;
    
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

typedef struct tag_Winks_EngineContext_s
{
    WINKS_CHN_ID engChn;                     /*引擎消息通道*/
    int InitSettingOk;                       /*存储的设置文件是否ok*/
    int http;                                /*引擎调用的HTTP句柄*/
    WINKS_TIMER_ID httpTimer;                /*http连网定时器，用于监控调用http的时间*/
    Winks_EngineState_e curState;            /*当前引擎状态*/
    Winks_EngineSubState_e subState;         /*擎状态的子状态*/
    Winks_EngineApp_e   app;                 /* 记录应用 */
    unsigned char isImsiEnabled;             /*标志:用户是否注册winks*/
    unsigned char ifRemindRegister;          /*标志:是否提示用户"注册". o:不提示;1:提示*/
    unsigned char isCallCCOk;                /*标志:电话彩像是否ok 0:未下载, 1:正在下载, 2:失败, 0xff:成功*/
    //unsigned char   IsPeriodUpdating;        /* 是否正在定时更新 [0]没有 [1]定时更新状态1,2 [2]定时更新更新好友 */
    unsigned char   pad;/* 2字节对齐 */
    unsigned long   lastState;
#ifdef WINKS_DUAL_SIM
    unsigned short SimStatus;                /* 高字节低2位表示已获取哪张卡的IMSI，b0--卡1 b1--卡2
                                                低字节表示当前激活的是哪张卡，      1--卡1  2--卡2 */
    int            CurInquireSimNo;          /* 当前查询的Sim卡号: 
                                                1--Sim卡1(对应Winks_GetImsi); 
                                                2--Sim卡2(对应Winks_GetImsi_2) */
    int            OtherSimRetryCnt;         /* 获取1卡imsi后, 重试读取另一卡imsi的次数, 2次未取到认为卡不存在 */
    int            NeedRestoreWapSimSel;     /* 保存手机平台的Wap Sim选择 */
    int            SavePhoneWapSimSel;       /* 保存手机平台的Wap Sim选择 */
#endif /* WINKS_DUAL_SIM */
    WinksStrgFileMap_s  StrgFileMap;
    Winks_Setting_s setting;                 /*全局设置信息*/
//    Winks_FriendList_in_RAM_s friendlist_ram;   /* 内存中的好友录 */
    /*Winks_EngDS_s *socketData_p;                   */ /*HTTP传输的数据*/
    void *communicationContext_p;            /*协议通讯上下文*/
    Winks_ConnectServerInfo_s server;        /*协议重连机制*/
    WINKS_TIMER_ID  PeriodPushTimer;         /*定时更新Timer*/
    WINKS_TIMER_ID  BuildCacheTimer;         /*创建彩像Cache的Timer*/
    WINKS_TIMER_ID  CallWaitTimer;           /*电话事件等待服务器返回彩像的Timer*/
    unsigned short  ProtocolMsgType;          /* 记录发送的协议MsgType */
    unsigned short  ProtocolFrameNo;          /* 记录发送的协议桢号 */
    unsigned short  getImsiCount;             /* 取IMSI失败后，延时重取TIMER */
    WINKS_TIMER_ID  getImsiTimer;             /* 取IMSI失败次数 */
    WinksStrgFile_CCShow_s  * pCCShow;              /* 彩像展示 */
    char    Call_NO[20/*WINKS_STRG_PHONE_LEN*/];   /* 电话事件号码字符串 */
    char    Call_ID[24/*WINKS_STRG_CC_ID_LEN*/];   /* 电话事件彩像ID */
    int     CallType;                        /* 来电或去电 */
    Winks_DateTime_s CallStartTime;          /* 电话事件开始时刻 */
#ifdef WINKS_DEBUG
    unsigned long usbFlagHistory[2];          /* 用于测试插拔USB线的状态。[0]记录次数,[1]记录状态位运算 from d0,*/
    Winks_DateTime_s v_dateTime;              /* 记录上一次定时更新的时间 */
    Winks_DateTime_s v_dateTime1;             /* 记录上一次定时更新服务关闭的时间 */
    int PTCloseCause;              /* 记录定时更新服务关闭原因 1--用户关 2--状态回滚 3--服务器不支持 4--Timer资源申请不到 5--彩像关闭*/
    char    ImsiCode[2][16];
#endif
}Winks_EngineContext;

#define WINKS_CACHETIMER_RETRY_SLOT     90

#define WINKS_CALLTIMER_WAIT_SLOT     5

#define WINKS_PERIODTIMER_RETRY_SLOT    120

#define WINKS_MAX_LEN_OF_MSG_GATEWAY 48
#define WINKS_ENGINE_RETURN_MSG_MAX_LEN (1024)
typedef struct tag_Winks_EngPrtReg_s
{
    char msgGateway[WINKS_MAX_LEN_OF_MSG_GATEWAY];       /*发送注册短信的网关*/
    unsigned long shortTime;  /*查询注册成功的短周期*/
    unsigned long longTime;  /*查询注册成功的长周期*/
    unsigned long count;  /*查询注册成功的计数器*/
    WINKS_TIMER_ID timerId;            /*查询注册成功的定时器*/
    Winks_server_list_s server;
    unsigned char msg[WINKS_ENGINE_RETURN_MSG_MAX_LEN];
}Winks_EngPrtReg_s;

#define WINKS_ENGINE_MAX_LEN_OF_UPLOAD (2048)
#define DIY_UPLOAD_RETRY_MAX        3
typedef struct tag_Winks_EngDiyComInfo_s
{
    char number[WINKS_FRIEND_NO_LEN]; /*定制好友的手机号码*/
    unsigned char image_type;   /*彩像类型*/
    unsigned char image_count;                              /*彩像总张数(n)*/
    char image_files[WINKS_MAX_NUM_OF_IMAGE_LIST][WINKS_PATH_MAX];
    char welcome[WINKS_MAX_LEN_OF_WELCOME_INFO]; /*问候语*/

    unsigned char   curIndex;               /*当前上传第几张(x/n)*/
    /*unsigned char   sendPercent;*/            /*已发送数据的百分比(100为全部发送)*/
    unsigned char   RetryCnt;           /*上传出错重传计数*/
    unsigned long curFileSize;     /*当前上传文件的大小(size of(x))*/
    unsigned short  subIndex;               /*当前上传的为第几块(y/m)*/
    unsigned short  subTotalCount;          /*当前彩像分为几块(将curIndex分为m块)*/
    unsigned long   curSubSize;
}Winks_EngDiyComInfo_s;

typedef struct tag_Winks_EngDiyProgInfo_s
{
    unsigned char   image_count;            /*彩像总张数(n),目前只为1，可不考虑*/
    unsigned char   curIndex;               /*当前上传第几张(x/n),目前只为1，可不考虑*/
    unsigned short  subTotalCount;          /*当前彩像分为几块(将curIndex分为m块)*/
    unsigned short  subIndex;               /*当前上传的为第几块(y/m)*/
}Winks_EngDiyProgInfo_s;

typedef struct tag_Winks_EngUpdComInfo_s
{
    unsigned long   patchTotal;         /*分片下载的总片数*/
    unsigned long   patchCount;         /*分片下载的计数器*/
    unsigned long   prtRetIndex;        /*服务器返回的序列号*/
}Winks_EngUpdComInfo_s;

/* 好友列表通知类型 */
typedef enum {
    E_FL_UPD_NONE = 0,  /* 无更新 */
    E_FL_UPD_NOTIFY,    /* 有更新 */
    E_FL_UPD_SELFRIEND, /* 选择好友录 */
    E_FL_UPD_IF_SELMCARD   /* 是否选择存储卡 */
} Winks_FLNotifyType_e;

typedef struct tag_Winks_EngPeriodComInfo_s
{
    unsigned char   FA_UpdFlag;         /*用户好友关系更新标志 0--无更新,1--有更新,2--选择好友录*/
    int   Reserved;                     /*保留 -- 上传手机统计信息请求*/
}Winks_EngPeriodComInfo_s;

typedef struct tag_Winks_EngTimeFlag_s
{
    unsigned long Type; /* 低4位依次表示哪种时间戳:名片(bit0)、好友彩像(bit1)、片头彩像(bit2)、运营商彩像(bit3) */
    char TimeFlag[WINKS_MAX_LEN_OF_TIME_FLAG];
}Winks_EngTimeFlag_s;

#if 0
typedef struct tag_Winks_EngRegComInfo_s
{
    char msgGateway[WINKS_MAX_LEN_OF_MSG_GATEWAY];       /*发送注册短信的网关*/
    unsigned long shortTime;  /*查询注册成功的短周期*/
    unsigned long longTime;  /*查询注册成功的长周期*/
    unsigned long count;  /*查询注册成功的计数器*/
    WINKS_TIMER_ID timerId;            /*查询注册成功的定时器*/
    unsigned short priCCImageTotal;   /*更新片头彩像个数*/
    unsigned short priCCImageCount;   /*更新片头彩像计数器*/
    unsigned short busCCImageTotal; /*更新运营商彩像个数*/
    unsigned short busCCImageCount; /*更新运营商彩像计数器*/
    unsigned long  filesize;        /*上传的文件大小*/
    unsigned short patchTotal;          /*分片下载的总片数*/
    unsigned short patchCount;        /*分片下载的计数器*/
    unsigned short patchSize;       /*当前包的大小*/
    unsigned short imageTotal;
    unsigned short imageCount;
    unsigned char msg[WINKS_ENGINE_RETURN_MSG_MAX_LEN];
}Winks_EngRegComInfo_s;

typedef struct tag_Winks_EngUpdComInfo_s
{
    unsigned short priCCImageTotal;   /*更新片头彩像个数*/
    unsigned short priCCImageCount;   /*更新片头彩像计数器*/
    unsigned short busCCImageTotal; /*更新运营商彩像个数*/
    unsigned short busCCImageCount; /*更新运营商彩像计数器*/
    unsigned short patchTotal;          /*分片下载的总片数*/
    unsigned short patchCount;        /*分片下载的计数器*/
    unsigned short imageTotal;
    unsigned short imageCount;
}Winks_EngUpdComInfo_s;

#endif

typedef struct tag_Winks_EngDLCCInfo_s
{
    unsigned short   Total;         /* 需要下载的彩像总数 */
    unsigned short   Count;         /* 下载彩像计数 */
    unsigned short   FinCnt;        /* 下载成功彩像数 */
    unsigned short   ErrCnt;        /* 下载失败彩像数 */
}Winks_EngDLCCInfo_s;

#if 1
typedef struct tag_Winks_EngCCDownloadContext1_s
{
    WinksStrgFile_NeedDown_s ND;
#define ND_NO           ND.NO
#define ND_ID           ND.ID
#define ND_cc_type      ND.cc_type
#define ND_img_cnt      ND.img_cnt
#define ND_interval     ND.interval

    unsigned char   img_idx;    /* [O/I]第几张图片 */
    unsigned char   img_type;   /* [O/I]彩像文件类型 */
    unsigned char   GreetLen;   /* [Sum]问候语长度 */
    unsigned char   GreetValid; /* [I]问候语是否有效 */
    unsigned short  CC_Idx;
    unsigned short  GreetStr[101];/* [O] */
    unsigned long   FileSize;   /* [O]文件大小 */
    int             ReqIdx;
    
    Winks_EngDLCCInfo_s DLS;
#define DLS_Cnt         DLS.Total
#define DLS_Idx         DLS.Count
#define DLS_Fin         DLS.FinCnt
#define DLS_Err         DLS.ErrCnt

}Winks_EngCCDownloadContext1_s;
#endif

typedef struct tag_Winks_EngineRetMsgBody_s
{
    unsigned char msg[WINKS_ENGINE_RETURN_MSG_MAX_LEN];

}Winks_EngineRetMsgBody_s;

typedef struct tag_Winks_EngineToUiReturnMsg_s
{
    unsigned long err_state;
    unsigned long err_code;
    char    msg_body[1];
}Winks_EngineToUiReturnMsg;

typedef int (* Winks_EngAnlArr_f)(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data);
typedef int (* Winks_EngAnlEnd_f)(unsigned long *ret_code, void *ret_data);
typedef struct {
    /* 消息类型 */
    unsigned short msgType;
    /* 数据解析函数 , 不能为NULL*/
    Winks_EngAnlArr_f parseFunc;
    /* 处理解析函数的返回数据 */
    Winks_EngAnlEnd_f retData;
} winks_DSA_H_Table;

typedef struct {
    unsigned short type;    /* 结构字段属性 */
    unsigned long len;      /* 有效数据长度 */
    unsigned long offset;   /* 偏移 */
} wink_DSA_StructInfo;


typedef struct tag_Winks_CcReq_s/*用于请求彩像资源*/
{
    char            ID[40/*WINKS_STRG_CC_ID_LEN*/];           /* 彩像ID */
    unsigned char   cc_type;    /* 彩像类型 */
    unsigned char   img_cnt;    /* 共几张图片 */
    unsigned char   img_idx;    /* 第几张图片 */
}Winks_CcReq_s;


/*测试查表结构*/
typedef struct tag_Winks_TestModeTable
{
    char keyString[WINKS_TEST_KEY_WORDS_LEN+1];
    int key;
}Winks_EngDSItem_s;


/*引擎初始化*/
int Winks_EngineInit(void);
/*引擎注销*/
int Winks_EngineDestroy(void);

#ifdef WINKS_DUAL_SIM
/* 当两张卡时,用户选择哪张卡激活 */
int Winks_UserSetActiveSim(int SimNo);
#endif /* WINKS_DUAL_SIM */
/*用户注册彩像业务，供UI使用。返回值:0成功,-1失败。*/
int Winks_UserRegist(void);

/*用户更新好友列录*/
int Winks_UserResetFriendList(void);

/*保存用户选择好友录，friend_list:用"|"分隔的号码.例如"13900000001|13900020002|"*/
int Winks_UserSetFriendList(char *friend_list, unsigned long friend_list_len);

#if 0//def WINKS_SUPPORT_MEMORY_CARD
/* 当手机空间不足且存储卡空间充足时询问用户是否使用存储卡,当选择存储卡时调用此函数 */
int Winks_UserSelMCard(int YesOrNo);
#endif /* WINKS_SUPPORT_MEMORY_CARD */
#if 1
int Winks_UserSetFriendBegin(void);
int Winks_UserSetFriendAddItem(Winks_FriendItem_s *item);
int Winks_UserSetFriendEnd(int commit_flag);
#endif
/*取漫游限制连网的标志. allowedFlag:0限制;1不限制*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag);

/*判断是否注册用户。 regFlag:0未注册，1已注册*/
int Winks_isUserRegistered(unsigned char *regFlag);

/*用户DIY设置*/
typedef struct tag_Winks_DiyInfo_s
{
    char number[WINKS_FRIEND_NO_LEN]; /*定制好友的手机号码*/
    unsigned char image_type;   /*彩像类型*/
    unsigned char image_count;
	char image_files[WINKS_MAX_NUM_OF_IMAGE_LIST][WINKS_PATH_MAX]; /* UTF-8 编码 */
	char welcome[WINKS_MAX_LEN_OF_WELCOME_INFO]; /* 问候语(UTF-8 编码) */
} Winks_DiyInfo_s;
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info);

#ifdef WINKS_DEL_PV2_0    
/*用户个性化设置: 同步服务器*/
typedef struct tag_Winks_SelfSetting_s
{
    /* 1: 开启, 0: 关闭 */
    unsigned char ccSwitch;  /* 对方展示我的彩像(上传服务器+本地保存) */
    unsigned char ShowCC;  /* 我展示对方彩像(本地保存) */
} Winks_SelfSetting_s;
int Winks_UserUploadSelfSetting(Winks_SelfSetting_s *self_info);
#else
typedef struct tag_Winks_SelfSetting_s
{
    /* 1: 开启, 0: 关闭 */
    unsigned char ccSwitch;  /* 对方展示我的彩像(上传服务器+本地保存) */
    unsigned char ShowCC;  /* 我展示对方彩像(本地保存) */
} Winks_SelfSetting_s;
#define Winks_UserUploadSelfSetting(x) 
#endif
/* 用户连接方式设置:Push方式或定时更新 */
int Winks_UserSetConnectWay(unsigned long ConnWay);

/*取当前接入点*/
int Winks_GetCurAccessPoint(unsigned long *accessPoint);
/*设置当前接入点*/
int Winks_SetCurAccessPoint(unsigned long accessPoint);

#define WINKS_REAL_TIME_INFO_OPT_USER_STATE (1UL)
#define WINKS_REAL_TIME_INFO_OPT_SHOW_FLAG (2UL)
#define WINKS_REAL_TIME_INFO_OPT_CARD_SWITCH (4UL)
#define WINKS_REAL_TIME_INFO_OPT_USER_STATE_MSG (8UL)
/*实时信息结构，同步服务器*/
typedef struct tag_Winks_RealTimeInfo_s
{
    unsigned long opt;
    unsigned char userState;    /*      用户状态标志:
                                正常 ： 01H；
                                离开 ： 02H；
                                会议 ： 03H；
                                忙碌 ： 04H；
                                不便 ： 05H
                            */
    unsigned char showFlag; /*  "对方是否展示我的彩像"功能开关:
                                01H : 用户开启彩像服务
                                02H : 用户关闭彩像服务
                            */
    unsigned char cardSwitch;   /*  "是否展示随电名片"功能开关:
                                允许公开 ：     01H
                                不允许公开为：  02H
                            */
    unsigned char userStateMsg[1];/*用户状态文字:(预留接口，暂不使用)
                            */
}Winks_RealTimeInfo_s;

#define winks_retLen_EQ_msgLen(rlen, msgLen) do\
{ \
    if ((unsigned int)rlen != (unsigned int)msgLen) \
    { \
        Winks_fail("rlen != msgLen"); \
        *ret_code = WINKS_ENG_ERROR_CODE_4; \
        return 0; \
    } \
} while (s_do_while)

static int winks_EngInitCCCache();
static int winks_EngRebulidCCCache();
static int winks_EngRecruitDownload();

static int SendBufReInit(Winks_SendFrame_s* pSF, int NewSize);
static int SendBufRestore(Winks_SendFrame_s *pSF);
static int AddSendFrameHead(Winks_SendFrame_s* pSF, unsigned short msg_type,unsigned short sequence_num);
static int AddBaseAttr(Winks_SendFrame_s* pSF, unsigned short AttrId,unsigned char *pStrean, int Size);
static int AddL1Attr(Winks_SendFrame_s* pSF,unsigned short AttrId, void *pData);
static int AddL2Attr(Winks_SendFrame_s* pSF,unsigned short AttrId, void *pData);
static int AddL3Attr(Winks_SendFrame_s* pSF,unsigned short AttrId, void *pData);
static int AddFrameItem(Winks_SendFrame_s* pSF, unsigned short AttrId, void *pData);
static int AddSendFrameData(Winks_SendFrame_s* pSF, unsigned short AttrId, void *pData);
static char *GetProtocolLanguage(void);

static int winks_SendData(unsigned char server,char *data, unsigned long len);
static int winks_BootingCheckOfSimCard(char *imsiCode, unsigned long len);
static int winks_EngineSendDataToServer(unsigned long state, unsigned long sub_state, void *data);

static void winks_StartBuildCacheTimer(void);
static void winks_StopBuildCacheTimer(void);
static void winks_StartCallWaitTimer(void);
static void winks_StopCallWaitTimer(void);
static int winks_StartPeriodPushTimer(void);
static int winks_DelayPeriodPushTimer(void);
static int winks_StopPeriodPushTimer(int cause);
static int Winks_SubStateRollback(void);
static int Winks_CurStateRollback(int ClearAll);
static unsigned long winks_GetDsaLong(unsigned long *oRet, const unsigned char *buff, unsigned long offset,  unsigned long len);
static unsigned short winks_GetDsaShort(unsigned short *oRet, const unsigned char *buff, unsigned long offset,  unsigned long len);
static int winks_SetDsaLong(unsigned char *obuff, unsigned char buff_len,  unsigned long len);
static int winks_SetDsaShort(unsigned char *obuff, unsigned char buff_len,  unsigned short len);

static int winks_EngDataStreamAnalyze(int http, unsigned long length, unsigned long *ret_code);
static unsigned long winks_EngDsaDo_struct(int http, unsigned long offset, unsigned long *ret_code, wink_DSA_StructInfo *data_info);
static int winks_EngDsaDo_M_array(int http, unsigned long offset, unsigned short msgType, unsigned long len, unsigned long *ret_code);

/* 3.1.15~3.1.19 */
static int winks_EngDsaDo_LFFEB(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFEC(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFED(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFEF(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFF0(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFF9(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFFD(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFFF(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFFA(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFF8(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFF7(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);
static int winks_EngDsaDo_LFFF6(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData);


static int winks_EngDsaDo_M8005(int http, unsigned long offset, unsigned long len, unsigned long *ret_code, Winks_ConnectWayOperation_s *cwo);
static int winks_EngDsaDo_M8008(int http, unsigned long offset, unsigned long len, unsigned long *ret_code);
static int winks_EngDsaDo_M8009(int http, unsigned long offset, unsigned long len, unsigned long *ret_code);


static int winks_EngDsaDo_HFFF8(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data);
static int winks_EngDsaDo_HFFFA(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data);
static int winks_EngDsaDo_HFFFB(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data);
static int winks_EngDsaDo_HFFFC(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data);
static int winks_EngDsaDo_HFFFF(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data);


static int winks_retData_HFFF8(unsigned long *ret_code, void *data);
/*
static int winks_EngDSCreate(unsigned long *ds_id);
static int winks_EngDSDestroy(unsigned long ds_id);
static int winks_EngDSInit(int http, unsigned long len, unsigned long ds_id);
*/
static int Winks_TestModeManager(char * keyStr);

#endif /*_WK_ENFNC_H_*/
