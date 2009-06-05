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
#define WINKS_ENGINE_APP_QUEUE_SIZE WINKS_ENGINE_APP_PRI_NUM /*����Ӧ��ģ���ջ��С*/
#define WINKS_ENAPP_NAME_MAX_LEN 12 /*����Ӧ�����Ƴ���*/
#define WINKS_ENGINE_APP_HANDLER_NULL 0 /*����Ӧ�ÿվ��*/
#define WINKS_ENGINE_APP_TIMER_NUM 2 /*����Ӧ�ö�ʱ������*/

typedef enum tag_Winks_EngineAppPri_e
{
    WINKS_ENGINE_APP_PRI_NORMAL = 0x00,     /* ����������:��ʱ���£��Զ�������*/
    WINKS_ENGINE_APP_PRI_USER = 0x01,       /* �û��������û������Ĳ������缤�DIY���ֶ����µ�*/
    WINKS_ENGINE_APP_PRI_CALL,              /* �绰���� �����紥��������������*/
    WINKS_ENGINE_APP_PRI_NUM                /* ���ȼ�����*/
}Winks_EngineAppPri_e;

typedef unsigned long WINKS_ENAPP_HND; /*����Ӧ�þ��*/
typedef int (*WINKS_ENGAPP_PROCESS)(WINKS_ENAPP_HND appHnd, unsigned long msg, void *msgData, unsigned long msgDataSize, void *addData); /*����Ӧ�ûص�����*/

 /*����Ӧ�õĿ��ƿ�*/
typedef struct tag_Winks_EngineAppControl_s
{
    WINKS_ENAPP_HND         appHnd; /*����Ӧ�þ��*/
    WINKS_ENGAPP_PROCESS    prcFnc; /*����Ӧ�ô�����*/
    void *                  addData; /*����Ӧ�õ�����*/
    Winks_EngineAppPri_e    appPri; /*����Ӧ�õ����ȼ�*/
    char appName[WINKS_ENAPP_NAME_MAX_LEN]; /*����Ӧ�õ�����*/
    WINKS_TIMER_ID timer[WINKS_ENGINE_APP_TIMER_NUM]; /*����Ӧ�õĶ�ʱ�����*/
}Winks_EngineAppControl_s;

/*����Ӧ�����Ʋ���Ӧ�þ��
 * function: winks_FindEngAppHndByName
 * parameters:
 *   char * appName: ����Ӧ�õ�����
 * return:
 *   �ɹ�����:Ӧ�þ��(>0)
 *   ʧ�ܷ���:0(WINKS_ENGIEN_APP_HANDLER_NULL)
 */
WINKS_ENAPP_HND winks_FindEngAppHndByName(char * appName);

/*����Ӧ�����Ʋ���Ӧ�þ��
 * function: winks_GetEngAppAddData
 * parameters:
 *   WINKS_ENAPP_HND appHnd: Ӧ�þ��(>0)
 * return:
 *   �ɹ�����:addDataָ��
 *   ʧ�ܷ���:NULL
 */
void * winks_GetEngAppAddData(WINKS_ENAPP_HND appHnd);

 /*��������������Ӧ��(���缤�����̺�DIY���̾͸���һ������Ӧ�á�
 * function: Winks_EngAppStart
 * parameters:
 *   Winks_EngineAppPri_e appPri: Ӧ�����ȼ�
 *   WINKS_ENGAPP_PROCESS prcFn: ����ص�����
 *   char * appName: ����Ӧ�õ�����
 *   void * addData: (in)����ָ��
 * return:
 *   �ɹ�����:Ӧ�þ��(>0)
 *   ʧ�ܷ���:0(WINKS_ENGIEN_APP_HANDLER_NULL)
 */
WINKS_ENAPP_HND Winks_EngAppStart(Winks_EngineAppPri_e appPri, WINKS_ENGAPP_PROCESS prcFnc, char * appName, void *addData);

/*�˳���ע��������Ӧ��(���缤�����̺�DIY���̾͸���һ������Ӧ�á�
 * function: Winks_EngAppStop
 * parameters:
 *   WINKS_ENAPP_HND appHnd: Ӧ�þ��(>0)
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_EngAppStop(WINKS_ENAPP_HND appHnd);

/*����Ӧ�ô���TIMER��
 * function: Winks_EngCreateTimer
 * parameters:
 *   WINKS_ENAPP_HND appHnd: Ӧ�þ��(>0)
 *   WINKS_TICKS timeOutPeriod:������ʱ���Ķ�ʱ����
 *   unsigned long flag: ������ʱ�������Ա�־��
 *                       WINKS_TIMER_DEFAULT: Ĭ��
 *                       WINKS_TIMER_AUTO_START:������; 
 *                       WINKS_TIMER_AUTO_LOOP: ѭ������;
 * return:
 *   �ɹ�����:��ʱ�����(>0)
 *   ʧ�ܷ���:0
 */
