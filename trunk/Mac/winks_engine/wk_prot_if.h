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
**                    Э�������������ݽṹ
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
    int     code;   /* �����롣codeΪ0ʱ��ʾ����ִ�гɹ��� */
    union {
        char * text;    /* ����ִ��ʧ��ʱ���ı�������Ϣ (utf-8��ʽ) */
        char * greeting;/* ����ִ�гɹ�ʱ���ص���ʾ��Ϣ (utf-8��ʽ) */
    }rst_txt;
    char * debug;  /* ������Ϣ (utf-8��ʽ) */
    //struct Prot_debug_attr_s * debug;  /* ������Ϣ */
}WinksProt_result_s;

struct Prot_sms_s
{
    struct Prot_sms_s * pnext;
    char * number;
    char * prefix;
};

struct Prot_address_s
{
    char  * winks;  /* ����ҵ���������URL����ַ */
    //char  * media;  /* ý���������URL����ַ */
    char  * store;  /* �����̳�URL����ַ */
  #if 0
    char  * sms;    /* �����ط����� */
  #else
    int     sms_num;
    struct Prot_sms_s * sms;
  #endif
    int     push;   /* �ն�push���ն˿� */
};

enum Prot_subscription_e
{
    WINKS_SUBSCRIPTION_NONE = 0,
    WINKS_SUBSCRIPTION_LIMITED,     /* �����û�ѡ�� */
    WINKS_SUBSCRIPTION_TRIAL,       /* ������ѡ�� */
    WINKS_SUBSCRIPTION_ADWARE,      /* ���ܹ���û�ѡ�� */
    WINKS_SUBSCRIPTION_SUBSCRIER,   /* �����û�ѡ�� */
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
    struct Prot_subscription_s subscription;    /* ������ģʽ */
    unsigned int    limited_diy:1,      /* ����DIY�����д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    limited_vcard:1,    /* ʹ��������Ƭ���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    limited_activity:1, /* �û�����ģʽ���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    _rsv0:1,

                    trial_diy:1,      /* ����DIY�����д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    trial_vcard:1,    /* ʹ��������Ƭ���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    trial_activity:1, /* �û�����ģʽ���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    _rsv1:1,

                    adware_diy:1,      /* ����DIY�����д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    adware_vcard:1,    /* ʹ��������Ƭ���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    adware_activity:1, /* �û�����ģʽ���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    _rsv2:1,

                    subscriber_diy:1,      /* ����DIY�����д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    subscriber_vcard:1,    /* ʹ��������Ƭ���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    subscriber_activity:1, /* �û�����ģʽ���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
                    _rsv3:1,

                    vcash:1;    /* ʹ�õ㿨���д��ֶ��Ǳ�ʾ�򿪴˹��ܣ�ֵΪ�˹����Ƿ���Ҫ���� */
};

struct Prot_connection_s
{
    char  * proxy;                  /* ���������IP */
    short   proxyPort;              /* ����������˿� */
    char  * apn;                    /* �������� */
};

struct Prot_client_s
{
    int     autoSubscribe;
};

/* ���������������Ϣ */
typedef struct tag_WinksProt_ServiceConfig_s
{
    struct Prot_address_s * address;
    struct Prot_connection_s * connection;
    struct Prot_config_s  * config;    /* �������� */
    struct Prot_client_s  * client;
}WinksProt_serviceConfig_s;

/* ����ѡ�minFee - ��С�ʷѸ������ڣ�mostRecent - �����¸������ڣ�custom - �Զ��� */
typedef enum tag_WinksTag_updateConfig_e
{
    WINKS_updateConfig_none = 0,        /* �� */
    WINKS_updateConfig_minFee,          /* minFee - ��С�ʷѸ������ڣ� */
    WINKS_updateConfig_mostRecent,      /* mostRecent - �����¸������ڣ� */
    WINKS_updateConfig_custom,          /* custom - �Զ��� */
    WINKS_updateConfig_max              
}WinksTag_updateConfig_e;


struct Prot_updateConfig_s
{
    int     option;                     /* WinksTag_updateConfig_e */
    int     period;
};

