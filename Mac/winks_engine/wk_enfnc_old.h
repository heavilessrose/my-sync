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
    WINKS_MSG_ENG2UI_NONE           = 0,    /* XX �޹���Ϣ */
    
/* �ɹ���������Ϣ */
    
/* 1 ע�ἰ��ʼ������ */
    WINKS_MSG_ENG2UI_SETUP_SVR_OK   = 0x11, /* XX �������÷������ɹ� */
    WINKS_MSG_ENG2UI_SMS_REG        ,       /* δע�ᣬ��Ҫ����ע�����     [Winks_UserSendRegSms] */
    WINKS_MSG_ENG2UI_SMS_REG_OK     ,       /* ����ע��OK */
    WINKS_MSG_ENG2UI_INIT           ,       /* XX ���ڳ�ʼ��... */
    WINKS_MSG_ENG2UI_INIT_OK        ,       /* ��ʼ����� */
/* 2 ���º���¼ */
    WINKS_MSG_ENG2UI_FL_OK          = 0x21, /* XX �����б�������� */
    WINKS_MSG_ENG2UI_FL_NOTIFY      ,       /* ֪ͨ�û��к��Ѹ��� */
    WINKS_MSG_ENG2UI_FL_SEL_FRIEND  ,       /* �����б�������ϣ�ѡ�����¼ [Winks_UserSetFriendList] */
    WINKS_MSG_ENG2UI_FRIEND_SET     ,       /* XX �ϴ�����¼�ɹ� */
    WINKS_MSG_ENG2UI_FRIEND_UPD     ,       /* XX �ϴ�����¼�����غ������� */
    WINKS_MSG_ENG2UI_DL_FRIENDCC    ,       /* �������غ��Ѳ���... */
    WINKS_MSG_ENG2UI_FRIEND_OK      ,       /* ����¼������� */
#if 0//def WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_FL_SEL_MCARD   ,       /* �����б�������ϣ�ѡ��洢�� [Winks_UserSelMCard] */
#endif
    
/* 3 ͬ���������õ������� */
    WINKS_MSG_ENG2UI_SETUP_OK       = 0x31, /* ͬ���������óɹ� */
    
/* 4 �ϴ�DIY */
    WINKS_MSG_ENG2UI_DIY            = 0x41, /* XX �����ϴ�DIY... */
    WINKS_MSG_ENG2UI_DIY_OK         ,       /* �ϴ�DIY�ɹ� */
    
/* 5 ���¼����ѯ������ע��״̬ */
    WINKS_MSG_ENG2UI_REREG_OK       = 0x51, /* ���¼���ɹ� */

/* 6 �绰���� */
    WINKS_MSG_ENG2UI_CALLCC_GRT_OK  = 0x61, /* ��ȡ�û��ʺ���ɹ� */
    WINKS_MSG_ENG2UI_CALLCC_OK      ,       /* ��ȡ�û��ʺ���ɹ� */
    WINKS_MSG_ENG2UI_CALLCC_OTHER   ,       /* XX��ȡƬͷ��Ӫ�̲��� */
    
/* 7 ��̨���� */
    WINKS_MSG_ENG2UI_PUSH           = 0x71, /* XX ��̨���ڶ�ʱ���·���... */
    WINKS_MSG_ENG2UI_PUSH_FRIEND    ,       /* XX ��̨���ڸ��º���... */
    WINKS_MSG_ENG2UI_PUSH_OK        ,       /* XX ��̨��ʱ���·������ */
    WINKS_MSG_ENG2UI_PUSH_CHECK_FL  ,       /* XX ��̨��ʱ���·������,����Ҫ�������б� */

    WINKS_MSG_ENG2UI_INITCACHE_FL   ,       /* XX ��̨�������غ����б��Ա��ʼ��Cache... */
    WINKS_MSG_ENG2UI_INITCACHE      ,       /* XX ��̨���ڳ�ʼ��Cache... */
    WINKS_MSG_ENG2UI_INITCACHE_OK   ,       /* XX ��̨��ʼ��Cache��� */

    WINKS_MSG_ENG2UI_RECACHE_TRIM   ,       /* XX ��̨�������ػ�����Ϣ���ؽ�Cache... */
    WINKS_MSG_ENG2UI_RECACHE        ,       /* XX ��̨�����ؽ�Cache... */
    WINKS_MSG_ENG2UI_RECACHE_OK     ,       /* XX ��̨�ؽ�Cache��� */

    WINKS_MSG_ENG2UI_RECRUIT_DL     ,       /* XX ��̨���ڲ������ص绰����... */
    WINKS_MSG_ENG2UI_RECRUIT_DL_OK  ,       /* XX ��̨�������ص绰������� */

