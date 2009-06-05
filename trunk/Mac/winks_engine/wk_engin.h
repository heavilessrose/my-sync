/*
File:		wk_engin.h
Author:		Huaxm
Create Time:17/03/2008
*/
#ifndef _WK_ENGIN_H_
#define _WK_ENGIN_H_

#ifdef __cplusplus
extern "C" {
#endif

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
    
    WINKS_MSG_ENG2UI_FRIEND         = 71,   /* 正在更新好友录... */
    WINKS_MSG_ENG2UI_FRIEND_OK              /* 好友录更新完毕 */
}WinksEng2UiMsg_Friend_e;
    
typedef enum tag_WinksEng2UiMsg_Diy_e
{
/* 3 上传DIY */
    WINKS_MSG_ENG2UI_DIY_FAIL       = -29,  /* 上传DIY失败 */
    WINKS_MSG_ENG2UI_SCRIPT_FAIL,           /* 定制彩像失败 */

    WINKS_MSG_ENG2UI_DIY            = 21,   /* 正在上传DIY... [WinksProgress_s] */
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

/* 片头运营商按特殊好友处理 */
#define WINKS_DFT_CC_NO     "1"     /* 默认彩像ID */
#define WINKS_PRI_CC_NO     "11"    /* 片头彩像起始ID */
#define WINKS_PRI_CC_MAX    1       /* 片头彩像最大个数 <10 */
#define WINKS_BUS_CC_NO     "21"    /* 运营商彩像起始ID */
#define WINKS_BUS_CC_MAX    1       /* 运营商彩像最大数 <10 */

typedef struct tag_WinksStrgFile_CCShow_s
{
    int             RetStatus;                         /* 彩像是否与该号码对应
                                                          0 -- ok
                                                          1 -- 默认彩像
                                                          2 -- 片头 
                                                          3 -- 运营商 */
    char            NO[16/*WINKS_STRG_PHONE_LEN*/];    /* 电话号码字符串 */
    char            Name[32/*WINKS_STRG_NICK_LEN*/];   /* 好友昵称 */
    char            ID[40/*WINKS_STRG_CC_ID_LEN*/];    /* 彩像ID */
    unsigned short  CC_Idx;     /* 彩像Idx */
    unsigned short  interval;   /* 图片播放间隔 */
    unsigned char   img_type;   /* 彩像类型 WinksFileExt_e*/
    unsigned char   img_cnt;    /* 图片数 最大为4 */
    unsigned char   GreetLen;   /* 问候语长度 */
    unsigned char   SaveType;   /* 彩像存储类型 0--文件 1--内存 */
    unsigned char * pCCImage;   /* 存储的彩像内存数据 */
    unsigned long   CCImageSize;/* 存储的彩像内存数据大小 */
    char            CCImgPath[4][256];     /* 存储的彩像路径 */
    short           GreetStr[101/*WINKS_STRG_GREET_LEN/2*/];     /* 存储的彩像问候语 */
}WinksStrgFile_CCShow_s;

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


/*引擎初始化*/
int Winks_EngineInit(void);
/*引擎注销*/
int Winks_EngineDestroy(void);

#ifdef WINKS_DUAL_SIM
/* 当两张卡时,用户选择哪张卡激活 */
int Winks_UserSetActiveSim(int SimNo);
#endif /* WINKS_DUAL_SIM */


/*-------------------------
        激活流程UI接口
---------------------------*/
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

/*检查升级版本，0:后台升级, 1用户手动升级*/
int Winks_UpgradeStart(int type);

/*用户添加好友或修改好友昵称（昵称为unicode编码）*/
int Winks_StrgFriendAdd(char *pNo, unsigned char *pNick);
/* 获取好友个数 */
int Winks_StrgFriendCnt(void);
/* 读取好友信息，BeginIdx从0开始，将返回值作为BeginIdx继续调用，直至返回为负值，读取全部好友 */
int Winks_StrgFriendGet( int BeginIdx, 
                               char *pNo, int NoLen,
                               unsigned char *pNick, int NickLen);
/*用户删除好友*/
int Winks_StrgFriendDel(char *pNo);
/*用户删除全部好友*/
int Winks_StrgFriendDelAll(void);
/*获取用户彩像信息*/
int Winks_StrgGetCCShow(char *pNo, int dir, WinksStrgFile_CCShow_s *pCCShow);

/* 直接获取用户彩像 */
int Winks_UserGetCCDirectly(char * pNo);
/*用户更新好友录*/
int Winks_UserResetFriendList(void);
int Winks_UserUpdateFriendListStop(void);
/*用户更新cash用户彩像*/
int Winks_UserResetCacheFriendList(void);
int Winks_UserUpdateCacheFriendListStop(void);
/*用户上传DIY及定制给好友
  diy_info->action: 0 - 只上传不定制; 1 - 上传并定制
  diy_info->pnolist: 在action为1时使用，定制好友的手机号码串，以|分隔，为NULL表示所有人 
  diy_info->name: 用户设置的彩像名称，unicode编码，前2字节为0则取文件名
  diy_info->greet字符串为空(Unicode前2字节为0) : 不设置问候语
  diy_info->path字符串为空(Unicode前2字节为0) : 只修改问候语
*/
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info);
int Winks_UserUploadDiyStop(void);
/*上传设置项，备份*/
int Winks_UserUploadSetting(void);
/*下载设置项*/
int Winks_UserDownLoadSetting(void);
/*停止下载或上传设置项*/
int Winks_SynsetStop(void);