/* �û�������Ϣ */
typedef struct tag_WinksProt_userConfig_s
{
    char  * proxy;                  /* ���������IP */
    short   proxyPort;              /* ����������˿� */
    char  * apn;                    /* �������� */
    int     durationAfterHangup;    /* �һ������չʾʱ�䣨����ƣ� */
    unsigned int showWinks:1,       /* չʾ����
                                       true չʾ����
                                       false ��չʾ���� */
            showMode:3,             /* ͨ�����ã�Ϊ����ֵ��ϣ�
                                       0x01-ͨ��ǰչʾ
                                       0x02-ͨ����չʾ
                                       0x04-ͨ����չʾ */
            roaming:1,              /* ��������
                                       true ����ʱչʾ����
                                       false ����ʱ��չʾ���� */
            autorun:1,              /* ��������������
                                       true ����������
                                       false ������������ */
            updateAfterHangup:1,    /* �Ҷϵ绰���������
                                       true �ҶϺ���²�������
                                       false �ҶϺ󲻸��²������� */
            upgradeMode:1,          /* ������ʽ����
                                       auto �Զ�����
                                       manual �ֶ����� */
            vcardOverwrite:1,       /* �����Ƭ���·�ʽ����
                                       auto �Զ�����
                                       manual �ֶ����� */
            connectInCall:1,        /* �绰�¼��Ƿ�������ȡ����
                                       true ������ȡ����
                                       false ��������ȡ���� */
            support3G:1;            /* �Ƿ�ͬʱ֧��3G
                                       true ͬʱ֧��2.5G��3Gģʽ
                                       false ֻ֧��2.5Gģʽ */
    unsigned int showWinks_Mask:1,       
            showMode_Mask:3,             
            roaming_Mask:1,              
            autorun_Mask:1,              
            updateAfterHangup_Mask:1,    
            upgradeMode_Mask:1,          
            vcardOverwrite_Mask:1,       
            connectInCall_Mask:1,        
            support3G_Mask:1;            
    int     networkTimeout;         /* �ն��������ӵĳ�ʱʱ��(��) */
    int     splashDuration;         /* Ƭͷչʾʱ��(��)��0ʱ��չʾ */
    int     updateOptionMinFee;     /* ��ʱ������ʡ�ʷ�ģʽ,ʱ������*/
    int     updateOptionMostRecent; /* ��ʱ������������ģʽ,ʱ������*/
    struct Prot_updateConfig_s * updateConfig;
}WinksProt_userConfig_s;

/* �ն����� */
typedef struct tag_WinksProt_userConfigEx_s
{
    int     AllOrDiff;      /* 0--diff; 1--all */
    WinksProt_userConfig_s * userConfig;
}WinksProt_userConfigEx_s;


/* �ն����� */
typedef struct tag_WinksProt_clientConfig_s
{
    void * data;
}WinksProt_clientConfig_s;

/* ��Ϣ�� */
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
    int     registerMethod; /* ע�᷽�� WinksTag_registerMethod_e */
};

/* �������� */
typedef struct tag_ServicesType_s
{
    enum Prot_subscription_e subscription;    /* ������ģʽ */
    unsigned int    diy:1,      /* DIY���� */
                    vcard:1,    /* �����Ƭ���� */
                    activity:1; /* �û�����ģʽ */
    unsigned int    vcash;      /* �㿨���� */
}ServicesType_s;

/* �û��ķ���״̬��Ϣ */
typedef struct tag_WinksProt_userServiceStatus_s
{
    int     IsRegistered;   /* 0--δע�ᣬ1--��ע�� */
    union {
        struct Prot_unregistered_s unregistered;   /* �û�δע�� */
        struct{
            ServicesType_s  service;/* �û��ѿ�ͨ�ķ��� */
            char  * uid;    /* �û���ʶ */
        }reg;
    }RegStat;
}WinksProt_userServiceStatus_s;

typedef struct tag_WinksProt_shared_s
{
    int     status;                 /* �ҵ�״̬���龰ģʽ�� */
    char  * statusText;             /* ״̬�� (utf-8��ʽ) */
    char  * activity;               /* �û���ǰ� (utf-8��ʽ) */
    char  * name;                   /* �����˿��������� (utf-8��ʽ) */
    char  * message;                /* �����˿�������Ϣ (utf-8��ʽ) */
    unsigned int    showWinks:1,    /* �Ƿ���ʾ���� */
                    showVcard:1;    /* �Ƿ���ʾ�����Ƭ */
    unsigned int    showWinks_Mask:1,
                    showVcard_Mask:1;
}WinksProt_shared_s;

enum Prot_wt_e
{
    WINKS_WT_SPLASH = 0,
    WINKS_WT_CARRIER
};

