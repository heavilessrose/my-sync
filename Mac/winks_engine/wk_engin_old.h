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

typedef struct tag_Winks_EngineToUiReturnMsg_s
{
	unsigned long err_state;
	unsigned long err_code;
	char    msg_body[1];
}Winks_EngineToUiReturnMsg;

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

#define WINKS_ENGINE_RETURN_MSG_MAX_LEN (1024)
typedef struct tag_Winks_EngineRetMsgBody_s
{
    unsigned char msg[WINKS_ENGINE_RETURN_MSG_MAX_LEN];

}Winks_EngineRetMsgBody_s;

typedef struct tag_Winks_EngUpdComInfo_s
{
    unsigned long   patchTotal;         /*分片下载的总片数*/
    unsigned long   patchCount;         /*分片下载的计数器*/
    unsigned long   prtRetIndex;        /*服务器返回的序列号*/
}Winks_EngUpdComInfo_s;

typedef struct tag_Winks_EngDLCCInfo_s
{
    unsigned short   Total;         /* 需要下载的彩像总数 */
    unsigned short   Count;         /* 下载彩像计数 */
    unsigned short   FinCnt;        /* 下载成功彩像数 */
    unsigned short   ErrCnt;        /* 下载失败彩像数 */
}Winks_EngDLCCInfo_s;

typedef struct tag_Winks_EngDiyProgInfo_s
{
    unsigned char   image_count;            /*彩像总张数(n),目前只为1，可不考虑*/
    unsigned char   curIndex;               /*当前上传第几张(x/n),目前只为1，可不考虑*/
    unsigned short  subIndex;               /*当前上传的为第几块(y/m)*/
    unsigned short  subTotalCount;          /*当前彩像分为几块(将curIndex分为m块)*/
}Winks_EngDiyProgInfo_s;

/* with end char '\0' */

/* 服务器下发的彩像ID最大长度 */
#define WINKS_MAX_LEN_OF_COLOR_CALL_ID 40 // 52
/* 一个彩像中的图片最大个数 */
#define WINKS_MAX_NUM_OF_IMAGE_LIST 1 /*4*/

/* 问候语字符最大个数 */
#define WINKS_MAX_NUM_OF_WELCOME_INFO 100
/* 问候语最大长度(UTF-8 编码) */
#define WINKS_MAX_LEN_OF_WELCOME_INFO (100 * 3 + 1) /* bytes */

/* 好友号码最大长度 */
#define WINKS_FRIEND_NO_LEN 40
/* 好友名字最大长度 */
#define WINKS_FRIEND_NAME_LEN 24 // 20

/* 本机号码最大长度 */
#define WINKS_LOCAL_NO_LEN 40

/* 彩像类型 */
typedef enum {
    E_CC_Friend = 1,      /* 好友彩像 */
    E_CC_Title = 2,       /* 片头彩像 */
    E_CC_Operator = 3    /* 运营商彩像 */
} Winks_CcType_e;

/* 设置选项 */
typedef enum {
    E_SETTING_NULL = 0,

    E_SETTING_INTEGER = 100,
    E_SETTING_SHOW_CC_TITLE,
    E_SETTING_SHOW_CTRL,        /* 展示设置 */
    E_SETTING_SHOW_CASE,        /* 通话设置 */
    E_SETTING_CONNECT_POINT,    /* 网络接入点 */
    E_SETTING_ROAM_SWITCH,      /* 漫游开关 */
    E_SETTING_CLIENT_CONNECT_WAY,       /* 终端连接方式 */
    E_SETTING_CLIENT_CONNECT_TIMEOUT,   /* 终端连接超时 */

    E_SETTING_PUSH_PORT,        /* PUSH 端口 */
    E_SETTING_LONG_CYCLE,       /* 长周期 */
    E_SETTING_SHORT_CYCLE,      /* 短周期 */
    E_SETTING_BCARD_ATTR,           /* 本机名片属性 */
    E_SETTING_FRIEND_LIST_MAX_CNT,  /* 手机预存好友记录最大值 */
    E_SETTING_TaO_MAX_CNT,          /* 手机预存运营商和片头最大值 */
    E_SETTING_TEMP_FILE_CNT,         /* 预占的临时文件个数 */
    E_SETTING_STORAGE_LOCATION,     /* 彩像存储位置 */
//    E_SETTING_GREET_UPDATE_FLAG,    /* 问候语更新标志 */

    E_SETTING_STRING = 200,
    E_SETTING_STR_PHONE_NO,     /* 本机号码 */
    E_SETTING_STR_SMS_GW,       /* 短信网关号码 */
    E_SETTING_STR_TIME_FLAG,     /* 时间戳 */

    E_SETTING_STRUCT = 300,
    E_SETTING_STRUCT_COMM_SERVER,       /* 通讯服务器 */
    E_SETTING_STRUCT_PROXY_SERVER,      /* HTTP 代理服务器 */
    E_SETTING_STRUCT_CONNECT_WAY,      /* 服务器连接方式 */

    E_SETTING_ALL
} Winks_SettingType_e;