WINKS_TIMER_ID Winks_EngCreateTimer(WINKS_ENAPP_HND appHnd, WINKS_TICKS timeOutPeriod, unsigned long flag);

/*����Ӧ��ɾ����ʱ����
 * function: Winks_EngDestroyTimer
 * parameters:
 *   WINKS_ENAPP_HND appHnd: Ӧ�þ��(>0)
 *   WINKS_TIMER_ID timerId: ��ʱ�����(>0)
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_EngDestroyTimer(WINKS_ENAPP_HND appHnd, WINKS_TIMER_ID timerId);
#endif/*Engine framwork end.*/


typedef struct tag_WinksSrvConfig_s
{
    unsigned int    diy:1,          /*server֧�ֹ���:DIY */
                    vcard:1,        /*server֧�ֹ���:VCARD */
                    activity:1,     /*server֧�ֹ���:ACTIVITY */
                    vcash:1,        /*server֧�ֹ���:VCASH */
                    end1:1;      
    
    unsigned int    limited:1,      /*����ģʽ:limited */
                    trial:1,        /*����ģʽ:trial */
                    adware:1,       /*����ģʽ:adware */
                    subscriber:1,   /*����ģʽ:subscriber */
                    end2:1;   
}WinksSrvConfig_s;

#if 1
/***********************************************/
/***********    Engine manager    **************/
/***********************************************/
#define WINKS_HTTP_NULL (-1)        /*HTTP��Ч���*/
typedef enum tag_Winks_EngineState_e
{
    WINKS_ENGINE_STATE_INIT = 0x00,/*0000  ��ʼ״̬*/
    WINKS_ENGINE_STATE_START = 0x01,/*0001  ��ʼ��״̬*/
    WINKS_ENGINE_STATE_DISABLED = 0x02,/*0010  ��ʼ�����״̬*/
    WINKS_ENGINE_STATE_ENABLED = 0x06,/*0110  �Ѽ������״̬*/
//    WINKS_ENGINE_STATE_SHOWA = 0x0a,/*1010  δ�����ҵ绰�¼�״̬*/
//    WINKS_ENGINE_STATE_SHOWB = 0x0e,/*1110  �Ѽ����ҵ绰�¼�״̬*/
    WINKS_ENGINE_STATE_NUMBER
}Winks_EngineState_e;

#define Winks_IsEngStateCall(state)  (state&WINKS_BIT_FALG_D3)
#define Winks_IsEngStateRegister(state)  (state&WINKS_BIT_FALG_D2)
#define Winks_IsEngStateInitOk(state)  (state&WINKS_BIT_FALG_D1)
#define Winks_SetEngStateregister(state)  (state = state|WINKS_BIT_FALG_D2)
#define Winks_SetEngStateUnRegister(state)  (state = Winks_IsEngStateRegister(state)?state^WINKS_BIT_FALG_D2:state)


#define WINKS_MSG_ENG2UI_NONE       0       /* �޹���Ϣ */
    
typedef enum tag_WinksEng2UiMsg_SysErr_e
{
/* ������Ϣ */
    WINKS_MSG_ENG2UI_SVR_NO_ANSWER  = -127, /* ��������Ӧ�� */
    WINKS_MSG_ENG2UI_UNREG_USER     ,       /* ��ע���û����� */
    WINKS_MSG_ENG2UI_ROAMING_LIMIT  ,       /* ��������  */
    WINKS_MSG_ENG2UI_ENG_BUSY       ,       /* ����æ */
    WINKS_MSG_ENG2UI_DIAL_INT       ,       /* ���绰�ж� */
    WINKS_MSG_ENG2UI_STORAGE_LIMIT  ,       /* �洢�ռ䲻�� */
    WINKS_MSG_ENG2UI_CC_CLOSED      ,       /* ��������ر�  */
#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_MCARD_NOT_EXIST,       /* �洢�������� */
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    WINKS_MSG_ENG2UI_SYS_FILES_ERR  ,       /* ϵͳ�ļ����ƻ� */
    WINKS_MSG_ENG2UI_CC_FILES_ERR   ,       /* �����ļ����ƻ� */
#ifdef WINKS_DUAL_SIM
    WINKS_MSG_ENG2UI_SIM_SEL_ERR    ,       /* �û�δѡ��SIM�� */
    WINKS_MSG_ENG2UI_SIM_UNUSABLE   ,       /* �û�ѡ���Sim�������� */
#endif /* WINKS_DUAL_SIM */
    WINKS_MSG_ENG2UI_NO_SUBSCRIPTION,       /* ���Ƕ����û� */
    WINKS_MSG_ENG2UI_USER_DISABLED   ,      /* �û������� */
    WINKS_MSG_ENG2UI_NO_NUMBER      ,       /* û�е绰������Ҫ���� */

    WINKS_MSG_ENG2UI_PROT_NO        ,       /* �������·�Э��Ų�ƥ�� */
    WINKS_MSG_ENG2UI_HTTP_ERRCODE   ,       /* �յ�Http������ */
    WINKS_MSG_ENG2UI_SRV_ERRCODE    ,       /* �յ����������ش����� */
    WINKS_MSG_ENG2UI_SRV_ERRDATA    ,       /* ���������������д��� */
    WINKS_MSG_ENG2UI_MEM            ,       /* �ڴ�������� */
    WINKS_MSG_ENG2UI_FS             ,       /* �ļ�ϵͳ���� */
    WINKS_MSG_ENG2UI_HTTP           ,       /* HTTP���� */
    WINKS_MSG_ENG2UI_HTTP_TIMEOUT   ,       /* HTTP��ʱ���� */
    WINKS_MSG_ENG2UI_PROT_PACK      ,       /* Э���� */
    WINKS_MSG_ENG2UI_PROT_PARSE     ,       /* Э����� */
    WINKS_MSG_ENG2UI_XML_GEN        ,       /* ����xml���� */
    WINKS_MSG_ENG2UI_XML_PARSE      ,       /* xml�������� */
    WINKS_MSG_ENG2UI_APP_ERR        ,       /* Ӧ�õ��ô� */
    WINKS_MSG_ENG2UI_ENG_ERR        ,       /* ������ô� */
    
    WINKS_MSG_ENG2UI_UNKNOWN        = -80   /* δ֪���� */
}WinksEng2UiMsg_SysErr_e;