/* ������Ϣ */

/* 1 ע�ἰ��ʼ������ */
    WINKS_MSG_ENG2UI_REG_FAIL       = 0x8011,   /* ע��ʧ�� */
    WINKS_MSG_ENG2UI_INIT_FAIL      ,           /* ��ʼ��ʧ�� */

/* 2 ���º���¼ */
    WINKS_MSG_ENG2UI_FL_FAIL        = 0x8021,   /* ���º���¼ʧ�� */

/* 3 ͬ���������õ������� */
    WINKS_MSG_ENG2UI_SETUP_FAIL     = 0x8031,   /* ͬ����������ʧ�� */

/* 4 �ϴ�DIY */
    WINKS_MSG_ENG2UI_DIY_FAIL       = 0x8041,   /* �ϴ�DIYʧ�� */

/* 5 ���¼����ѯ������ע��״̬ */
    WINKS_MSG_ENG2UI_REREG_FAIL     = 0x8051,   /* ���¼���ʧ�� */

/* 6 �绰���� */
    WINKS_MSG_ENG2UI_CALLCC_GRT_FAIL  = 0x8061, /* �ʺ�������ʧ�� */
    WINKS_MSG_ENG2UI_CALLCC_FAIL      ,         /* �û���������ʧ�� */
    WINKS_MSG_ENG2UI_CALLCC_TIMEOUT   ,         /* �û��������س�ʱ */
    WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL ,         /* XX��ȡƬͷ��Ӫ�̲���ʧ�� */

/* 7 ��̨���� */
    WINKS_MSG_ENG2UI_PUSH_FAIL      = 0x8071,   /* XX ��̨��ʱ���·���ʧ�� */

    WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL      ,   /* XX ��̨���غ����б�ʧ�� */
    WINKS_MSG_ENG2UI_INITCACHE_FAIL         ,   /* XX ��̨��ʼ��Cacheʧ�� */

    WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL      ,   /* XX ��̨���ػ�����Ϣʧ�� */
    WINKS_MSG_ENG2UI_RECACHE_FAIL           ,   /* XX ��̨�ؽ�Cacheʧ�� */

    WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL        ,   /* XX ��̨�������ص绰����ʧ�� */

/* ����Э����� */
    WINKS_ENG_ERROR_CALL_FUNC = 0x8101,     /* ���ú������� */
    WINKS_ENG_ERROR_PROT_NO     ,           /* �������·�Э��Ų�ƥ�� */
    WINKS_ENG_ERROR_SVR_RET     ,           /* �������������ݴ��� */
    WINKS_ENG_ERROR_DATA_PARSE  ,           /* �������������ݽ������� */
    WINKS_ENG_ERROR_DATA_RSV    ,           /* ��������ʧ��*/
/* ϵͳ���� */
    WINKS_MSG_ENG2UI_SVR_NO_ANSWER  = 0x8201,   /* ��������Ӧ�� */
    WINKS_MSG_ENG2UI_UNREG_USER     ,           /* ��ע���û� */
    WINKS_MSG_ENG2UI_ROAMING_LIMIT  ,           /* ��������  */
    WINKS_MSG_ENG2UI_ENG_BUSY       ,           /* ����æ,�Ժ����� */
    WINKS_MSG_ENG2UI_DIAL_INT       ,           /* ���汻�绰�ж� */
    WINKS_MSG_ENG2UI_STORAGE_LIMIT  ,           /* �洢�ռ䲻�� */
    WINKS_MSG_ENG2UI_CC_CLOSED      ,           /* ��������ر�  */