typedef enum {
    E_GFLAG_NORMAL = 0,     /* 正常 */
    E_GFLAG_NO_MC = -100,   /* 没有存储卡 */
    E_GFLAG_NEW_MC = -101  /* 更换存储卡 */
} Winks_GreetingFlag_e;

/* 不再使用, 即将删除 */
#define E_I_SHOW_OTHER 0x01
#define E_OTHER_SHOW_ME 0x02

/* 展示设置 */
typedef enum {
    E_CC_SHOW_OFF = 0,
    E_CC_SHOW_ON = 1
} Winks_ShowCtrl_e;

/* 通话设置 */
typedef enum {
	E_CALL_CONNECT = 0x01,
	E_CALL_TALK = 0x02,
	E_CALL_HANGUP = 0x04
} Winks_ShowCase_e;

/* 彩像存储位置 */
typedef enum
{
    E_STORAGE_PHONE = 0,    /* 手机 */
    E_STORAGE_MCARD         /* 存储卡 */
} Winks_StorageLocation_s;

/* 漫游开关 */
typedef enum
{
	 WINKS_ROAM_SWITCH_OFF = 0,
	 WINKS_ROAM_SWITCH_ON = 1
 } Winks_RoamSwitch_s;

/* 声音开关 */
typedef enum
{
	WINKS_AUDIO_SWITCH_OFF = 0,
	WINKS_AUDIO_SWITCH_ON = 1
} Winks_AudioSwitch_s;

/* 彩像图片类型 */
typedef enum {
	E_IMAGE_JPG = 1,
	E_IMAGE_PNG,
	E_IMAGE_TIFF,
	E_IMAGE_SPL,
	E_IMAGE_SWF,
	E_IMAGE_GIF,

	E_IMAGE_PPT
} Winks_CcImageType_e;

typedef struct {
	char ID[WINKS_MAX_LEN_OF_COLOR_CALL_ID];	/* 彩像ID */
	unsigned short show_times;		/* 展示次数 */
	unsigned short int_times;		/* 被中断次数 */
	unsigned long time;				/* 展示时间 */
} Winks_CC4Stat_s;

/* DIY彩像 */
typedef struct
{
	char number[WINKS_FRIEND_NO_LEN]; /*定制好友的手机号码*/
	unsigned char image_type;	/*彩像类型*/
	unsigned char image_count;
	char image_files[WINKS_MAX_NUM_OF_IMAGE_LIST][WINKS_PATH_MAX]; /* UTF-8 编码 */
	char welcome[WINKS_MAX_LEN_OF_WELCOME_INFO]; /* 问候语(UTF-8 编码) */
} Winks_DiyInfo_s;

/* 本机呼叫方向 */
typedef enum {
	E_DIAL_IN = 1,
	E_DIAL_OUT
} Winks_DialDirection_e;

#if 0
typedef struct {
	unsigned short index;		/* 彩像索引 */
	unsigned short _pad;		/* for 4 bytes alignment */

	unsigned char img_type;		/* 图片类型 */
	unsigned char img_cnt;		    /* 图片张数 */
	unsigned short interval;		/* 多张图片的播放间隔 */
	unsigned long play_time;		/* 播放时间(片头使用) */

    char img_path[WINKS_MAX_NUM_OF_IMAGE_LIST][WINKS_PATH_MAX]; /* 图片存储路径(UTF-8 编码) */
    char grt[WINKS_MAX_LEN_OF_WELCOME_INFO];        /* 问候语(UTF-8 编码) */
    Winks_GreetingFlag_e GFlag;      /* 是否特殊问候语 */
} Winks_CC4UI_s;
#else
typedef struct tag_WinksStrgFile_CCShow_s
{
    char            NO[16/*WINKS_STRG_PHONE_LEN*/];    /* 电话号码字符串 */
    char            Name[32/*WINKS_STRG_NICK_LEN*/];   /* 好友昵称 */
    char            ID[40/*WINKS_STRG_CC_ID_LEN*/];    /* 彩像ID */
    unsigned short  CC_Idx;     /* 彩像Idx */
    unsigned short  interval;   /* 图片播放间隔 */
    unsigned char   img_type;   /* 彩像类型 */
    unsigned char   img_cnt;    /* 图片数 最大为4 */
    unsigned char   GreetLen;   /* 问候语长度 */
    unsigned char   SaveType;   /* 彩像存储类型 0--文件 1--内存 */
    unsigned char * pCCImage;   /* 存储的彩像内存数据 */
    unsigned long   CCImageSize;/* 存储的彩像内存数据大小 */
    char            CCImgPath[4][256];     /* 存储的彩像路径 */
    short           GreetStr[101/*WINKS_STRG_GREET_LEN/2*/];     /* 存储的彩像问候语 */
}WinksStrgFile_CCShow_s;
#endif