/* 输入推荐号码的数组RcmdArr，号码个数为RcmdNum，
   RcmdGreet--邀请问候语 */
int Winks_UserRecommend(int RcmdNum, Winks_RecommendInfo_s * RcmdArr, 
                                  unsigned char * RcmdGreet);
int Winks_UserRecommendStop(void);

/* retBuff -- 返回的内存缓冲区 
   retLen -- 输入内存缓冲区大小，输出实际使用的内存缓冲区大小
   IsDft -- 是否获取默认的配置
*/
int Winks_UserGetCCShowSetup(char * retBuff, int * retLen, int IsDft);

/*获取通话号码的彩像*/
int Winks_UserGetCCShow(char * phonenumber, int dir, WinksStrgFile_CCShow_s *pCCShow);
/*停止展示彩像*/
int Winks_UserStopCCShow(void);

/*设置开机不提示用户注册。*/
int Winks_UserRememberImsi(void);

/* 关闭彩像 */
int Winks_UserCloseWinkCC(void);
/* 开启彩像 */
int Winks_UserOpenWinkCC(void);

/*取漫游限制连网的标志. allowedFlag:0限制;1不限制*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag);

/*判断是否注册用户。 regFlag:0未注册，1已注册*/
int Winks_isUserRegistered(unsigned char *regFlag);
/*判断是否沉默用户。 SilenceFlag:0非沉默，1沉默用户*/
int Winks_isUserSilenced(unsigned char *SilenceFlag);
/* 用户连接方式设置:Push方式或定时更新 */
//int Winks_UserSetConnectWay(unsigned long ConnWay);

/*
打开Wap网页: 彩像商城/我的彩像库/定制业务/购买彩像
输入:
    UrlType -- Winks_UrlType_e
    wid -- 当UrlType为WINKS_URLTYPE_PURCHASE时必须填写该参数，其他为NULL
*/
int Winks_UserOpenWapPage(int UrlType, char * wid);

/*设置通话设置,从不选到某些项被选时需做一次定时更新。反之从选到不选要停止定时更新。
 * function: Winks_SetShowCaseProc
 * parameters:
 *   unsigned long val: 通话设置：WINKS_SETVAL_CCSHOW_RING|WINKS_SETVAL_CCSHOW_TALK|WINKS_SETVAL_CCSHOW_HANGUP
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetShowCaseProc(unsigned long val);

/* 获取定时更新参数 */
int Winks_GetPeriodUpdate(Winks_PeriodUpdateInfo_s * pPeriodUpdateInfo);
/* 设置定时更新参数 */
int Winks_SetPeriodUpdate(Winks_PeriodUpdateInfo_s * pPeriodUpdateInfo);

int Winks_StrgCheckDiskFiles(void);

/* 记录此次彩像展示情况。返回负值表示失败 */
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted);

#ifdef __cplusplus
}
#endif

#endif/*_WK_ENGIN_H_*/
