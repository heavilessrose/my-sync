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
    
    WINKS_MSG_ENG2UI_FRIEND         = 71,   /* ���ڸ��º���¼... */
    WINKS_MSG_ENG2UI_FRIEND_OK              /* ����¼������� */
}WinksEng2UiMsg_Friend_e;
    
typedef enum tag_WinksEng2UiMsg_Diy_e
{
/* 3 �ϴ�DIY */
    WINKS_MSG_ENG2UI_DIY_FAIL       = -29,  /* �ϴ�DIYʧ�� */
    WINKS_MSG_ENG2UI_SCRIPT_FAIL,           /* ���Ʋ���ʧ�� */

    WINKS_MSG_ENG2UI_DIY            = 21,   /* �����ϴ�DIY... [WinksProgress_s] */
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

/* Ƭͷ��Ӫ�̰�������Ѵ��� */
#define WINKS_DFT_CC_NO     "1"     /* Ĭ�ϲ���ID */
#define WINKS_PRI_CC_NO     "11"    /* Ƭͷ������ʼID */
#define WINKS_PRI_CC_MAX    1       /* Ƭͷ���������� <10 */
#define WINKS_BUS_CC_NO     "21"    /* ��Ӫ�̲�����ʼID */
#define WINKS_BUS_CC_MAX    1       /* ��Ӫ�̲�������� <10 */

typedef struct tag_WinksStrgFile_CCShow_s
{
    int             RetStatus;                         /* �����Ƿ���ú����Ӧ
                                                          0 -- ok
                                                          1 -- Ĭ�ϲ���
                                                          2 -- Ƭͷ 
                                                          3 -- ��Ӫ�� */
    char            NO[16/*WINKS_STRG_PHONE_LEN*/];    /* �绰�����ַ��� */
    char            Name[32/*WINKS_STRG_NICK_LEN*/];   /* �����ǳ� */
    char            ID[40/*WINKS_STRG_CC_ID_LEN*/];    /* ����ID */
    unsigned short  CC_Idx;     /* ����Idx */
    unsigned short  interval;   /* ͼƬ���ż�� */
    unsigned char   img_type;   /* �������� WinksFileExt_e*/
    unsigned char   img_cnt;    /* ͼƬ�� ���Ϊ4 */
    unsigned char   GreetLen;   /* �ʺ��ﳤ�� */
    unsigned char   SaveType;   /* ����洢���� 0--�ļ� 1--�ڴ� */
    unsigned char * pCCImage;   /* �洢�Ĳ����ڴ����� */
    unsigned long   CCImageSize;/* �洢�Ĳ����ڴ����ݴ�С */
    char            CCImgPath[4][256];     /* �洢�Ĳ���·�� */
    short           GreetStr[101/*WINKS_STRG_GREET_LEN/2*/];     /* �洢�Ĳ����ʺ��� */
}WinksStrgFile_CCShow_s;

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


/*�����ʼ��*/
int Winks_EngineInit(void);
/*����ע��*/
int Winks_EngineDestroy(void);

#ifdef WINKS_DUAL_SIM
/* �����ſ�ʱ,�û�ѡ�����ſ����� */
int Winks_UserSetActiveSim(int SimNo);
#endif /* WINKS_DUAL_SIM */


/*-------------------------
        ��������UI�ӿ�
---------------------------*/
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

/*��������汾��0:��̨����, 1�û��ֶ�����*/
int Winks_UpgradeStart(int type);

/*�û���Ӻ��ѻ��޸ĺ����ǳƣ��ǳ�Ϊunicode���룩*/
int Winks_StrgFriendAdd(char *pNo, unsigned char *pNick);
/* ��ȡ���Ѹ��� */
int Winks_StrgFriendCnt(void);
/* ��ȡ������Ϣ��BeginIdx��0��ʼ��������ֵ��ΪBeginIdx�������ã�ֱ������Ϊ��ֵ����ȡȫ������ */
int Winks_StrgFriendGet( int BeginIdx, 
                               char *pNo, int NoLen,
                               unsigned char *pNick, int NickLen);
/*�û�ɾ������*/
int Winks_StrgFriendDel(char *pNo);
/*�û�ɾ��ȫ������*/
int Winks_StrgFriendDelAll(void);
/*��ȡ�û�������Ϣ*/
int Winks_StrgGetCCShow(char *pNo, int dir, WinksStrgFile_CCShow_s *pCCShow);

/* ֱ�ӻ�ȡ�û����� */
int Winks_UserGetCCDirectly(char * pNo);
/*�û����º���¼*/
int Winks_UserResetFriendList(void);
int Winks_UserUpdateFriendListStop(void);
/*�û�����cash�û�����*/
int Winks_UserResetCacheFriendList(void);
int Winks_UserUpdateCacheFriendListStop(void);
/*�û��ϴ�DIY�����Ƹ�����
  diy_info->action: 0 - ֻ�ϴ�������; 1 - �ϴ�������
  diy_info->pnolist: ��actionΪ1ʱʹ�ã����ƺ��ѵ��ֻ����봮����|�ָ���ΪNULL��ʾ������ 
  diy_info->name: �û����õĲ������ƣ�unicode���룬ǰ2�ֽ�Ϊ0��ȡ�ļ���
  diy_info->greet�ַ���Ϊ��(Unicodeǰ2�ֽ�Ϊ0) : �������ʺ���
  diy_info->path�ַ���Ϊ��(Unicodeǰ2�ֽ�Ϊ0) : ֻ�޸��ʺ���
*/
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info);
int Winks_UserUploadDiyStop(void);
/*�ϴ����������*/
int Winks_UserUploadSetting(void);
/*����������*/
int Winks_UserDownLoadSetting(void);
/*ֹͣ���ػ��ϴ�������*/
int Winks_SynsetStop(void);