typedef struct tag_Winks_FriendItem_s
{
	char number[WINKS_FRIEND_NO_LEN];	/* 好友手机号码 */
	char name[WINKS_FRIEND_NAME_LEN];	/* 好友昵称 */
    unsigned long flag;	           /* 是否在本地好友中*/
} Winks_FriendItem_s;


/**
*   将utf8格式的字符串转换为unicode。返回转换的unicode 字符个数, -1表示失败。

*   @in_utf8: 输入的utf8字符串
*   @out_unicode: 输出的unicode字符串, 小端方式
*   @size: 输出的可用长度
*/
int Winks_Utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size);

int Winks_set_setting_value(Winks_SettingType_e type, unsigned long value);
int Winks_get_setting_value(Winks_SettingType_e type, unsigned long *value);

/*获取是否展示片头标志。1: 播放, 其它: 不播*/
int Winks_if_show_titleCC(void);

/*获取是否展示彩像标志*/
int Winks_if_show_cc(void);

/* 判断彩像是否打开 */
int Winks_if_open_cc(void);

/* 设置播放时机 */
int Winks_set_show_case(unsigned long flag);

/* 获取播放时机 */
int Winks_get_show_case(unsigned long *flag);

/*判断号码是否紧急号码函数*/
int Winks_if_emergency_number(const char *no);

/* 获取片头和运营商彩像 */
//int Winks_next_title_cc(Winks_CC4UI_s *cc4ui);
//int Winks_next_operator_cc(Winks_CC4UI_s *cc4ui);

/* 获取默认彩像 */
//int Winks_get_default_cc(Winks_CC4UI_s *cc4ui);
/* 获取好友彩像 */
//int Winks_get_friend_cc(const char *friend_no, Winks_DialDirection_e direction, Winks_CC4UI_s *cc4ui);
//int Winks_get_friend_name(const char *friend_no, char *friend_name, unsigned long len);
//int Winks_modify_friend_name(const char *friend_no, const char *friend_name);

/* 获取本地好友录 */
//int Winks_get_friend_cnt(void);
//int Winks_get_friend_list(Winks_FriendItem_s *array, unsigned long begin, unsigned long cnt);

/* 记录此次彩像播放情况。interrupted: 0 - 未打断, 1 - 被打断  */
//int Winks_log_cc_show(unsigned long index, unsigned long time, unsigned char interrupted);

/*
*   排序好友列表。
*   NOTE: 要求array中的number与name是unicode编码。
*/
//void Winks_SortFriendList(Winks_FriendItem_s *array, unsigned int count);

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

/*用户确认注册，发送注册短信息,0:取消注册，1:发送注册*/
int Winks_UserSendRegSms(int reg_flag);

/*用户更新好友录：获取服务器下发的好友列表成员个数*/
//int Winks_GetFullFriendListCount(void);

/*用户更新好友录：取好友列表中的好友*/
//int Winks_UserGetFullFriendItem(Winks_FriendItem_s *item, unsigned long index);

/*保存用户选择好友录，friend_list:用"|"分隔的号码.例如"13900000001|13900020002|"*/
int Winks_UserSetFriendList(char *friend_list, unsigned long friend_list_len);

#if 0//def WINKS_SUPPORT_MEMORY_CARD
/* 当手机空间不足且存储卡空间充足时询问用户是否使用存储卡,当选择存储卡时调用此函数 */
int Winks_UserSelMCard(int YesOrNo);
#endif /* WINKS_SUPPORT_MEMORY_CARD */

/*用户更新好友列录*/
int Winks_UserResetFriendList(void);
/*用户DIY设置:上传服务器*/
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info);
int Winks_UserGetCCShow(char *pNo, int dir, WinksStrgFile_CCShow_s *pCCShow);
int Winks_UserStopCCShow();
/*设置开机不提示用户注册。*/
int Winks_UserRememberImsi(void);
/*停止引擎*/
int Winks_EngineStop(void);
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
/* 关闭彩像 */
int Winks_UserCloseWinkCC(void);
/* 开启彩像 */
int Winks_UserOpenWinkCC(void);
/* 用户连接方式设置:Push方式或定时更新 */
int Winks_UserSetConnectWay(unsigned long ConnWay);