/* ����ý���ļ����� */
typedef struct tag_WinksFileType_s
{
    struct tag_WinksFileType_s * pnext;
    char  * url;    /* ý���ļ�����Դλ�� */
    int     w;      /* ��� */
    int     h;      /* �߶� */
}WinksFileType_s;

/* ��Ƭ���� */
typedef struct tag_VcardType_s
{
    char  * name;       /* ���� (utf-8��ʽ) */
    char  * mobile;     /* ��ʵ���� */
    char  * corporation;/* ��˾ (utf-8��ʽ) */
    char  * title;      /* ְλ (utf-8��ʽ) */
    char  * phone;      /* �绰 */
    char  * email;      /* �����ʼ� */
    char  * fax;        /* ���� */
    char  * address;    /* ��ַ (utf-8��ʽ) */
}VcardType_s;

/* ����չʾ��Ϣ */
typedef struct tag_WinksProt_winksShow_s
{
    char  * wid;                    /* �����ʶ */
    unsigned short    media_num;    /* �������� */
    unsigned short    thumbnail_num;/* ����ͼ�� */
    WinksFileType_s * media;        /* ý���ļ� */
    WinksFileType_s * thumbnail;    /* ��������ͼ */
    char *  greeting;       /* �ʺ��� (utf-8��ʽ) */
    int     greetValid;     /* �ʺ����Ƿ���Ч */
    int     status;                 /* �ҵ�״̬���龰ģʽ�� */
    char * statusText;     /* ״̬�� (utf-8��ʽ) */
    char * message;        /* ���Ի�ǩ�� */
    VcardType_s * vcard;            /* ��Ƭ��Ϣ */
}WinksProt_winksShow_s;

struct Prot_pbi_s
{
    struct Prot_pbi_s * pnext;
    char  * n;  /* ���� (utf-8��ʽ) */
    char  * p;  /* �绰���� */
};

/* �绰�� */
typedef struct tag_WinksProt_phoneBook_s
{
    int     pbi_num;
    struct Prot_pbi_s * pbi;   /* �绰����Ŀ */
}WinksProt_phoneBook_s;

/* �û��������� */
typedef struct tag_WinksProt_UserWinksType_s
{
    char  * wid;
    char  * name;  /* �������� (utf-8��ʽ) */
    int             censor:3,   /* �������״̬ */
                    diy:1;      /* �û��ϴ������־ */
    unsigned long   duration;
    unsigned long   price;   /* ����۸� */
    int     state;  /*����״̬��
                        0: unrelated - ���û��޹صĲ���
                        1: usable - ����ֱ��ʹ�õĲ���
                        2: expired - ���ڲ���*/
    char  * expire; /* �������ʱ�� */
}WinksProt_UserWinksType_s;

struct Prot_pno_s
{
    struct Prot_pno_s * pnext;
    char  * p;  /* �绰���� */
};

typedef struct tag_WinksProt_winksScript_s
{
    char  * wid;    /* �����ʶ�б� */
    char  * greeting;   /* �ʺ��� (utf-8��ʽ) */
    int     IsForAll;   /* 0--�绰�б�; 1--���к��� */
    struct Prot_pno_s * pno;       /*һ��绰�����б�*/
}WinksProt_winksScript_s;

struct Prot_url_s
{
    struct Prot_url_s * pnext;
    char  * url;    /* ������ļ���URLλ�� */
};

/* �ն��������Ϣ */
typedef struct tag_WinksProt_softwarePackage_s
{
    char  * name;       /* ��������� (utf-8��ʽ) */
    char  * version;    /* ������汾 */
    char  * file;       /* ����url */
    int     optional;   /* optional */
    char  * description;   /* ����汾����˵�� (utf-8��ʽ) */
}WinksProt_softwarePackage_s;

/* ע�᷽����sms - ���ŷ�ʽע�᣻auto - �Զ�ʶ���û��ֻ��� */
typedef enum tag_WinksTag_registerMethod_e
{
    WINKS_registerMethod_none = 0,  /* �� */
    WINKS_registerMethod_sms,       /* sms - ���ŷ�ʽע�᣻ */
    WINKS_registerMethod_push,      /* push - push��ʽ */
    WINKS_registerMethod_auto       /* auto - �Զ�ʶ���û��ֻ��� */
}WinksTag_registerMethod_e;