#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_MCARD_NOT_EXIST,           /* �洢�������� */
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    WINKS_MSG_ENG2UI_SYS_FILES_ERR  ,           /* ϵͳ�ļ����ƻ� */
//#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_CC_FILES_ERR   ,           /* �����ļ����ƻ� */
//#endif /* WINKS_SUPPORT_MEMORY_CARD */
#ifdef WINKS_DUAL_SIM
    WINKS_MSG_ENG2UI_SIM_SEL_ERR    ,           /* ��ʾ�û�ѡ��Ҫ�����Sim�� */
    WINKS_MSG_ENG2UI_SIM_UNUSABLE   ,           /* �û�ѡ���Sim�������� */
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
    unsigned char *pSendBuf;    /*�������������*/
    unsigned char baseBuff[WINKS_SEND_FRAME_LEN];   /*�����Ƚϴ�,�������ռ�,����ԭ������ĵ�ַ,�ȷ��������ٻָ�*/
    unsigned long   Len;         /*�ܳ���*/
    unsigned long   Off;         /*ƫ��*/
    int   ErrCode;     /* ������̳��ֵĳ�����Ϣ�� (WINKS_ENG_ERROR_CODE_x) */
}Winks_SendFrame_s;

/* ���ڶԺ����б����� */
typedef struct {
    char data[WINKS_FRIEND_NO_LEN];
    unsigned int index;
    unsigned int flag;    /* ��ʶ��ͬ�ĺ������� */
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
    WINKS_ENGINE_APP_NULL = 0x00,       /* �� */
    WINKS_ENGINE_APP_REG = 0x01,        /* ���� */
    WINKS_ENGINE_APP_UPDATE_FRIEND,     /* ���º���¼ */
    WINKS_ENGINE_APP_SYNC_SETTING,      /* ͬ������ */
    WINKS_ENGINE_APP_UPLOAD_DIY,        /* �ϴ�DIY */
    WINKS_ENGINE_APP_REREG,             /* ���¼��� */
#ifdef WINKS_SUPPORT_3G
    WINKS_ENGINE_APP_CALL_DOWNLOAD,     /* �绰���� */
#endif /* WINKS_SUPPORT_3G */
/* ��̨���� */
    WINKS_ENGINE_APP_INIT_CACHE,        /* ��ʼ��Cache */
    WINKS_ENGINE_APP_REBULID_CACHE,     /* �ؽ�Cache */
    WINKS_ENGINE_APP_PERIOD_UPDATE,     /* ��ʱ���� */
    WINKS_ENGINE_APP_RECRUIT_DOWNLOAD,  /* �������� */
    WINKS_ENGINE_APP_NUMBER
}Winks_EngineApp_e;

typedef struct tag_Winks_ConnectServerInfo_s
{
    unsigned char srv;                                /*0:���÷�����; 1:ͨѶ������*/
    unsigned char staDataSrv;                   /*����ͨѶ����������ʼ�±�*/
    unsigned char tryCnt;                           /*��������*/
    unsigned char datainFlag;                    /*����ͨѶ���������յ��Ƿ��յ����ݱ�־*/
    char *data;
    unsigned long len;
}Winks_ConnectServerInfo_s;
    
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