/* 获取下载彩像的进度 */
int Winks_GetDLCCProgress(Winks_EngDLCCInfo_s *pDLCCInfo);

/*取当前接入点*/
int Winks_GetCurAccessPoint(unsigned long *accessPoint);
/*设置当前接入点*/
int Winks_SetCurAccessPoint(unsigned long accessPoint);

/*获取好友最大个数函数*/
int Winks_GetMaxCountOfFriendList(unsigned long *count);

/* 无效端口 */
#define WINKS_INVALID_PORT (0xFFFF + 1)

typedef struct {
    char ip_addr[WINKS_IP_ADDR_LEN];
    unsigned long port;
} Winks_ProxyServer_s;

#ifndef WINKS_APN_WITH_PROXY_IN_PLATFORM
/* 保存HTTP代理服务器 */
int Winks_SetProxyServer(const Winks_ProxyServer_s *proxy);
#endif/*WINKS_SUPPORT_APN_BAND_PROXY*/
/* 读取HTTP代理服务器 */
int Winks_GetProxyServer(Winks_ProxyServer_s *proxy);

/*取漫游限制连网的标志. allowedFlag:0限制;1不限制*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag);

/*判断是否注册用户。 regFlag:0未注册，1已注册*/
int Winks_isUserRegistered(unsigned char *regFlag);

/* 服务器提供的终端连接方式 */
typedef enum tag_Winks_ConnectWays_e
{
    CONNECT_WAY_LONG_CONNECT = 1 << 1,    /* 长连接 */
    CONNECT_WAY_LONG_ATTACH = 1 << 2,     /* 长附着 */
    CONNECT_WAY_NON_ATTACH = 1 << 3,      /* 非附着 */
    CONNECT_WAY_SERVER_PUSH = 1 << 4,      /* 短信PUSH */
    CONNECT_WAY_TIMER_WAKE_UP = 1 << 5    /* 定时唤醒 */
} Winks_ConnectWays_e;

typedef struct
{
    unsigned long operation;        /*低16位的每一位表示一种不同的连接方式，高16位表示默认的连接方式。[operation&(operation>>16)!=0]*/
    unsigned long spTimeGiveUp; /*Push方式，服务器放弃终端的时间*/
    unsigned long twuTimeSlot;      /*定时唤醒时间间隔*/
    unsigned long twuTimeLimit;     /*定时唤醒网络超时时间*/

} Winks_ConnectWayOperation_s;

/*获取运营商提供的连接方式的种类函数*/
int Winks_GetSupportedConnectWays(Winks_ConnectWayOperation_s *operation);

/*保存终端连接方式函数*/
int Winks_SetPhoneConnectWay(unsigned long connect_way);

/*获取终端连接方式函数*/
int Winks_GetPhoneConnectWay(unsigned long *connect_way);

/* 函数Winks_check_storage_files() 返回值 */
#define WINKS_STRG_CHECK_RET_0 0        /* 存储文件均存在 */
#define WINKS_STRG_CHECK_RET_1 (-1)     /* 丢失关键文件, 重建成功, 提示用户重新注册 */
#define WINKS_STRG_CHECK_RET_2 (-2)     /* 丢失关键文件, 重建失败, 无法使用彩像软件 */
#define WINKS_STRG_CHECK_RET_3 (-3)     /* 重建非关键文件成功 */
#define WINKS_STRG_CHECK_RET_4 (-4)     /* 重建非关键文件失败 */
#define WINKS_STRG_CHECK_RET_5 (-5)     /* 彩像图片丢失, 可以重新下载 */
#define WINKS_STRG_CHECK_RET_6 (-6)     /* 彩像图片丢失, 无法重新下载 */

/**
*    检测一系列存储文件是否存在。
*   [flag]
*       0: 检查存储配置文件和彩像图片。
        1: 只检查存储配置文件, 不检查彩像图片。
    [fnc return]:
        0: 正常或自修复OK,
        1: 彩像文件丢失，需要重新下载，
        3: 重要文件被破坏，需要重新激活,
*/
//int Winks_check_storage_files(int flag);

#if 0//def WINKS_SUPPORT_MEMORY_CARD
int Winks_check_mc_status(void);
#endif

#ifdef __cplusplus
}
#endif

#endif/*_WK_ENGIN_H_*/