typedef enum tag_WinksEng2UiMsg_Regist_e
{
/* 1 ע�ἰ��ʼ������ */
    WINKS_MSG_ENG2UI_SETUP_SVR_FAIL = -9   /* �������÷�����ʧ�� */
    ,WINKS_MSG_ENG2UI_REG_FAIL              /* ע��ʧ�� */
    ,WINKS_MSG_ENG2UI_ULPB_FAIL             /* �ϴ��绰��ʧ�� */
    ,WINKS_MSG_ENG2UI_INIT_FAIL             /* ��ʼ��ʧ�� */
    
    ,WINKS_MSG_ENG2UI_SETUP_SVR      = 1    /* �����������÷�����... */
    ,WINKS_MSG_ENG2UI_SETUP_SVR_OK          /* �������÷������ɹ� */
    ,WINKS_MSG_ENG2UI_SMS_REG               /* δע�ᣬ��Ҫ����ע����� */
    ,WINKS_MSG_ENG2UI_WAIT_PUSH             /* δע�ᣬ�ȴ�����push */
    ,WINKS_MSG_ENG2UI_REG                   /* ����ע��... */
    ,WINKS_MSG_ENG2UI_REG_OK                /* ע��OK [ע��ɹ��ʺ���(utf8��ʽ)] */
    ,WINKS_MSG_ENG2UI_ULPB_CONFIRM          /* �û�ȷ���Ƿ��ϴ��绰�� */
    ,WINKS_MSG_ENG2UI_ULPB                  /* �����ϴ��绰��... */
    ,WINKS_MSG_ENG2UI_ULPB_OK               /* �ϴ��绰����� */
    ,WINKS_MSG_ENG2UI_INIT                  /* ���ڳ�ʼ��... */
    ,WINKS_MSG_ENG2UI_INIT_OK               /* ��ʼ����� */
    ,WINKS_MSG_ENG2UI_QUERY_OK              /* ��ѯ�û�״̬��� */
}WinksEng2UiMsg_Regist_e;

typedef enum tag_WinksEng2UiMsg_Friend_e
{
/* 2 ���º���¼ */
    WINKS_MSG_ENG2UI_FRIEND_FAIL    = -79,  /* ���º���¼ʧ�� */
    
    WINKS_MSG_ENG2UI_FRIEND         = 71,   /* ���ڸ��º���¼...[WinksProgress_s] */
    WINKS_MSG_ENG2UI_FRIEND_OK              /* ����¼������� */
}WinksEng2UiMsg_Friend_e;
    
typedef enum tag_WinksEng2UiMsg_Diy_e
{
/* 3 �ϴ�DIY */
    WINKS_MSG_ENG2UI_DIY_FAIL       = -29,  /* �ϴ�DIYʧ�� */
    WINKS_MSG_ENG2UI_SCRIPT_FAIL,           /* ���Ʋ���ʧ�� */

    WINKS_MSG_ENG2UI_DIY            = 21,   /* �����ϴ�DIY... */
    WINKS_MSG_ENG2UI_DIY_OK,                /* �ϴ�DIY�ɹ�[wid����id] */
    WINKS_MSG_ENG2UI_SCRIPT_OK              /* ���Ʋ���ɹ� */
}WinksEng2UiMsg_Diy_e;