/* �����Ƽ����������RcmdArr���������ΪRcmdNum��
   RcmdGreet--�����ʺ��� */
int Winks_UserRecommend(int RcmdNum, Winks_RecommendInfo_s * RcmdArr, 
                                  unsigned char * RcmdGreet);
int Winks_UserRecommendStop(void);

/* retBuff -- ���ص��ڴ滺���� 
   retLen -- �����ڴ滺������С�����ʵ��ʹ�õ��ڴ滺������С
   IsDft -- �Ƿ��ȡĬ�ϵ�����
*/
int Winks_UserGetCCShowSetup(char * retBuff, int * retLen, int IsDft);

/*��ȡͨ������Ĳ���*/
int Winks_UserGetCCShow(char * phonenumber, int dir, WinksStrgFile_CCShow_s *pCCShow);
/*ֹͣչʾ����*/
int Winks_UserStopCCShow(void);

/*���ÿ�������ʾ�û�ע�ᡣ*/
int Winks_UserRememberImsi(void);

/* �رղ��� */
int Winks_UserCloseWinkCC(void);
/* �������� */
int Winks_UserOpenWinkCC(void);

/*ȡ�������������ı�־. allowedFlag:0����;1������*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag);

/*�ж��Ƿ�ע���û��� regFlag:0δע�ᣬ1��ע��*/
int Winks_isUserRegistered(unsigned char *regFlag);
/*�ж��Ƿ��Ĭ�û��� SilenceFlag:0�ǳ�Ĭ��1��Ĭ�û�*/
int Winks_isUserSilenced(unsigned char *SilenceFlag);
/* �û����ӷ�ʽ����:Push��ʽ��ʱ���� */
//int Winks_UserSetConnectWay(unsigned long ConnWay);

/*
��Wap��ҳ: �����̳�/�ҵĲ����/����ҵ��/�������
����:
    UrlType -- Winks_UrlType_e
    wid -- ��UrlTypeΪWINKS_URLTYPE_PURCHASEʱ������д�ò���������ΪNULL
*/
int Winks_UserOpenWapPage(int UrlType, char * wid);

/*����ͨ������,�Ӳ�ѡ��ĳЩ�ѡʱ����һ�ζ�ʱ���¡���֮��ѡ����ѡҪֹͣ��ʱ���¡�
 * function: Winks_SetShowCaseProc
 * parameters:
 *   unsigned long val: ͨ�����ã�WINKS_SETVAL_CCSHOW_RING|WINKS_SETVAL_CCSHOW_TALK|WINKS_SETVAL_CCSHOW_HANGUP
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetShowCaseProc(unsigned long val);

/* ��ȡ��ʱ���²��� */
int Winks_GetPeriodUpdate(Winks_PeriodUpdateInfo_s * pPeriodUpdateInfo);
/* ���ö�ʱ���²��� */
int Winks_SetPeriodUpdate(Winks_PeriodUpdateInfo_s * pPeriodUpdateInfo);

int Winks_StrgCheckDiskFiles(void);

/* ��¼�˴β���չʾ��������ظ�ֵ��ʾʧ�� */
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted);

#ifdef __cplusplus
}
#endif

#endif/*_WK_ENGIN_H_*/