typedef struct tag_Winks_EngineContext_s
{
    WINKS_CHN_ID engChn;                     /*������Ϣͨ��*/
    int InitSettingOk;                       /*�洢�������ļ��Ƿ�ok*/
    int http;                                /*������õ�HTTP���*/
    WINKS_TIMER_ID httpTimer;                /*http������ʱ�������ڼ�ص���http��ʱ��*/
    Winks_EngineState_e curState;            /*��ǰ����״̬*/
    Winks_EngineSubState_e subState;         /*��״̬����״̬*/
    Winks_EngineApp_e   app;                 /* ��¼Ӧ�� */
    unsigned char isImsiEnabled;             /*��־:�û��Ƿ�ע��winks*/
    unsigned char ifRemindRegister;          /*��־:�Ƿ���ʾ�û�"ע��". o:����ʾ;1:��ʾ*/
    unsigned char isCallCCOk;                /*��־:�绰�����Ƿ�ok 0:δ����, 1:��������, 2:ʧ��, 0xff:�ɹ�*/
    //unsigned char   IsPeriodUpdating;        /* �Ƿ����ڶ�ʱ���� [0]û�� [1]��ʱ����״̬1,2 [2]��ʱ���¸��º��� */
    unsigned char   pad;/* 2�ֽڶ��� */
    unsigned long   lastState;
#ifdef WINKS_DUAL_SIM
    unsigned short SimStatus;                /* ���ֽڵ�2λ��ʾ�ѻ�ȡ���ſ���IMSI��b0--��1 b1--��2
                                                ���ֽڱ�ʾ��ǰ����������ſ���      1--��1  2--��2 */
    int            CurInquireSimNo;          /* ��ǰ��ѯ��Sim����: 
                                                1--Sim��1(��ӦWinks_GetImsi); 
                                                2--Sim��2(��ӦWinks_GetImsi_2) */
    int            OtherSimRetryCnt;         /* ��ȡ1��imsi��, ���Զ�ȡ��һ��imsi�Ĵ���, 2��δȡ����Ϊ�������� */
    int            NeedRestoreWapSimSel;     /* �����ֻ�ƽ̨��Wap Simѡ�� */
    int            SavePhoneWapSimSel;       /* �����ֻ�ƽ̨��Wap Simѡ�� */
#endif /* WINKS_DUAL_SIM */
    WinksStrgFileMap_s  StrgFileMap;
    Winks_Setting_s setting;                 /*ȫ��������Ϣ*/
//    Winks_FriendList_in_RAM_s friendlist_ram;   /* �ڴ��еĺ���¼ */
    /*Winks_EngDS_s *socketData_p;                   */ /*HTTP���������*/
    void *communicationContext_p;            /*Э��ͨѶ������*/
    Winks_ConnectServerInfo_s server;        /*Э����������*/
    WINKS_TIMER_ID  PeriodPushTimer;         /*��ʱ����Timer*/
    WINKS_TIMER_ID  BuildCacheTimer;         /*��������Cache��Timer*/
    WINKS_TIMER_ID  CallWaitTimer;           /*�绰�¼��ȴ����������ز����Timer*/
    unsigned short  ProtocolMsgType;          /* ��¼���͵�Э��MsgType */
    unsigned short  ProtocolFrameNo;          /* ��¼���͵�Э����� */
    unsigned short  getImsiCount;             /* ȡIMSIʧ�ܺ���ʱ��ȡTIMER */
    WINKS_TIMER_ID  getImsiTimer;             /* ȡIMSIʧ�ܴ��� */
    WinksStrgFile_CCShow_s  * pCCShow;              /* ����չʾ */
    char    Call_NO[20/*WINKS_STRG_PHONE_LEN*/];   /* �绰�¼������ַ��� */
    char    Call_ID[24/*WINKS_STRG_CC_ID_LEN*/];   /* �绰�¼�����ID */
    int     CallType;                        /* �����ȥ�� */
    Winks_DateTime_s CallStartTime;          /* �绰�¼���ʼʱ�� */
#ifdef WINKS_DEBUG
    unsigned long usbFlagHistory[2];          /* ���ڲ��Բ��USB�ߵ�״̬��[0]��¼����,[1]��¼״̬λ���� from d0,*/
    Winks_DateTime_s v_dateTime;              /* ��¼��һ�ζ�ʱ���µ�ʱ�� */
    Winks_DateTime_s v_dateTime1;             /* ��¼��һ�ζ�ʱ���·���رյ�ʱ�� */
    int PTCloseCause;              /* ��¼��ʱ���·���ر�ԭ�� 1--�û��� 2--״̬�ع� 3--��������֧�� 4--Timer��Դ���벻�� 5--����ر�*/
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
    char msgGateway[WINKS_MAX_LEN_OF_MSG_GATEWAY];       /*����ע����ŵ�����*/
    unsigned long shortTime;  /*��ѯע��ɹ��Ķ�����*/
    unsigned long longTime;  /*��ѯע��ɹ��ĳ�����*/
    unsigned long count;  /*��ѯע��ɹ��ļ�����*/
    WINKS_TIMER_ID timerId;            /*��ѯע��ɹ��Ķ�ʱ��*/
    Winks_server_list_s server;
    unsigned char msg[WINKS_ENGINE_RETURN_MSG_MAX_LEN];
}Winks_EngPrtReg_s;