typedef enum tag_WinksEng2UiMsg_Recommend_e
{
/* 4 �Ƽ������� */
    WINKS_MSG_ENG2UI_RECOMMEND_FAIL = -39,  /* �Ƽ�ʧ�� */

    WINKS_MSG_ENG2UI_RECOMMEND      = 30,   /* �����Ƽ�... */
    WINKS_MSG_ENG2UI_RECOMMEND_OK           /* �Ƽ��ɹ� */
}WinksEng2UiMsg_Recommend_e;

typedef enum tag_WinksEng2UiMsg_Call_e
{
/* 5 �绰���� */
    WINKS_MSG_ENG2UI_CALL_FAIL      = -49,  /* ��ȡͨ���û�����ʧ�� */
    WINKS_MSG_ENG2UI_CALL_TIMEOUT   ,       /* ��ȡͨ���û�����ʱ */

    WINKS_MSG_ENG2UI_CALL           = 41,   /* ���ڻ�ȡͨ���û�����... */
    WINKS_MSG_ENG2UI_CALL_OK                /* ��ȡͨ���û�����ɹ� */
}WinksEng2UiMsg_Call_e;

typedef enum tag_WinksEng2UiMsg_Setup_e
{
/* 6 ͬ���������õ������� */
    WINKS_MSG_ENG2UI_SETUP_FAIL     = -59,  /* ͬ����������ʧ�� */
    WINKS_MSG_ENG2UI_SRVCFG_FAIL,           /* ����������ʧ�� */
    WINKS_MSG_ENG2UI_USERCFG_FAIL,          /* ��������ʧ�� */
    WINKS_MSG_ENG2UI_ULSTAT_FAIL,           /* ͳ����Ϣ�ϴ�ʧ�� */
    WINKS_MSG_ENG2UI_ULLOG_FAIL,            /* log��Ϣ�ϴ�ʧ�� */
    
    WINKS_MSG_ENG2UI_SETUP          = 51,   /* ����ͬ����������... */
    WINKS_MSG_ENG2UI_SETUP_OK,              /* ͬ���������óɹ� */
    WINKS_MSG_ENG2UI_SRVCFG_OK,             /* ���������óɹ� */
    WINKS_MSG_ENG2UI_USERCFG_OK,            /* �������óɹ� */
    WINKS_MSG_ENG2UI_ULSTAT_OK,             /* ͳ����Ϣ�ϴ��ɹ� */
    WINKS_MSG_ENG2UI_ULLOG_OK               /* log��Ϣ�ϴ��ɹ� */
}WinksEng2UiMsg_Setup_e;