/* ������ģʽ���� */
typedef enum tag_WinksTag_SubscriptionModeType_e
{
    WINKS_SubscriptionModeType_none = 0,  /* �� */
    WINKS_SubscriptionModeType_limited,
    WINKS_SubscriptionModeType_trial,
    WINKS_SubscriptionModeType_adware,
    WINKS_SubscriptionModeType_subscriber
}WinksTag_SubscriptionModeType_e;

/* ������״̬���� */
typedef enum tag_WinksTag_ServiceStatusType_e
{
    WINKS_BoolType_none = 0,
    WINKS_BoolType_false,
    WINKS_BoolType_true
}WinksTag_ServiceStatusType_e;

/* ����ģʽ */
typedef enum tag_WinksTag_upgradeMode_e
{
    WINKS_upgradeMode_none = 0,
    WINKS_upgradeMode_auto,
    WINKS_upgradeMode_manual
}WinksTag_upgradeMode_e;


/* �ҵ�״̬���龰ģʽ��*/
typedef enum tag_WinksTag_status_e
{
    WINKS_status_none = 0,  
    WINKS_status_normal,  
    WINKS_status_metting
}WinksTag_status_e;

/* �������״̬ */
typedef enum tag_WinksTag_censor_e
{
    WINKS_censor_none = 0,  
    WINKS_censor_passed,  
    WINKS_censor_waiting,  
    WINKS_censor_rejected
}WinksTag_censor_e;

/* ����״̬��unrelated - ���û��޹صĲ���usable - ����ֱ��ʹ�õĲ���expired - ���ڲ��� */
typedef enum tag_WinksTag_state_e
{
    WINKS_state_none = 0,       /* �� */
    WINKS_state_unrelated,      /* unrelated - ���û��޹صĲ��� */
    WINKS_state_usable,         /* usable - ����ֱ��ʹ�õĲ��� */
    WINKS_state_expired         /* expired - ���ڲ��� */
}WinksTag_state_e;