#define WINKS_ENGINE_MAX_LEN_OF_UPLOAD (2048)
#define DIY_UPLOAD_RETRY_MAX        3
typedef struct tag_Winks_EngDiyComInfo_s
{
    char number[WINKS_FRIEND_NO_LEN]; /*���ƺ��ѵ��ֻ�����*/
    unsigned char image_type;   /*��������*/
    unsigned char image_count;                              /*����������(n)*/
    char image_files[WINKS_MAX_NUM_OF_IMAGE_LIST][WINKS_PATH_MAX];
    char welcome[WINKS_MAX_LEN_OF_WELCOME_INFO]; /*�ʺ���*/

    unsigned char   curIndex;               /*��ǰ�ϴ��ڼ���(x/n)*/
    /*unsigned char   sendPercent;*/            /*�ѷ������ݵİٷֱ�(100Ϊȫ������)*/
    unsigned char   RetryCnt;           /*�ϴ������ش�����*/
    unsigned long curFileSize;     /*��ǰ�ϴ��ļ��Ĵ�С(size of(x))*/
    unsigned short  subIndex;               /*��ǰ�ϴ���Ϊ�ڼ���(y/m)*/
    unsigned short  subTotalCount;          /*��ǰ�����Ϊ����(��curIndex��Ϊm��)*/
    unsigned long   curSubSize;
}Winks_EngDiyComInfo_s;

typedef struct tag_Winks_EngDiyProgInfo_s
{
    unsigned char   image_count;            /*����������(n),ĿǰֻΪ1���ɲ�����*/
    unsigned char   curIndex;               /*��ǰ�ϴ��ڼ���(x/n),ĿǰֻΪ1���ɲ�����*/
    unsigned short  subTotalCount;          /*��ǰ�����Ϊ����(��curIndex��Ϊm��)*/
    unsigned short  subIndex;               /*��ǰ�ϴ���Ϊ�ڼ���(y/m)*/
}Winks_EngDiyProgInfo_s;

typedef struct tag_Winks_EngUpdComInfo_s
{
    unsigned long   patchTotal;         /*��Ƭ���ص���Ƭ��*/
    unsigned long   patchCount;         /*��Ƭ���صļ�����*/
    unsigned long   prtRetIndex;        /*���������ص����к�*/
}Winks_EngUpdComInfo_s;

/* �����б�֪ͨ���� */
typedef enum {
    E_FL_UPD_NONE = 0,  /* �޸��� */
    E_FL_UPD_NOTIFY,    /* �и��� */
    E_FL_UPD_SELFRIEND, /* ѡ�����¼ */
    E_FL_UPD_IF_SELMCARD   /* �Ƿ�ѡ��洢�� */
} Winks_FLNotifyType_e;

typedef struct tag_Winks_EngPeriodComInfo_s
{
    unsigned char   FA_UpdFlag;         /*�û����ѹ�ϵ���±�־ 0--�޸���,1--�и���,2--ѡ�����¼*/
    int   Reserved;                     /*���� -- �ϴ��ֻ�ͳ����Ϣ����*/
}Winks_EngPeriodComInfo_s;

typedef struct tag_Winks_EngTimeFlag_s
{
    unsigned long Type; /* ��4λ���α�ʾ����ʱ���:��Ƭ(bit0)�����Ѳ���(bit1)��Ƭͷ����(bit2)����Ӫ�̲���(bit3) */
    char TimeFlag[WINKS_MAX_LEN_OF_TIME_FLAG];
}Winks_EngTimeFlag_s;