typedef enum tag_WinksEng2UiMsg_Upgrade_e
{
/* 7 �汾���� */
    WINKS_MSG_ENG2UI_RUPGRADE_FAIL = -69,  /* ����ʧ�� */

    WINKS_MSG_ENG2UI_RUPGRADE      = 61,   /* ���ڼ���°汾... */
    WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW  ,   /* û���°汾 */
    WINKS_MSG_ENG2UI_RUPGRADE_NEW      ,   /* ���°汾 */
    WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK       /* �°汾���سɹ�����װ */
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
    unsigned long   Notify;         /* ��16λ��ʾ���ڴ���������Ϣ;��16Ϊ��ʾ���յ�����Ϣ 
                                       ��Ϣ��λ��ǣ��ο�����WINKS_NOTIFY_MASK */
    unsigned long   filenumber_log; /* �ļ��� (WINKS_NOTIFY_upldstat/upldlog��Ч) */
    unsigned long   filenumber_stat;/* �ļ��� (WINKS_NOTIFY_upldstat/upldlog��Ч) */

    int             Bit;            /* 0~WINKS_NOTIFY_BITS,��λ��ȡ��Ϣ������ */
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
	struct tag_WinksStrList_s * pnext;	//����ָ��
	char  * str;    /* �绰���� */
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

/*Ӧ�����*/
typedef enum tag_Winks_EngProType_e
{
    WINKS_ENGPROTYPE_REGISTER = 0x00    /*����*/
    ,WINKS_ENGPROTYPE_INIT              /*��ʼ��*/
    ,WINKS_ENGPROTYPE_REGULARLY_UPDATE  /*��ʱ����*/
    ,WINKS_ENGPROTYPE_DIY               /*DIY����*/
    ,WINKS_ENGPROTYPE_RECOMMEND         /*�Ƽ�*/
    ,WINKS_ENGPROTYPE_QUREY_US          /*�û����Ͳ�ѯ*/
    ,WINKS_ENGPROTYPE_UPDATE_FRIENDS    /*���º���*/
    ,WINKS_ENGPROTYPE_UPLOAD_PB         /*�ϴ��绰��*/
    ,WINKS_ENGPROTYPE_UPDATE_SW         /*�������*/
    ,WINKS_ENGPROTYPE_CALL_UPDATE       /*�绰�¼�����*/
    ,WINKS_ENGPROTYPE_HUNGUP_UPDATE     /*�ҶϺ����*/
    ,WINKS_ENGPROTYPE_UPDATE_SC         /*���·�����������Ϣ*/
    ,WINKS_ENGPROTYPE_UPDATE_UC         /*�����û�������Ϣ*/
    ,WINKS_ENGPROTYPE_UPDATE_LS         /*���±���������*/
    ,WINKS_ENGPROTYPE_UPLOAD_LOG        /*�ϴ�log��־*/
    ,WINKS_ENGPROTYPE_UPLOAD_STA        /*�ϴ�ͳ����Ϣ*/
    ,WINKS_ENGPROTYPE_NUMBER
}Winks_EngProType_e;

/*��ϢWINKS_MSG_ENGINE_APP_RETMSG/WINKS_MSG_ENGINE_UIRETMSG�Ľṹ�����������������ص���Ϣ��.*/
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
    void * pUserMap;                /* �û�ռ�� */
    void * pFriendMap;              /* ����ռ�� */
    unsigned short UserNum;         /* �û��� */
    unsigned short FriendNum;       /* ������ */
    unsigned char * pCCCache;       /* ��̬����ͼƬCache */
    unsigned long   CCCacheOff;     /* ͼƬCache���� */
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

/*�û�DIY����*/
typedef struct tag_Winks_DiyInfo_s
{
    int     action;  /* 0 - ֻ�ϴ�������; 1 - �ϴ������� */
    char  * pnolist; /* ��actionΪ1ʱʹ�ã����ƺ��ѵ��ֻ����봮����|�ָ�����|��β��ΪNULL��ʾ������ */
    unsigned char name[40]; /* �û����õĲ�������(unicode����) */
    unsigned char path[WINKS_PATH_MAX]; /* �ļ�·��(unicode����) */
    unsigned char greet[202];         /* �ʺ���(unicode����) */
} Winks_DiyInfo_s;

typedef struct tag_Winks_RecommendInfo_s
{
	unsigned char name[16]; /* ����(unicode����) */
	char    pno[16];        /* �绰���� */
} Winks_RecommendInfo_s;

typedef enum tag_Winks_UrlType_e
{
    WINKS_URLTYPE_CCSHOP =0,  /* �����̳� */
    WINKS_URLTYPE_FAVORITE,   /* �ҵĲ���� */
    WINKS_URLTYPE_SCRIPT,     /* ����ҵ�� */
    WINKS_URLTYPE_PURCHASE,   /* ������� */

    WINKS_URLTYPE_INTRO,      /* �������� */
    WINKS_URLTYPE_HELP,       /* ����ҳ�� */
    WINKS_URLTYPE_ABOUT,      /* ����ҳ�� */
    WINKS_URLTYPE_SETTINGS,   /* �������� */
    WINKS_URLTYPE_DIYCUSTOMIZE,/* DIY���� */

    WINKS_URLTYPE_MAX
}Winks_UrlType_e;

typedef struct tag_WinksEngCall_s
{
    int              CallCCStatus;      /* ���ز���״̬ WinksCallCCStatus_e */
    int              CallType;          /* �����ȥ�� */
    Winks_DateTime_s CallStartTime;     /* �绰�¼���ʼʱ�� */
    char             NO[WINKS_STRG_PHONE_LEN];    /* �绰�����ַ��� */
    WinksStrgFile_CCShow_s  CCShow;     /* ����չʾ */
}WinksEngCall_s;

typedef struct tag_Winks_EngineContext_s
{
    WINKS_CHN_ID    engChn;                 /* ������Ϣͨ��*/
    Winks_EngineState_e curState;           /* ��ǰ����״̬*/
    unsigned long   engAppSequence;
    Winks_EngineAppControl_s engAppStack[WINKS_ENGINE_APP_QUEUE_SIZE];     /*����Ӧ�ÿ��ƿ�ջ*/
    int             http;                   /* ������õ�HTTP���*/
    WINKS_TIMER_ID  httpTimer;              /* http������ʱ�������ڼ�ص���http��ʱ��*/
    unsigned char * pSaveSendData;          /* ���淢�����ݣ��Ա����·��� */

    unsigned long   ProtNo;                 /* Э���WinksProtocolEnum_e */
    unsigned long   FrameNo;                /* ��¼֡�� */
    unsigned char   ifRemindRegister;       /* ��־:�Ƿ���ʾ�û�"ע��". o:����ʾ;1:��ʾ*/
    unsigned char   ifHttpReady;            /* �ж��Ƿ�ɹ�������һ��? */
    unsigned short  getImsiCount;           /* ȡIMSIʧ�ܺ���ʱ��ȡTIMER */
    WINKS_TIMER_ID  getImsiTimer;           /* ȡIMSIʧ�ܴ��� */

    WINKS_TIMER_ID  CallWaitTimer;          /* �绰�¼��ȴ����������ز����Timer */
    WINKS_TIMER_ID  PeriodPushTimer;        /* ��ʱ����Timer */
    WINKS_TIMER_ID  BuildCacheTimer;        /* ��������Cache��Timer */
    WINKS_TIMER_ID  ServerMsgTimer;         /* ���������Ϣ��Timer */
    Winks_SrvNotify_s SrvNotify;            /* ������֪ͨ����Ϣ */
    WinksEngCall_s  *pCall;                 /* ��¼�绰�¼������Ϣ */
    unsigned short  SupportDimNum;          
    unsigned short  SupportExtNum;          
    unsigned short  SupportDim[16][2];      /* ֧�ֵĿ�߱� */
    unsigned char   SupportExt[16];         /* ֧�ֵ��ļ���չ������ */
#ifdef WINKS_DUAL_SIM
    unsigned short  SimStatus;              /* ���ֽڵ�2λ��ʾ�ѻ�ȡ���ſ���IMSI��d0--��1 d1--��2
                                               ���ֽڸ�4λ��ʾ��ȡ���ſ���IMSIʧ�ܣ�d4--��1 d5--��2
                                               ���ֽڱ�ʾ��ǰ����������ſ���      1--��1  2--��2 */
    int             CurInquireSimNo;        /* ��ǰ��ѯ��Sim����: 
                                               1--Sim��1(��ӦWinks_GetImsi); 
                                               2--Sim��2(��ӦWinks_GetImsi_2) */
    int             OtherSimRetryCnt;       /* ��ȡ1��imsi��, ���Զ�ȡ��һ��imsi�Ĵ���, 2��δȡ����Ϊ�������� */
    int             NeedRestoreWapSimSel;   /* �����ֻ�ƽ̨��Wap Simѡ�� */
    int             SavePhoneWapSimSel;     /* �����ֻ�ƽ̨��Wap Simѡ�� */
#endif /* WINKS_DUAL_SIM */
    WinksStrgFileMap_s  StrgFileMap;
#ifdef WINKS_DEBUG
    unsigned long usbFlagHistory[2];         /* ���ڲ��Բ��USB�ߵ�״̬��[0]��¼����,[1]��¼״̬λ���� from d0,*/
    Winks_DateTime_s v_dateTime;             /* ��¼��һ�ζ�ʱ���µ�ʱ�� */
    Winks_DateTime_s v_dateTime1;            /* ��¼��һ�ζ�ʱ���·���رյ�ʱ�� */
    int PTCloseCause;              /* ��¼��ʱ���·���ر�ԭ�� 1--�û��� 2--״̬�ع� 3--��������֧�� 4--Timer��Դ���벻�� 5--����ر�*/
    char    ImsiCode[2][16];
    void  * TstProtMem;
    int     DbgCount;
#endif
}Winks_EngineContext;
#endif/*Engine manager.*/

/* Э������: �������水ָ����Э�鷢�͵�������
����:
    ProtocolNo  : ��Ҫ�����Э��� 
    indata      : ��Ҫ����Ĳ���(�޲�������Ϊ��)
    inLen       : ��������ĳ���(�޲�������Ϊ0)
���:
����:
    �ɹ���ʧ�ܡ�
*/
int Winks_EngProtReq(WinksProtocolEnum_e ProtocolNo, void * inData, int inLen);

int Winks_SendLogToServer(void);
/* 
    ��ȡ�绰��
    ����ǰ�����ڴ��pMemPool�Ա�洢�绰�����ȴ�����������Ҫ�ͷ��ڴ��
*/
int Winks_EngReadPhoneBook( Winks_MemPool_s * pMemPool,
                                       WinksProt_phoneBook_s ** ppphoneBook );
#ifdef WINKS_DUAL_SIM
/* ���ֽڵ�2λ��ʾ�������ſ���  b0--��1; b1--��2
   ���ֽڱ�ʾ��ǰ����������ſ���1--��1;  2--��2
*/
unsigned short Winks_GetSimStatus(void);
#endif
/*
    ע����ϵ��ã�������Ҫ��״̬
*/
int Winks_EngRegistOk(void);
/*
    �ϴ��绰��������Ƭͷ��Ӫ�̲������������������ʱ����
*/
int Winks_EngInitOk(void);

typedef enum tag_WinksResultCode_e
{
    WINKS_RST_CODE_OK   = 0,

/* 1~999    ϵͳ���ô��� */
    RESOURCE_NOT_FOUND  = 1,    /* ��Դδ�ҵ� */
    ACCESS_DENIED       = 2,    /* ���ʱ��ܾ� */
    INVALID_PARAMETER   = 3,    /* ���ò���(�β�)�� */
    BAD_ARGUMENTS       = 4,    /* ������ */
    NOT_IMPLEMENTED     = 5,    /* ������δʵ�� */
    DATABASE_EXCEPTION  = 201,  /* ���ݿ�����쳣 */

/* 1001~1999    ����ͨ�Ŵ��� */
    PROTOCOL_VERSION_NOT_SUPPORTED  = 1001, /* ��֧�ֵ�Э��汾 */

/* 5001~5999    ����ҵ���߼����� */
    SERVICE_NO_SUBSCRIPTION         = 5001, /* ���Ƕ����û� */
    SERVICE_MEMBER_DISABLED         = 5002, /* �û������� */
    SERVICE_NOT_MEMBER              = 5003, /* ���ǲ����û� */
    
    WINKS_RST_CODE_MAX
}WinksResultCode_e;

/* 
    �������ر���
*/
typedef struct tag_Winks_fileinfo_s
{
    WinksFileExt_e  ext;        /* �ļ���չ�� */
    unsigned long   filesize;   /*  */
    Winks_MemPool_s* pMem;      /* ������ص��ļ����� */
    unsigned long   blocklen;   /* ��ǰ���ؿ��ܴ�С(Ӧ�����ڴ��б���pMem->offset�ۼ�ֵ���) */
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
    int     Step;   /* ��ȡһ������Ĳ�����,GetCCStep_e */
    int     Pri;    /* Ӧ�����ȼ� */
    int     AppType;/* Ӧ������ */
    char  * AppName;
/* ԭʼ���� */
    WinksCCDownType_e CCDownType;     /*  */
    WinksStrList_s  * pPNoRoot;       /* �绰������б� */
    WinksStrList_s  * pPNoCurr;       /* ��ǰ�绰���� */
    unsigned short    Total;          /* �绰������ */
    unsigned short    Curr;           /* ��ǰ�ڼ��� */
/* ��1�������� */
    WinksFileType_s * mediaurl;       /* ��Ų����url */
    WinksFileType_s * thumburl;       /* �������ͼ��url */
    unsigned char     media_num;      /* �������� */
    unsigned char     media_idx;      /* �������к� */
    unsigned char     thumbnail_num;  /* ����ͼ�� */
    unsigned char     thumbnail_idx;  /* ����ͼ���к� */
    char              wid[WINKS_PROT_WID_MAX];/* ����ID */
    unsigned char     greet[202];     /* �ʺ���(unicode����) */
    unsigned char     greetlen;       /* �ʺ��ﳤ��(�ֽ���)��������255��ʾɾ����ǰ�ʺ��� */
    unsigned char     img_type;       /* ͼƬ���� WinksFileExt_e */
    unsigned short    interval;       /* ͼƬ���ż�� */
    int               status;         /* �ҵ�״̬ */
    unsigned char     statustxt[100]; /* �ҵ�״̬�ַ���(unicode����) */
    int               statustxtlen;   /* �ҵ�״̬�ַ�����(�ֽ���) */
/* �����ļ���Ҫ������ */
    unsigned short    CCIdx;          /* ����ID */
    unsigned short    SaveIdx;        /* �����ļ������кţ���1�α������ʼ��CCImage���ٴ��ļ������
                                         ������һ���ļ���ر��ļ����SaveIdx��0 */
    WinksStrgFile_CCImage_s CCImage;      /* ���洢�ļ�ʹ�õ��ļ���Ϣ */
}WinksCCDownCtxt_s;

typedef enum tag_Winks_PeriodUpdateMethod_e
{
    WINKS_PERIODUPDATEMETHOD_SAVEMONEY =0,  /* ��ʡ���ø��� */
    WINKS_PERIODUPDATEMETHOD_DATANEW,       /* �������¸��� */
    WINKS_PERIODUPDATEMETHOD_CUSTOM,        /* �û��Զ��� */
    WINKS_PERIODUPDATEMETHOD_MAX
}Winks_PeriodUpdateMethod_e;

typedef struct tag_Winks_PeriodUpdateInfo_s
{
    int             Method;     /* ���·�ʽ Winks_PeriodUpdateMethod_e */
    unsigned char   Hour;       /* ����ʱ��--ʱ */
    unsigned char   Min;        /* ����ʱ��--�� */
    unsigned short  Period;     /* ��������(�Է���Ϊ��λ) */
}Winks_PeriodUpdateInfo_s;

#define PU_SAVEMONEY_DFT_PERIOD     (24*60)     /* 1�� */
#define PU_DATANEW_DFT_PERIOD       (2*60)      /* 2Сʱ */

int Winks_EngCCDownCtxtCreate(WinksCCDownType_e CCDownType, char *pNo, WinksCCDownCtxt_s ** ppCCDownCtxt);
int Winks_EngCCDownCtxtDestory(WinksCCDownCtxt_s * pCCDownCtxt);
int Winks_EngCCDownCtxtSaveUrl( WinksFileType_s ** ppurlList, 
                                           WinksFileType_s * pWinksUrl );
int Winks_EngCCDownCtxtSaveVCard(VcardType_s * vcard);
/* �����ʺ����Ҳ��Һ��뼰������Ϣ��
   �ɹ������Ƿ���Ҫ���ز��� */
int Winks_EngSearchCCInfo(WinksCCDownCtxt_s *pDownCC, int NeedWashOut);
/* �յ����ز���ĵ�1��ʱ���ã�
   �ɹ����ز���IDX */
int Winks_EngSaveCCInfo(WinksCCDownCtxt_s *pDownCC, int NeedWashOut);
/* ������ͨ������Ĳ������øú��� */
int Winks_EngUpdateCCShow(void);
/* ������ͨ������Ĳ���ʧ�� */
int Winks_EngUpdateCCShowFail(void);
/* ������Cache�����ɹ����� */
int Winks_EngCCCacheOk(void);
/* ��ע����ϳ�ʼ��Ƭͷ��Ӫ�̲���ʱ����, PriΪ���ȼ� */
int Winks_EngInitSpecialCC(int Pri);
/* ���������صĴ����봦����
   retUIMsg������ŷ���UI����Ϣ
   ����ֵ: -1 -- ����ʧ��
            0 -- δ����
            1 -- �Ѵ���
*/
int Winks_EngResultErrDftHandler(WinksProt_result_s * presult, int *retUIMsg);

/* 
    �������UI�İ���body����Ϣ
*/
int Winks_EngPostMsgToUI(int Pri, Winks_EngProType_e appType, int msg, void *data, int size);

int winks_StrgDeleteUnUsedCC(unsigned short *CCIdxLeft, int CCLeftCnt);
int Winks_GetCCStart(int Pri, WinksCCDownType_e CCDownType, char * pNo);
int Winks_GetCCStop(WinksCCDownType_e CCDownType);

/*�����㷨(base64)
 * function: Winks_Encode
 * parameters:
 *   char *src: ����
 *   int src_len: ���ĳ���
 *   char *dst: ����
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
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
����:
    Pri --- ���ȼ�
    Type--- SynsetType_e
    pData:�� Type == SYNSET_TYPE_UPLOAD_STAT��SYNSET_TYPE_UPLOAD_LOGʱ,����ΪUploadFile.fileNumber
             Type == SYNSET_TYPE_SETUSERCONFIGʱ,����ΪWinksProt_userConfig_s�ṹ
*/
int Winks_SynsetStart(int Pri, SynsetType_e Type, void * pData);
int Winks_SynsetStop(void);

/*-------------------------
        �������̽ӿ�
---------------------------*/
#define WINKS_SMS_GATEWAY_NUM 2         /*��������������*/
#define WINKS_SMS_GATEWAY_LEN 16        /*����������󳤶�*/
#define WINKS_SMS_PREFIX_LEN 16          /*����ǰ׺��󳤶�*/

#define WINKS_ENGAPP_REG_FREGIST 0x01   /*ǰ̨����*/
#define WINKS_ENGAPP_REG_BREGIST 0x02   /*��̨����*/
#define WINKS_ENGAPP_REG_QUERYUS 0x03   /*�û����Բ�ѯ���Ƿ񼤻���á��˶��û�*/
#define WINKS_ENGAPP_REG_UPLOADPB 0x04  /*�ϴ��绰��*/

#define WINKS_REG_RUNCTR_PAUSE 1
#define WINKS_REG_RUNCTR_STOP 2

int Winks_UserRegistExt(unsigned long controller);
/*�û�ע�����ҵ�񣬹�UIʹ�á�����ֵ:0�ɹ�,-1ʧ�ܡ�*/
int Winks_UserRegist(void);
/*������ȡ�û�����״̬�����������غ������Winks_UserGetState��ȡ״̬��*/
int Winks_UserCheckState(void);
/*��ȡ�����û�����״̬������Winks_UserCheckState�����±���״̬*/
int Winks_UserGetState(int *state);
/*�˳��������̡���������ѯ�û�״̬����*/
int Winks_UserRegistStop(void);
/*�û�ȷ��ע�ᣬ����ע�����Ϣ,0:ȡ��ע�ᣬ1:����ע��*/
int Winks_UserSendRegSms(int reg_flag);
/*�û�ȷ���Ƿ��͵绰��������ֵ:0�ɹ� ����ʧ�ܡ�
  ��ע������Ҳ���Ե��øú����ϴ��绰��
ulpb_flag: 0ȡ���ϴ���1�ϴ��绰��*/
int Winks_UserUploadPhoneBook(int ulpb_flag);
/*    ȡ���浱ǰ��״̬��*/
int Winks_GetEngineCurrentState(void);
/*    �������浱ǰ��״̬��*/
int Winks_SetEngineCurrentState(int state);
#endif 
