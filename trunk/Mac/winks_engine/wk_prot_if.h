#ifndef _WK_PROT_IF_H_
#define _WK_PROT_IF_H_

#ifdef __cplusplus
extern "C" {
#endif


///////////////////////////////////////////////////////////////////////////////
//           M A C R O   D E C L A R E
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//           S T R U C T   D E C L A R E
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************
**                    协议解析输出的数据结构
******************************************************************************/
/*
struct Prot_debug_attr_s
{
//unknown
    int     val;
};
*/
typedef struct tag_WinksProt_result_s
{
    int     code;   /* 返回码。code为0时表示操作执行成功。 */
    union {
        char * text;    /* 操作执行失败时的文本描述信息 (utf-8格式) */
        char * greeting;/* 操作执行成功时返回的提示信息 (utf-8格式) */
    }rst_txt;
    char * debug;  /* 调试信息 (utf-8格式) */
    //struct Prot_debug_attr_s * debug;  /* 调试信息 */
}WinksProt_result_s;

struct Prot_sms_s
{
    struct Prot_sms_s * pnext;
    char * number;
    char * prefix;
};

struct Prot_address_s
{
    char  * winks;  /* 彩像业务服务器的URL基地址 */
    //char  * media;  /* 媒体服务器的URL基地址 */
    char  * store;  /* 彩像商城URL基地址 */
  #if 0
    char  * sms;    /* 短信特服号码 */
  #else
    int     sms_num;
    struct Prot_sms_s * sms;
  #endif
    int     push;   /* 终端push接收端口 */
};

enum Prot_subscription_e
{
    WINKS_SUBSCRIPTION_NONE = 0,
    WINKS_SUBSCRIPTION_LIMITED,     /* 受限用户选项 */
    WINKS_SUBSCRIPTION_TRIAL,       /* 试用者选项 */
    WINKS_SUBSCRIPTION_ADWARE,      /* 接受广告用户选项 */
    WINKS_SUBSCRIPTION_SUBSCRIER,   /* 付费用户选项 */
    WINKS_SUBSCRIPTION_MAX
};
/*
struct Prot_subscription_attr_limited_s
{
//unknown
    int     val;
};

struct Prot_subscription_attr_trial_s
{
//unknown
    int     val;
};

struct Prot_subscription_attr_adware_s
{
//unknown
    int     val;
};

struct Prot_subscription_attr_subscriber_s
{
//unknown
    int     val;
};

struct Prot_subscription_s
{
    union{
        struct Prot_subscription_attr_limited_s     limited;
        struct Prot_subscription_attr_trial_s       trial;
        struct Prot_subscription_attr_adware_s      adware;
        struct Prot_subscription_attr_subscriber_s  subscriber;
    }attr;
};
*/
struct Prot_subscription_s
{
    //enum Prot_subscription_e attr;
    unsigned int    limited:1,
                    trial:1,
                    adware:1,
                    subscriber:1;
};

struct Prot_config_s
{
    struct Prot_subscription_s subscription;    /* 服务订阅模式 */
    unsigned int    limited_diy:1,      /* 可以DIY彩像，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    limited_vcard:1,    /* 使用来电名片，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    limited_activity:1, /* 用户场景模式，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    _rsv0:1,

                    trial_diy:1,      /* 可以DIY彩像，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    trial_vcard:1,    /* 使用来电名片，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    trial_activity:1, /* 用户场景模式，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    _rsv1:1,

                    adware_diy:1,      /* 可以DIY彩像，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    adware_vcard:1,    /* 使用来电名片，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    adware_activity:1, /* 用户场景模式，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    _rsv2:1,

                    subscriber_diy:1,      /* 可以DIY彩像，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    subscriber_vcard:1,    /* 使用来电名片，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    subscriber_activity:1, /* 用户场景模式，有此字段是表示打开此功能，值为此功能是否需要订阅 */
                    _rsv3:1,

                    vcash:1;    /* 使用点卡，有此字段是表示打开此功能，值为此功能是否需要订阅 */
};

struct Prot_connection_s
{
    char  * proxy;                  /* 代理服务器IP */
    short   proxyPort;              /* 代理服务器端口 */
    char  * apn;                    /* 网络接入点 */
};

struct Prot_client_s
{
    int     autoSubscribe;
};

/* 彩像服务器配置信息 */
typedef struct tag_WinksProt_ServiceConfig_s
{
    struct Prot_address_s * address;
    struct Prot_connection_s * connection;
    struct Prot_config_s  * config;    /* 服务配置 */
    struct Prot_client_s  * client;
}WinksProt_serviceConfig_s;

/* 更新选项：minFee - 最小资费更新周期；mostRecent - 最快更新更新周期；custom - 自定义 */
typedef enum tag_WinksTag_updateConfig_e
{
    WINKS_updateConfig_none = 0,        /* 空 */
    WINKS_updateConfig_minFee,          /* minFee - 最小资费更新周期； */
    WINKS_updateConfig_mostRecent,      /* mostRecent - 最快更新更新周期； */
    WINKS_updateConfig_custom,          /* custom - 自定义 */
    WINKS_updateConfig_max              
}WinksTag_updateConfig_e;


struct Prot_updateConfig_s
{
    int     option;                     /* WinksTag_updateConfig_e */
    int     period;
};

/* 用户配置信息 */
typedef struct tag_WinksProt_userConfig_s
{
    char  * proxy;                  /* 代理服务器IP */
    short   proxyPort;              /* 代理服务器端口 */
    char  * apn;                    /* 网络接入点 */
    int     durationAfterHangup;    /* 挂机后彩像展示时间（按秒计） */
    unsigned int showWinks:1,       /* 展示设置
                                       true 展示彩像
                                       false 不展示彩像 */
            showMode:3,             /* 通话设置，为下列值组合：
                                       0x01-通话前展示
                                       0x02-通话中展示
                                       0x04-通话后展示 */
            roaming:1,              /* 漫游设置
                                       true 漫游时展示彩像
                                       false 漫游时不展示彩像 */
            autorun:1,              /* 开机自启动设置
                                       true 开机自启动
                                       false 开机不自启动 */
            updateAfterHangup:1,    /* 挂断电话后更新设置
                                       true 挂断后更新彩像内容
                                       false 挂断后不更新彩像内容 */
            upgradeMode:1,          /* 升级方式设置
                                       auto 自动升级
                                       manual 手动升级 */
            vcardOverwrite:1,       /* 随电名片更新方式设置
                                       auto 自动更新
                                       manual 手动更新 */
            connectInCall:1,        /* 电话事件是否联网获取彩像
                                       true 联网获取彩像
                                       false 不联网获取彩像 */
            support3G:1;            /* 是否同时支持3G
                                       true 同时支持2.5G和3G模式
                                       false 只支持2.5G模式 */
    unsigned int showWinks_Mask:1,       
            showMode_Mask:3,             
            roaming_Mask:1,              
            autorun_Mask:1,              
            updateAfterHangup_Mask:1,    
            upgradeMode_Mask:1,          
            vcardOverwrite_Mask:1,       
            connectInCall_Mask:1,        
            support3G_Mask:1;            
    int     networkTimeout;         /* 终端网络连接的超时时间(秒) */
    int     splashDuration;         /* 片头展示时间(秒)，0时不展示 */
    int     updateOptionMinFee;     /* 定时更新最省资费模式,时间周期*/
    int     updateOptionMostRecent; /* 定时更新数据最新模式,时间周期*/
    struct Prot_updateConfig_s * updateConfig;
}WinksProt_userConfig_s;

/* 终端配置 */
typedef struct tag_WinksProt_userConfigEx_s
{
    int     AllOrDiff;      /* 0--diff; 1--all */
    WinksProt_userConfig_s * userConfig;
}WinksProt_userConfigEx_s;


/* 终端配置 */
typedef struct tag_WinksProt_clientConfig_s
{
    void * data;
}WinksProt_clientConfig_s;

/* 消息项 */
struct Prot_mi_s
{
    struct Prot_mi_s * pnext;
    char * mid;
    WinksProt_serviceConfig_s * serviceConfig;
    WinksProt_clientConfig_s  * clientConfig;
};

typedef struct tag_WinksProt_messageList_s
{
    int     mi_num;
    struct Prot_mi_s * mi;
}WinksProt_messageList_s;

struct Prot_unregistered_s
{
    int     registerMethod; /* 注册方法 WinksTag_registerMethod_e */
};

/* 服务类型 */
typedef struct tag_ServicesType_s
{
    enum Prot_subscription_e subscription;    /* 服务订阅模式 */
    unsigned int    diy:1,      /* DIY功能 */
                    vcard:1,    /* 随电名片功能 */
                    activity:1; /* 用户场景模式 */
    unsigned int    vcash;      /* 点卡计数 */
}ServicesType_s;

/* 用户的服务状态信息 */
typedef struct tag_WinksProt_userServiceStatus_s
{
    int     IsRegistered;   /* 0--未注册，1--已注册 */
    union {
        struct Prot_unregistered_s unregistered;   /* 用户未注册 */
        struct{
            ServicesType_s  service;/* 用户已开通的服务 */
            char  * uid;    /* 用户标识 */
        }reg;
    }RegStat;
}WinksProt_userServiceStatus_s;

typedef struct tag_WinksProt_shared_s
{
    int     status;                 /* 我的状态（情景模式） */
    char  * statusText;             /* 状态语 (utf-8格式) */
    char  * activity;               /* 用户当前活动 (utf-8格式) */
    char  * name;                   /* 其他人看到的名称 (utf-8格式) */
    char  * message;                /* 其他人看到的消息 (utf-8格式) */
    unsigned int    showWinks:1,    /* 是否显示彩像 */
                    showVcard:1;    /* 是否显示随电名片 */
    unsigned int    showWinks_Mask:1,
                    showVcard_Mask:1;
}WinksProt_shared_s;

enum Prot_wt_e
{
    WINKS_WT_SPLASH = 0,
    WINKS_WT_CARRIER
};

/* 彩像媒体文件类型 */
typedef struct tag_WinksFileType_s
{
    struct tag_WinksFileType_s * pnext;
    char  * url;    /* 媒体文件的资源位置 */
    int     w;      /* 宽度 */
    int     h;      /* 高度 */
}WinksFileType_s;

/* 名片类型 */
typedef struct tag_VcardType_s
{
    char  * name;       /* 姓名 (utf-8格式) */
    char  * mobile;     /* 真实号码 */
    char  * corporation;/* 公司 (utf-8格式) */
    char  * title;      /* 职位 (utf-8格式) */
    char  * phone;      /* 电话 */
    char  * email;      /* 电子邮件 */
    char  * fax;        /* 传真 */
    char  * address;    /* 地址 (utf-8格式) */
}VcardType_s;

/* 彩像展示信息 */
typedef struct tag_WinksProt_winksShow_s
{
    char  * wid;                    /* 彩像标识 */
    unsigned short    media_num;    /* 彩像张数 */
    unsigned short    thumbnail_num;/* 缩略图数 */
    WinksFileType_s * media;        /* 媒体文件 */
    WinksFileType_s * thumbnail;    /* 彩像缩略图 */
    char *  greeting;       /* 问候语 (utf-8格式) */
    int     greetValid;     /* 问候语是否有效 */
    int     status;                 /* 我的状态（情景模式） */
    char * statusText;     /* 状态语 (utf-8格式) */
    char * message;        /* 个性化签名 */
    VcardType_s * vcard;            /* 名片信息 */
}WinksProt_winksShow_s;

struct Prot_pbi_s
{
    struct Prot_pbi_s * pnext;
    char  * n;  /* 姓名 (utf-8格式) */
    char  * p;  /* 电话号码 */
};

/* 电话本 */
typedef struct tag_WinksProt_phoneBook_s
{
    int     pbi_num;
    struct Prot_pbi_s * pbi;   /* 电话本条目 */
}WinksProt_phoneBook_s;

/* 用户彩像类型 */
typedef struct tag_WinksProt_UserWinksType_s
{
    char  * wid;
    char  * name;  /* 彩像名称 (utf-8格式) */
    int             censor:3,   /* 彩像审核状态 */
                    diy:1;      /* 用户上传彩像标志 */
    unsigned long   duration;
    unsigned long   price;   /* 彩像价格 */
    int     state;  /*彩像状态：
                        0: unrelated - 与用户无关的彩像；
                        1: usable - 可以直接使用的彩像；
                        2: expired - 过期彩像*/
    char  * expire; /* 彩像过期时间 */
}WinksProt_UserWinksType_s;

struct Prot_pno_s
{
    struct Prot_pno_s * pnext;
    char  * p;  /* 电话号码 */
};

typedef struct tag_WinksProt_winksScript_s
{
    char  * wid;    /* 彩像标识列表 */
    char  * greeting;   /* 问候语 (utf-8格式) */
    int     IsForAll;   /* 0--电话列表; 1--所有号码 */
    struct Prot_pno_s * pno;       /*一组电话号码列表*/
}WinksProt_winksScript_s;

struct Prot_url_s
{
    struct Prot_url_s * pnext;
    char  * url;    /* 软件包文件的URL位置 */
};

/* 终端软件包信息 */
typedef struct tag_WinksProt_softwarePackage_s
{
    char  * name;       /* 软件包名称 (utf-8格式) */
    char  * version;    /* 软件包版本 */
    char  * file;       /* 升级url */
    int     optional;   /* optional */
    char  * description;   /* 软件版本发行说明 (utf-8格式) */
}WinksProt_softwarePackage_s;

/* 注册方法：sms - 短信方式注册；auto - 自动识别用户手机号 */
typedef enum tag_WinksTag_registerMethod_e
{
    WINKS_registerMethod_none = 0,  /* 空 */
    WINKS_registerMethod_sms,       /* sms - 短信方式注册； */
    WINKS_registerMethod_push,      /* push - push方式 */
    WINKS_registerMethod_auto       /* auto - 自动识别用户手机号 */
}WinksTag_registerMethod_e;

/* 服务订阅模式类型 */
typedef enum tag_WinksTag_SubscriptionModeType_e
{
    WINKS_SubscriptionModeType_none = 0,  /* 空 */
    WINKS_SubscriptionModeType_limited,
    WINKS_SubscriptionModeType_trial,
    WINKS_SubscriptionModeType_adware,
    WINKS_SubscriptionModeType_subscriber
}WinksTag_SubscriptionModeType_e;

/* 服务订阅状态类型 */
typedef enum tag_WinksTag_ServiceStatusType_e
{
    WINKS_BoolType_none = 0,
    WINKS_BoolType_false,
    WINKS_BoolType_true
}WinksTag_ServiceStatusType_e;

/* 升级模式 */
typedef enum tag_WinksTag_upgradeMode_e
{
    WINKS_upgradeMode_none = 0,
    WINKS_upgradeMode_auto,
    WINKS_upgradeMode_manual
}WinksTag_upgradeMode_e;


/* 我的状态（情景模式）*/
typedef enum tag_WinksTag_status_e
{
    WINKS_status_none = 0,  
    WINKS_status_normal,  
    WINKS_status_metting
}WinksTag_status_e;

/* 彩像审核状态 */
typedef enum tag_WinksTag_censor_e
{
    WINKS_censor_none = 0,  
    WINKS_censor_passed,  
    WINKS_censor_waiting,  
    WINKS_censor_rejected
}WinksTag_censor_e;

/* 彩像状态：unrelated - 与用户无关的彩像；usable - 可以直接使用的彩像；expired - 过期彩像 */
typedef enum tag_WinksTag_state_e
{
    WINKS_state_none = 0,       /* 空 */
    WINKS_state_unrelated,      /* unrelated - 与用户无关的彩像； */
    WINKS_state_usable,         /* usable - 可以直接使用的彩像； */
    WINKS_state_expired         /* expired - 过期彩像 */
}WinksTag_state_e;

/* 协议返回类型分类 */
typedef enum tag_WinksProtRetType_e
{
/*  返回类型------------------------------输出参数  */
    WINKS_PROT_RET_TYPE_null = 0,         /* 空 */
    WINKS_PROT_RET_TYPE_result,           /* WinksProt_result_s */
    WINKS_PROT_RET_TYPE_serviceConfig,    /* WinksProt_serviceConfig_s */
    WINKS_PROT_RET_TYPE_userServiceStatus,/* WinksProt_userServiceStatus_s */
    WINKS_PROT_RET_TYPE_serviceMode,      /* ServicesType_s */
    WINKS_PROT_RET_TYPE_userConfig,       /* WinksProt_userConfig_s */
    WINKS_PROT_RET_TYPE_winksShow,        /* WinksProt_winksShow_s */
    WINKS_PROT_RET_TYPE_phoneBook,        /* WinksProt_phoneBook_s */
    WINKS_PROT_RET_TYPE_UserWinksType,    /* WinksProt_UserWinksType_s */
    WINKS_PROT_RET_TYPE_winksScript,      /* WinksProt_winksScript_s */
    WINKS_PROT_RET_TYPE_shared,           /* WinksProt_shared_s */
    WINKS_PROT_RET_TYPE_softwarePackage,  /* WinksProt_softwarePackage_s */
    WINKS_PROT_RET_TYPE_messageList,      /* WinksProt_messageList_s */
    WINKS_PROT_RET_TYPE_str,              /* char * str */
    WINKS_PROT_RET_TYPE_file,             /* Winks_fileinfo_s (defined in wk_enfnc.h) */
    WINKS_PROT_RET_TYPE_MAX
}WinksProtRetType_e;

typedef struct tag_WinksProt_ParseData_s
{
    WinksProtRetType_e RetType;
    void *  pData;
}WinksProt_ParseData_s;

/******************************************************************************
**                    协议打包需要输入的数据结构
******************************************************************************/
typedef enum tag_WinksServerEnum_e
{
    WINKS_SRV_SETUP = 0,    /* 配置服务器 */
    WINKS_SRV_COMM,         /* 通讯服务器 */
    WINKS_SRV_DATA,         /* 数据服务器 */
    WINKS_SRV_LOG,          /* 调试log服务器 */
    WINKS_SRV_MAX
}WinksServerEnum_e;

/* 6.2.8.	上传DIY彩像(UploadWinks) */
typedef struct tag_WinksProt_UploadWinks_s
{
    char  * name;               /* 彩像名称 (utf-8格式) [必须] */
    char  * ext;                /* 文件扩展名 [可选] */
    unsigned char * attachName; /* 图片附件完整路径 (unicode格式) [必须] */
    unsigned long   FileSize;
    int     attachNameLen;      /* 路径长度(字节数) */
    char  * pContenttype;       /* 媒体类型，如image/jpg [必须] */
}WinksProt_UploadWinks_s;

/* 6.2.16.	上传用户统计信息(UploadUserStat) */
typedef struct tag_WinksProt_UploadUserStat_s
{
    char  * startTime;          /* 统计开始时间 */
    char  * endTime;            /* 统计结束时间 */
    unsigned char * attachName; /* 文件附件完整路径 (unicode格式) [必须] */
    int     attachNameLen;      /* 路径长度(字节数) */
    char  * pContenttype;       /* csv格式的统计内容，如txt/plain  */
}WinksProt_UploadUserStat_s;

/* 6.3.2.	查询特殊彩像展示信息(ShowSpecialWinks) */
typedef enum tag_WinksSpecialWinks_e
{
    WINKS_SPECIAL_SPLASH = 0,
    WINKS_SPECIAL_CARRIER,
    WINKS_SPECIAL_MAX
}WinksSpecialWinks_e;

/* 6.2.12.	购买彩像(PurchaseWinks) */
typedef struct tag_WinksProt_winksPO_s
{
    int     count;          /* 数量 */
    int     price;          /* 单价 */
    int     amount;         /* 总金额 */
    int     duration;       /* 总服务时长 */
    char  * wid;            /* 彩像标识 */
}WinksProt_winksPO_s;

typedef enum tag_WinksUploadFileType_e
{
    WINKS_UPLD_FILETYPE_log = 0,
    WINKS_UPLD_FILETYPE_stat,
    WINKS_UPLD_FILETYPE_MAX
}WinksUploadFileType_e;

/* 6.4.1.	上传文件(UploadFile) */
typedef struct tag_WinksProt_UploadFile_s
{
    int     fileType;           /* WinksUploadFileType_e
                                   上传文件类型，当前协议支持的类型如下：
                                   stat 统计信息
                                   log 日志信息 */
    int     fileNumber;         /* 服务器通知消息里指定的序列号 */
    unsigned char * attachName; /* 图片附件完整路径 (unicode格式) [必须] */
    int     attachNameLen;      /* 路径长度(字节数) */
    char  * pContenttype;       /* 媒体类型，如image/jpg [必须] */
}WinksProt_UploadFile_s;

typedef enum tag_WinksMessageIdType_e
{
    WINKS_MESSAGEIDTYPE_NONE = 0,
    WINKS_MESSAGEIDTYPE_SCFG,   /* 1 服务配置 */
    WINKS_MESSAGEIDTYPE_CCFG,   /* 2 用户配置 */
    WINKS_MESSAGEIDTYPE_MAX
}WinksMessageIdType_e;

typedef struct tag_WinksProt_confimMessage_s
{
    int     mid;                /* WinksMessageIdType_e */
}WinksProt_confimMessage_s;

typedef struct tag_WinksSetServiceMode_s
{
    char    uid[WINKS_SERVER_UID_MAX_LEN];
    ServicesType_s serviceMode;      /* 待设置的服务模式 */
}WinksSetServiceMode_s; /*激活流程中设置服务模式时使用*/


typedef enum tag_WinksProtocolEnum_e
{
/*  协议编号--------------------------------输入参数  */
/*          --------------------------------       返回类型 */
    WINKS_PROTOCOL_NONE = 0,
    
    WINKS_PROTOCOL_GETSERVICECONFIG = 101,  /* null */
                                                /* WINKS_PROT_RET_TYPE_serviceConfig */
    WINKS_PROTOCOL_AUTH = 201,              /* null */
                                                /* WINKS_PROT_RET_TYPE_userServiceStatus */
    WINKS_PROTOCOL_SETSERVICEMODE,          /* ServicesType_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_GETSERVICEMODE,          /* null */
                                                /* WINKS_PROT_RET_TYPE_serviceMode */
    WINKS_PROTOCOL_GETUSERCONFIG,           /* null */
                                                /* WINKS_PROT_RET_TYPE_userConfig */
    WINKS_PROTOCOL_SETUSERCONFIG,           /* WinksProt_userConfig_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_MERGEPHONEBOOK = 206,    /* WinksProt_phoneBook_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_GETPHONEBOOK,            /* null */
                                                /* WINKS_PROT_RET_TYPE_phoneBook */
    WINKS_PROTOCOL_UPLOADWINKS,             /* WinksProt_UploadWinks_s */
                                                /* WINKS_PROT_RET_TYPE_UserWinksType */
    WINKS_PROTOCOL_GETWINKSSCRIPT,          /* char * phoneNumber */
                                                /* WINKS_PROT_RET_TYPE_winksScript */
    WINKS_PROTOCOL_CREATEWINKSSCRIPT,       /* WinksProt_winksScript_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_SETSHARED,               /* WinksProt_shared_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_GETSHARED,               /* null */
                                                /* WinksProt_shared_s */
    WINKS_PROTOCOL_RECOMMENDSERVICE = 213,  /* WinksProt_phoneBook_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_PURCHASEWINKS,           /* WinksProt_winksPO_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_GETUSERWINKS,            /* char * wid */
                                                /* WINKS_PROT_RET_TYPE_UserWinksType */
    WINKS_PROTOCOL_SHOWWINKS = 301,         /* char * phoneNumber */
                                                /* WINKS_PROT_RET_TYPE_winksShow */
    WINKS_PROTOCOL_SHOWSPECIALWINKS,        /* WinksSpecialWinks_e wt */
                                                /* WINKS_PROT_RET_TYPE_winksShow */
    WINKS_PROTOCOL_GETURLFILE,              /* char  * url */
                                                /* WINKS_PROT_RET_TYPE_file */
    WINKS_PROTOCOL_UPLOADFILE = 401,        /* WinksProt_UploadFile_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_GETMESSAGE = 501,        /* null */
                                                /* WINKS_PROT_RET_TYPE_messageList */
    WINKS_PROTOCOL_CONFIRMMESSAGE,          /* WinksProt_confimMessage_s */
                                                /* WINKS_PROT_RET_TYPE_result */
    WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE = 601,/* null */    
                                                /* WINKS_PROT_RET_TYPE_softwarePackage */
    WINKS_PROTOCOL_MAX
}WinksProtocolEnum_e;

typedef enum tag_WinksHttpMethod_e
{
    WINKS_METHOD_GET = 0,   /* WINKS_HTTP_MEGET */
    WINKS_METHOD_POST,      /* WINKS_HTTP_MEPOST */
    WINKS_METHOD_MAX
}WinksHttpMethod_e;

typedef struct tag_WinksProt_PackData_s
{
    unsigned char   method;         /* WinksHttpMethod_e */
    unsigned char   server;         /* WinksServerEnum_e */
    unsigned char   IsOnlyPath;     /* 0--数据,1--路径(diy上传传给http路径) */
    unsigned char   _pad;           
    unsigned long   ContentLen;     /* diy上传时需要填写 */
    unsigned char * pPath;          /* diy上传时需要填写(unicode编码) */
    int             PathLen;        /* diy上传时需要填写(所占字节数) */
    char  * url;
    char  * pContenttype;           /* diy上传时需要填写，其他为text/xml */
    char  * pData;
    int     DataLen;
}WinksProt_PackData_s;

#define WINKS_PROT_UID_MAX  60
#define WINKS_PROT_WID_MAX  16
///////////////////////////////////////////////////////////////////////////////
//            F U N C T I O N     I N T E R F A C E
///////////////////////////////////////////////////////////////////////////////
#include "wk_xmlgb.h"
/* 协议解析: 将Http返回的xml文档数据解析为引擎可识别的结构体数据
输入:
    pXmlTree    : xml树
输出:
    ppParseData : 解析出来的协议相关结构体数据
返回:
     0 -- 成功
    -1 -- 解析失败
    -2 -- 申请内存失败
*/
int Winks_ProtParse( Winks_XmlElement_s * pXmlTree, 
                          WinksProt_ParseData_s ** ppParseData );
int Winks_ProtParseFree(WinksProt_ParseData_s * pParseData);

/* 协议打包: 将模块输入的结构体数据打包成xml文档 
输入:
    ProtocolNo  : 需要打包的协议号
    indata      : 需要输入的参数(无参数可以为空)
    inLen       : 输入参数的长度(无参数可以为0)
输出:
    ppProtPack  : 打包出来提供给http发送的结构体数据，
                  pProtPack->pData为实际发送的xml数据
返回:
    成功或失败。
*/
int Winks_ProtPack( WinksProtocolEnum_e ProtocolNo, void  * inData, int inLen,
                         WinksProt_PackData_s ** ppProtPack );
int Winks_ProtPackFree(WinksProt_PackData_s * pProtPack);

#ifdef __cplusplus
}
#endif

#endif/*_WK_PROT_IF_H_*/