#if 0
typedef struct tag_Winks_EngRegComInfo_s
{
    char msgGateway[WINKS_MAX_LEN_OF_MSG_GATEWAY];       /*����ע����ŵ�����*/
    unsigned long shortTime;  /*��ѯע��ɹ��Ķ�����*/
    unsigned long longTime;  /*��ѯע��ɹ��ĳ�����*/
    unsigned long count;  /*��ѯע��ɹ��ļ�����*/
    WINKS_TIMER_ID timerId;            /*��ѯע��ɹ��Ķ�ʱ��*/
    unsigned short priCCImageTotal;   /*����Ƭͷ�������*/
    unsigned short priCCImageCount;   /*����Ƭͷ���������*/
    unsigned short busCCImageTotal; /*������Ӫ�̲������*/
    unsigned short busCCImageCount; /*������Ӫ�̲��������*/
    unsigned long  filesize;        /*�ϴ����ļ���С*/
    unsigned short patchTotal;          /*��Ƭ���ص���Ƭ��*/
    unsigned short patchCount;        /*��Ƭ���صļ�����*/
    unsigned short patchSize;       /*��ǰ���Ĵ�С*/
    unsigned short imageTotal;
    unsigned short imageCount;
    unsigned char msg[WINKS_ENGINE_RETURN_MSG_MAX_LEN];
}Winks_EngRegComInfo_s;

typedef struct tag_Winks_EngUpdComInfo_s
{
    unsigned short priCCImageTotal;   /*����Ƭͷ�������*/
    unsigned short priCCImageCount;   /*����Ƭͷ���������*/
    unsigned short busCCImageTotal; /*������Ӫ�̲������*/
    unsigned short busCCImageCount; /*������Ӫ�̲��������*/
    unsigned short patchTotal;          /*��Ƭ���ص���Ƭ��*/
    unsigned short patchCount;        /*��Ƭ���صļ�����*/
    unsigned short imageTotal;
    unsigned short imageCount;
}Winks_EngUpdComInfo_s;

#endif

typedef struct tag_Winks_EngDLCCInfo_s
{
    unsigned short   Total;         /* ��Ҫ���صĲ������� */
    unsigned short   Count;         /* ���ز������ */
    unsigned short   FinCnt;        /* ���سɹ������� */
    unsigned short   ErrCnt;        /* ����ʧ�ܲ����� */
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

    unsigned char   img_idx;    /* [O/I]�ڼ���ͼƬ */
    unsigned char   img_type;   /* [O/I]�����ļ����� */
    unsigned char   GreetLen;   /* [Sum]�ʺ��ﳤ�� */
    unsigned char   GreetValid; /* [I]�ʺ����Ƿ���Ч */
    unsigned short  CC_Idx;
    unsigned short  GreetStr[101];/* [O] */
    unsigned long   FileSize;   /* [O]�ļ���С */
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
    /* ��Ϣ���� */
    unsigned short msgType;
    /* ���ݽ������� , ����ΪNULL*/
    Winks_EngAnlArr_f parseFunc;
    /* ������������ķ������� */
    Winks_EngAnlEnd_f retData;
} winks_DSA_H_Table;

typedef struct {
    unsigned short type;    /* �ṹ�ֶ����� */
    unsigned long len;      /* ��Ч���ݳ��� */
    unsigned long offset;   /* ƫ�� */
} wink_DSA_StructInfo;


typedef struct tag_Winks_CcReq_s/*�������������Դ*/
{
    char            ID[40/*WINKS_STRG_CC_ID_LEN*/];           /* ����ID */
    unsigned char   cc_type;    /* �������� */
    unsigned char   img_cnt;    /* ������ͼƬ */
    unsigned char   img_idx;    /* �ڼ���ͼƬ */
}Winks_CcReq_s;


/*���Բ��ṹ*/
typedef struct tag_Winks_TestModeTable
{
    char keyString[WINKS_TEST_KEY_WORDS_LEN+1];
    int key;
}Winks_EngDSItem_s;


/*�����ʼ��*/
int Winks_EngineInit(void);
/*����ע��*/
int Winks_EngineDestroy(void);

#ifdef WINKS_DUAL_SIM
/* �����ſ�ʱ,�û�ѡ�����ſ����� */
int Winks_UserSetActiveSim(int SimNo);
#endif /* WINKS_DUAL_SIM */
/*�û�ע�����ҵ�񣬹�UIʹ�á�����ֵ:0�ɹ�,-1ʧ�ܡ�*/
int Winks_UserRegist(void);

/*�û����º�����¼*/
int Winks_UserResetFriendList(void);