/* Э�鷵�����ͷ��� */
typedef enum tag_WinksProtRetType_e
{
/*  ��������------------------------------�������  */
    WINKS_PROT_RET_TYPE_null = 0,         /* �� */
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
**                    Э������Ҫ��������ݽṹ
******************************************************************************/
typedef enum tag_WinksServerEnum_e
{
    WINKS_SRV_SETUP = 0,    /* ���÷����� */
    WINKS_SRV_COMM,         /* ͨѶ������ */
    WINKS_SRV_DATA,         /* ���ݷ����� */
    WINKS_SRV_LOG,          /* ����log������ */
    WINKS_SRV_MAX
}WinksServerEnum_e;

/* 6.2.8.	�ϴ�DIY����(UploadWinks) */
typedef struct tag_WinksProt_UploadWinks_s
{
    char  * name;               /* �������� (utf-8��ʽ) [����] */
    char  * ext;                /* �ļ���չ�� [��ѡ] */
    unsigned char * attachName; /* ͼƬ��������·�� (unicode��ʽ) [����] */
    unsigned long   FileSize;
    int     attachNameLen;      /* ·������(�ֽ���) */
    char  * pContenttype;       /* ý�����ͣ���image/jpg [����] */
}WinksProt_UploadWinks_s;

/* 6.2.16.	�ϴ��û�ͳ����Ϣ(UploadUserStat) */
typedef struct tag_WinksProt_UploadUserStat_s
{
    char  * startTime;          /* ͳ�ƿ�ʼʱ�� */
    char  * endTime;            /* ͳ�ƽ���ʱ�� */
    unsigned char * attachName; /* �ļ���������·�� (unicode��ʽ) [����] */
    int     attachNameLen;      /* ·������(�ֽ���) */
    char  * pContenttype;       /* csv��ʽ��ͳ�����ݣ���txt/plain  */
}WinksProt_UploadUserStat_s;

/* 6.3.2.	��ѯ�������չʾ��Ϣ(ShowSpecialWinks) */
typedef enum tag_WinksSpecialWinks_e
{
    WINKS_SPECIAL_SPLASH = 0,
    WINKS_SPECIAL_CARRIER,
    WINKS_SPECIAL_MAX
}WinksSpecialWinks_e;

/* 6.2.12.	�������(PurchaseWinks) */
typedef struct tag_WinksProt_winksPO_s
{
    int     count;          /* ���� */
    int     price;          /* ���� */
    int     amount;         /* �ܽ�� */
    int     duration;       /* �ܷ���ʱ�� */
    char  * wid;            /* �����ʶ */
}WinksProt_winksPO_s;

typedef enum tag_WinksUploadFileType_e
{
    WINKS_UPLD_FILETYPE_log = 0,
    WINKS_UPLD_FILETYPE_stat,
    WINKS_UPLD_FILETYPE_MAX
}WinksUploadFileType_e;

/* 6.4.1.	�ϴ��ļ�(UploadFile) */
typedef struct tag_WinksProt_UploadFile_s
{
    int     fileType;           /* WinksUploadFileType_e
                                   �ϴ��ļ����ͣ���ǰЭ��֧�ֵ��������£�
                                   stat ͳ����Ϣ
                                   log ��־��Ϣ */
    int     fileNumber;         /* ������֪ͨ��Ϣ��ָ�������к� */
    unsigned char * attachName; /* ͼƬ��������·�� (unicode��ʽ) [����] */
    int     attachNameLen;      /* ·������(�ֽ���) */
    char  * pContenttype;       /* ý�����ͣ���image/jpg [����] */
}WinksProt_UploadFile_s;

typedef enum tag_WinksMessageIdType_e
{
    WINKS_MESSAGEIDTYPE_NONE = 0,
    WINKS_MESSAGEIDTYPE_SCFG,   /* 1 �������� */
    WINKS_MESSAGEIDTYPE_CCFG,   /* 2 �û����� */
    WINKS_MESSAGEIDTYPE_MAX
}WinksMessageIdType_e;

typedef struct tag_WinksProt_confimMessage_s
{
    int     mid;                /* WinksMessageIdType_e */
}WinksProt_confimMessage_s;

typedef struct tag_WinksSetServiceMode_s
{
    char    uid[WINKS_SERVER_UID_MAX_LEN];
    ServicesType_s serviceMode;      /* �����õķ���ģʽ */
}WinksSetServiceMode_s; /*�������������÷���ģʽʱʹ��*/


typedef enum tag_WinksProtocolEnum_e
{
/*  Э����--------------------------------�������  */
/*          --------------------------------       �������� */
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
    unsigned char   IsOnlyPath;     /* 0--����,1--·��(diy�ϴ�����http·��) */
    unsigned char   _pad;           
    unsigned long   ContentLen;     /* diy�ϴ�ʱ��Ҫ��д */
    unsigned char * pPath;          /* diy�ϴ�ʱ��Ҫ��д(unicode����) */
    int             PathLen;        /* diy�ϴ�ʱ��Ҫ��д(��ռ�ֽ���) */
    char  * url;
    char  * pContenttype;           /* diy�ϴ�ʱ��Ҫ��д������Ϊtext/xml */
    char  * pData;
    int     DataLen;
}WinksProt_PackData_s;

#define WINKS_PROT_UID_MAX  60
#define WINKS_PROT_WID_MAX  16
///////////////////////////////////////////////////////////////////////////////
//            F U N C T I O N     I N T E R F A C E
///////////////////////////////////////////////////////////////////////////////
#include "wk_xmlgb.h"
/* Э�����: ��Http���ص�xml�ĵ����ݽ���Ϊ�����ʶ��Ľṹ������
����:
    pXmlTree    : xml��
���:
    ppParseData : ����������Э����ؽṹ������
����:
     0 -- �ɹ�
    -1 -- ����ʧ��
    -2 -- �����ڴ�ʧ��
*/
int Winks_ProtParse( Winks_XmlElement_s * pXmlTree, 
                          WinksProt_ParseData_s ** ppParseData );
int Winks_ProtParseFree(WinksProt_ParseData_s * pParseData);

/* Э����: ��ģ������Ľṹ�����ݴ����xml�ĵ� 
����:
    ProtocolNo  : ��Ҫ�����Э���
    indata      : ��Ҫ����Ĳ���(�޲�������Ϊ��)
    inLen       : ��������ĳ���(�޲�������Ϊ0)
���:
    ppProtPack  : ��������ṩ��http���͵Ľṹ�����ݣ�
                  pProtPack->pDataΪʵ�ʷ��͵�xml����
����:
    �ɹ���ʧ�ܡ�
*/
int Winks_ProtPack( WinksProtocolEnum_e ProtocolNo, void  * inData, int inLen,
                         WinksProt_PackData_s ** ppProtPack );
int Winks_ProtPackFree(WinksProt_PackData_s * pProtPack);

#ifdef __cplusplus
}
#endif

#endif/*_WK_PROT_IF_H_*/