/*�����û�ѡ�����¼��friend_list:��"|"�ָ��ĺ���.����"13900000001|13900020002|"*/
int Winks_UserSetFriendList(char *friend_list, unsigned long friend_list_len);

#if 0//def WINKS_SUPPORT_MEMORY_CARD
/* ���ֻ��ռ䲻���Ҵ洢���ռ����ʱѯ���û��Ƿ�ʹ�ô洢��,��ѡ��洢��ʱ���ô˺��� */
int Winks_UserSelMCard(int YesOrNo);
#endif /* WINKS_SUPPORT_MEMORY_CARD */
#if 1
int Winks_UserSetFriendBegin(void);
int Winks_UserSetFriendAddItem(Winks_FriendItem_s *item);
int Winks_UserSetFriendEnd(int commit_flag);
#endif
/*ȡ�������������ı�־. allowedFlag:0����;1������*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag);

/*�ж��Ƿ�ע���û��� regFlag:0δע�ᣬ1��ע��*/
int Winks_isUserRegistered(unsigned char *regFlag);

/*�û�DIY����*/
typedef struct tag_Winks_DiyInfo_s
{
    char number[WINKS_FRIEND_NO_LEN]; /*���ƺ��ѵ��ֻ�����*/
    unsigned char image_type;   /*��������*/
    unsigned char image_count;
	char image_files[WINKS_MAX_NUM_OF_IMAGE_LIST][WINKS_PATH_MAX]; /* UTF-8 ���� */
	char welcome[WINKS_MAX_LEN_OF_WELCOME_INFO]; /* �ʺ���(UTF-8 ����) */
} Winks_DiyInfo_s;
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info);

#ifdef WINKS_DEL_PV2_0    
/*�û����Ի�����: ͬ��������*/
typedef struct tag_Winks_SelfSetting_s
{
    /* 1: ����, 0: �ر� */
    unsigned char ccSwitch;  /* �Է�չʾ�ҵĲ���(�ϴ�������+���ر���) */
    unsigned char ShowCC;  /* ��չʾ�Է�����(���ر���) */
} Winks_SelfSetting_s;
int Winks_UserUploadSelfSetting(Winks_SelfSetting_s *self_info);
#else
typedef struct tag_Winks_SelfSetting_s
{
    /* 1: ����, 0: �ر� */
    unsigned char ccSwitch;  /* �Է�չʾ�ҵĲ���(�ϴ�������+���ر���) */
    unsigned char ShowCC;  /* ��չʾ�Է�����(���ر���) */
} Winks_SelfSetting_s;
#define Winks_UserUploadSelfSetting(x) 
#endif
/* �û����ӷ�ʽ����:Push��ʽ��ʱ���� */
int Winks_UserSetConnectWay(unsigned long ConnWay);

/*ȡ��ǰ�����*/
int Winks_GetCurAccessPoint(unsigned long *accessPoint);
/*���õ�ǰ�����*/
int Winks_SetCurAccessPoint(unsigned long accessPoint);

#define WINKS_REAL_TIME_INFO_OPT_USER_STATE (1UL)
#define WINKS_REAL_TIME_INFO_OPT_SHOW_FLAG (2UL)
#define WINKS_REAL_TIME_INFO_OPT_CARD_SWITCH (4UL)
#define WINKS_REAL_TIME_INFO_OPT_USER_STATE_MSG (8UL)
/*ʵʱ��Ϣ�ṹ��ͬ��������*/
typedef struct tag_Winks_RealTimeInfo_s
{
    unsigned long opt;
    unsigned char userState;    /*      �û�״̬��־:
                                ���� �� 01H��
                                �뿪 �� 02H��
                                ���� �� 03H��
                                æµ �� 04H��
                                ���� �� 05H
                            */
    unsigned char showFlag; /*  "�Է��Ƿ�չʾ�ҵĲ���"���ܿ���:
                                01H : �û������������
                                02H : �û��رղ������
                            */
    unsigned char cardSwitch;   /*  "�Ƿ�չʾ�����Ƭ"���ܿ���:
                                ������ ��     01H
                                ��������Ϊ��  02H
                            */
    unsigned char userStateMsg[1];/*�û�״̬����:(Ԥ���ӿڣ��ݲ�ʹ��)
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
